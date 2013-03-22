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

#include <limits.h>

#include "http_core.h"
#include "http_request.h"

#include "modsecurity.h"
#include "apache2.h"
#include "http_main.h"
#include "http_connection.h"

#include "apr_optional.h"
#include "mod_log_config.h"

#include "msc_logging.h"
#include "msc_util.h"

#include "ap_mpm.h"
#include "scoreboard.h"

#include "apr_version.h"

#if defined(WITH_LUA)
#include "msc_lua.h"
#endif


/* ModSecurity structure */

msc_engine DSOLOCAL *modsecurity = NULL;

/* Global module variables; these are used for the Apache-specific functionality */

char DSOLOCAL *chroot_dir = NULL;

char DSOLOCAL *new_server_signature = NULL;

static char *real_server_signature = NULL;

char DSOLOCAL *guardianlog_name = NULL;

apr_file_t DSOLOCAL *guardianlog_fd = NULL;

char DSOLOCAL *guardianlog_condition = NULL;

unsigned long int DSOLOCAL msc_pcre_match_limit = 0;

unsigned long int DSOLOCAL msc_pcre_match_limit_recursion = 0;

unsigned long int DSOLOCAL conn_read_state_limit = 0;

unsigned long int DSOLOCAL conn_write_state_limit = 0;

#if defined(WIN32) || defined(VERSION_NGINX)
int (*modsecDropAction)(request_rec *r) = NULL;
#endif
static int server_limit, thread_limit;

typedef struct {
    int child_num;
    int thread_num;
} sb_handle;

/* -- Miscellaneous functions -- */

/**
* \brief Print informations from used libraries
*
* \param mp Pointer to memory pool
*/
static void version(apr_pool_t *mp) {
    char *pcre_vrs = NULL;

    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: APR compiled version=\"%s\"; "
            "loaded version=\"%s\"", APR_VERSION_STRING, apr_version_string());

    if (strstr(apr_version_string(),APR_VERSION_STRING) == NULL)    {
        ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Loaded APR do not match with compiled!");
    }

    pcre_vrs = apr_psprintf(mp,"%d.%d ", PCRE_MAJOR, PCRE_MINOR);

    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: PCRE compiled version=\"%s\"; "
            "loaded version=\"%s\"", pcre_vrs, pcre_version());

    if (strstr(pcre_version(),pcre_vrs) == NULL)    {
        ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Loaded PCRE do not match with compiled!");
    }

    /* Lua version function was removed in current 5.1. Need to check in future versions if it's back */
#if defined(WITH_LUA)
    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: LUA compiled version=\"%s\"",LUA_VERSION);
#endif /* WITH_LUA */

    ap_log_error(APLOG_MARK, APLOG_NOTICE, 0, NULL,
            "ModSecurity: LIBXML compiled version=\"%s\"",LIBXML_DOTTED_VERSION);
}


/**
 * Intercepts transaction, using the method specified
 * in the structure itself. MUST return an HTTP status code,
 * which will be used to terminate the transaction.
 */
