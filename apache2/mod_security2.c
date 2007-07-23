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
#include <limits.h>

#include "http_core.h"
#include "http_request.h"

#include "modsecurity.h"
#include "apache2.h"
#include "msc_logging.h"
#include "msc_util.h"


/* ModSecurity structure */

msc_engine DSOLOCAL *modsecurity = NULL;


/* Global module variables; these are used for the Apache-specific functionality */

char DSOLOCAL *chroot_dir = NULL;

unsigned int DSOLOCAL chroot_completed = 0;

char DSOLOCAL *new_server_signature = NULL;

char DSOLOCAL *real_server_signature = NULL;

char DSOLOCAL *guardianlog_name = NULL;

apr_file_t DSOLOCAL *guardianlog_fd = NULL;

char DSOLOCAL *guardianlog_condition = NULL;


/* -- Miscellaneous functions -- */

/**
 * Intercepts transaction, using the method specified
 * in the structure itself. MUST return an HTTP status code,
 * which will be used to terminate the transaction.
 */
int perform_interception(modsec_rec *msr) {
    msre_actionset *actionset = NULL;
    const char *message = NULL;
    const char *phase_text = "";
    const char *intreq_text = "";
    int is_initial_req = ap_is_initial_req(msr->r);
    int status = DECLINED;
    int log_level = 1;

    /* Check for an initial request */

    if (is_initial_req != 1) {
        if (msr->r->main != NULL) {
            intreq_text = "Sub-Request: ";
        }
        else if (msr->r->prev != NULL) {
            intreq_text = "Internal Redirect: ";
        }
        else {
            intreq_text = "Internal Request: ";
        }
    }

    /* Sanity checks first. */

    if (msr->was_intercepted == 0) {
        msr_log(msr, 1, "Internal Error: Asked to intercept request but was_intercepted is zero");
        return DECLINED;
    }

    if (msr->phase > 4) {
        msr_log(msr, 1, "Internal Error: Asked to intercept request in phase %i.", msr->phase);
        msr->was_intercepted = 0;
        return DECLINED;
    }

    /* OK, we're good to go. */

    actionset = msr->intercept_actionset;
    phase_text = apr_psprintf(msr->mp, " (phase %i)", msr->phase);

    /* By default we log at level 1 but we switch to 4
     * if a nolog action was used or this is not the initial request
     * to hide the message.
     */
    log_level = ((actionset->log != 1) || (is_initial_req != 1)) ? 4 : 1;

    /* Pause the request first (if configured and the initial request). */
    if (actionset->intercept_pause && (is_initial_req == 1)) {
        msr_log(msr, (log_level > 3 ? log_level : log_level + 1), "Pausing transaction for "
            "%i msec.", actionset->intercept_pause);
        /* apr_sleep accepts microseconds */
        apr_sleep((apr_interval_time_t)(actionset->intercept_pause * 1000));
    }

    /* Determine how to respond and prepare the log message. */
    switch(actionset->intercept_action) {
        case ACTION_DENY :
            if (actionset->intercept_status != 0) {
                status = actionset->intercept_status;
                message = apr_psprintf(msr->mp, "%sAccess denied with code %i%s.",
                    intreq_text, status, phase_text);
            } else {
                log_level = 1;
                status = HTTP_INTERNAL_SERVER_ERROR;
                message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                    "(Internal Error: Invalid status code requested %i).",
                    intreq_text, phase_text, actionset->intercept_status);
            }
            break;

        case ACTION_PROXY :
            if (msr->phase < 3) {
                if (ap_find_linked_module("mod_proxy.c") == NULL) {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                        "(Configuration Error: Proxy action to %s requested but mod_proxy not found).",
                        intreq_text, phase_text,
                        log_escape_nq(msr->mp, actionset->intercept_uri));
                } else {
                    msr->r->filename = apr_psprintf(msr->mp, "proxy:%s", actionset->intercept_uri);
                    msr->r->proxyreq = PROXYREQ_REVERSE;
                    msr->r->handler = "proxy-server";
                    status = OK;
                    message = apr_psprintf(msr->mp, "%sAccess denied using proxy to%s %s.",
                        intreq_text, phase_text,
                        log_escape_nq(msr->mp, actionset->intercept_uri));
                }
            } else {
                log_level = 1;
                status = HTTP_INTERNAL_SERVER_ERROR;
                message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                    "(Configuration Error: Proxy action requested but it does not work in output phases).",
                    intreq_text, phase_text);
            }
            break;

        case ACTION_DROP :
            /* ENH This does not seem to work on Windows. Is there a
             *     better way to drop a connection anyway?
             */
            #ifndef WIN32
            {
                extern module core_module;
                apr_socket_t *csd = ap_get_module_config(msr->r->connection->conn_config,
                    &core_module);
                
                if (csd) {
                    if (apr_socket_close(csd) == APR_SUCCESS) {
                        status = HTTP_FORBIDDEN;
                        message = apr_psprintf(msr->mp, "%sAccess denied with connection close%s.",
                            intreq_text, phase_text);
                    } else {
                        log_level = 1;
                        status = HTTP_INTERNAL_SERVER_ERROR;
                        message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                            "(Error: Connection drop requested but failed to close the "
                            " socket).",
                            intreq_text, phase_text);
                    }
                } else {
                    log_level = 1;
                    status = HTTP_INTERNAL_SERVER_ERROR;
                    message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                        "(Error: Connection drop requested but socket not found.",
                        intreq_text, phase_text);
                }
            }
            #else
            log_level = 1;
            status = HTTP_INTERNAL_SERVER_ERROR;
            message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                "(Error: Connection drop not implemented on this platform).",
                intreq_text, phase_text);
            #endif
            break;

        case ACTION_REDIRECT :
            apr_table_setn(msr->r->headers_out, "Location", actionset->intercept_uri);
            if ((actionset->intercept_status == 301)||(actionset->intercept_status == 302)
                ||(actionset->intercept_status == 303)||(actionset->intercept_status == 307))
            {
                status = actionset->intercept_status;
            } else {
                status = HTTP_MOVED_TEMPORARILY;
            }
            message = apr_psprintf(msr->mp, "%sAccess denied with redirection to %s using "
                "status %i%s.",
                intreq_text,
                log_escape_nq(msr->mp, actionset->intercept_uri), status,
                phase_text);
            break;

        case ACTION_ALLOW :
            status = DECLINED;
            message = apr_psprintf(msr->mp, "%sAccess allowed%s.", intreq_text, phase_text);
            msr->was_intercepted = 0;
            break;

        default :
            log_level = 1;
            status = HTTP_INTERNAL_SERVER_ERROR;
            message = apr_psprintf(msr->mp, "%sAccess denied with code 500%s "
                "(Internal Error: invalid interception action %i).",
                intreq_text, phase_text, actionset->intercept_action);
            break;
    }

    /* Log the message now. */
    msc_alert(msr, log_level, actionset, message, msr->intercept_message);

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
    msr->error_messages = apr_array_make(msr->mp, 5, sizeof(error_message *));
    msr->alerts = apr_array_make(msr->mp, 5, sizeof(char *));

    msr->server_software = real_server_signature;
    msr->local_addr = r->connection->local_ip;
    msr->local_port = r->connection->local_addr->port;

    msr->remote_addr = r->connection->remote_ip;
    msr->remote_port = r->connection->remote_addr->port;

    msr->request_line = r->the_request;
    msr->request_uri = r->uri;
    msr->request_method = r->method;
    msr->query_string = r->args;
    msr->request_protocol = r->protocol;
    msr->request_headers = apr_table_copy(msr->mp, r->headers_in);
    msr->hostname = ap_get_server_name(r);

    /* Invoke the engine to continue with initialisation */
    if (modsecurity_tx_init(msr) < 0) return NULL;

    store_tx_context(msr, r);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Transaction context created (dcfg %x).", msr->dcfg1);
    }

    msr->msc_rule_mptmp = NULL;

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

    server_version = (char *)ap_get_server_version();
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
 * Pre-configuration initialisation hook.
 */
