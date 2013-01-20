/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#include <stdlib.h>

#include "apr_global_mutex.h"

#include "modsecurity.h"
#include "msc_parsers.h"
#include "msc_util.h"
#include "msc_xml.h"
#include "apr_version.h"

unsigned long int DSOLOCAL unicode_codepage = 0;

int DSOLOCAL *unicode_map_table = NULL;

/**
 * Format an alert message.
 */
const char * msc_alert_message(modsec_rec *msr, msre_actionset *actionset, const char *action_message,
    const char *rule_message)
{
    const char *message = NULL;

    if (rule_message == NULL) rule_message = "Unknown error.";

    if (action_message == NULL) {
        message = apr_psprintf(msr->mp, "%s%s",
            rule_message, msre_format_metadata(msr, actionset));
    }
    else {
        message = apr_psprintf(msr->mp, "%s %s%s", action_message,
            rule_message, msre_format_metadata(msr, actionset));
    }

    return message;
}

/**
 * Log an alert message to the log, adding the rule metadata at the end.
 */
void msc_alert(modsec_rec *msr, int level, msre_actionset *actionset, const char *action_message,
    const char *rule_message)
{
    const char *message = msc_alert_message(msr, actionset, action_message, rule_message);

    msr_log(msr, level, "%s", message);
}

#if 0
/**
 * Return phase name associated with the given phase number.
 */
static const char *phase_name(int phase) {
    switch(phase) {
        case 1 :
            return "REQUEST_HEADERS";
            break;
        case 2 :
            return "REQUEST_BODY";
            break;
        case 3 :
            return "RESPONSE_HEADERS";
            break;
        case 4 :
            return "RESPONSE_BODY";
            break;
        case 5 :
            return "LOGGING";
            break;
    }
    
    return "INVALID";
}
#endif

/**
 * Creates and initialises a ModSecurity engine instance.
 */
msc_engine *modsecurity_create(apr_pool_t *mp, int processing_mode) {
    msc_engine *msce = NULL;

    msce = apr_pcalloc(mp, sizeof(msc_engine));
    if (msce == NULL) return NULL;

    msce->mp = mp;
    msce->processing_mode = processing_mode;

    msce->msre = msre_engine_create(msce->mp);
    if (msce->msre == NULL) return NULL;
    msre_engine_register_default_variables(msce->msre);
    msre_engine_register_default_operators(msce->msre);
    msre_engine_register_default_tfns(msce->msre);
    msre_engine_register_default_actions(msce->msre);
    // TODO: msre_engine_register_default_reqbody_processors(msce->msre);

    return msce;
}

/**
 * Initialise the modsecurity engine. This function must be invoked
 * after configuration processing is complete as Apache needs to know the
 * username it is running as.
 */
int modsecurity_init(msc_engine *msce, apr_pool_t *mp) {
    apr_status_t rc;

    /* Serial audit log mutext */
    rc = apr_global_mutex_create(&msce->auditlog_lock, NULL, APR_LOCK_DEFAULT, mp);
    if (rc != APR_SUCCESS) {
        //ap_log_error(APLOG_MARK, APLOG_ERR, rv, s, "mod_security: Could not create modsec_auditlog_lock");
        //return HTTP_INTERNAL_SERVER_ERROR;
        return -1;
    }

#if !defined(MSC_TEST)
#ifdef __SET_MUTEX_PERMS
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    rc = ap_unixd_set_global_mutex_perms(msce->auditlog_lock);
#else
    rc = unixd_set_global_mutex_perms(msce->auditlog_lock);
#endif
    if (rc != APR_SUCCESS) {
        // ap_log_error(APLOG_MARK, APLOG_ERR, rc, s, "mod_security: Could not set permissions on modsec_auditlog_lock; check User and Group directives");
        // return HTTP_INTERNAL_SERVER_ERROR;
        return -1;
    }
#endif /* SET_MUTEX_PERMS */

    rc = apr_global_mutex_create(&msce->geo_lock, NULL, APR_LOCK_DEFAULT, mp);
    if (rc != APR_SUCCESS) {
        return -1;
    }

#ifdef __SET_MUTEX_PERMS
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    rc = ap_unixd_set_global_mutex_perms(msce->geo_lock);
#else
    rc = unixd_set_global_mutex_perms(msce->geo_lock);
#endif
    if (rc != APR_SUCCESS) {
        return -1;
    }
#endif /* SET_MUTEX_PERMS */
#endif

    return 1;
}