int perform_interception(modsec_rec *msr) {
    msre_actionset *actionset = NULL;
    const char *message = NULL;
    const char *phase_text = "";
    unsigned int pause = 0;
    int status = DECLINED;
    int log_level = 1;

    /* Sanity checks first. */

    if (msr->was_intercepted == 0) {
        msr_log(msr, 1, "Internal Error: Asked to intercept request but was_intercepted is zero");
        return DECLINED;
    }

    if (msr->phase > 4) {
        msr_log(msr, 1, "Internal Error: Asked to intercept request in phase %d.", msr->phase);
        msr->was_intercepted = 0;
        return DECLINED;
    }

    /* OK, we're good to go. */

    actionset = msr->intercept_actionset;
    phase_text = apr_psprintf(msr->mp, " (phase %d)", msr->phase);

    /* By default we log at level 1 but we switch to 4
     * if a nolog action was used or this is not the initial request
     * to hide the message.
     */
    log_level = (actionset->log != 1) ? 4 : 1;

    /* Pause the request first (if configured and the initial request). */
    if (actionset->intercept_pause != NULL) {
        if(strstr(actionset->intercept_pause,"%{") != NULL) {
            msc_string *var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

            var->value = (char *)actionset->intercept_pause;
            var->value_len = strlen(actionset->intercept_pause);
            expand_macros(msr, var, NULL, msr->mp);

            pause = atoi(var->value);
            if ((pause == LONG_MAX)||(pause == LONG_MIN)||(pause <= 0))
                pause = 0;

            msr_log(msr, (log_level > 3 ? log_level : log_level + 1), "Pausing transaction for "
                    "%d msec.", pause);
            /* apr_sleep accepts microseconds */
            apr_sleep((apr_interval_time_t)(pause * 1000));
        } else {
            pause = atoi(actionset->intercept_pause);
            if ((pause == LONG_MAX)||(pause == LONG_MIN)||(pause <= 0))
                pause = 0;
            msr_log(msr, (log_level > 3 ? log_level : log_level + 1), "Pausing transaction for "
                    "%d msec.", pause);
            /* apr_sleep accepts microseconds */
            apr_sleep((apr_interval_time_t)(pause * 1000));
        }
    }

    /* Determine how to respond and prepare the log message. */
    switch(actionset->intercept_action) {
        case ACTION_DENY :
            if (actionset->intercept_status != 0) {
                status = actionset->intercept_status;
                message = apr_psprintf(msr->mp, "Access denied with code %d%s.",
                        status, phase_text);
            } else {
                log_level = 1;
                status = HTTP_INTERNAL_SERVER_ERROR;
                message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                        "(Internal Error: Invalid status code requested %d).",
                        phase_text, actionset->intercept_status);
            }
            break;

        case ACTION_PROXY :
            if (msr->phase < 3) {
                if (ap_find_linked_module("mod_proxy.c") == NULL) {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                        "(Configuration Error: Proxy action to %s requested but mod_proxy not found).",
                        phase_text,
                        log_escape_nq(msr->mp, actionset->intercept_uri));
                } else {
                    msr->r->filename = apr_psprintf(msr->mp, "proxy:%s", actionset->intercept_uri);
                    msr->r->proxyreq = PROXYREQ_REVERSE;
                    msr->r->handler = "proxy-server";
                    status = OK;
                    message = apr_psprintf(msr->mp, "Access denied using proxy to%s %s.",
                        phase_text,
                        log_escape_nq(msr->mp, actionset->intercept_uri));
                }
            } else {
                log_level = 1;
                status = HTTP_INTERNAL_SERVER_ERROR;
                message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                    "(Configuration Error: Proxy action requested but it does not work in output phases).",
                    phase_text);
            }
            break;

        case ACTION_DROP :
            /* ENH This does not seem to work on Windows. Is there a
             *     better way to drop a connection anyway?
             */
            #if !defined(WIN32) && !defined(VERSION_NGINX)
            {
                extern module core_module;
                apr_socket_t *csd = ap_get_module_config(msr->r->connection->conn_config,
                    &core_module);

                if (csd) {
                    if (apr_socket_close(csd) == APR_SUCCESS) {
                        status = HTTP_FORBIDDEN;
                        message = apr_psprintf(msr->mp, "Access denied with connection close%s.",
                            phase_text);
                    } else {
                        log_level = 1;
                        status = HTTP_INTERNAL_SERVER_ERROR;
                        message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                            "(Error: Connection drop requested but failed to close the "
                            " socket).",
                            phase_text);
                    }
                } else {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                        "(Error: Connection drop requested but socket not found.",
                        phase_text);
                }
            }
            #else
            {
                if (modsecDropAction == NULL) {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                        "(Error: Connection drop not implemented on this platform.",
                        phase_text);
                } else if (modsecDropAction(msr->r) == 0) {
                    status = HTTP_FORBIDDEN;
                    message = apr_psprintf(msr->mp, "Access denied with connection close%s.",
                        phase_text);
                } else {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                        "(Error: Connection drop request failed.",
                        phase_text);
                }
            }
            #endif
            break;

        case ACTION_REDIRECT :
            if(strstr(actionset->intercept_uri,"%{") != NULL) {
                msc_string *var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));

                var->value = (char *)actionset->intercept_uri;
                var->value_len = strlen(actionset->intercept_uri);
                expand_macros(msr, var, NULL, msr->mp);

                apr_table_setn(msr->r->headers_out, "Location", var->value);
                if ((actionset->intercept_status == 301)||(actionset->intercept_status == 302)
                        ||(actionset->intercept_status == 303)||(actionset->intercept_status == 307))
                {
                    status = actionset->intercept_status;
                } else {
                    status = HTTP_MOVED_TEMPORARILY;
                }
                message = apr_psprintf(msr->mp, "Access denied with redirection to %s using "
                        "status %d%s.",
                        log_escape_nq(msr->mp, var->value), status,
                        phase_text);
            } else {
                apr_table_setn(msr->r->headers_out, "Location", actionset->intercept_uri);
                if ((actionset->intercept_status == 301)||(actionset->intercept_status == 302)
                        ||(actionset->intercept_status == 303)||(actionset->intercept_status == 307))
                {
                    status = actionset->intercept_status;
                } else {
                    status = HTTP_MOVED_TEMPORARILY;
                }
                message = apr_psprintf(msr->mp, "Access denied with redirection to %s using "
                        "status %d%s.",
                        log_escape_nq(msr->mp, actionset->intercept_uri), status,
                        phase_text);
            }
            break;

        case ACTION_ALLOW :
            status = DECLINED;
            message = apr_psprintf(msr->mp, "Access allowed%s.", phase_text);
            msr->was_intercepted = 0;
            msr->allow_scope = ACTION_ALLOW;
            break;

        case ACTION_PAUSE :
            status = DECLINED;
            message = apr_psprintf(msr->mp, "Paused Access%s.", phase_text);
            msr->was_intercepted = 0;
            msr->allow_scope = ACTION_ALLOW;
            break;

        case ACTION_ALLOW_PHASE :
            status = DECLINED;
            message = apr_psprintf(msr->mp, "Access to phase allowed%s.", phase_text);
            msr->was_intercepted = 0;
            msr->allow_scope = ACTION_ALLOW_PHASE;
            break;

        case ACTION_ALLOW_REQUEST :
            status = DECLINED;
            message = apr_psprintf(msr->mp, "Access to request allowed%s.", phase_text);
            msr->was_intercepted = 0;
            msr->allow_scope = ACTION_ALLOW_REQUEST;
            break;

        default :
            log_level = 1;
            status = HTTP_INTERNAL_SERVER_ERROR;
            message = apr_psprintf(msr->mp, "Access denied with code 500%s "
                    "(Internal Error: invalid interception action %d).",
                    phase_text, actionset->intercept_action);
            break;
    }

    /* If the level is not high enough to add an alert message, but "auditlog"
     * is enabled, then still add the message. */
    if ((log_level > 3) && (actionset->auditlog != 0)) {
        *(const char **)apr_array_push(msr->alerts) = msc_alert_message(msr, actionset, NULL, message);
    }

    /* Log the message now. */
    msc_alert(msr, log_level, actionset, message, msr->intercept_message);

    /* However, this will mark the txn relevant again if it is <= 3,
     * which will mess up noauditlog.  We need to compensate for this
     * so that we do not increment twice when auditlog is enabled and
     * prevent incrementing when auditlog is disabled.
     */
    if ((actionset->auditlog == 0) && (log_level <= 3)) {
        msr->is_relevant--;
    }

    return status;
}

/**
 * Retrieves a previously stored transaction context by
 * looking at the main request, and the previous requests.
 */
static modsec_rec *retrieve_tx_context(request_rec *r) {
    modsec_rec *msr = NULL;
    request_rec *rx = NULL;

    /* Look in the current request first. */
    msr = (modsec_rec *)apr_table_get(r->notes, NOTE_MSR);
    if (msr != NULL) {
        msr->r = r;
        return msr;
    }

    /* If this is a subrequest then look in the main request. */
    if (r->main != NULL) {
        msr = (modsec_rec *)apr_table_get(r->main->notes, NOTE_MSR);
        if (msr != NULL) {
            msr->r = r;
            return msr;
        }
    }

    /* If the request was redirected then look in the previous requests. */
    rx = r->prev;
    while(rx != NULL) {
        msr = (modsec_rec *)apr_table_get(rx->notes, NOTE_MSR);
        if (msr != NULL) {
            msr->r = r;
            return msr;
        }
        rx = rx->prev;
    }

    return NULL;
}

