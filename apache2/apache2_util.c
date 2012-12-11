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

#include "modsecurity.h"
#include "apache2.h"
#include "http_core.h"
#include "util_script.h"

/**
 * Sends a brigade with an error bucket down the filter chain.
 */
apr_status_t send_error_bucket(modsec_rec *msr, ap_filter_t *f, int status) {
    apr_bucket_brigade *brigade = NULL;
    apr_bucket *bucket = NULL;

    /* Set the status line explicitly for the error document */
    f->r->status_line = ap_get_status_line(status);

    brigade = apr_brigade_create(f->r->pool, f->r->connection->bucket_alloc);
    if (brigade == NULL) return APR_EGENERAL;

    bucket = ap_bucket_error_create(status, NULL, f->r->pool, f->r->connection->bucket_alloc);
    if (bucket == NULL) return APR_EGENERAL;

    APR_BRIGADE_INSERT_TAIL(brigade, bucket);

    bucket = apr_bucket_eos_create(f->r->connection->bucket_alloc);
    if (bucket == NULL) return APR_EGENERAL;

    APR_BRIGADE_INSERT_TAIL(brigade, bucket);

    ap_pass_brigade(f->next, brigade);

    /* NOTE:
     * It may not matter what we do from the filter as it may be too
     * late to even generate an error (already sent to client).  Nick Kew
     * recommends to return APR_EGENERAL in hopes that the handler in control
     * will notice and do The Right Thing.  So, that is what we do now.
     */

    return APR_EGENERAL;
}

/**
 * Execute system command. First line of the output will be returned in
 * the "output" parameter.
 */
int apache2_exec(modsec_rec *msr, const char *command, const char **argv, char **output) {
    apr_procattr_t *procattr = NULL;
    apr_proc_t *procnew = NULL;
    apr_status_t rc = APR_SUCCESS;
    const char *const *env = NULL;
    apr_file_t *script_out = NULL;
    request_rec *r = msr->r;

    if (argv == NULL) {
        argv = apr_pcalloc(r->pool, 3 * sizeof(char *));
        argv[0] = command;
        argv[1] = NULL;
    }

    ap_add_cgi_vars(r);
    ap_add_common_vars(r);

    /* PHP hack, getting around its silly security checks. */
    apr_table_add(r->subprocess_env, "PATH_TRANSLATED", command);
    apr_table_add(r->subprocess_env, "REDIRECT_STATUS", "302");

    env = (const char * const *)ap_create_environment(r->pool, r->subprocess_env);
    if (env == NULL) {
        msr_log(msr, 1, "Exec: Unable to create environment.");
        return -1;
    }

    procnew = apr_pcalloc(r->pool, sizeof(*procnew));
    if (procnew == NULL) {
        msr_log(msr, 1, "Exec: Unable to allocate %lu bytes.", (unsigned long)sizeof(*procnew));
        return -1;
    }

    apr_procattr_create(&procattr, r->pool);
    if (procattr == NULL) {
        msr_log(msr, 1, "Exec: Unable to create procattr.");
        return -1;
    }

    apr_procattr_io_set(procattr, APR_NO_PIPE, APR_FULL_BLOCK, APR_NO_PIPE);
    apr_procattr_cmdtype_set(procattr, APR_SHELLCMD);

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Exec: %s", log_escape_nq(r->pool, command));
    }

    rc = apr_proc_create(procnew, command, argv, env, procattr, r->pool);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Exec: Execution failed: %s (%s)", log_escape_nq(r->pool, command),
            get_apr_error(r->pool, rc));
        return -1;
    }

    apr_pool_note_subprocess(r->pool, procnew, APR_KILL_AFTER_TIMEOUT);

    script_out = procnew->out;
    if (!script_out) {
        msr_log(msr, 1, "Exec: Failed to get script output pipe.");
        return -1;
    }

    apr_file_pipe_timeout_set(script_out, r->server->timeout);

    /* Now read from the pipe. */
    {
        char buf[260] = "";
        char *p = buf;
        apr_size_t nbytes = 255;
        apr_status_t rc2;

        rc2 = apr_file_read(script_out, buf, &nbytes);
        if (rc2 == APR_SUCCESS) {
            buf[nbytes] = 0;

            /* if there is more than one line ignore them */
            while(*p != 0) {
                if (*p == 0x0a) *p = 0;
                p++;
            }

            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Exec: First line from script output: \"%s\"",
                    log_escape(r->pool, buf));
            }

            if (output != NULL) *output = apr_pstrdup(r->pool, buf);

            /* Soak up the remaining data. */
            nbytes = 255;
            while(apr_file_read(script_out, buf, &nbytes) == APR_SUCCESS) nbytes = 255;
        } else {
            msr_log(msr, 1, "Exec: Execution failed while reading output: %s (%s)",
                log_escape_nq(r->pool, command),
                get_apr_error(r->pool, rc2));
            return -1;
        }
    }

    apr_proc_wait(procnew, NULL, NULL, APR_WAIT);

    return 1;
}

