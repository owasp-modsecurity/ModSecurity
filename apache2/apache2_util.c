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
#include "modsecurity.h"
#include "apache2.h"
#include "http_core.h"
#include "util_script.h"

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
        msr_log(msr, 1, "Exec: Unable to allocate %i bytes.", sizeof(*procnew));
        return -1;
    }

    apr_procattr_create(&procattr, r->pool);
    if (procattr == NULL) {
        msr_log(msr, 1, "Exec: Unable to create procattr.");
        return -1;
    }

    apr_procattr_io_set(procattr, APR_NO_PIPE, APR_FULL_BLOCK, APR_NO_PIPE);

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

            msr_log(msr, 4, "Exec: First line from script output: \"%s\"",
                log_escape(r->pool, buf));

            if (output != NULL) *output = apr_pstrdup(r->pool, buf);

            /* Soak up the remaining data. */
            nbytes = 255;
            while(apr_file_read(script_out, buf, &nbytes) == APR_SUCCESS) nbytes = 255;
        } else {
            msr_log(msr, 1, "Exec: Execution failed: %s (%s)", log_escape_nq(r->pool, command),
                get_apr_error(r->pool, rc2));
            return -1;
        }
    }

    apr_proc_wait(procnew, NULL, NULL, APR_WAIT);

    return 1;
}

/**
 * Record the current time and store for later.
 */
void record_time_checkpoint(modsec_rec *msr, int checkpoint_no) {
    char note[100], note_name[100];
    apr_time_t now;

    now = apr_time_now();
    switch(checkpoint_no) {
        case 1 :
            msr->time_checkpoint_1 = now;
            break;
        case 2 :
            msr->time_checkpoint_2 = now;
            break;
        case 3 :
            msr->time_checkpoint_3 = now;
            break;
        default :
            msr_log(msr, 1, "Internal Error: Unknown checkpoint: %i", checkpoint_no);
            return;
            break;
    }

    /* Apache-specific stuff. */
    apr_snprintf(note, 99, "%" APR_TIME_T_FMT, (now - msr->request_time));
    apr_snprintf(note_name, 99, "mod_security-time%i", checkpoint_no);
    apr_table_set(msr->r->notes, note_name, note);

    msr_log(msr, 4, "Time #%i: %s", checkpoint_no, note);
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
 * Internal log helper function. Use msr_log instead.
 */
void internal_log(request_rec *r, directory_config *dcfg, modsec_rec *msr,
    int level, const char *text, va_list ap)
{
    apr_size_t nbytes, nbytes_written;
    apr_file_t *debuglog_fd = NULL;
    int filter_debug_level = 0;
    char str1[1024] = "";
    char str2[1256] = "";

    /* Find the logging FD and look up the logging level in the configuration. */
    if (dcfg != NULL) {
        if ((dcfg->debuglog_fd != NULL)&&(dcfg->debuglog_fd != NOT_SET_P)) {
            debuglog_fd = dcfg->debuglog_fd;
        }
        if (dcfg->debuglog_level != NOT_SET) filter_debug_level = dcfg->debuglog_level;
    }

    /* Return immediately if we don't have where to write
     * or if the log level of the message is higher than
     * wanted in the log.
     */
    if ((level > 3)&&( (debuglog_fd == NULL) || (level > filter_debug_level) )) return;

    /* Construct the message. */
    apr_vsnprintf(str1, sizeof(str1), text, ap);
    apr_snprintf(str2, sizeof(str2), "[%s] [%s/sid#%lx][rid#%lx][%s][%i] %s\n",
        current_logtime(msr->mp), ap_get_server_name(r), (unsigned long)(r->server),
        (unsigned long)r, ((r->uri == NULL) ? "" : log_escape_nq(msr->mp, r->uri)),
        level, str1);

    /* Write to the debug log. */
    if ((debuglog_fd != NULL)&&(level <= filter_debug_level)) {
        nbytes = strlen(str2);
        apr_file_write_full(debuglog_fd, str2, nbytes, &nbytes_written);
    }

    /* Send message levels 1-3 to the Apache error log too. */
    if (level <= 3) {
        char *unique_id = (char *)get_env_var(r, "UNIQUE_ID");
        char *hostname = (char *)r->hostname;

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

        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, r->server,
            "[client %s] ModSecurity: %s%s [uri \"%s\"]%s", r->connection->remote_ip, str1,
            hostname, log_escape(msr->mp, r->unparsed_uri), unique_id);

        /* Add this message to the list. */
        if (msr != NULL) {
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
    internal_log(msr->r, msr->txcfg, msr, level, text, ap);
    va_end(ap);
}


/**
 * Converts an Apache error log message into one line of text.
 */
char *format_error_log_message(apr_pool_t *mp, error_message *em) {
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
        s_line = apr_psprintf(mp, "[line %i] ", em->line);
        if (s_line == NULL) return NULL;
    }

    s_level = apr_psprintf(mp, "[level %i] ", em->level);
    if (s_level == NULL) return NULL;

    if (em->status != 0) {
        s_status = apr_psprintf(mp, "[status %i] ", em->status);
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