/**
 * Stores transaction context where it can be found in subsequent
 * phases, redirections, or subrequests.
 */
static void store_tx_context(modsec_rec *msr, request_rec *r) {
    apr_table_setn(r->notes, NOTE_MSR, (void *)msr);
}

/**
 * Creates a new transaction context.
 */
static modsec_rec *create_tx_context(request_rec *r) {
    apr_allocator_t *allocator = NULL;
    modsec_rec *msr = NULL;

    msr = (modsec_rec *)apr_pcalloc(r->pool, sizeof(modsec_rec));
    if (msr == NULL) return NULL;

    apr_allocator_create(&allocator);
    apr_allocator_max_free_set(allocator, 1024);
    apr_pool_create_ex(&msr->mp, r->pool, NULL, allocator);
    if (msr->mp == NULL) return NULL;
    apr_allocator_owner_set(allocator, msr->mp);

    msr->modsecurity = modsecurity;
    msr->r = r;
    msr->r_early = r;
    msr->request_time = r->request_time;
    msr->dcfg1 = (directory_config *)ap_get_module_config(r->per_dir_config,
            &security2_module);

#if defined(WITH_LUA)
    #ifdef CACHE_LUA
#if LUA_VERSION_NUM > 501
    msr->L = luaL_newstate();
#else
    msr->L = lua_open();
#endif
    luaL_openlibs(msr->L);
    #endif
#endif


    /**
     * Create a special user configuration. This is where
     * explicit instructions will be stored. This will be used
     * to override the default settings (and to override the
     * configuration in the second phase, dcfg2, with the user
     * setting executed in the first phase.
     */
    msr->usercfg = create_directory_config(msr->mp, NULL);
    if (msr->usercfg == NULL) return NULL;

    /* Create a transaction context and populate
     * it using the directory config we just
     * got from Apache.
     */
    msr->txcfg = create_directory_config(msr->mp, NULL);
    if (msr->txcfg == NULL) return NULL;

    if (msr->dcfg1 != NULL) {
        msr->txcfg = merge_directory_configs(msr->mp, msr->txcfg, msr->dcfg1);
        if (msr->txcfg == NULL) return NULL;
    }
    init_directory_config(msr->txcfg);

    msr->txid = get_env_var(r, "UNIQUE_ID");
    if (msr->txid == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, r->server,
                "ModSecurity: ModSecurity requires mod_unique_id to be installed.");
        return NULL;
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Initialising transaction (txid %s).", msr->txid);
    }

    /* Populate tx fields */
    msr->error_messages = apr_array_make(msr->mp, 5, sizeof(error_message_t *));
    msr->alerts = apr_array_make(msr->mp, 5, sizeof(char *));

    msr->server_software = real_server_signature;
    msr->local_addr = r->connection->local_ip;
    msr->local_port = r->connection->local_addr->port;

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    msr->remote_addr = r->connection->remote_ip;
    msr->remote_port = r->connection->remote_addr->port;
#else
    msr->remote_addr = r->connection->client_ip;
    msr->remote_port = r->connection->client_addr->port;
    msr->useragent_ip = r->useragent_ip;
#endif

    msr->request_line = r->the_request;
    msr->request_uri = r->uri;
    msr->request_method = r->method;
    msr->query_string = r->args;
    msr->request_protocol = r->protocol;
    msr->request_headers = apr_table_copy(msr->mp, r->headers_in);
    msr->hostname = ap_get_server_name(r);

    msr->msc_rule_mptmp = NULL;

    /* Invoke the engine to continue with initialisation */
    if (modsecurity_tx_init(msr) < 0) {
        msr_log(msr, 1, "Failed to initialise transaction (txid %s).", msr->txid);
        return NULL;
    }

    store_tx_context(msr, r);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Transaction context created (dcfg %pp).", msr->dcfg1);
    }

    return msr;
}


/* -- Hooks -- */

/*
 * Change the signature of the server if change was requested in
 * configuration. We do this by locating the signature in server
 * memory and writing over it.
 */
static apr_status_t change_server_signature(server_rec *s) {
    char *server_version = NULL;

    if (new_server_signature == NULL) return 0;

    server_version = (char *)apache_get_server_version();

    if (server_version == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
                "SecServerSignature: Apache returned null as signature.");
        return -1;
    }

    if (strlen(server_version) >= strlen(new_server_signature)) {
        strcpy(server_version, new_server_signature);
    }
    else {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
                "SecServerSignature: original signature too short. Please set "
                "ServerTokens to Full.");
        return -1;
    }

    /* Check that it really changed.  This assumes that what we got was
     * not a copy and this could change in future versions of Apache.
     */
    server_version = (char *)apache_get_server_version();
    if ((server_version == NULL) || (strcmp(server_version, new_server_signature) != 0)) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s, "SecServerSignature: Failed to change server signature to \"%s\".", new_server_signature);
        return 0;
    }
    else {
        ap_log_error(APLOG_MARK, APLOG_DEBUG | APLOG_NOERRNO, 0, s, "SecServerSignature: Changed server signature to \"%s\".", server_version);
    }

    return 1;
}

/**
 * Executed at the end of server lifetime to cleanup the allocated resources.
 */
static apr_status_t module_cleanup(void *data) {
    modsecurity_shutdown(modsecurity);
    return APR_SUCCESS;
}

/**
 * Generate a single variable for use with mod_log_config.
 */
static const char *modsec_var_log_handler(request_rec *r, char *name) {
    modsec_rec *msr = NULL;

    if (name == NULL) return NULL;

    msr = retrieve_tx_context(r);
    if (msr == NULL) return NULL;

    return construct_single_var(msr, name);
}

/**
 * Pre-configuration initialisation hook.
 */