/**
 * Returns a new string that contains the error
 * message for the given return code.
 */
char *get_apr_error(apr_pool_t *p, apr_status_t rc) {
    char *text = apr_pcalloc(p, 201);
    if (text == NULL) return NULL;
    apr_strerror(rc, text, 200);
    return text;
}

/**
 * Retrieve named environment variable.
 */
char *get_env_var(request_rec *r, char *name) {
    char *result = (char *)apr_table_get(r->notes, name);

    if (result == NULL) {
        result = (char *)apr_table_get(r->subprocess_env, name);
    }

    if (result == NULL) {
        result = getenv(name);
    }

    return result;
}

/**
 * Extended internal log helper function. Use msr_log instead. If fixup is
 * true, the message will be stripped of any trailing newline and any
 * required bytes will be escaped.
 */
static void internal_log_ex(request_rec *r, directory_config *dcfg, modsec_rec *msr,
    int level, int fixup, const char *text, va_list ap)
{
    apr_size_t nbytes, nbytes_written;
    apr_file_t *debuglog_fd = NULL;
    int filter_debug_level = 0;
    char str1[1024] = "";
    char str2[1256] = "";

    /* Find the logging FD and determine the logging level from configuration. */
    if (dcfg != NULL) {
        if ((dcfg->debuglog_fd != NULL)&&(dcfg->debuglog_fd != NOT_SET_P)) {
            debuglog_fd = dcfg->debuglog_fd;
        }

        if (dcfg->debuglog_level != NOT_SET) {
            filter_debug_level = dcfg->debuglog_level;
        }
    }

    /* Return immediately if we don't have where to write
     * or if the log level of the message is higher than
     * wanted in the log.
     */
    if ((level > 3)&&( (debuglog_fd == NULL) || (level > filter_debug_level) )) return;

    /* Construct the message. */
    apr_vsnprintf(str1, sizeof(str1), text, ap);
    if (fixup) {
        int len = strlen(str1);

        /* Strip line ending. */
        if (len && str1[len - 1] == '\n') {
            str1[len - 1] = '\0';
        }
        if (len > 1 && str1[len - 2] == '\r') {
            str1[len - 2] = '\0';
        }
    }

    /* Construct the log entry. */
    apr_snprintf(str2, sizeof(str2), 
        "[%s] [%s/sid#%pp][rid#%pp][%s][%d] %s\n",
        current_logtime(msr->mp), ap_get_server_name(r), (r->server),
        r, ((r->uri == NULL) ? "" : log_escape_nq(msr->mp, r->uri)),
        level, (fixup ? log_escape_nq(msr->mp, str1) : str1));

    /* Write to the debug log. */
    if ((debuglog_fd != NULL)&&(level <= filter_debug_level)) {
        nbytes = strlen(str2);
        apr_file_write_full(debuglog_fd, str2, nbytes, &nbytes_written);
    }

    /* Send message levels 1-3 to the Apache error log and 
     * add it to the message list in the audit log. */
    if (level <= 3) {
        char *unique_id = (char *)get_env_var(r, "UNIQUE_ID");
        char *hostname = (char *)msr->hostname;

        if (unique_id != NULL) {
            unique_id = apr_psprintf(msr->mp, " [unique_id \"%s\"]",
                log_escape(msr->mp, unique_id));
        }
        else unique_id = "";

        if (hostname != NULL) {
            hostname = apr_psprintf(msr->mp, " [hostname \"%s\"]",
                log_escape(msr->mp, hostname));
        }
        else hostname = "";

#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER > 2
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, r->server,
            "[client %s] ModSecurity: %s%s [uri \"%s\"]%s", r->useragent_ip ? r->useragent_ip : r->connection->client_ip, str1,
            hostname, log_escape(msr->mp, r->uri), unique_id);
#else
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, r->server,
            "[client %s] ModSecurity: %s%s [uri \"%s\"]%s", r->connection->remote_ip, str1,
            hostname, log_escape(msr->mp, r->uri), unique_id);