/**
 * Performs per-child (new process) initialisation.
 */
void modsecurity_child_init(msc_engine *msce) {
    /* Need to call this once per process before any other XML calls. */
    xmlInitParser();

    if (msce->auditlog_lock != NULL) {
        apr_status_t rc = apr_global_mutex_child_init(&msce->auditlog_lock, NULL, msce->mp);
        if (rc != APR_SUCCESS) {
            // ap_log_error(APLOG_MARK, APLOG_ERR, rs, s, "Failed to child-init auditlog mutex");
        }
    }

    if (msce->geo_lock != NULL) {
        apr_status_t rc = apr_global_mutex_child_init(&msce->geo_lock, NULL, msce->mp);
        if (rc != APR_SUCCESS) {
            // ap_log_error(APLOG_MARK, APLOG_ERR, rs, s, "Failed to child-init geo mutex");
        }
    }

}

/**
 * Releases resources held by engine instance.
 */
void modsecurity_shutdown(msc_engine *msce) {
    if (msce == NULL) return;
}

/**
 *
 */
static void modsecurity_persist_data(modsec_rec *msr) {
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    apr_time_t time_before, time_after;
    int i;

    time_before = apr_time_now();

    /* Collections, store & remove stale. */
    arr = apr_table_elts(msr->collections);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        apr_table_t *col = (apr_table_t *)te[i].val;

        /* Only store those collections that changed. */
        if (apr_table_get(msr->collections_dirty, te[i].key)) {
            collection_store(msr, col);
        }
    }

    time_after = apr_time_now();

    msr->time_storage_write += time_after - time_before;

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Recording persistent data took %" APR_TIME_T_FMT
            " microseconds.", msr->time_gc);
    }

    /* Remove stale collections. */
    srand(time(NULL));

    if (rand() < RAND_MAX/100) {
        arr = apr_table_elts(msr->collections);
        te = (apr_table_entry_t *)arr->elts;
        for (i = 0; i < arr->nelts; i++) {
            collections_remove_stale(msr, te[i].key);
        }

        msr->time_gc = apr_time_now() - time_after;

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Garbage collection took %" APR_TIME_T_FMT
                " microseconds.", msr->time_gc);
        }
    }
}

/**
 *
 */
static apr_status_t modsecurity_tx_cleanup(void *data) {
    modsec_rec *msr = (modsec_rec *)data;
    char *my_error_msg = NULL;
    
    if (msr == NULL) return APR_SUCCESS;    

    /* Multipart processor cleanup. */
    if (msr->mpd != NULL) multipart_cleanup(msr);

    /* XML processor cleanup. */
    if (msr->xml != NULL) xml_cleanup(msr);

    // TODO: Why do we ignore return code here?
    modsecurity_request_body_clear(msr, &my_error_msg);
    if (my_error_msg != NULL) {
        msr_log(msr, 1, "%s", my_error_msg);
    }

#if defined(WITH_LUA)
    #ifdef CACHE_LUA
    if(msr->L != NULL)  lua_close(msr->L);
    #endif
#endif

    return APR_SUCCESS;
}

/**
 *
 */