static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    static APR_OPTIONAL_FN_TYPE(ap_register_log_handler) *log_pfn_register;

    /* Initialise ModSecurity engine */
    modsecurity = modsecurity_create(mp, MODSEC_ONLINE);
    if (modsecurity == NULL) {
        ap_log_error(APLOG_MARK, APLOG_STARTUP, 0, NULL,
                "ModSecurity: Failed to initialise engine.");
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    log_pfn_register = APR_RETRIEVE_OPTIONAL_FN(ap_register_log_handler);
    if (log_pfn_register) {
        log_pfn_register(mp, "M", modsec_var_log_handler, 0);
    }

    return OK;
}

/**
 * Main (post-configuration) module initialisation.
 */
static int hook_post_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp, server_rec *s) {
    void *init_flag = NULL;
    int first_time = 0;

    /* ENH Figure out a way to validate config before we start
     * - skipafter: need to make sure we found all of our targets
     */

    /* Figure out if we are here for the first time */
    apr_pool_userdata_get(&init_flag, "modsecurity-init-flag", s->process->pool);
    if (init_flag == NULL) {
        first_time = 1;
        apr_pool_userdata_set((const void *)1, "modsecurity-init-flag",
                apr_pool_cleanup_null, s->process->pool);
    } else {
        modsecurity_init(modsecurity, mp);
    }

    /* Store the original server signature */
    real_server_signature = apr_pstrdup(mp, apache_get_server_version());

    /* Make some space in the server signature for later */
    if (new_server_signature != NULL) {
        ap_add_version_component(mp, new_server_signature);
        change_server_signature(s);
    }

#if (!(defined(WIN32) || defined(NETWARE)))

    /* Internal chroot functionality */

    if (chroot_dir != NULL) {

        /* ENH Is it safe to simply return with an error, instead
         *      of using exit()?
         */

        if (first_time == 0) {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                    "ModSecurity: chroot checkpoint #2 (pid=%ld ppid=%ld)", (long)getpid(), (long)getppid());

            if (chdir(chroot_dir) < 0) {
                ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
                        "ModSecurity: chroot failed, unable to chdir to %s, errno=%d (%s)",
                        chroot_dir, errno, strerror(errno));
                exit(1);
            }

            if (chroot(chroot_dir) < 0) {
                ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
                        "ModSecurity: chroot failed, path=%s, errno=%d(%s)",
                        chroot_dir, errno, strerror(errno));
                exit(1);
            }

            if (chdir("/") < 0) {
                ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, s,
                        "ModSecurity: chdoot failed, unable to chdir to /, errno=%d (%s)",
                        errno, strerror(errno));
                exit(1);
            }

            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                    "ModSecurity: chroot successful, path=%s", chroot_dir);
        } else {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                    "ModSecurity: chroot checkpoint #1 (pid=%ld ppid=%ld)", (long)getpid(), (long)getppid());
        }
    }
#endif

    /* Schedule main cleanup for later, when the main pool is destroyed. */
    apr_pool_cleanup_register(mp, (void *)s, module_cleanup, apr_pool_cleanup_null);

    /* Log our presence to the error log. */
    if (first_time) {
        ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                "%s configured.", MODSEC_MODULE_NAME_FULL);

        version(mp);

        /* If we've changed the server signature make note of the original. */
        if (new_server_signature != NULL) {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                    "Original server signature: %s", real_server_signature);
        }
    }

    srand((unsigned int)(time(NULL) * getpid()));

    return OK;
}

/**
 * Initialisation performed for every new child process.
 */
static void hook_child_init(apr_pool_t *mp, server_rec *s) {
    modsecurity_child_init(modsecurity);
}

/**
 * Initial request processing, executed immediatelly after
 * Apache receives the request headers. This function wil create
 * a transaction context.
 */
static int hook_request_early(request_rec *r) {
    modsec_rec *msr = NULL;
    int rc = DECLINED;

    /* This function needs to run only once per transaction
     * (i.e. subrequests and redirects are excluded).
     */
    if ((r->main != NULL)||(r->prev != NULL)) {
        return DECLINED;
    }

    /* Initialise transaction context and
     * create the initial configuration.
     */
    msr = create_tx_context(r);
    if (msr == NULL) return DECLINED;

#ifdef REQUEST_EARLY

    /* Are we allowed to continue? */
    if (msr->txcfg->is_enabled == MODSEC_DISABLED) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Processing disabled, skipping (hook request_early).");
        }
        return DECLINED;
    }

    /* Process phase REQUEST_HEADERS */
    if (modsecurity_process_phase(msr, PHASE_REQUEST_HEADERS) > 0) {
        rc = perform_interception(msr);
    }

    if (    (msr->txcfg->is_enabled != MODSEC_DISABLED)
            && (msr->txcfg->reqbody_access == 1)
            && (rc == DECLINED))
    {
        /* Check request body limit (non-chunked requests only). */
        if (msr->request_content_length > msr->txcfg->reqbody_limit) {
            msr_log(msr, 1, "Request body (Content-Length) is larger than the "
                    "configured limit (%ld).", msr->txcfg->reqbody_limit);
            if(msr->txcfg->is_enabled != MODSEC_DETECTION_ONLY)
                return HTTP_REQUEST_ENTITY_TOO_LARGE;
        }
    }
#endif
    return rc;
}

/**
 * Invoked as the first hook in the handler chain, this function
 * executes the second phase of ModSecurity request processing.
 */
static int hook_request_late(request_rec *r) {
    char *my_error_msg = NULL;
    modsec_rec *msr = NULL;
    int rc;

    /* This function needs to run only once per transaction
     * (i.e. subrequests and redirects are excluded).
     */
    if ((r->main != NULL)||(r->prev != NULL)) {
        return DECLINED;
    }

    /* Find the transaction context and make sure
     * we are supposed to proceed.
     */
    msr = retrieve_tx_context(r);
    if (msr == NULL) {
        /* If we can't find the context that probably means it's
         * a subrequest that was not initiated from the outside.
         */
        return DECLINED;
    }

    /* Has this phase been completed already? */
    if (msr->phase_request_body_complete) {
        msr_log(msr, 1, "Internal Error: Attempted to process the request body more than once.");
        return DECLINED;
    }

    msr->phase_request_body_complete = 1;
    msr->remote_user = r->user;

    /* Get the second configuration context. */
    msr->dcfg2 = (directory_config *)ap_get_module_config(r->per_dir_config,
            &security2_module);

    /* Create a transaction context. */
    msr->txcfg = create_directory_config(msr->mp, NULL);
    if (msr->txcfg == NULL) return DECLINED;
    if (msr->dcfg2 != NULL) {
        msr->txcfg = merge_directory_configs(msr->mp, msr->txcfg, msr->dcfg2);
        if (msr->txcfg == NULL) return DECLINED;
    }

    /* Update with the explicit user settings. */
    msr->txcfg = merge_directory_configs(msr->mp, msr->txcfg, msr->usercfg);

    init_directory_config(msr->txcfg);

    if (msr->txcfg->is_enabled == 0) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Processing disabled, skipping (hook request_late).");
        }
        return DECLINED;
    }