static int hook_pre_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp) {
    /* Initialise ModSecurity engine */
    modsecurity = modsecurity_create(mp, MODSEC_ONLINE);
    if (modsecurity == NULL) {
        /* ENH Since s not available, how do we log from here? stderr?
         * ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
         *    "ModSecurity: Failed to initialise engine.");
         */
        return HTTP_INTERNAL_SERVER_ERROR;
    }    

    return OK;
}

/**
 * Main (post-configuration) module initialisation.
 */
static int hook_post_config(apr_pool_t *mp, apr_pool_t *mp_log, apr_pool_t *mp_temp, server_rec *s) {
    void *init_flag = NULL;
    int first_time = 0;

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
    real_server_signature = apr_pstrdup(mp, ap_get_server_version());

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
                "ModSecurity: chroot checkpoint #2 (pid=%i ppid=%i)", getpid(), getppid());

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

            chroot_completed = 1;

            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                "ModSecurity: chroot successful, path=%s", chroot_dir);
        } else {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                "ModSecurity: chroot checkpoint #1 (pid=%i ppid=%i)", getpid(), getppid());
        }
    }
    #endif

    /* Schedule main cleanup for later, when the main pool is destroyed. */
    apr_pool_cleanup_register(mp, (void *)s, module_cleanup, apr_pool_cleanup_null);

    /* Log our presence to the error log. */
    if (first_time) {
        if (new_server_signature != NULL) {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                "ModSecurity for Apache %s configured - %s", MODULE_RELEASE, real_server_signature);
        }
        else {
            ap_log_error(APLOG_MARK, APLOG_NOTICE | APLOG_NOERRNO, 0, s,
                "ModSecurity for Apache %s configured", MODULE_RELEASE);
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
 * Apache receives the request headers.
 */
static int hook_request_early(request_rec *r) {
    modsec_rec *msr = NULL;
    int rc;

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

    /* Are we allowed to continue? */
    if (msr->txcfg->is_enabled == MODSEC_DISABLED) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Processing disabled, skipping (hook request_early).");
        }
        return DECLINED;
    }

    /* Check request body limit (should only trigger on non-chunked requests). */
    if (msr->request_content_length > msr->txcfg->reqbody_limit) {
        msr_log(msr, 1, "Request body is larger than the "
                     "configured limit (%lu).", msr->txcfg->reqbody_limit);
        return HTTP_REQUEST_ENTITY_TOO_LARGE;
    }

    /* Process phase REQUEST_HEADERS */
    rc = DECLINED;
    if (modsecurity_process_phase(msr, PHASE_REQUEST_HEADERS) > 0) {
        rc = perform_interception(msr);
    }

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
        if (msr->was_intercepted) {
            msr_log(msr, 4, "Phase REQUEST_BODY request already intercepted. Intercepting additional request.");
            return perform_interception(msr);
        }
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Phase REQUEST_BODY already complete, skipping.");
        }
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

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Second phase starting (dcfg %x).", msr->dcfg2);
    }

    /* Figure out whether or not to extract multipart files. */
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
                msr_log(msr, 1, "%s", my_error_msg);
                return HTTP_INTERNAL_SERVER_ERROR;
                break;
            case -4 : /* Timeout. */
                r->connection->keepalive = AP_CONN_CLOSE;
                return HTTP_REQUEST_TIME_OUT;
                break;
            case -5 : /* Request body limit reached. */
                r->connection->keepalive = AP_CONN_CLOSE;
                return HTTP_REQUEST_ENTITY_TOO_LARGE;
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
     */
    // TODO We still need to keep a copy of the original headers
    //      to log in the audit log.
    msr->request_headers = apr_table_copy(msr->mp, r->headers_in);

    /* Process phase REQUEST_BODY */
    record_time_checkpoint(msr, 1);

    rc = DECLINED;
    if (modsecurity_process_phase(msr, PHASE_REQUEST_BODY) > 0) {
        rc = perform_interception(msr);
    }

    record_time_checkpoint(msr, 2);

    return rc;
}