apr_status_t modsecurity_tx_init(modsec_rec *msr) {
    const char *s = NULL;
    const apr_array_header_t *arr;
    char *semicolon = NULL;
    char *comma = NULL;
    apr_table_entry_t *te;
    int i;

    /* Register TX cleanup */
    apr_pool_cleanup_register(msr->mp, msr, modsecurity_tx_cleanup, apr_pool_cleanup_null);

    /* Initialise C-L */
    msr->request_content_length = -1;
    s = apr_table_get(msr->request_headers, "Content-Length");
    if (s != NULL) {
        msr->request_content_length = strtol(s, NULL, 10);
    }

    /* Figure out whether this request has a body */
    msr->reqbody_chunked = 0;
    msr->reqbody_should_exist = 0;
    if (msr->request_content_length == -1) {
        /* There's no C-L, but is chunked encoding used? */
        char *transfer_encoding = (char *)apr_table_get(msr->request_headers, "Transfer-Encoding");
        if ((transfer_encoding != NULL)&&(strstr(transfer_encoding, "chunked") != NULL)) {
            msr->reqbody_should_exist = 1;
            msr->reqbody_chunked = 1;
        }
    } else {
        /* C-L found */
        msr->reqbody_should_exist = 1;
    }

    /* Initialise C-T */
    msr->request_content_type = NULL;
    s = apr_table_get(msr->request_headers, "Content-Type");
    if (s != NULL) msr->request_content_type = s;

    /* Decide what to do with the request body. */
    if ((msr->request_content_type != NULL)
       && (strncasecmp(msr->request_content_type, "application/x-www-form-urlencoded", 33) == 0))
    {
        /* Always place POST requests with
         * "application/x-www-form-urlencoded" payloads in memory.
         */
        msr->msc_reqbody_storage = MSC_REQBODY_MEMORY;
        msr->msc_reqbody_spilltodisk = 0;
        msr->msc_reqbody_processor = "URLENCODED";
    } else {
        /* If the C-L is known and there's more data than
         * our limit go to disk straight away.
         */
        if ((msr->request_content_length != -1)
           && (msr->request_content_length > msr->txcfg->reqbody_inmemory_limit))
        {
            msr->msc_reqbody_storage = MSC_REQBODY_DISK;
        }

        /* In all other cases, try using the memory first
         * but switch over to disk for larger bodies.
         */
        msr->msc_reqbody_storage = MSC_REQBODY_MEMORY;
        msr->msc_reqbody_spilltodisk = 1;

        if (msr->request_content_type != NULL) {
            if (strncasecmp(msr->request_content_type, "multipart/form-data", 19) == 0) {
                msr->msc_reqbody_processor = "MULTIPART";
            }
        }
    }

    /* Check if we are forcing buffering, then use memory only. */
    if (msr->txcfg->reqbody_buffering != REQUEST_BODY_FORCEBUF_OFF) {
        msr->msc_reqbody_storage = MSC_REQBODY_MEMORY;
        msr->msc_reqbody_spilltodisk = 0;
    }

    /* Initialise arguments */
    msr->arguments = apr_table_make(msr->mp, 32);
    if (msr->arguments == NULL) return -1;
    if (msr->query_string != NULL) {
        int invalid_count = 0;

        if (parse_arguments(msr, msr->query_string, strlen(msr->query_string),
            msr->txcfg->argument_separator, "QUERY_STRING", msr->arguments,
            &invalid_count) < 0)
        {
            msr_log(msr, 1, "Initialisation: Error occurred while parsing QUERY_STRING arguments.");
            return -1;
        }

        if (invalid_count) {
            msr->urlencoded_error = 1;
        }
    }

    msr->arguments_to_sanitize = apr_table_make(msr->mp, 16);
    if (msr->arguments_to_sanitize == NULL) return -1;
    msr->request_headers_to_sanitize = apr_table_make(msr->mp, 16);
    if (msr->request_headers_to_sanitize == NULL) return -1;
    msr->response_headers_to_sanitize = apr_table_make(msr->mp, 16);
    if (msr->response_headers_to_sanitize == NULL) return -1;
    msr->pattern_to_sanitize = apr_table_make(msr->mp, 32);
    if (msr->pattern_to_sanitize == NULL) return -1;

    /* remove targets */
    msr->removed_targets = apr_table_make(msr->mp, 16);
    if (msr->removed_targets == NULL) return -1;

    /* Initialise cookies */
    msr->request_cookies = apr_table_make(msr->mp, 16);
    if (msr->request_cookies == NULL) return -1;

    /* Initialize matched vars */
    msr->matched_vars = apr_table_make(msr->mp, 8);
    if (msr->matched_vars == NULL) return -1;
    apr_table_clear(msr->matched_vars);

    if(msr->txcfg->max_rule_time > 0)   {
        msr->perf_rules = apr_table_make(msr->mp, 8);
        if (msr->perf_rules == NULL) return -1;
        apr_table_clear(msr->perf_rules);
    }

    /* Locate the cookie headers and parse them */
    arr = apr_table_elts(msr->request_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        if (strcasecmp(te[i].key, "Cookie") == 0) {
            if (msr->txcfg->cookie_format == COOKIES_V0) {
                semicolon = apr_pstrdup(msr->mp, te[i].val);
                while((*semicolon != 0)&&(*semicolon != ';')) semicolon++;
                if(*semicolon == ';')    {
                    parse_cookies_v0(msr, te[i].val, msr->request_cookies, ";");
                } else  {
                    comma = apr_pstrdup(msr->mp, te[i].val);
                    while((*comma != 0)&&(*comma != ',')) comma++;
                    if(*comma == ',')    {
                        comma++;
                        if(*comma == 0x20)   {// looks like comma is the separator
                            if (msr->txcfg->debuglog_level >= 5) {
                                msr_log(msr, 5, "Cookie v0 parser: Using comma as a separator. Semi-colon was not identified!");
                            }
                            parse_cookies_v0(msr, te[i].val, msr->request_cookies, ",");
                        } else {
                            parse_cookies_v0(msr, te[i].val, msr->request_cookies, ";");
                        }
                    } else  {
                        parse_cookies_v0(msr, te[i].val, msr->request_cookies, ";");
                    }
                }
            } else {
                parse_cookies_v1(msr, te[i].val, msr->request_cookies);
            }
        }
    }

    /* Collections. */
    msr->tx_vars = apr_table_make(msr->mp, 32);
    if (msr->tx_vars == NULL) return -1;

    msr->geo_vars = apr_table_make(msr->mp, 8);
    if (msr->geo_vars == NULL) return -1;

    msr->collections_original = apr_table_make(msr->mp, 8);
    if (msr->collections_original == NULL) return -1;
    msr->collections = apr_table_make(msr->mp, 8);
    if (msr->collections == NULL) return -1;
    msr->collections_dirty = apr_table_make(msr->mp, 8);
    if (msr->collections_dirty == NULL) return -1;

    /* Other */
    msr->tcache = NULL;
    msr->tcache_items = 0;

    msr->matched_rules = apr_array_make(msr->mp, 16, sizeof(void *));
    if (msr->matched_rules == NULL) return -1;

    msr->matched_var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
    if (msr->matched_var == NULL) return -1;

    msr->highest_severity = 255; /* high, invalid value */

    msr->removed_rules = apr_array_make(msr->mp, 16, sizeof(char *));
    if (msr->removed_rules == NULL) return -1;

    msr->removed_rules_tag = apr_array_make(msr->mp, 16, sizeof(char *));
    if (msr->removed_rules_tag == NULL) return -1;

    msr->removed_rules_msg = apr_array_make(msr->mp, 16, sizeof(char *));
    if (msr->removed_rules_msg == NULL) return -1;

    return 1;
}