#ifndef REQUEST_EARLY
    /* Phase 1 */
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "First phase starting (dcfg %pp).", msr->dcfg2);
    }

    /* Process phase REQUEST_HEADERS */
    if (modsecurity_process_phase(msr, PHASE_REQUEST_HEADERS) > 0) {
        /* There was a match; see if we need to intercept. */
        rc = perform_interception(msr);
        if (rc != DECLINED) {
            /* Intercepted */
            return rc;
        }
    }
#endif

    /* The rule engine could have been disabled in phase 1. */
    if (msr->txcfg->is_enabled == MODSEC_DISABLED) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Skipping phase 2 as the rule engine was disabled by a rule in phase 1.");
        }

        return DECLINED;
    }

    /* Phase 2 */
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Second phase starting (dcfg %pp).", msr->dcfg2);
    }

    /* Check that the request body is not too long, but only
     * if configuration allows for request body access.
     */
    msr->inbound_error = 0;
    if (msr->txcfg->reqbody_access == 1) {
        /* Check request body limit (non-chunked requests only). */
        if (msr->request_content_length > msr->txcfg->reqbody_limit) {

            if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT)) {
                msr->inbound_error = 1;
                msr_log(msr, 1, "Request body (Content-Length) is larger than the "
                        "configured limit (%ld). Deny with status (%d)", msr->txcfg->reqbody_limit, HTTP_REQUEST_ENTITY_TOO_LARGE);
                return HTTP_REQUEST_ENTITY_TOO_LARGE;
            } else if ((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)){
                msr->inbound_error = 1;
                msr_log(msr, 1, "Request body (Content-Length) is larger than the "
                        "configured limit (%ld).", msr->txcfg->reqbody_limit);
            } else if ((msr->txcfg->is_enabled == MODSEC_DETECTION_ONLY) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)){
                msr_log(msr, 1, "Request body (Content-Length) is larger than the "
                        "configured limit (%ld).", msr->txcfg->reqbody_limit);
                msr->inbound_error = 1;
            } else  {
                msr_log(msr, 1, "Request body (Content-Length) is larger than the "
                        "configured limit (%ld).", msr->txcfg->reqbody_limit);
                msr->inbound_error = 1;
            }
        }
    }

    /* Figure out whether to extract multipart files. */
    if ((msr->txcfg->upload_keep_files != KEEP_FILES_OFF) /* user might want to keep them */
            || (msr->txcfg->upload_validates_files)) /* user might want to validate them */
    {
        msr->upload_extract_files = 1;
        msr->upload_remove_files = 1;
    }

    rc = read_request_body(msr, &my_error_msg);
    if (rc < 0) {
        switch(rc) {
            case -1 :
                if (my_error_msg != NULL) {
                    msr_log(msr, 1, "%s", my_error_msg);
                }
                return HTTP_INTERNAL_SERVER_ERROR;
                break;
            case -4 : /* Timeout. */
                if (my_error_msg != NULL) {
                    msr_log(msr, 4, "%s", my_error_msg);
                }
                r->connection->keepalive = AP_CONN_CLOSE;
                return HTTP_REQUEST_TIME_OUT;
                break;
            case -5 : /* Request body limit reached. */
                msr->inbound_error = 1;
                if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT))    {
                    r->connection->keepalive = AP_CONN_CLOSE;
                    if (my_error_msg != NULL) {
                        msr_log(msr, 1, "%s. Deny with code (%d)", my_error_msg, HTTP_REQUEST_ENTITY_TOO_LARGE);
                    }
                    return HTTP_REQUEST_ENTITY_TOO_LARGE;
                } else  {
                    if (my_error_msg != NULL) {
                        msr_log(msr, 1, "%s", my_error_msg);
                    }
                }
                break;
            case -6 : /* EOF when reading request body. */
                if (my_error_msg != NULL) {
                    msr_log(msr, 4, "%s", my_error_msg);
                }
                r->connection->keepalive = AP_CONN_CLOSE;
                return HTTP_BAD_REQUEST;
                break;
            default :
                /* allow through */
                break;
        }

        msr->msc_reqbody_error = 1;
        msr->msc_reqbody_error_msg = my_error_msg;
    }

    /* Update the request headers. They might have changed after
     * the body was read (trailers).
     *
     * TODO We might still want to hold onto the original headers
     *      so that we can log them. Keeping them is probably not
     *      going to increase our memory requirements (because all
     *      headers are allocated from the request memory pool
     *      anyway).
     */
    msr->request_headers = apr_table_copy(msr->mp, r->headers_in);

    /* Process phase REQUEST_BODY */
    rc = DECLINED;
    if (modsecurity_process_phase(msr, PHASE_REQUEST_BODY) > 0) {
        rc = perform_interception(msr);
    }

    if(msr->txcfg->stream_inbody_inspection && msr->msc_reqbody_read)    {
        const char *clen = NULL;
        clen = apr_psprintf(msr->mp,"%"APR_SIZE_T_FMT,msr->stream_input_length);
        if(clen)
            apr_table_setn(r->headers_in, "Content-Length",clen);
    }

    /* Remove the compression ability indications the client set,
     * but only if we need to disable backend compression.
     */
    if (msr->txcfg->disable_backend_compression) {
        apr_table_unset(r->headers_in, "Accept-Encoding");
        apr_table_unset(r->headers_in, "TE");
    }

    return rc;
}

/**
 * Invoked every time Apache has something to write to the error log.
 */
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
static void hook_error_log(const ap_errorlog_info *info, const char *errstr)
#else
static void hook_error_log(const char *file, int line, int level, apr_status_t status,
        const server_rec *s, const request_rec *r, apr_pool_t *mp, const char *fmt)