/**
 * Invoked every time Apache has something to write to the error log.
 */
static void hook_error_log(const char *file, int line, int level, apr_status_t status,
    const server_rec *s, const request_rec *r, apr_pool_t *mp, const char *fmt)
{
    modsec_rec *msr = NULL;
    error_message *em = NULL;

    if (r == NULL) return;
    msr = retrieve_tx_context((request_rec *)r);

    /* Create a context for requests we never had the chance to process */ 
    if ((msr == NULL)
        && ((level & APLOG_LEVELMASK) < APLOG_DEBUG)
        && apr_table_get(r->subprocess_env, "UNIQUE_ID"))
    {
        msr = create_tx_context((request_rec *)r); 
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
    em = (error_message *)apr_pcalloc(msr->mp, sizeof(error_message));
    if (em == NULL) return;

    if (file != NULL) em->file = apr_pstrdup(msr->mp, file);
    em->line = line;
    em->level = level;
    em->status = status;
    if (fmt != NULL) em->message = apr_pstrdup(msr->mp, fmt);

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

    *(const error_message **)apr_array_push(msr->error_messages) = em;
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

    str2 = apr_psprintf(msr->mp, "%" APR_TIME_T_FMT " %" APR_TIME_T_FMT " \"%s\" %i",
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
        msr_log(msr, 1, "Audit Log: Atomic PIPE write buffer too small: %i", PIPE_BUF);
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

    /* Only proceed to add the filter if the engine is enabled. */
    if (msr->txcfg->is_enabled == 0) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Processing disabled, skipping.");
        }

        return;
    }

    /* Add the input filter, but only if we need it to run. */
    if (msr->if_status == IF_STATUS_WANTS_TO_RUN) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Adding input forwarding filter (r %x).", r);
        }

        ap_add_input_filter("MODSECURITY_IN", msr, r, r->connection);
    }

    /* We always add the output filter because that's where we need to
     * initiate our 3rd and 4th processing phases from. The filter is
     * smart enough not to buffer the data if it is not supposed to.
     */
    if (msr->of_status != OF_STATUS_COMPLETE) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_filter: Adding output filter (r %x).", r);
        }

        ap_add_output_filter("MODSECURITY_OUT", msr, r, r->connection);
    }
}