/**
 *
 */
static int is_response_status_relevant(modsec_rec *msr, int status) {
    char *my_error_msg = NULL;
    apr_status_t rc;
    char buf[32];

    /* ENH: Setting is_relevant here will cause an audit even if noauditlog
     * was set for the last rule that matched.  Is this what we want?
     */

    if ((msr->txcfg->auditlog_relevant_regex == NULL)
        ||(msr->txcfg->auditlog_relevant_regex == NOT_SET_P))
    {
        return 0;
    }

    apr_snprintf(buf, sizeof(buf), "%d", status);

    rc = msc_regexec(msr->txcfg->auditlog_relevant_regex, buf, strlen(buf), &my_error_msg);
    if (rc >= 0) return 1;
    if (rc == PCRE_ERROR_NOMATCH) return 0;

    msr_log(msr, 1, "Regex processing failed (rc %d): %s", rc, my_error_msg);
    
    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_request_headers(modsec_rec *msr) {
    apr_time_t time_before;
    apr_status_t rc = 0;
    
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Starting phase REQUEST_HEADERS.");
    }
    
    time_before = apr_time_now();

    if (msr->txcfg->ruleset != NULL) {
        rc = msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }
    
    msr->time_phase1 = apr_time_now() - time_before;

    return rc;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_request_body(modsec_rec *msr) {
    apr_time_t time_before;
    apr_status_t rc = 0;

    
    if ((msr->allow_scope == ACTION_ALLOW_REQUEST)||(msr->allow_scope == ACTION_ALLOW)) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase REQUEST_BODY (allow used).");
        }
        
        return 0;
    } else {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Starting phase REQUEST_BODY.");
        }
    }
    
    time_before = apr_time_now();

    if (msr->txcfg->ruleset != NULL) {
        rc = msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }
    
    msr->time_phase2 = apr_time_now() - time_before;

    return rc;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_response_headers(modsec_rec *msr) {
    apr_time_t time_before;
    apr_status_t rc = 0;
    
    if (msr->allow_scope == ACTION_ALLOW) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase RESPONSE_HEADERS (allow used).");
        }
        
        return 0;
    } else {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Starting phase RESPONSE_HEADERS.");
        }
    }
    
    time_before = apr_time_now();

    if (msr->txcfg->ruleset != NULL) {
        rc = msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }
    
    msr->time_phase3 = apr_time_now() - time_before;

    return rc;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_response_body(modsec_rec *msr) {
    apr_time_t time_before;
    apr_status_t rc = 0;
    
    if (msr->allow_scope == ACTION_ALLOW) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase RESPONSE_BODY (allow used).");
        }
        
        return 0;
    } else {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Starting phase RESPONSE_BODY.");
        }
    }
    
    time_before = apr_time_now();

    if (msr->txcfg->ruleset != NULL) {
        rc = msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }
    
    msr->time_phase4 = apr_time_now() - time_before;


    return rc;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_logging(modsec_rec *msr) {
    apr_time_t time_before, time_after;
    
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Starting phase LOGGING.");
    }
    
    time_before = apr_time_now();

    if (msr->txcfg->ruleset != NULL) {
        msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }
    
    modsecurity_persist_data(msr);
    
    time_after = apr_time_now();
    msr->time_phase5 = time_after - time_before;

    /* Is this request relevant for logging purposes? */
    if (msr->is_relevant == 0) {
        /* Check the status */
        msr->is_relevant += is_response_status_relevant(msr, msr->r->status);

        /* If we processed two requests and statuses are different then
         * check the other status too.
         */
        if (msr->r_early->status != msr->r->status) {
            msr->is_relevant += is_response_status_relevant(msr, msr->r_early->status);
        }
    }

    /* Figure out if we want to keep the files (if there are any, of course). */
    if ((msr->txcfg->upload_keep_files == KEEP_FILES_ON)
        || ((msr->txcfg->upload_keep_files == KEEP_FILES_RELEVANT_ONLY)&&(msr->is_relevant)))
    {
        msr->upload_remove_files = 0;
    } else {
        msr->upload_remove_files = 1;
    }

    /* Are we configured for audit logging? */
    switch(msr->txcfg->auditlog_flag) {
        case AUDITLOG_OFF :
            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Audit log: Not configured to run for this request.");
            }
            
            return DECLINED;
            break;

        case AUDITLOG_RELEVANT :
            if (msr->is_relevant == 0) {
                if (msr->txcfg->debuglog_level >= 4) {
                    msr_log(msr, 4, "Audit log: Ignoring a non-relevant request.");
                }
                
                return DECLINED;
            }
            break;

        case AUDITLOG_ON :
            /* All right, do nothing */
            break;

        default :
            msr_log(msr, 1, "Internal error: Could not determine if auditing is needed, so forcing auditing.");
            break;
    }

    /* Invoke the Audit logger */
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Audit log: Logging this transaction.");
    }

    sec_audit_logger(msr);
    
    msr->time_logging = apr_time_now() - time_after;    

    return 0;
}