#endif
{
    modsec_rec *msr = NULL;
    error_message_t *em = NULL;

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    if (info == NULL) return;
    if (info->r == NULL) return;
#else
    if (r == NULL) return;
#endif

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    msr = retrieve_tx_context((request_rec *)info->r);
#else
    msr = retrieve_tx_context((request_rec *)r);
#endif

    /* Create a context for requests we never had the chance to process */
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    if ((msr == NULL)
        && ((info->level & APLOG_LEVELMASK) < APLOG_DEBUG)
        && apr_table_get(info->r->subprocess_env, "UNIQUE_ID"))
#else
    if ((msr == NULL)
        && ((level & APLOG_LEVELMASK) < APLOG_DEBUG)
        && apr_table_get(r->subprocess_env, "UNIQUE_ID"))
#endif
    {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
        msr = create_tx_context((request_rec *)info->r);
#else
        msr = create_tx_context((request_rec *)r);
#endif
        if (msr->txcfg->debuglog_level >= 9) {
            if (msr == NULL) {
                msr_log(msr, 9, "Failed to create context after request failure.");
            }
            else {
                msr_log(msr, 9, "Context created after request failure.");
            }
        }
    }
    if (msr == NULL) return;

    /* Store the error message for later */
    em = (error_message_t *)apr_pcalloc(msr->mp, sizeof(error_message_t));
    if (em == NULL) return;

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    if (info->file != NULL) em->file = apr_pstrdup(msr->mp, info->file);
    em->line = info->line;
    em->level = info->level;
    em->status = info->status;
    if (info->format != NULL) em->message = apr_pstrdup(msr->mp, info->format);
#else
    if (file != NULL) em->file = apr_pstrdup(msr->mp, file);
    em->line = line;
    em->level = level;
    em->status = status;
    if (fmt != NULL) em->message = apr_pstrdup(msr->mp, fmt);
#endif
    /* Remove \n from the end of the message */
    if (em->message != NULL) {
        char *p = (char *)em->message;
        while(*p != '\0') {
            if ((*(p + 1) == '\0')&&(*p == '\n')) {
                *p = '\0';
                break;
            }
            p++;
        }
    }

    *(const error_message_t **)apr_array_push(msr->error_messages) = em;
}


/**
 * Guardian logger is used to interface to the external
 * script for web server protection - httpd_guardian.
 */
static void sec_guardian_logger(request_rec *r, request_rec *origr, modsec_rec *msr) {
    char *str1, *str2, *text;
    char *modsec_message = "-";
    int modsec_rating = 0; /* not used yet */
    apr_size_t nbytes, nbytes_written;
    apr_time_t duration = (apr_time_now() - origr->request_time);
    int limit, was_limited;

    /* bail out if we do not have where to write */
    if ((guardianlog_name == NULL)||(guardianlog_fd == NULL)) return;

    /* process the condition, if we have one */
    if (guardianlog_condition != NULL) {
        if (*guardianlog_condition == '!') {
            if (apr_table_get(r->subprocess_env, guardianlog_condition + 1) != NULL) {
                return;
            }
        }
        else {
            if (apr_table_get(r->subprocess_env, guardianlog_condition) == NULL) {
                return;
            }
        }
    }

    /*
     * Log format is as follows:
     *
     * %V %h %l %u %t "%r" %>s %b "%{Referer}i" "%{User-agent}i" %{UNIQUE_ID}e
     * "SESSION_ID" %T %D "MODSEC_MESSAGE" MODSEC_RATING
     *
     * The fields SESSION_ID, MODSEC_MESSAGE, and MODSEC_RATING are not used at the moment.
     */

    str2 = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT " %" APR_TIME_T_FMT " \"%s\" %d",
            duration, apr_time_sec(duration), log_escape(msr->mp, modsec_message), modsec_rating);
    if (str2 == NULL) return;

    /* We do not want the index line to be longer than 3980 bytes. */
    limit = 3980;
    was_limited = 0;

    /* If we are logging to a pipe we need to observe and
     * obey the pipe atomic write limit - PIPE_BUF. For
     * more details see the discussion in sec_guardian_logger,
     * above.
     */
    if (msr->txcfg->auditlog_name[0] == '|') {
        if (PIPE_BUF < limit) {
            limit = PIPE_BUF;
        }
    }

    limit = limit - strlen(str2) - 5;
    if (limit <= 0) {
        msr_log(msr, 1, "Audit Log: Atomic PIPE write buffer too small: %d", PIPE_BUF);
        return;
    }

    str1 = construct_log_vcombinedus_limited(msr, limit, &was_limited);
    if (str1 == NULL) return;

    if (was_limited == 0) {
        text = apr_psprintf(msr->mp, "%s %s \n", str1, str2);
    } else {
        text = apr_psprintf(msr->mp, "%s %s L\n", str1, str2);
    }
    if (text == NULL) return;

    nbytes = strlen(text);
    apr_file_write_full(guardianlog_fd, text, nbytes, &nbytes_written);
}

/**
 * Invoked at the end of each transaction.
 */
static int hook_log_transaction(request_rec *r) {
    const apr_array_header_t *arr = NULL;
    request_rec *origr = NULL;
    modsec_rec *msr = NULL;

    msr = retrieve_tx_context(r);
    if (msr == NULL) {
        return DECLINED;
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Initialising logging.");
    }

    /* Find the first (origr) and the last (r) request */
    origr = r;
    while(origr->prev) {
        origr = origr->prev;
    }
    while(r->next) {
        r = r->next;
    }

    /* At this point r is the last request in the
     * chain. However, we now need to detect a case when
     * a bad ErrorDocument was used and back out of it. That's
     * how Apache does it internally. Except where Apache knows
     * exactly what is happening we will have to rely on the missing
     * headers in the final request to detect this condition.
     */
    arr = apr_table_elts(r->headers_out);
    while ((arr->nelts == 0)&&(r->prev != NULL)) {
        r = r->prev;
        arr = apr_table_elts(r->headers_out);
    }

    msr->r = r;
    msr->response_status = r->status;
    msr->status_line = ((r->status_line != NULL)
            ? r->status_line : ap_get_status_line(r->status));
    msr->response_protocol = get_response_protocol(origr);
    msr->response_headers = apr_table_copy(msr->mp, r->headers_out);
    if (!r->assbackwards) msr->response_headers_sent = 1;
    msr->bytes_sent = r->bytes_sent;
    msr->local_user = r->user;
    msr->remote_user = r->connection->remote_logname;

    /* -- Guardian -- */

    sec_guardian_logger(r, origr, msr);

    /* Invoke the engine to do the rest of the work now. */
    modsecurity_process_phase(msr, PHASE_LOGGING);

    return DECLINED;
}