#if 0
/**
 * Invoked whenever Apache starts processing an error. A chance
 * to insert ourselves into the output filter chain.
 */
static void hook_insert_error_filter(request_rec *r) {
    modsec_rec *msr = NULL;

    /* Find the transaction context and make sure we are
     * supposed to proceed.
     */

    /* TODO Insert filter but make a note that it's the error
     *      response the filter would be receiving.
     */

    msr = retrieve_tx_context(r);
    if (msr == NULL) return;

    if (msr->txcfg->is_enabled == 0) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Processing disabled, skipping.");
        }
        return;
    }
    
    if (msr->of_status != OF_STATUS_COMPLETE) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Adding output filter (r %x).", r);
        }
        ap_add_output_filter("MODSECURITY_OUT", msr, r, r->connection);
    } else {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Hook insert_error_filter: Output buffering already complete.");
        }
    }
}
#endif

/**
 * This function is exported for other Apache modules to
 * register new transformation functions.
 */
static void modsec_register_tfn(const char *name, void *fn) {
    if (modsecurity != NULL) {
        msre_engine_tfn_register(modsecurity->msre, name, fn);
    }
}

/**
 * This function is exported for other Apache modules to
 * register new operators.
 */
static void modsec_register_operator(const char *name, void *fn_init, void *fn_exec) {
    if (modsecurity != NULL) {
        msre_engine_op_register(modsecurity->msre, name, fn_init, fn_exec);
    }
}

/**
 * Registers module hooks with Apache.
 */
static void register_hooks(apr_pool_t *mp) {
    static const char *postconfig_beforeme_list[] = {
        "mod_unique_id.c",
        "mod_ssl.c",
        NULL
    };
    static const char *postconfig_afterme_list[] = {
        "mod_fcgid.c",
        "mod_cgid.c",
        NULL
    };
    static const char *postread_beforeme_list[] = {
        "mod_unique_id.c",
        NULL
    };
    static const char *postread_afterme_list[] = {
        "mod_log_forensic.c",
        NULL
    };

    /* Export optional functions. */
    APR_REGISTER_OPTIONAL_FN(modsec_register_tfn);
    APR_REGISTER_OPTIONAL_FN(modsec_register_operator);

    /* Main hooks */
    ap_hook_pre_config(hook_pre_config, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_post_config(hook_post_config, postconfig_beforeme_list,
        postconfig_afterme_list, APR_HOOK_REALLY_LAST);
    ap_hook_child_init(hook_child_init, NULL, NULL, APR_HOOK_MIDDLE);

    /* Our own hook to handle RPC transactions (not used at the moment).
     * // ap_hook_handler(hook_handler, NULL, NULL, APR_HOOK_MIDDLE);
     */
    
    /* Transaction processing hooks */
    ap_hook_post_read_request(hook_request_early,
        postread_beforeme_list, postread_afterme_list, APR_HOOK_REALLY_FIRST);

    ap_hook_fixups(hook_request_late, NULL, NULL, APR_HOOK_REALLY_FIRST);

    /* Logging */
    ap_hook_error_log(hook_error_log, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(hook_log_transaction, NULL, NULL, APR_HOOK_MIDDLE);

    /* Filter hooks */
    ap_hook_insert_filter(hook_insert_filter, NULL, NULL, APR_HOOK_FIRST);
    /* ap_hook_insert_error_filter(hook_insert_error_filter, NULL, NULL, APR_HOOK_FIRST); */

    ap_register_input_filter("MODSECURITY_IN", input_filter,
        NULL, AP_FTYPE_CONTENT_SET);
    ap_register_output_filter("MODSECURITY_OUT", output_filter,
        NULL, AP_FTYPE_CONTENT_SET);
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