#endif

        /* Add this message to the list. */
        if (msr != NULL) {
            /* Force relevency if this is an alert */
            msr->is_relevant++;

            *(const char **)apr_array_push(msr->alerts) = apr_pstrdup(msr->mp, str1);
        }
    }

    return;
}

/**
 * Logs one message at the given level to the debug log (and to the
 * Apache error log if the message is important enough.
 */
void msr_log(modsec_rec *msr, int level, const char *text, ...) {
    va_list ap;

    va_start(ap, text);
    internal_log_ex(msr->r, msr->txcfg, msr, level, 0, text, ap);
    va_end(ap);
}


/**
 * Logs one message at level 3 to the debug log and to the
 * Apache error log. This is intended for error callbacks.
 */
void msr_log_error(modsec_rec *msr, const char *text, ...) {
    va_list ap;

    va_start(ap, text);
    internal_log_ex(msr->r, msr->txcfg, msr, 3, 1, text, ap);
    va_end(ap);
}

/**
 * Logs one message at level 4 to the debug log and to the
 * Apache error log. This is intended for warning callbacks.
 *
 * The 'text' will first be escaped.
 */
void msr_log_warn(modsec_rec *msr, const char *text, ...) {
    va_list ap;

    va_start(ap, text);
    internal_log_ex(msr->r, msr->txcfg, msr, 4, 1, text, ap);
    va_end(ap);
}


/**
 * Converts an Apache error log message into one line of text.
 */
char *format_error_log_message(apr_pool_t *mp, error_message_t *em) {
    char *s_file = "", *s_line = "", *s_level = "";
    char *s_status = "", *s_message = "";
    char *msg = NULL;

    if (em == NULL) return NULL;

    if (em->file != NULL) {
        s_file = apr_psprintf(mp, "[file \"%s\"] ",
            log_escape(mp, (char *)em->file));
        if (s_file == NULL) return NULL;
    }

    if (em->line > 0) {
        s_line = apr_psprintf(mp, "[line %d] ", em->line);
        if (s_line == NULL) return NULL;
    }

    s_level = apr_psprintf(mp, "[level %d] ", em->level);
    if (s_level == NULL) return NULL;

    if (em->status != 0) {
        s_status = apr_psprintf(mp, "[status %d] ", em->status);
        if (s_status == NULL) return NULL;
    }

    if (em->message != NULL) {
        s_message = log_escape_nq(mp, em->message);
        if (s_message == NULL) return NULL;
    }

    msg = apr_psprintf(mp, "%s%s%s%s%s", s_file, s_line, s_level, s_status, s_message);
    if (msg == NULL) return NULL;

    return msg;
}

/**
 * Determines the reponse protocol Apache will use (or has used)
 * to respond to the given request.
 */
const char *get_response_protocol(request_rec *r) {
    int proto_num = r->proto_num;

    if (r->assbackwards) {
        return NULL;
    }

    if (proto_num > HTTP_VERSION(1,0)
        && apr_table_get(r->subprocess_env, "downgrade-1.0"))
    {
        proto_num = HTTP_VERSION(1,0);
    }

    if (proto_num == HTTP_VERSION(1,0)
        && apr_table_get(r->subprocess_env, "force-response-1.0"))
    {
        return "HTTP/1.0";
    }

    return AP_SERVER_PROTOCOL;
}