/**
 * Invoked right before request processing begins. This is
 * when we need to decide if we want to hook into the output
 * filter chain.
 */
static void hook_insert_filter(request_rec *r) {
    modsec_rec *msr = NULL;

    /* Find the transaction context first. */
    msr = retrieve_tx_context(r);
    if (msr == NULL) return;

    /* Add the input filter, but only if we need it to run. */
    if (msr->if_status == IF_STATUS_WANTS_TO_RUN) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Adding input forwarding filter %s(r %pp).",
                    (((r->main != NULL)||(r->prev != NULL)) ? "for subrequest " : ""), r);
        }

        ap_add_input_filter("MODSECURITY_IN", msr, r, r->connection);
    }

    /* The output filters only need to be added only once per transaction
     * (i.e. subrequests and redirects are excluded).
     */
    if ((r->main != NULL)||(r->prev != NULL)) {
        return;
    }

    /* Only proceed to add the second filter if the engine is enabled. */
    if (msr->txcfg->is_enabled == 0) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Processing disabled, skipping.");
        }

        return;
    }

    /* We always add the output filter because that's where we need to
     * initiate our 3rd and 4th processing phases from. The filter is
     * smart enough not to buffer the data if it is not supposed to.
     */
    if (msr->of_status != OF_STATUS_COMPLETE) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Adding output filter (r %pp).", r);
        }

        ap_add_output_filter("MODSECURITY_OUT", msr, r, r->connection);
    }
}

// TODO: Holding off on this for now (needs more testing)
/**
 * Invoked whenever Apache starts processing an error. A chance
 * to insert ourselves into the output filter chain.
 */
static void hook_insert_error_filter(request_rec *r) {
    modsec_rec *msr = NULL;

    /* Find the transaction context and make sure we are
     * supposed to proceed.
     */
    msr = retrieve_tx_context(r);
    if (msr == NULL) return;

    /* Do not run if not enabled. */
    if (msr->txcfg->is_enabled == 0) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Processing disabled, skipping.");
        }
        return;
    }

    /* Do not run if the output filter already completed. This will
     * happen if we intercept in phase 4.
     */
    if (msr->of_status != OF_STATUS_COMPLETE) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Adding output filter (r %pp).", r);
        }

        /* Make a note that the output we will be receiving is a
         * result of error processing.
         */
        msr->of_is_error = 1;

        ap_add_output_filter("MODSECURITY_OUT", msr, r, r->connection);
    } else {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Output buffering already complete.");
        }
    }
}

#if (!defined(NO_MODSEC_API))
/**
 * This function is exported for other Apache modules to
 * register new transformation functions.
 */
static void modsec_register_tfn(const char *name, void *fn) {
    if (modsecurity != NULL) {
        msre_engine_tfn_register(modsecurity->msre, name, (fn_tfn_execute_t)fn);
    }
}

/**
 * This function is exported for other Apache modules to
 * register new operators.
 */
static void modsec_register_operator(const char *name, void *fn_init, void *fn_exec) {
    if (modsecurity != NULL) {
        msre_engine_op_register(modsecurity->msre, name, (fn_op_param_init_t)fn_init, (fn_op_execute_t)fn_exec);
    }
}

/**
 * \brief Connetion hook to limit the number of
 * connections in BUSY state
 *
 * \param conn Pointer to connection struct
 *
 * \retval DECLINED On failure
 * \retval OK On Success
 */
static int hook_connection_early(conn_rec *conn)
{
    sb_handle *sb = conn->sbh;
    int i, j;
    unsigned long int ip_count = 0, ip_count_w = 0;
    worker_score *ws_record = NULL;
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
    ap_sb_handle_t *sbh = NULL;
#endif

    if(sb != NULL && (conn_read_state_limit > 0 || conn_write_state_limit > 0))   {

        ws_record = &ap_scoreboard_image->servers[sb->child_num][sb->thread_num];
        if(ws_record == NULL)
            return DECLINED;

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
        apr_cpystrn(ws_record->client, conn->client_ip, sizeof(ws_record->client));
#else
        apr_cpystrn(ws_record->client, conn->remote_ip, sizeof(ws_record->client));
#endif
        for (i = 0; i < server_limit; ++i) {
            for (j = 0; j < thread_limit; ++j) {

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                sbh = conn->sbh;
                if (sbh == NULL)        {
                    return DECLINED;
                }

                ws_record = ap_get_scoreboard_worker(sbh);
#else
                ws_record = ap_get_scoreboard_worker(i, j);
#endif

                if(ws_record == NULL)
                    return DECLINED;

                switch (ws_record->status) {
                    case SERVER_BUSY_READ:
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                        if (strcmp(conn->client_ip, ws_record->client) == 0)
                            ip_count++;
#else
                        if (strcmp(conn->remote_ip, ws_record->client) == 0)
                            ip_count++;
#endif
                        break;
                    case SERVER_BUSY_WRITE:
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
                        if (strcmp(conn->client_ip, ws_record->client) == 0)
                            ip_count_w++;
#else
                        if (strcmp(conn->remote_ip, ws_record->client) == 0)
                            ip_count_w++;
#endif
                        break;
                    default:
                        break;
                }
            }
        }

        if ((conn_read_state_limit > 0) && (ip_count > conn_read_state_limit)) {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
            ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Access denied with code 400. Too many threads [%ld] of %ld allowed in READ state from %s - Possible DoS Consumption Attack [Rejected]", ip_count,conn_read_state_limit,conn->client_ip);
#else
            ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Access denied with code 400. Too many threads [%ld] of %ld allowed in READ state from %s - Possible DoS Consumption Attack [Rejected]", ip_count,conn_read_state_limit,conn->remote_ip);
#endif
            return OK;
        } else if ((conn_write_state_limit > 0) && (ip_count_w > conn_write_state_limit)) {
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
            ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Access denied with code 400. Too many threads [%ld] of %ld allowed in WRITE state from %s - Possible DoS Consumption Attack [Rejected]", ip_count_w,conn_write_state_limit,conn->client_ip);
#else
            ap_log_error(APLOG_MARK, APLOG_WARNING, 0, NULL, "ModSecurity: Access denied with code 400. Too many threads [%ld] of %ld allowed in WRITE state from %s - Possible DoS Consumption Attack [Rejected]", ip_count_w,conn_write_state_limit,conn->remote_ip);
#endif
            return OK;
        } else {
            return DECLINED;
        }
    }

    return DECLINED;
}