/**
 * Processes one transaction phase. The phase number does not
 * need to be explicitly provided since it's already available
 * in the modsec_rec structure.
 */
apr_status_t modsecurity_process_phase(modsec_rec *msr, unsigned int phase) {
    /* Check if we should run. */
    if ((msr->was_intercepted)&&(phase != PHASE_LOGGING)) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase %d as request was already intercepted.", phase);
        }
        
        return 0;
    }

    /* Do not process the same phase twice. */
    if (msr->phase >= phase) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase %d because it was previously run (at %d now).",
                phase, msr->phase);
        }
        
        return 0;
    }

    msr->phase = phase;

    /* Clear out the transformation cache at the start of each phase */
    if (msr->txcfg->cache_trans == MODSEC_CACHE_ENABLED) {
        if (msr->tcache) {
            apr_hash_index_t *hi;
            void *dummy;
            apr_table_t *tab;
            const void *key;
            apr_ssize_t klen;
            #ifdef CACHE_DEBUG
            apr_pool_t *mp = msr->msc_rule_mptmp;
            const apr_array_header_t *ctarr;
            const apr_table_entry_t *ctelts;
            msre_cache_rec *rec;
            int cn = 0;
            int ri;
            #else
            apr_pool_t *mp = msr->mp;
            #endif

            for (hi = apr_hash_first(mp, msr->tcache); hi; hi = apr_hash_next(hi)) {
                apr_hash_this(hi, &key, &klen, &dummy);
                tab = (apr_table_t *)dummy;

                if (tab == NULL) continue;

                #ifdef CACHE_DEBUG
                /* Dump the cache out as we clear */
                ctarr = apr_table_elts(tab);
                ctelts = (const apr_table_entry_t*)ctarr->elts;
                for (ri = 0; ri < ctarr->nelts; ri++) {
                    cn++;
                    rec = (msre_cache_rec *)ctelts[ri].val;
                    if (rec->changed) {
                        if (msr->txcfg->debuglog_level >= 9) {
                            msr_log(msr, 9, "CACHE: %5d) hits=%d key=%pp %x;%s=\"%s\" (%pp - %pp)",
                                cn, rec->hits, key, rec->num, rec->path, log_escape_nq_ex(mp, rec->val, rec->val_len),
                                rec->val, rec->val + rec->val_len);
                        }
                    }
                    else {
                        if (msr->txcfg->debuglog_level >= 9) {
                            msr_log(msr, 9, "CACHE: %5d) hits=%d key=%pp %x;%s=<no change>",
                                cn, rec->hits, key, rec->num, rec->path);
                        }
                    }
                }
                #endif

                apr_table_clear(tab);
                apr_hash_set(msr->tcache, key, klen, NULL);
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Cleared transformation cache for phase %d", msr->phase);
            }
        }

        msr->tcache_items = 0;
        msr->tcache = apr_hash_make(msr->mp);
        if (msr->tcache == NULL) return -1;
    }

    switch(phase) {
        case 1 :
            return modsecurity_process_phase_request_headers(msr);
        case 2 :
            return modsecurity_process_phase_request_body(msr);
        case 3 :
            return modsecurity_process_phase_response_headers(msr);
        case 4 :
            return modsecurity_process_phase_response_body(msr);
        case 5 :
            return modsecurity_process_phase_logging(msr);
        default :
            msr_log(msr, 1, "Invalid processing phase: %d", msr->phase);
            break;
    }

    return -1;
}
