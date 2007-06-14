/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2007 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
 */
#include <stdlib.h>

#include "apr_global_mutex.h"

#include "modsecurity.h"
#include "msc_parsers.h"
#include "msc_util.h"

/**
 * Log an alert message to the log, adding the rule metadata at the end.
 */
void msc_alert(modsec_rec *msr, int level, msre_actionset *actionset, const char *action_message,
    const char *rule_message)
{
    const char *message = NULL;

    if (rule_message == NULL) rule_message = "Unknown error.";

    message = apr_psprintf(msr->mp, "%s %s%s", action_message,
        rule_message, msre_format_metadata(msr, actionset));

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

    #ifdef __SET_MUTEX_PERMS
    rc = unixd_set_global_mutex_perms(msce->auditlog_lock);
    if (rc != APR_SUCCESS) {
        // ap_log_error(APLOG_MARK, APLOG_ERR, rc, s, "mod_security: Could not set permissions on modsec_auditlog_lock; check User and Group directives");
        // return HTTP_INTERNAL_SERVER_ERROR;
        return -1;
    }
    #endif

    return 1;
}

/**
 * Performs per-child (new process) initialisation.
 */
void modsecurity_child_init(msc_engine *msce) {
    if (msce->auditlog_lock != NULL) {
        apr_status_t rc = apr_global_mutex_child_init(&msce->auditlog_lock, NULL, msce->mp);
        if (rc != APR_SUCCESS) {
            // ap_log_error(APLOG_MARK, APLOG_ERR, rs, s, "Failed to child-init auditlog mutex");
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
static apr_status_t modsecurity_tx_cleanup(void *data) {
    modsec_rec *msr = (modsec_rec *)data;
    const apr_array_header_t *arr;
    apr_table_entry_t *te;
    int collect_garbage = 0;
    int i;
    
    if (msr == NULL) return APR_SUCCESS;

    if (rand() < RAND_MAX/100) {
        collect_garbage = 1;
    }

    /* Collections, store & remove stale. */
    arr = apr_table_elts(msr->collections);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        apr_table_t *col = (apr_table_t *)te[i].val;

        /* Only store those collections that changed. */
        if (apr_table_get(msr->collections_dirty, te[i].key)) {
            collection_store(msr, col);
        }

        if (collect_garbage) {
            collections_remove_stale(msr, te[i].key);
        }
    }

    /* Multipart processor cleanup. */
    if (msr->mpd != NULL) multipart_cleanup(msr);

    #ifdef WITH_LIBXML2
    /* XML processor cleanup. */
    if (msr->xml != NULL) xml_cleanup(msr);
    #endif

    modsecurity_request_body_clear(msr);

    return APR_SUCCESS;
}

/**
 *
 */
apr_status_t modsecurity_tx_init(modsec_rec *msr) {
    const char *s = NULL;
    const apr_array_header_t *arr;
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
    }

    msr->arguments_to_sanitise = apr_table_make(msr->mp, 16);
    if (msr->arguments_to_sanitise == NULL) return -1;
    msr->request_headers_to_sanitise = apr_table_make(msr->mp, 16);
    if (msr->request_headers_to_sanitise == NULL) return -1;
    msr->response_headers_to_sanitise = apr_table_make(msr->mp, 16);
    if (msr->response_headers_to_sanitise == NULL) return -1;

    /* Initialise cookies */
    msr->request_cookies = apr_table_make(msr->mp, 16);
    if (msr->request_cookies == NULL) return -1;

    /* Locate the cookie headers and parse them */
    arr = apr_table_elts(msr->request_headers);
    te = (apr_table_entry_t *)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        if (strcasecmp(te[i].key, "Cookie") == 0) {
            if (msr->txcfg->cookie_format == COOKIES_V0) {
                parse_cookies_v0(msr, te[i].val, msr->request_cookies);
            } else {
                parse_cookies_v1(msr, te[i].val, msr->request_cookies);
            }
        }
    }

    /* Collections. */
    msr->tx_vars = apr_table_make(msr->mp, 32);
    if (msr->tx_vars == NULL) return -1;

    msr->collections = apr_table_make(msr->mp, 8);
    if (msr->collections == NULL) return -1;
    msr->collections_dirty = apr_table_make(msr->mp, 8);
    if (msr->collections_dirty == NULL) return -1;

    return 1;
}

/**
 *
 */
static int is_response_status_relevant(modsec_rec *msr, int status) {
    char *my_error_msg = NULL;
    apr_status_t rc;
    char buf[32];

    if ((msr->txcfg->auditlog_relevant_regex == NULL)
        ||(msr->txcfg->auditlog_relevant_regex == NOT_SET_P))
    {
        return 0;
    }

    apr_snprintf(buf, sizeof(buf), "%i", status);

    rc = msc_regexec(msr->txcfg->auditlog_relevant_regex, buf, strlen(buf), &my_error_msg);
    if (rc >= 0) return 1;
    if (rc == PCRE_ERROR_NOMATCH) return 0;

    msr_log(msr, 1, "Regex processing failed (rc %i): %s", rc, my_error_msg);
    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_request_headers(modsec_rec *msr) {
    msr_log(msr, 4, "Starting phase REQUEST_HEADERS.");

    if (msr->txcfg->ruleset != NULL) {
        return msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }

    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_request_body(modsec_rec *msr) {
    msr_log(msr, 4, "Starting phase REQUEST_BODY.");

    if (msr->txcfg->ruleset != NULL) {
        return msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }

    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_response_headers(modsec_rec *msr) {
    msr_log(msr, 4, "Starting phase RESPONSE_HEADERS.");

    if (msr->txcfg->ruleset != NULL) {
        return msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }

    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_response_body(modsec_rec *msr) {
    msr_log(msr, 4, "Starting phase RESPONSE_BODY.");

    if (msr->txcfg->ruleset != NULL) {
        return msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }

    return 0;
}

/**
 *
 */
static apr_status_t modsecurity_process_phase_logging(modsec_rec *msr) {
    msr_log(msr, 4, "Starting phase LOGGING.");

    if (msr->txcfg->ruleset != NULL) {
        msre_ruleset_process_phase(msr->txcfg->ruleset, msr);
    }

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
            msr_log(msr, 4, "Audit log: Not configured to run for this request.");
            return DECLINED;
            break;

        case AUDITLOG_RELEVANT :
            if (msr->is_relevant == 0) {
                msr_log(msr, 4, "Audit log: Ignoring a non-relevant request.");
                return DECLINED;
            }
            break;

        case AUDITLOG_ON :
            /* All right, do nothing */
            break;

        default :
            return HTTP_INTERNAL_SERVER_ERROR;
            break;
    }

    /* Invoke the Audit logger */
    msr_log(msr, 4, "Audit log: Logging this transaction.");

    sec_audit_logger(msr);

    return 0;
}

/**
 * Processes one transaction phase. The phase number does not
 * need to be explicitly provided since it's already available
 * in the modsec_rec structure.
 */
apr_status_t modsecurity_process_phase(modsec_rec *msr, int phase) {
    msr->phase = phase;

    switch(phase) {
        case 1 :
            return modsecurity_process_phase_request_headers(msr);
            break;
        case 2 :
            return modsecurity_process_phase_request_body(msr);
            break;
        case 3 :
            return modsecurity_process_phase_response_headers(msr);
            break;
        case 4 :
            return modsecurity_process_phase_response_body(msr);
            break;
        case 5 :
            return modsecurity_process_phase_logging(msr);
            break;
        default :
            msr_log(msr, 1, "Invalid processing phase: %i", msr->phase);
            return -1;
            break;
    }

    return 0;
}