/**
 * This function is exported for other Apache modules to
 * register new variables.
 */
static void modsec_register_variable(const char *name, unsigned int type,
        unsigned int argc_min, unsigned int argc_max,
        void *fn_validate, void *fn_generate,
        unsigned int is_cacheable, unsigned int availability) {
    if (modsecurity != NULL) {
        msre_engine_variable_register(modsecurity->msre, name, type, argc_min, argc_max, (fn_var_validate_t)fn_validate, (fn_var_generate_t)fn_generate, is_cacheable, availability);
    }
    else {
        fprintf(stderr,"modsecurity is NULL\n");
    }
}

/**
 * This function is exported for other Apache modules to
 * register new request body processors.
 */
static void modsec_register_reqbody_processor(const char *name,
        void *fn_init,
        void *fn_process,
        void *fn_complete)
{
    if (modsecurity != NULL) {

        msre_engine_reqbody_processor_register(modsecurity->msre, name,
                (fn_reqbody_processor_init_t)fn_init,
                (fn_reqbody_processor_init_t)fn_process,
                (fn_reqbody_processor_init_t)fn_complete);
    }
}

#endif

/**
 * Registers module hooks with Apache.
 */
static void register_hooks(apr_pool_t *mp) {
    static const char *const postconfig_beforeme_list[] = {
        "mod_unique_id.c",
        "mod_ssl.c",
        NULL
    };

    static const char *const postconfig_afterme_list[] = {
        "mod_fcgid.c",
        "mod_cgid.c",
        NULL
    };

    static const char *const postread_beforeme_list[] = {
        "mod_rpaf.c",
        "mod_rpaf-2.0.c",
        "mod_extract_forwarded2.c",
        "mod_remoteip.c",
        "mod_custom_header.c",
        "mod_breach_realip.c",
        "mod_breach_trans.c",
        "mod_unique_id.c",
        NULL
    };

    static const char *const postread_afterme_list[] = {
        "mod_log_forensic.c",
        NULL
    };

    static const char *const transaction_afterme_list[] = {
        "mod_log_config.c",
        NULL
    };

    static const char *const fixups_beforeme_list[] = {
        "mod_env.c",
        NULL
    };

    /* Add the MODSEC_2.x compatibility defines */
    *(char **)apr_array_push(ap_server_config_defines) = apr_pstrdup(mp, "MODSEC_2.5");

    /* Add the MODSEC_a.b define */
    *(char **)apr_array_push(ap_server_config_defines) = apr_psprintf(mp, "MODSEC_%s.%s", MODSEC_VERSION_MAJOR, MODSEC_VERSION_MINOR);

#if (!defined(NO_MODSEC_API))
    /* Export optional functions. */
    APR_REGISTER_OPTIONAL_FN(modsec_register_tfn);
    APR_REGISTER_OPTIONAL_FN(modsec_register_operator);
    APR_REGISTER_OPTIONAL_FN(modsec_register_variable);
    APR_REGISTER_OPTIONAL_FN(modsec_register_reqbody_processor);
#endif

    /* For connection level hook */
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_THREADS, &thread_limit);
    ap_mpm_query(AP_MPMQ_HARD_LIMIT_DAEMONS, &server_limit);

    /* Main hooks */
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_post_config(hook_post_config, postconfig_beforeme_list,
        postconfig_afterme_list, APR_HOOK_REALLY_LAST);
    ap_hook_child_init(hook_child_init, NULL, NULL, APR_HOOK_MIDDLE);

    /* Our own hook to handle RPC transactions (not used at the moment).
     * // ap_hook_handler(hook_handler, NULL, NULL, APR_HOOK_MIDDLE);
     */

    /* Connection processing hooks */
    ap_hook_process_connection(hook_connection_early, NULL, NULL, APR_HOOK_FIRST);

    /* Transaction processing hooks */
    ap_hook_post_read_request(hook_request_early,
        postread_beforeme_list, postread_afterme_list, APR_HOOK_REALLY_FIRST);

    ap_hook_fixups(hook_request_late, fixups_beforeme_list, NULL, APR_HOOK_REALLY_FIRST);

    /* Logging */
    ap_hook_error_log(hook_error_log, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(hook_log_transaction, NULL, transaction_afterme_list, APR_HOOK_MIDDLE);

    /* Filter hooks */
    ap_hook_insert_filter(hook_insert_filter, NULL, NULL, APR_HOOK_FIRST);

    ap_hook_insert_error_filter(hook_insert_error_filter, NULL, NULL, APR_HOOK_FIRST);

    ap_register_input_filter("MODSECURITY_IN", input_filter,
        NULL, AP_FTYPE_CONTENT_SET);

    /* Ensure that the output filter runs before other modules so that
     * we get a request that has a better chance of not being modified:
     *
     * Currently:
     *   mod_expires = -2
     *   mod_cache   = -1
     *   mod_deflate = -1
     *   mod_headers =  0
     */

    ap_register_output_filter("MODSECURITY_OUT", output_filter,
        NULL, AP_FTYPE_CONTENT_SET - 3);

}

/* Defined in apache2_config.c */
extern const command_rec module_directives[];

/* Module entry points */
module AP_MODULE_DECLARE_DATA security2_module = {
    STANDARD20_MODULE_STUFF,
    create_directory_config,
    merge_directory_configs,
    NULL,    /* create_server_config */
    NULL,    /* merge_server_configs */
    module_directives,
    register_hooks
};
