/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
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
#include <sys/stat.h>

#include "re.h"
#include "msc_logging.h"
#include "httpd.h"
#include "apr_strings.h"
#include "apr_global_mutex.h"
#include "msc_util.h"

#include "apr_version.h"
#include <libxml/xmlversion.h>

#ifdef WITH_YAJL
#include <yajl/yajl_gen.h>
#include "msc_logging_json.h"
#endif

/**
 * Write the supplied data to the audit log (if the FD is ready), update
 * the size counters, update the hash context.
 */
static int sec_auditlog_write(modsec_rec *msr, const char *data, unsigned int len) {
    apr_size_t nbytes_written, nbytes = len;
    apr_status_t rc;

    /* Do nothing if there's no data. */
    if (data == NULL) return -1;

    /* Update size counters and the hash calculation. We always do this,
     * even in cases where write fails. That will make it easier to detect
     * problems with partial writes.
     */
    msr->new_auditlog_size += len;
    apr_md5_update(&msr->new_auditlog_md5ctx, data, len);

    /* Do not write if we do not have a file descriptor. */
    if (msr->new_auditlog_fd == NULL) return -1;

    /* Write data to file. */
    rc = apr_file_write_full(msr->new_auditlog_fd, data, nbytes, &nbytes_written);
    if (rc != APR_SUCCESS) {
        char errstr[1024];

        msr_log(msr, 1, "Audit log: Failed writing (requested %" APR_SIZE_T_FMT
            " bytes, written %" APR_SIZE_T_FMT "): %s", nbytes, nbytes_written,
            apr_strerror(rc, errstr, sizeof(errstr)));

        /* Concurrent log format: don't leak file handle. */
        if (msr->txcfg->auditlog_type == AUDITLOG_CONCURRENT) {
            apr_file_close(msr->new_auditlog_fd);
        }

        /* Set to NULL to prevent more than one error message on
         * out-of-disk-space events and to prevent further attempts
         * to write to the same file in this request.
         *
         * Serial log format: Note that, as we opened the file through the
         * pool mechanism of the APR, we do not need to close the file
         * here. It will be closed automatically at the end of the request.
         */
        msr->new_auditlog_fd = NULL;

        return -1;
    }

    return 1;
}

/**
 * Constructs a log line in vcombined log format trying to truncate
 * some of the fields to make the log line shorter than _limit bytes.
 */
char *construct_log_vcombinedus_limited(modsec_rec *msr, int _limit, int *was_limited) {
    char *hostname;
    char *local_user, *remote_user;
    char *referer, *user_agent, *uniqueid;
    const char *sessionid;
    char *the_request, *bytes_sent;
    int limit = _limit;

    /* hostname */
    hostname = (msr->hostname == NULL ? "-" : log_escape_nq(msr->mp, msr->hostname));

    /* remote log name */
    if (msr->remote_user == NULL) remote_user = "-";
    else remote_user = log_escape_nq(msr->mp, msr->remote_user);

    /* authenticated user */
    if (msr->local_user == NULL) local_user = "-";
    else local_user = log_escape_nq(msr->mp, msr->local_user);

    /* unique id */
    if (msr->txid == NULL) uniqueid = "-";
    else uniqueid = log_escape(msr->mp, msr->txid);

    /* referer */
    referer = "-";
    /*
    referer = (char *)apr_table_get(msr->request_headers, "Referer");
    if (referer == NULL) referer = "-";
    else referer = log_escape(msr->mp, referer);
    */

    /* user agent */
    user_agent = "-";
    /*
    user_agent = (char *)apr_table_get(msr->request_headers, "User-Agent");
    if (user_agent == NULL) user_agent = "-";
    else user_agent = log_escape(msr->mp, user_agent);
    */

    /* sessionid */
    sessionid = (msr->sessionid == NULL) ? "-" : log_escape(msr->mp, msr->sessionid);

    the_request = (msr->request_line == NULL) ? "" : log_escape(msr->mp, msr->request_line);

    bytes_sent = apr_psprintf(msr->mp, "%" APR_OFF_T_FMT, msr->bytes_sent);

    /* first take away the size of the
     * information we must log
     */
    limit -= 22;                                 /* spaces and double quotes */
    limit -= strlen(hostname);                   /* server name or IP */
    limit -= strlen(msr->remote_addr);           /* remote IP */
    limit -= 28;                                 /* current_logtime */
    limit -= 3;                                  /* status */
    limit -= strlen(bytes_sent);                 /* bytes sent */
    limit -= strlen(uniqueid);                   /* unique id */
    limit -= strlen(sessionid);                  /* session id */

    if (limit <= 0) {
        msr_log(msr, 1, "GuardianLog: Atomic pipe write size too small: %d", PIPE_BUF);
        return NULL;
    }

    /* we hope to be able to squeeze everything in */
    if (limit < (int)(strlen(remote_user) + strlen(local_user) + strlen(referer)
        + strlen(user_agent) + strlen(the_request)))
    {
        /* Boo hoo hoo, there's not enough space available. */
        *was_limited = 1;

        /* Let's see if we can reduce the size of something. This
         * is a very crude approach but it seems to work for our
         * needs.
         */
        if (strlen(remote_user) > 32) {
            msr_log(msr, 9, "GuardianLog: Reduced remote_user to 32.");
            remote_user[32] = '\0';
        }
        limit -= strlen(remote_user);

        if (strlen(local_user) > 32) {
            msr_log(msr, 9, "GuardianLog: Reduced local_user to 32.");
            local_user[32] = '\0';
        }
        limit -= strlen(local_user);

        if (strlen(referer) > 64) {
            msr_log(msr, 9, "GuardianLog: Reduced referer to 64.");
            referer[64] = '\0';
        }
        limit -= strlen(referer);

        if (strlen(user_agent) > 64) {
            msr_log(msr, 9, "GuardianLog: Reduced user_agent to 64.");
            user_agent[64] = '\0';
        }
        limit -= strlen(user_agent);

        if (limit <= 0) {
            msr_log(msr, 1, "GuardianLog: Atomic pipe write size too small: %d.", PIPE_BUF);
            return NULL;
        }

        /* use what's left for the request line */
        if ((int)strlen(the_request) > limit) {
            the_request[limit] = '\0';
            msr_log(msr, 9, "GuardianLog: Reduced the_request to %d bytes.", limit);
        }
    } else {
        /* Yay! We have enough space! */
        *was_limited = 0;
    }

    return apr_psprintf(msr->mp, "%s %s %s %s [%s] \"%s\" %u %s \"%s\" \"%s\" %s \"%s\"",
        hostname, msr->remote_addr, remote_user,
        local_user, current_logtime(msr->mp), the_request,
        msr->response_status, bytes_sent, referer, user_agent,
        uniqueid, sessionid
    );
}

/**
 * Checks if the provided string is a valid audit log parts specification.
 */
int is_valid_parts_specification(char *p) {
    char c, *t = p;

    while((c = *(t++)) != '\0') {
        if ((c != AUDITLOG_PART_ENDMARKER)&&((c < AUDITLOG_PART_FIRST)||(c > AUDITLOG_PART_LAST))) {
            return 0;
        }
    }

    return 1;
}

/**
 * Constructs a filename that will be used to store an
 * audit log entry.
 */
static char *construct_auditlog_filename(apr_pool_t *mp, const char *uniqueid) {
    apr_time_exp_t t;
    char tstr[300];
    apr_size_t len;

    apr_time_exp_lt(&t, apr_time_now());

    apr_strftime(tstr, &len, 299, "/%Y%m%d/%Y%m%d-%H%M/%Y%m%d-%H%M%S", &t);
    return apr_psprintf(mp, "%s-%s", tstr, uniqueid);
}

/**
 * Creates a random 8-character string that
 * consists of hexadecimal numbers, to be used
 * as an audit log boundary.
 */
static char *create_auditlog_boundary(request_rec *r) {
#ifdef LINUX_S390
    int data = swap_int32(rand());
#else
    unsigned long data = rand();
#endif
    /* Do note that I tried using apr_generate_random_bytes but it turned
     * out to be terribly slow for some reason. Needs further investigation.
     */
    return bytes2hex(r->pool, (void *)&data, 4);
}

/**
 * Sanitises the request line by removing the parameters
 * that have been marked as sensitive.
 */
static void sanitize_request_line(modsec_rec *msr) {
    const apr_array_header_t *tarr = NULL;
    const apr_table_entry_t *telts = NULL;
    const apr_array_header_t *tarr_pattern = NULL;
    const apr_table_entry_t *telts_pattern = NULL;
    msc_parm *mparm = NULL;
    int i, k;
    char *qspos;
    char *buf = NULL;
    int sanitized_partial = 0;
    int sanitize_matched = 0;

    /* Locate the query string. */
    qspos = strstr(msr->request_line, "?");
    if (qspos == NULL) return;
    qspos++;

    /* Loop through the list of sensitive parameters. */
    tarr = apr_table_elts(msr->arguments_to_sanitize);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msc_arg *arg = (msc_arg *)telts[i].val;
        /* Only look at the parameters that appeared in the query string. */
        if (strcmp(arg->origin, "QUERY_STRING") == 0) {
            char *pat = NULL;
            char *p;
            int j, arg_min, arg_max;

            /* Go to the beginning of the parameter. */
            p = qspos;
            j = arg->value_origin_offset;
            while((*p != '\0')&&(j--)) p++;
            if (*p == '\0') {
                msr_log(msr, 1, "Unable to sanitize variable \"%s\" at offset %u of QUERY_STRING"
                        "because the request line is too short.",
                        log_escape_ex(msr->mp, arg->name, arg->name_len),
                        arg->value_origin_offset);
                continue;
            }

            tarr_pattern = apr_table_elts(msr->pattern_to_sanitize);
            telts_pattern = (const apr_table_entry_t*)tarr_pattern->elts;

            sanitized_partial = 0;
            sanitize_matched = 0;
            buf = apr_psprintf(msr->mp, "%s",p);
            for ( k = 0; k < tarr_pattern->nelts; k++)  {
                if(strncmp(telts_pattern[k].key,arg->name,strlen(arg->name)) ==0 ) {
                    mparm = (msc_parm *)telts_pattern[k].val;
                    pat = strstr(buf,mparm->value);
                    if(mparm->pad_1 == -1)
                        sanitize_matched = 1;

                    if (pat != NULL) {
                        j = strlen(mparm->value);
                        arg_min = j;
                        arg_max = 1;
                        while((*pat != '\0')&&(j--)) {
                            if(arg_max > mparm->pad_2)  {
                                int off = (strlen(mparm->value) - arg_max);
                                int pos = (mparm->pad_1-1);
                                if(off > pos)    {
                                    *pat = '*';
                                }
                            }
                            arg_max++;
                            arg_min--;
                            pat++;
                        }
                    }
                    sanitized_partial = 1;
                }
            }

            if(sanitized_partial == 1 && sanitize_matched == 0)  {
                while(*buf != '\0') {
                    *p++ = *buf++;
                }
                continue;
            } else {
                /* Write over the value. */
                j = arg->value_origin_len;
                while((*p != '\0')&&(j--)) {
                    *p++ = '*';
                }

                if (*p == '\0') {
                    msr_log(msr, 1, "Unable to sanitize variable \"%s\" at offset %u (size %d) "
                            "of QUERY_STRING because the request line is too short.",
                            log_escape_ex(msr->mp, arg->name, arg->name_len),
                            arg->value_origin_offset, arg->value_origin_len);
                    continue;
                }
            }
        }
    }
}

/**
 * Output the Producer header.
 */
static void sec_auditlog_write_producer_header(modsec_rec *msr) {
    char **signatures = NULL;
    char *text = NULL;
    int i;

    /* Try to write everything in one go. */
    if (msr->txcfg->component_signatures->nelts == 0) {
        text = apr_psprintf(msr->mp, "Producer: %s.\n", MODSEC_MODULE_NAME_FULL);
        sec_auditlog_write(msr, text, strlen(text));

        return;
    }

    /* Start with the ModSecurity signature. */
    text = apr_psprintf(msr->mp, "Producer: %s", MODSEC_MODULE_NAME_FULL);
    sec_auditlog_write(msr, text, strlen(text));


    /* Then loop through the components and output individual signatures. */
    signatures = (char **)msr->txcfg->component_signatures->elts;
    for(i = 0; i < msr->txcfg->component_signatures->nelts; i++) {
        text = apr_psprintf(msr->mp, "; %s", (char *)signatures[i]);
        sec_auditlog_write(msr, text, strlen(text));
    }

    sec_auditlog_write(msr, ".\n", 2);
}

#ifdef WITH_YAJL
/**
 * Ouput the Producer header into a JSON generator
 */
static void sec_auditlog_write_producer_header_json(modsec_rec *msr, yajl_gen g) {
    char **signatures = NULL;
    int i;

    // this is written no matter what
    yajl_string(g, "producer");

    /* Try to write verything in one go. */
    if (msr->txcfg->component_signatures->nelts == 0) {
        yajl_string(g, MODSEC_MODULE_NAME_FULL);

        return;
    }

    // we'll need an array if there are component signatures
    yajl_gen_array_open(g);

    /* Start with the ModSecurity signature. */
    yajl_string(g, MODSEC_MODULE_NAME_FULL);

    /* Then loop through the components and output individual signatures. */
    signatures = (char **)msr->txcfg->component_signatures->elts;
    for(i = 0; i < msr->txcfg->component_signatures->nelts; i++) {
        yajl_string(g, (char *)signatures[i]);
    }

    yajl_gen_array_close(g); // array for producers is finished
}
#endif

/*
* \brief This function will returns the next chain node
*
* \param current Pointer to current chined rule
* \param msr Pointer to modsec_rec
*
* \retval NULL On failure
* \retval next_rule On Success
*/
static msre_rule *return_chained_rule(const msre_rule *current, modsec_rec *msr)   {
    apr_array_header_t *arr = NULL;
    msre_rule **rules = NULL;
    msre_rule *rule = NULL, *next_rule = NULL;
    int i;
    int phase;

    if (current == NULL || current->actionset == NULL || current->ruleset == NULL)
        return NULL;

    phase = current->actionset->phase;

    switch (phase) {
        case PHASE_REQUEST_HEADERS :
            arr = current->ruleset->phase_request_headers;
            break;
        case PHASE_REQUEST_BODY :
            arr = current->ruleset->phase_request_body;
            break;
        case PHASE_RESPONSE_HEADERS :
            arr = current->ruleset->phase_response_headers;
            break;
        case PHASE_RESPONSE_BODY :
            arr = current->ruleset->phase_response_body;
            break;
        case PHASE_LOGGING :
            arr = current->ruleset->phase_logging;
            break;
        default :
            msr_log(msr, 1, "Logging: Invalid phase %d",phase);
            return NULL;
    }

    rules = (msre_rule **)arr->elts;
    for (i = 0; i < arr->nelts; i++) {
        rule = rules[i];
        if (rule != NULL)    {
            if (strncmp(current->unparsed,rule->unparsed,strlen(current->unparsed)) == 0) {

                if (i < arr->nelts -1)   {
                    next_rule = rules[i+1];
                } else  {
                    next_rule = rules[i];
                }

                if (next_rule == NULL || next_rule->chain_starter == NULL)
                    return NULL;

                if(current->chain_starter == NULL && next_rule->chain_starter != NULL)  {
                    if (strncmp(current->unparsed, next_rule->chain_starter->unparsed, strlen(current->unparsed)) != 0)
                        return NULL;
                }

                if(current->chain_starter != NULL && next_rule->chain_starter != NULL)  {
                    if (strncmp(current->chain_starter->unparsed, rule->chain_starter->unparsed, strlen(current->chain_starter->unparsed)) != 0)
                        return NULL;
                }

                return next_rule;
            }
        }
    }

    return NULL;
}

/*
* \brief This function will check if a chained rule
* appears into matched array.
*
* \param msr Pointer to modsec_rec
* \param next_rule Pointer to chained rule
*
* \retval 0 On failure
* \retval 1 On Success
*/
static int chained_is_matched(modsec_rec *msr, const msre_rule *next_rule) {
    int i = 0;
    const msre_rule *rule = NULL;

    for (i = 0; i < msr->matched_rules->nelts; i++) {
        rule = ((msre_rule **)msr->matched_rules->elts)[i];
        if (rule != NULL && (strncmp(rule->unparsed,next_rule->unparsed,strlen(rule->unparsed)) == 0))  {
            return 1;
        }
    }

    return 0;
}

#ifdef WITH_YAJL
/**
 * Write detailed information about performance metrics into a JSON generator
 */
static void format_performance_variables_json(modsec_rec *msr, yajl_gen g) {
    yajl_string(g, "stopwatch");
    yajl_gen_map_open(g);

    yajl_kv_int(g, "p1", msr->time_phase1);
    yajl_kv_int(g, "p2", msr->time_phase2);
    yajl_kv_int(g, "p3", msr->time_phase3);
    yajl_kv_int(g, "p4", msr->time_phase4);
    yajl_kv_int(g, "p5", msr->time_phase5);
    yajl_kv_int(g, "sr", msr->time_storage_read);
    yajl_kv_int(g, "sw", msr->time_storage_write);
    yajl_kv_int(g, "l", msr->time_logging);
    yajl_kv_int(g, "gc", msr->time_gc);

    yajl_gen_map_close(g);
}

/**
 * Write detailed information about a rule and its actionset into a JSON generator
 */
static void write_rule_json(modsec_rec *msr, const msre_rule *rule, yajl_gen g) {
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int been_opened = 0;
    int k;

    yajl_gen_map_open(g);

    yajl_string(g, "actionset");
    yajl_gen_map_open(g);
    if (rule->actionset->id) {
        yajl_kv_string(g, "id", log_escape(msr->mp, rule->actionset->id));
    }
    if (rule->actionset->rev) {
        yajl_kv_string(g, "rev", log_escape(msr->mp, rule->actionset->rev));
    }
    if (rule->actionset->version) {
        yajl_kv_string(g, "version", log_escape(msr->mp, rule->actionset->version));
    }
    if (rule->actionset->severity != NOT_SET) {
        yajl_kv_int(g, "severity", rule->actionset->severity);
    }
    if (rule->actionset->accuracy != NOT_SET) {
        yajl_kv_int(g, "accuracy", rule->actionset->accuracy);
    }
    if (rule->actionset->maturity != NOT_SET) {
        yajl_kv_int(g, "maturity", rule->actionset->maturity);
    }
    if (rule->actionset->phase != NOT_SET) {
        yajl_kv_int(g, "phase", rule->actionset->phase);
    }
    yajl_kv_bool(g, "is_chained", rule->actionset->is_chained || (rule->chain_starter != NULL));
    if (rule->actionset->is_chained && (rule->chain_starter == NULL)) {
        yajl_kv_bool(g, "chain_starter", 1);
    }

    // tags, lazily opened
    tarr = apr_table_elts(rule->actionset->actions);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (k = 0; k < tarr->nelts; k++) {
        msre_action *action = (msre_action *)telts[k].val;
        if (strcmp(telts[k].key, "tag") == 0) {
            msc_string *var = NULL;
            if (been_opened == 0) {
                yajl_string(g, "tags");
                yajl_gen_array_open(g);
                been_opened = 1;
            }

            // expand variables in the tag
            var = (msc_string *)apr_pcalloc(msr->mp, sizeof(msc_string));
            var->value = (char *)action->param;
            var->value_len = strlen(action->param);
            expand_macros(msr, var, NULL, msr->mp);

            yajl_string(g, log_escape(msr->mp, var->value));
        }
    }

    if (been_opened == 1) {
        yajl_gen_array_close(g);
    }

    yajl_gen_map_close(g);

    yajl_string(g, "operator");
    yajl_gen_map_open(g);
    yajl_kv_string(g, "operator", rule->op_name);
    yajl_kv_string(g, "operator_param", rule->op_param);
    yajl_kv_string(g, "target", rule->p1);
    yajl_kv_bool(g, "negated", rule->op_negated);
    yajl_gen_map_close(g);

    yajl_string(g, "config");
    yajl_gen_map_open(g);
    yajl_kv_string(g, "filename", rule->filename);
    yajl_kv_int(g, "line_num", rule->line_num);
    yajl_gen_map_close(g);

    yajl_kv_string(g, "unparsed", rule->unparsed);
    yajl_kv_bool(g, "is_matched", chained_is_matched(msr, rule));

    yajl_gen_map_close(g);
}

/*
 * Produce an audit log entry in JSON format.
 */
void sec_audit_logger_json(modsec_rec *msr) {
    const apr_array_header_t *arr = NULL;
    apr_table_entry_t *te = NULL;
    const apr_array_header_t *tarr_pattern = NULL;
    const apr_table_entry_t *telts_pattern = NULL;
    char *str1 = NULL, *str2 = NULL, *text = NULL;
    const msre_rule *rule = NULL, *next_rule = NULL;
    apr_size_t nbytes, nbytes_written;
    unsigned char md5hash[APR_MD5_DIGESTSIZE];
    int was_limited = 0;
    int present = 0;
    int wrote_response_body = 0;
    char *entry_filename, *entry_basename;
    apr_status_t rc;
    int i, limit, k, sanitized_partial, j;
    char *buf = NULL, *pat = NULL;
    msc_parm *mparm = NULL;
    int arg_min, arg_max, sanitize_matched;
    yajl_gen g;
    int been_opened = 0; // helper flag for conditionally opening maps
    const unsigned char *final_buf;
    size_t len;


    /* Return silently if we don't have a request line. This
     * means we will not be logging request timeouts.
     */
    if (msr->request_line == NULL) {
        msr_log(msr, 4, "Audit log: Skipping request whose request_line is null.");
        return;
    }

    /* Also return silently if we don't have a file descriptor. */
    if (msr->txcfg->auditlog_fd == NULL) {
        msr_log(msr, 4, "Audit log: Skipping request since there is nowhere to write to.");
        return;
    }

    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {
        /* Serial logging - we already have an open file
         * descriptor to write to.
         */
        msr->new_auditlog_fd = msr->txcfg->auditlog_fd;
    } else {
        /* Concurrent logging - we need to create a brand
         * new file for this request.
         */
        apr_md5_init(&msr->new_auditlog_md5ctx);

        msr->new_auditlog_filename = construct_auditlog_filename(msr->mp, msr->txid);
        if (msr->new_auditlog_filename == NULL) return;

        /* The audit log storage directory should be explicitly
         * defined. But if it isn't try to write to the same
         * directory where the index file is placed. Of course,
         * it is *very* bad practice to allow the Apache user
         * to write to the same directory where a root user is
         * writing to but it's not us that's causing the problem
         * and there isn't anything we can do about that.
         *
         * ENH Actually there is something we can do! We will make
         * SecAuditStorageDir mandatory, ask the user to explicitly
         * define the storage location *and* refuse to work if the
         * index and the storage location are in the same folder.
         */
        if (msr->txcfg->auditlog_storage_dir == NULL) {
            entry_filename = file_dirname(msr->mp, msr->txcfg->auditlog_name);
        }
        else {
            entry_filename = msr->txcfg->auditlog_storage_dir;
        }
        if (entry_filename == NULL) return;

        entry_filename = apr_psprintf(msr->mp, "%s%s", entry_filename, msr->new_auditlog_filename);
        if (entry_filename == NULL) return;
        entry_basename = file_dirname(msr->mp, entry_filename);
        if (entry_basename == NULL) return;

        /* IMP1 Surely it would be more efficient to check the folders for
         * the audit log repository base path in the configuration phase, to reduce
         * the work we do on every request. Also, since our path depends on time,
         * we could cache the time we last checked and don't check if we know
         * the folder is there.
         */
        rc = apr_dir_make_recursive(entry_basename, msr->txcfg->auditlog_dirperms, msr->mp);
        if ((rc != APR_SUCCESS) && (rc != APR_EEXIST)) {
            msr_log(msr, 1, "Audit log: Failed to create subdirectories: %s (%s)",
                entry_basename, get_apr_error(msr->mp, rc));
            return;
        }

        rc = apr_file_open(&msr->new_auditlog_fd, entry_filename,
            APR_WRITE | APR_TRUNCATE | APR_CREATE | APR_BINARY | APR_FILE_NOCLEANUP,
            msr->txcfg->auditlog_fileperms, msr->mp);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to create file: %s (%s)",
                entry_filename, get_apr_error(msr->mp, rc));
            return;
        }
    }

    /* Lock the mutex, but only if we are using serial format. */
    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {
        rc = apr_global_mutex_lock(msr->modsecurity->auditlog_lock);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to lock global mutex: %s",
                get_apr_error(msr->mp, rc));
        }
    }

    /**
     * allocate the buffer for the JSON generator
     * passing null will force yajl to use malloc/realloc/free
     * need to perf test using APR routines
     */
    g = yajl_gen_alloc(NULL);

    /**
     * don't pretty print JSON
     * this is harder to eyeball but much easier to parse programmatically
     */
    yajl_gen_config(g, yajl_gen_beautify, 0);

    yajl_gen_map_open(g); // IT BEGINS

    /* AUDITLOG_PART_HEADER */
    yajl_string(g, "transaction");
    yajl_gen_map_open(g); // transaction top-level key

    yajl_kv_string(g, "time", current_logtime(msr->mp));
    yajl_kv_string(g, "transaction_id", msr->txid);
    yajl_kv_string(g, "remote_address", msr->remote_addr);
    yajl_kv_int(g, "remote_port", (int)msr->remote_port); // msr->remote_port is unsigned, yajl wants signed
    yajl_kv_string(g, "local_address", msr->local_addr);
    yajl_kv_int(g, "local_port", (int)msr->local_port);

    yajl_gen_map_close(g); // transaction top-level key is finished

    yajl_string(g, "request");
    yajl_gen_map_open(g); // request top-level key

    /* AUDITLOG_PART_REQUEST_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_REQUEST_HEADERS) != NULL) {
        sanitize_request_line(msr);
        yajl_kv_string(g, "request_line", msr->request_line);

        yajl_string(g, "headers");
        yajl_gen_map_open(g); // separate map for request headers

        arr = apr_table_elts(msr->request_headers);
        te = (apr_table_entry_t *)arr->elts;

        tarr_pattern = apr_table_elts(msr->pattern_to_sanitize);
        telts_pattern = (const apr_table_entry_t*)tarr_pattern->elts;

        for (i = 0; i < arr->nelts; i++) {
            sanitized_partial = 0;
            sanitize_matched = 0;
            text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
            // write the key no matter what
            // since sanitization only occurs on the value
            yajl_string(g, te[i].key);
            if (apr_table_get(msr->request_headers_to_sanitize, te[i].key) != NULL) {
                buf = apr_psprintf(msr->mp, "%s",text+strlen(te[i].key)+2);
                for ( k = 0; k < tarr_pattern->nelts; k++)  {
                    if(strncmp(telts_pattern[k].key,te[i].key,strlen(te[i].key)) ==0 ) {
                        mparm = (msc_parm *)telts_pattern[k].val;
                        if(mparm->pad_1 == -1)
                            sanitize_matched = 1;
                        pat = strstr(buf,mparm->value);
                        if (pat != NULL)    {
                            j = strlen(mparm->value);
                            arg_min = j;
                            arg_max = 1;
                            while((*pat != '\0')&&(j--)) {
                                if(arg_max > mparm->pad_2)  {
                                    int off = strlen(mparm->value) - arg_max;
                                    int pos = mparm->pad_1-1;
                                    if(off > pos)    {
                                        *pat = '*';
                                    }
                                }
                                arg_max++;
                                arg_min--;
                                pat++;
                            }
                            sanitized_partial = 1;
                        }
                    }
                }

                if(sanitized_partial == 1 && sanitize_matched == 0)  {
                    yajl_string(g, buf);
                } else {
                    memset(buf, '*', strlen(buf)); // strlen also includes the appended newline on the header
                    yajl_string(g, buf);
                }
            } else {
            // we diverge from the original logic a bit because we always print the key
            // at this no point sanitization had occured, so we just print the value
                yajl_string(g, te[i].val);
            }
        }
        yajl_gen_map_close(g); // request headers map is finished
    }

    /* AUDITLOG_PART_REQUEST_BODY */

    /* Output this part of it was explicitly requested (C) or if it was the faked
     * request body that was requested (I) but we have no reason to fake it (it's
     * already in the correct format).
     */
    if ( (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_REQUEST_BODY) != NULL)
        || ( (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_FAKE_REQUEST_BODY) != NULL)
            && (msr->mpd == NULL) ) )
    {
        if (msr->msc_reqbody_read) {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;
            apr_array_header_t *sorted_args;
            unsigned int offset = 0, last_offset = 0;
            msc_arg *nextarg = NULL;
            int sanitize = 0; /* IMP1 Use constants for "sanitize" values. */
            char *my_error_msg = NULL;

            sorted_args = apr_array_make(msr->mp, 25, sizeof(const msc_arg *));

            /* First we need to sort the arguments that need to be
             * sanitized in descending order (we are using a stack structure
             * to store then so the order will be ascending when we start
             * popping them out). This is because we will
             * be reading the request body sequentially and must
             * sanitize it as we go.
             */

            for(;;) {
                nextarg = NULL;

                /* Find the next largest offset (excluding
                 * the ones we've used up already).
                 */
                tarr = apr_table_elts(msr->arguments_to_sanitize);
                telts = (const apr_table_entry_t*)tarr->elts;
                for(i = 0; i < tarr->nelts; i++) {
                    msc_arg *arg = (msc_arg *)telts[i].val;
                    if (arg->origin != NULL &&
                            ( strcmp(arg->origin, "BODY") != 0 && strcmp(arg->origin, "JSON") !=0) )
                        continue;

                    if (last_offset == 0) { /* The first time we're here. */
                        if (arg->value_origin_offset > offset) {
                            offset = arg->value_origin_offset;
                            nextarg = arg;
                        }
                    } else { /* Not the first time. */
                        if ((arg->value_origin_offset > offset)
                            &&(arg->value_origin_offset < last_offset))
                        {
                            offset = arg->value_origin_offset;
                            nextarg = arg;
                        }
                    }
                }

                /* If we don't have the next argument that means
                 * we're done here.
                 */
                if (nextarg == NULL) break;

                sanitize = 2; /* Means time to pop the next argument out. */
                last_offset = offset;
                offset = 0;
                { /* IMP1 Fix this ugly bit here. */
                    msc_arg **x = apr_array_push(sorted_args);
                    *x = nextarg;
                }
            }

            /* Now start retrieving the body chunk by chunk and
             * sanitize data in pieces.
             */

            rc = modsecurity_request_body_retrieve_start(msr, &my_error_msg);
            if (rc < 0) {
                msr_log(msr, 1, "Audit log: %s", my_error_msg);
            } else {
                msc_data_chunk *chunk = NULL;
                unsigned int chunk_offset = 0;
                unsigned int sanitize_offset = 0;
                unsigned int sanitize_length = 0;
                yajl_string(g, "body");
                yajl_gen_array_open(g); // use an array here because we're writing in chunks

                for(;;) {
                    rc = modsecurity_request_body_retrieve(msr, &chunk, -1, &my_error_msg);
                    if (chunk != NULL) {
                        /* Anything greater than 1 means we have more data to sanitize. */
                        while (sanitize > 1) {
                            msc_arg **arg = NULL;

                            if (sanitize == 2) {
                                /* Get the next argument from the stack. */
                                arg = (msc_arg **)apr_array_pop(sorted_args);
                                if (arg == NULL) sanitize = 0; /* We're done sanitising. */
                                else {
                                    /* Continue with sanitation to process the
                                     * retrieved argument.
                                     */
                                    sanitize = 1;
                                    sanitize_offset = (*arg)->value_origin_offset;
                                    sanitize_length = (*arg)->value_origin_len;
                                }
                            }

                            if (sanitize) {
                                /* Check if the data we want to sanitize is
                                 * stored in the current chunk.
                                 */
                                if (chunk_offset + chunk->length > sanitize_offset) {
                                    unsigned int soff; /* data offset within chunk */
                                    unsigned int len;  /* amount in this chunk to sanitize */

                                    soff = sanitize_offset - chunk_offset;

                                    if (soff + sanitize_length <= chunk->length) {
                                        /* The entire argument resides in the current chunk. */
                                        len = sanitize_length;
                                        sanitize = 2; /* Get another parameter to sanitize. */
                                    } else {
                                        /* Some work to do here but we'll need to seek
                                         * another chunk.
                                         */
                                        len = chunk->length - soff;
                                        sanitize_offset += len;
                                        sanitize_length -= len;
                                        sanitize = 1; /* It's OK to go to the next chunk. */
                                    }

                                    /* Yes, we actually write over the original data.
                                     * We shouldn't be needing it any more.
                                     */
                                    if (soff + len <= chunk->length) { /* double check */
                                        memset((char *)chunk->data + soff, '*', len);
                                    }
                                }
                            }
                        }

                        /* Write the sanitized chunk to the log
                         * and advance to the next chunk. */
                        yajl_string(g, chunk->data);
                        chunk_offset += chunk->length;
                    }

                    if (rc <= 0) {
                        break;
                    }
                }

                yajl_gen_array_close(g); // request body chunks array is finished

                if (rc < 0) {
                    msr_log(msr, 1, "Audit log: %s", my_error_msg);
                }

                modsecurity_request_body_retrieve_end(msr);
            }
        }
    }

    /* AUDITLOG_PART_FAKE_REQUEST_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_FAKE_REQUEST_BODY) != NULL) {
        if ((msr->msc_reqbody_read)&&(msr->mpd != NULL)) {
            char *buffer = NULL;

            buffer = multipart_reconstruct_urlencoded_body_sanitize(msr);
            if (buffer == NULL) {
                msr_log(msr, 1, "Audit log: Failed to reconstruct request body.");
            } else {
                yajl_kv_string(g, "fake_body", buffer);
            }
        }
    }

    yajl_gen_map_close(g); // request top-level key is finished

    yajl_string(g, "response");
    yajl_gen_map_open(g); // response top-level key

    /* AUDITLOG_PART_A_RESPONSE_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_A_RESPONSE_HEADERS) != NULL) {

        /* There are no response headers (or the status line) in HTTP 0.9 */
        if (msr->response_headers_sent) {
            yajl_kv_string(g, "protocol", msr->response_protocol);
            // as an integer, response status is easier to parse than status_line
            yajl_kv_int(g, "status", (int)msr->response_status);
            yajl_string(g, "headers");
            yajl_gen_map_open(g); // separate map for response headers

            /* Output headers */

            arr = apr_table_elts(msr->response_headers);
            te = (apr_table_entry_t *)arr->elts;

            tarr_pattern = apr_table_elts(msr->pattern_to_sanitize);
            telts_pattern = (const apr_table_entry_t*)tarr_pattern->elts;

            for (i = 0; i < arr->nelts; i++) {
                sanitized_partial = 0;
                sanitize_matched = 0;
                text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
                // write the key no matter what
                // since sanitization only occurs on the value
                yajl_string(g, te[i].key);
                if (apr_table_get(msr->response_headers_to_sanitize, te[i].key) != NULL) {
                    buf = apr_psprintf(msr->mp, "%s",text+strlen(te[i].key)+2);

                    for ( k = 0; k < tarr_pattern->nelts; k++)  {
                        if(strncmp(telts_pattern[k].key,te[i].key,strlen(te[i].key)) ==0 ) {
                            mparm = (msc_parm *)telts_pattern[k].val;
                            if(mparm->pad_1 == -1)
                                sanitize_matched = 1;
                            pat = strstr(buf,mparm->value);
                            if (pat != NULL)    {
                                j = strlen(mparm->value);
                                arg_min = j;
                                arg_max = 1;
                                while((*pat != '\0')&&(j--)) {
                                    if(arg_max > mparm->pad_2)  {
                                        int off = strlen(mparm->value) - arg_max;
                                        int pos = mparm->pad_1-1;
                                        if(off > pos)    {
                                            *pat = '*';
                                        }
                                    }
                                    arg_max++;
                                    arg_min--;
                                    pat++;
                                }
                                sanitized_partial = 1;
                            }
                        }
                    }

                    if(sanitized_partial == 1 && sanitize_matched == 0)  {
                        yajl_string(g, buf);
                    } else {
                        memset(buf, '*', strlen(buf));
                        yajl_string(g, buf);
                    }
                } else {
                    // we diverge from the original logic a bit because we always print the key
                    // at this point no sanitization had occured, so we just print the value
                    yajl_string(g, te[i].val);
                }
            }
            yajl_gen_map_close(g); // response headers map is finised
        }
    }

    apr_table_clear(msr->pattern_to_sanitize);

    /* AUDITLOG_PART_RESPONSE_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_RESPONSE_BODY) != NULL) {
        if (msr->resbody_data != NULL) {
            yajl_kv_string(g, "body", msr->resbody_data);
            wrote_response_body = 1;
        }
    }

    yajl_gen_map_close(g); // response top-level key is finished

    yajl_string(g, "audit_data");
    yajl_gen_map_open(g); // audit_data top-level key

    /* AUDITLOG_PART_TRAILER */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_TRAILER) != NULL) {
        apr_time_t now = apr_time_now();

        /* Messages */
        been_opened = 0;
        if (msr->alerts->nelts > 0) {
            yajl_string(g, "messages");
            yajl_gen_array_open(g);
            been_opened = 1;
        }
        for(i = 0; i < msr->alerts->nelts; i++) {
            yajl_string(g, ((char **)msr->alerts->elts)[i]);
        }
        if (been_opened == 1) {
            yajl_gen_array_close(g);
        }

        /* Apache error messages */
        been_opened = 0;
        if (msr->error_messages->nelts > 0) {
            yajl_string(g, "error_messages");
            yajl_gen_array_open(g);
            been_opened = 1;
        }
        for(i = 0; i < msr->error_messages->nelts; i++) {
            error_message_t *em = (((error_message_t **)msr->error_messages->elts)[i]);
            yajl_string(g, format_error_log_message(msr->mp, em));
        }
        if (been_opened == 1) {
            yajl_gen_array_close(g);
        }

        /* Action */
        if (msr->was_intercepted) {
            yajl_string(g, "action");
            yajl_gen_map_open(g);
            yajl_kv_bool(g, "intercepted", 1);
            yajl_kv_int(g, "phase", msr->intercept_phase);
            yajl_kv_string(g, "message", msr->intercept_message);
            yajl_gen_map_close(g);
        }

        /* Apache-Handler */
#ifdef LOG_NO_HANDLER
        if (msr->txcfg->debuglog_level >= 9)
#endif
        if (msr->r->handler != NULL) {
            yajl_kv_string(g, "handler", msr->r->handler);
        }


        /* Stopwatch2 */
#ifdef LOG_NO_STOPWATCH
        if (msr->txcfg->debuglog_level >= 9)
#endif
        format_performance_variables_json(msr, g);

        /* Our response body does not contain chunks */
        /* ENH Only write this when the output was chunked. */
        /* ENH Add info when request body was decompressed, dechunked too. */
#ifdef LOG_NO_DECHUNK
        if (msr->txcfg->debuglog_level >= 9)
#endif
        if (wrote_response_body) {
            yajl_kv_bool(g, "response_body_dechunked", 1);
        }

#ifdef LOG_NO_SERVER_CONTEXT
        if (msr->txcfg->debuglog_level >= 9) {
#endif
        sec_auditlog_write_producer_header_json(msr, g);

        /* Server */
        if (msr->server_software != NULL) {
            yajl_kv_string(g, "server", msr->server_software);
        }

        been_opened = 0;
        /* Sanitised arguments */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->arguments_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                if (been_opened == 0) {
                    yajl_string(g, "sanitized");
                    yajl_gen_map_open(g);
                    been_opened = 1;
                }

                yajl_string(g, "args");
                yajl_gen_array_open(g);
            }

            for(i = 0; i < tarr->nelts; i++) {
                msc_arg *arg = (msc_arg *)telts[i].val;
                // yay arrays actually make it easier here
                yajl_string(g, log_escape(msr->mp, arg->name));
            }
            if (tarr->nelts > 0) {
                yajl_gen_array_close(g);
            }
        }

        /* Sanitised request headers */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->request_headers_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                if (been_opened == 0) {
                    yajl_string(g, "sanitized");
                    yajl_gen_map_open(g);
                    been_opened = 1;
                }

                yajl_string(g, "request_headers");
                yajl_gen_array_open(g);
            }

            for(i = 0; i < tarr->nelts; i++) {
                yajl_string(g, log_escape(msr->mp, telts[i].key));
            }
            if (tarr->nelts > 0) {
                yajl_gen_array_close(g);
            }
        }

        /* Sanitised response headers */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->response_headers_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                if (been_opened == 0) {
                    yajl_string(g, "sanitized");
                    yajl_gen_map_open(g);
                    been_opened = 1;
                }

                yajl_string(g, "response_headers");
                yajl_gen_array_open(g);
            }

            for(i = 0; i < tarr->nelts; i++) {
                yajl_string(g, log_escape(msr->mp, telts[i].key));
            }
            if (tarr->nelts > 0) {
                yajl_gen_array_close(g);
            }
        }

        if (been_opened == 1) {
            yajl_gen_map_close(g); // sanitized args map is finished
        }
#ifdef LOG_NO_SERVER_CONTEXT
	}
#endif

        /* Web application info. */
        if ( ((msr->txcfg->webappid != NULL)&&(strcmp(msr->txcfg->webappid, "default") != 0))
            || (msr->sessionid != NULL) || (msr->userid != NULL))
        {
            yajl_string(g, "webapp_info");
            yajl_gen_map_open(g);

            if (msr->txcfg->webappid != NULL) {
                yajl_kv_string(g, "id", log_escape(msr->mp, msr->txcfg->webappid));
            }
            if (msr->sessionid != NULL) {
                yajl_kv_string(g, "session", log_escape(msr->mp, msr->sessionid));
            }
            if (msr->userid != NULL) {
                yajl_kv_string(g, "user_id", log_escape(msr->mp, msr->userid));
            }

            yajl_gen_map_close(g);
        }

        if ( ((msr->txcfg->sensor_id != NULL)&&(strcmp(msr->txcfg->sensor_id, "default") != 0)))
        {
            if(msr->txcfg->sensor_id != NULL) {
                yajl_kv_string(g, "sensor_id", log_escape(msr->mp, msr->txcfg->sensor_id));
            }
        }


        if (msr->txcfg->is_enabled > 0) {
            yajl_kv_string(g, "engine_mode", (msr->txcfg->is_enabled == 1 ? "DETECTION_ONLY" : "ENABLED"));
        }

        /* Rule performance time */
        if(msr->txcfg->max_rule_time > 0)   {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->perf_rules);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                yajl_string(g, "rules_performance_info");
                yajl_gen_map_open(g); // separate map for rule perf info
            }

            for(i = 0; i < tarr->nelts; i++) {
                yajl_kv_string(g, log_escape(msr->mp, telts[i].key), log_escape(msr->mp, telts[i].val));
            }
            if (tarr->nelts > 0) {
                yajl_gen_map_close(g); // map for rule perf info is finished
            }
        }
    }

    yajl_gen_map_close(g); // audit_data top-level key is finished

    /* AUDITLOG_PART_UPLOADS */
    if ((strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_UPLOADS) != NULL) && (msr->mpd != NULL)) {
        multipart_part **parts = NULL;
        unsigned int total_size = 0;
        int cfiles = 0;

        yajl_string(g, "uploads");
        yajl_gen_map_open(g);

        parts = (multipart_part **)msr->mpd->parts->elts;
        yajl_string(g, "info");
        yajl_gen_array_open(g); // separate array for upload info
        for(cfiles = 0; cfiles < msr->mpd->parts->nelts; cfiles++) {
            if (parts[cfiles]->type == MULTIPART_FILE) {
                if(parts[cfiles]->filename != NULL) {
                    yajl_gen_map_open(g);
                    yajl_kv_int(g, "file_size", parts[cfiles]->tmp_file_size);
                    yajl_kv_string(g, "file_name", log_escape(msr->mp, parts[cfiles]->filename));
                    yajl_kv_string(g, "content_type", parts[cfiles]->content_type ? parts[cfiles]->content_type : "<Unknown Content-Type>");
                    total_size += parts[cfiles]->tmp_file_size;
                    yajl_gen_map_close(g);
                }
            }
        }
        yajl_gen_array_close(g); // array for upload info is finished
        yajl_kv_int(g, "total", total_size);

        yajl_gen_map_close(g); // uploads top-level key is finished
    }

    /* AUDITLOG_PART_MATCHEDRULES */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_MATCHEDRULES) != NULL) {
        yajl_string(g, "matched_rules");
        yajl_gen_array_open(g); // matched_rules top-level key

        /* Matched Rules */

        for(i = 0; i < msr->matched_rules->nelts; i++) {
            rule = ((msre_rule **)msr->matched_rules->elts)[i];
            if ((rule != NULL) && (rule->actionset != NULL) && rule->actionset->is_chained && (rule->chain_starter == NULL)) {
                /*
                 * create a separate map for each rule chain
                 * this makes it a lot easier to search for partial chains
                 */
                yajl_gen_map_open(g); // map for this chain
                yajl_kv_bool(g, "chain", 1);
                yajl_string(g, "rules");
                yajl_gen_array_open(g); // array for the rules

                write_rule_json(msr, rule, g);
                do {
                    if (rule->ruleset != NULL)   {

                        next_rule = return_chained_rule(rule,msr);

                        if (next_rule != NULL)  {

                            present = chained_is_matched(msr,next_rule);

                            if (present == 1)   {
                                i++;
                            }
                            write_rule_json(msr, next_rule, g);
                        }
                    }
                    rule = next_rule;
                } while (rule != NULL && rule->actionset != NULL && rule->actionset->is_chained);
                yajl_gen_array_close(g);

                yajl_kv_bool(g, "full_chain_match", present); // if one of the rules didnt match, present is set to 0
                yajl_gen_map_close(g); // close the map for this chain
            } else  {
                yajl_gen_map_open(g);

                yajl_kv_bool(g, "chain", 0);
                yajl_string(g, "rules"); // this really should be 'rule', but we're keeping in line with other chain maps

                yajl_gen_array_open(g);
                if ((rule != NULL) && (rule->actionset != NULL) && !rule->actionset->is_chained && (rule->chain_starter == NULL)) {
                    write_rule_json(msr, rule, g);
                }
                yajl_gen_array_close(g);

                yajl_gen_map_close(g);
            }
        }
        yajl_gen_array_close(g); // matched_rules top-level key is finished

    }
    /* AUDITLOG_PART_ENDMARKER */

    /* finished building JSON */
    yajl_gen_map_close(g); // box it up!

    yajl_gen_get_buf(g, &final_buf, &len);
    sec_auditlog_write(msr, final_buf, len);

    yajl_gen_clear(g);
    yajl_gen_free(g);

    sec_auditlog_write(msr, "\n", 1);

    /* Return here if we were writing to a serial log
     * as it does not need an index file.
     */
    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {

        /* Unlock the mutex we used to serialise access to the audit log file. */
        rc = apr_global_mutex_unlock(msr->modsecurity->auditlog_lock);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to unlock global mutex: %s",
                    get_apr_error(msr->mp, rc));
        }

        return;
    }

    /* From here on only concurrent-style processing. */

    /* File handle might already be closed after write failure. */
    if (msr->new_auditlog_fd) {
        apr_file_close(msr->new_auditlog_fd);
    }

    /* Write an entry to the index file */

    /* Calculate hash of the entry. */
    apr_md5_final(md5hash, &msr->new_auditlog_md5ctx);

    str2 = apr_psprintf(msr->mp, "%s %d %d md5:%s", msr->new_auditlog_filename, 0,
            msr->new_auditlog_size, bytes2hex(msr->mp, md5hash, 16));
    if (str2 == NULL) return;

    /* We do not want the index line to be longer than 3980 bytes. */
    limit = 3980;
    was_limited = 0;

    /* If we are logging to a pipe we need to observe and
     * obey the pipe atomic write limit - PIPE_BUF. For
     * more details see the discussion in sec_guardian_logger code.
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
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Audit Log: Writing %" APR_SIZE_T_FMT " bytes to primary concurrent index", nbytes);
    }
    apr_file_write_full(msr->txcfg->auditlog_fd, text, nbytes, &nbytes_written);

    /* Write to the secondary audit log if we have one */
    if (msr->txcfg->auditlog2_fd != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Audit Log: Writing %" APR_SIZE_T_FMT " bytes to secondary concurrent index", nbytes);
        }
        apr_file_write_full(msr->txcfg->auditlog2_fd, text, nbytes, &nbytes_written);
    }
}
#endif

/*
 * Produce an audit log entry in native format.
 */
void sec_audit_logger_native(modsec_rec *msr) {
    const apr_array_header_t *arr = NULL;
    apr_table_entry_t *te = NULL;
    const apr_array_header_t *tarr_pattern = NULL;
    const apr_table_entry_t *telts_pattern = NULL;
    char *str1 = NULL, *str2 = NULL, *text = NULL;
    const msre_rule *rule = NULL, *next_rule = NULL;
    apr_size_t nbytes, nbytes_written;
    unsigned char md5hash[APR_MD5_DIGESTSIZE];
    int was_limited = 0;
    int present = 0;
    int wrote_response_body = 0;
    char *entry_filename, *entry_basename;
    apr_status_t rc;
    int i, limit, k, sanitized_partial, j;
    char *buf = NULL, *pat = NULL;
    msc_parm *mparm = NULL;
    int arg_min, arg_max, sanitize_matched;

    /* the boundary is used by both audit log types */
    msr->new_auditlog_boundary = create_auditlog_boundary(msr->r);

    /* Return silently if we don't have a request line. This
     * means we will not be logging request timeouts.
     */
    if (msr->request_line == NULL) {
        msr_log(msr, 4, "Audit log: Skipping request whose request_line is null.");
        return;
    }

    /* Also return silently if we don't have a file descriptor. */
    if (msr->txcfg->auditlog_fd == NULL) {
        msr_log(msr, 4, "Audit log: Skipping request since there is nowhere to write to.");
        return;
    }

    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {
        /* Serial logging - we already have an open file
         * descriptor to write to.
         */
        msr->new_auditlog_fd = msr->txcfg->auditlog_fd;
    } else {
        /* Concurrent logging - we need to create a brand
         * new file for this request.
         */
        apr_md5_init(&msr->new_auditlog_md5ctx);

        msr->new_auditlog_filename = construct_auditlog_filename(msr->mp, msr->txid);
        if (msr->new_auditlog_filename == NULL) return;

        /* The audit log storage directory should be explicitly
         * defined. But if it isn't try to write to the same
         * directory where the index file is placed. Of course,
         * it is *very* bad practice to allow the Apache user
         * to write to the same directory where a root user is
         * writing to but it's not us that's causing the problem
         * and there isn't anything we can do about that.
         *
         * ENH Actually there is something we can do! We will make
         * SecAuditStorageDir mandatory, ask the user to explicitly
         * define the storage location *and* refuse to work if the
         * index and the storage location are in the same folder.
         */
        if (msr->txcfg->auditlog_storage_dir == NULL) {
            entry_filename = file_dirname(msr->mp, msr->txcfg->auditlog_name);
        }
        else {
            entry_filename = msr->txcfg->auditlog_storage_dir;
        }
        if (entry_filename == NULL) return;

        entry_filename = apr_psprintf(msr->mp, "%s%s", entry_filename, msr->new_auditlog_filename);
        if (entry_filename == NULL) return;
        entry_basename = file_dirname(msr->mp, entry_filename);
        if (entry_basename == NULL) return;

        /* IMP1 Surely it would be more efficient to check the folders for
         * the audit log repository base path in the configuration phase, to reduce
         * the work we do on every request. Also, since our path depends on time,
         * we could cache the time we last checked and don't check if we know
         * the folder is there.
         */
        rc = apr_dir_make_recursive(entry_basename, msr->txcfg->auditlog_dirperms, msr->mp);
        if ((rc != APR_SUCCESS) && (rc != APR_EEXIST)) {
            msr_log(msr, 1, "Audit log: Failed to create subdirectories: %s (%s)",
                entry_basename, get_apr_error(msr->mp, rc));
            return;
        }

        rc = apr_file_open(&msr->new_auditlog_fd, entry_filename,
            APR_WRITE | APR_TRUNCATE | APR_CREATE | APR_BINARY | APR_FILE_NOCLEANUP,
            msr->txcfg->auditlog_fileperms, msr->mp);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to create file: %s (%s)",
                entry_filename, get_apr_error(msr->mp, rc));
            return;
        }
    }

    /* Lock the mutex, but only if we are using serial format. */
    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {
        rc = apr_global_mutex_lock(msr->modsecurity->auditlog_lock);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to lock global mutex: %s",
                get_apr_error(msr->mp, rc));
        }
    }


    /* AUDITLOG_PART_HEADER */
    text = apr_psprintf(msr->mp, "--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_HEADER);
    sec_auditlog_write(msr, text, strlen(text));
    /* Format: time transaction_id remote_addr remote_port local_addr local_port */

    text = apr_psprintf(msr->mp, "[%s] %s %s %u %s %u",
        current_logtime(msr->mp), msr->txid, msr->remote_addr, msr->remote_port,
        msr->local_addr, msr->local_port);
    sec_auditlog_write(msr, text, strlen(text));

    /* AUDITLOG_PART_REQUEST_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_REQUEST_HEADERS) != NULL) {
        text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_REQUEST_HEADERS);
        sec_auditlog_write(msr, text, strlen(text));

        sanitize_request_line(msr);
        sec_auditlog_write(msr, msr->request_line, strlen(msr->request_line));
        sec_auditlog_write(msr, "\n", 1);


        arr = apr_table_elts(msr->request_headers);
        te = (apr_table_entry_t *)arr->elts;

        tarr_pattern = apr_table_elts(msr->pattern_to_sanitize);
        telts_pattern = (const apr_table_entry_t*)tarr_pattern->elts;

        for (i = 0; i < arr->nelts; i++) {
            sanitized_partial = 0;
            sanitize_matched = 0;
            text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
            if (apr_table_get(msr->request_headers_to_sanitize, te[i].key) != NULL) {
                buf = apr_psprintf(msr->mp, "%s",text+strlen(te[i].key)+2);
                for ( k = 0; k < tarr_pattern->nelts; k++)  {
                    if(strncmp(telts_pattern[k].key,te[i].key,strlen(te[i].key)) ==0 ) {
                        mparm = (msc_parm *)telts_pattern[k].val;
                        if(mparm->pad_1 == -1)
                            sanitize_matched = 1;
                        pat = strstr(buf,mparm->value);
                        if (pat != NULL)    {
                            j = strlen(mparm->value);
                            arg_min = j;
                            arg_max = 1;
                            while((*pat != '\0')&&(j--)) {
                                if(arg_max > mparm->pad_2)  {
                                    int off = strlen(mparm->value) - arg_max;
                                    int pos = mparm->pad_1-1;
                                    if(off > pos)    {
                                        *pat = '*';
                                    }
                                }
                                arg_max++;
                                arg_min--;
                                pat++;
                            }
                            sanitized_partial = 1;
                        }
                    }
                }

                if(sanitized_partial == 1 && sanitize_matched == 0)  {
                    text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, buf);
                } else {
                    memset(text + strlen(te[i].key) + 2, '*', strlen(te[i].val));
                }
            }
            sec_auditlog_write(msr, text, strlen(text));
        }
    }

    /* AUDITLOG_PART_REQUEST_BODY */

    /* Output this part of it was explicitly requested (C) or if it was the faked
     * request body that was requested (I) but we have no reason to fake it (it's
     * already in the correct format).
     */
    if ( (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_REQUEST_BODY) != NULL)
        || ( (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_FAKE_REQUEST_BODY) != NULL)
            && (msr->mpd == NULL) ) )
    {
        if (msr->msc_reqbody_read) {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;
            apr_array_header_t *sorted_args;
            unsigned int offset = 0, last_offset = 0;
            msc_arg *nextarg = NULL;
            int sanitize = 0; /* IMP1 Use constants for "sanitize" values. */
            char *my_error_msg = NULL;

            sorted_args = apr_array_make(msr->mp, 25, sizeof(const msc_arg *));

            /* First we need to sort the arguments that need to be
             * sanitized in descending order (we are using a stack structure
             * to store then so the order will be ascending when we start
             * popping them out). This is because we will
             * be reading the request body sequentially and must
             * sanitize it as we go.
             */

            for(;;) {
                nextarg = NULL;

                /* Find the next largest offset (excluding
                 * the ones we've used up already).
                 */
                tarr = apr_table_elts(msr->arguments_to_sanitize);
                telts = (const apr_table_entry_t*)tarr->elts;
                for(i = 0; i < tarr->nelts; i++) {
                    msc_arg *arg = (msc_arg *)telts[i].val;
                    if (arg->origin != NULL &&
                            strcmp(arg->origin, "BODY") != 0)
                        continue;

                    if (last_offset == 0) { /* The first time we're here. */
                        if (arg->value_origin_offset > offset) {
                            offset = arg->value_origin_offset;
                            nextarg = arg;
                        }
                    } else { /* Not the first time. */
                        if ((arg->value_origin_offset > offset)
                            &&(arg->value_origin_offset < last_offset))
                        {
                            offset = arg->value_origin_offset;
                            nextarg = arg;
                        }
                    }
                }

                /* If we don't have the next argument that means
                 * we're done here.
                 */
                if (nextarg == NULL) break;

                sanitize = 2; /* Means time to pop the next argument out. */
                last_offset = offset;
                offset = 0;
                { /* IMP1 Fix this ugly bit here. */
                    msc_arg **x = apr_array_push(sorted_args);
                    *x = nextarg;
                }
            }

            /* Now start retrieving the body chunk by chunk and
             * sanitize data in pieces.
             */

            rc = modsecurity_request_body_retrieve_start(msr, &my_error_msg);
            if (rc < 0) {
                msr_log(msr, 1, "Audit log: %s", my_error_msg);
            } else {
                msc_data_chunk *chunk = NULL;
                unsigned int chunk_offset = 0;
                unsigned int sanitize_offset = 0;
                unsigned int sanitize_length = 0;
                text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_REQUEST_BODY);
                sec_auditlog_write(msr, text, strlen(text));

                for(;;) {
                    rc = modsecurity_request_body_retrieve(msr, &chunk, -1, &my_error_msg);
                    if (chunk != NULL) {
                        /* Anything greater than 1 means we have more data to sanitize. */
                        while (sanitize > 1) {
                            msc_arg **arg = NULL;

                            if (sanitize == 2) {
                                /* Get the next argument from the stack. */
                                arg = (msc_arg **)apr_array_pop(sorted_args);
                                if (arg == NULL) sanitize = 0; /* We're done sanitising. */
                                else {
                                    /* Continue with sanitation to process the
                                     * retrieved argument.
                                     */
                                    sanitize = 1;
                                    sanitize_offset = (*arg)->value_origin_offset;
                                    sanitize_length = (*arg)->value_origin_len;
                                }
                            }

                            if (sanitize) {
                                /* Check if the data we want to sanitize is
                                 * stored in the current chunk.
                                 */
                                if (chunk_offset + chunk->length > sanitize_offset) {
                                    unsigned int soff; /* data offset within chunk */
                                    unsigned int len;  /* amount in this chunk to sanitize */

                                    soff = sanitize_offset - chunk_offset;

                                    if (soff + sanitize_length <= chunk->length) {
                                        /* The entire argument resides in the current chunk. */
                                        len = sanitize_length;
                                        sanitize = 2; /* Get another parameter to sanitize. */
                                    } else {
                                        /* Some work to do here but we'll need to seek
                                         * another chunk.
                                         */
                                        len = chunk->length - soff;
                                        sanitize_offset += len;
                                        sanitize_length -= len;
                                        sanitize = 1; /* It's OK to go to the next chunk. */
                                    }

                                    /* Yes, we actually write over the original data.
                                     * We shouldn't be needing it any more.
                                     */
                                    if (soff + len <= chunk->length) { /* double check */
                                        memset((char *)chunk->data + soff, '*', len);
                                    }
                                }
                            }
                        }

                        /* Write the sanitized chunk to the log
                         * and advance to the next chunk. */
                        sec_auditlog_write(msr, chunk->data, chunk->length);
                        chunk_offset += chunk->length;
                    }

                    if (rc <= 0) {
                        break;
                    }
                }

                if (rc < 0) {
                    msr_log(msr, 1, "Audit log: %s", my_error_msg);
                }

                modsecurity_request_body_retrieve_end(msr);
            }
        }
    }

    /* AUDITLOG_PART_FAKE_REQUEST_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_FAKE_REQUEST_BODY) != NULL) {
        if ((msr->msc_reqbody_read)&&(msr->mpd != NULL)) {
            char *buffer = NULL;

            buffer = multipart_reconstruct_urlencoded_body_sanitize(msr);
            if (buffer == NULL) {
                msr_log(msr, 1, "Audit log: Failed to reconstruct request body.");
            } else {
                text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_FAKE_REQUEST_BODY);
                sec_auditlog_write(msr, text, strlen(text));
                sec_auditlog_write(msr, buffer, strlen(buffer));
            }
        }
    }

    /* AUDITLOG_PART_A_RESPONSE_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_A_RESPONSE_HEADERS) != NULL) {
        text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_A_RESPONSE_HEADERS);
        sec_auditlog_write(msr, text, strlen(text));

        /* There are no response headers (or the status line) in HTTP 0.9 */
        if (msr->response_headers_sent) {
            if (msr->status_line != NULL && msr->status_line[0] != '\0') {
                text = apr_psprintf(msr->mp, "%s %s\n", msr->response_protocol,
                        msr->status_line);
            } else {
                text = apr_psprintf(msr->mp, "%s %u\n", msr->response_protocol,
                        msr->response_status);
            }
            sec_auditlog_write(msr, text, strlen(text));

            /* Output headers */

            arr = apr_table_elts(msr->response_headers);
            te = (apr_table_entry_t *)arr->elts;

            tarr_pattern = apr_table_elts(msr->pattern_to_sanitize);
            telts_pattern = (const apr_table_entry_t*)tarr_pattern->elts;

            for (i = 0; i < arr->nelts; i++) {
                sanitized_partial = 0;
                sanitize_matched = 0;
                text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
                if (apr_table_get(msr->response_headers_to_sanitize, te[i].key) != NULL) {
                    buf = apr_psprintf(msr->mp, "%s",text+strlen(te[i].key)+2);

                    for ( k = 0; k < tarr_pattern->nelts; k++)  {
                        if(strncmp(telts_pattern[k].key,te[i].key,strlen(te[i].key)) ==0 ) {
                            mparm = (msc_parm *)telts_pattern[k].val;
                            if(mparm->pad_1 == -1)
                                sanitize_matched = 1;
                            pat = strstr(buf,mparm->value);
                            if (pat != NULL)    {
                                j = strlen(mparm->value);
                                arg_min = j;
                                arg_max = 1;
                                while((*pat != '\0')&&(j--)) {
                                    if(arg_max > mparm->pad_2)  {
                                        int off = strlen(mparm->value) - arg_max;
                                        int pos = mparm->pad_1-1;
                                        if(off > pos)    {
                                            *pat = '*';
                                        }
                                    }
                                    arg_max++;
                                    arg_min--;
                                    pat++;
                                }
                                sanitized_partial = 1;
                            }
                        }
                    }

                    if(sanitized_partial == 1 && sanitize_matched == 0)  {
                        text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, buf);
                    } else {
                        memset(text + strlen(te[i].key) + 2, '*', strlen(te[i].val));
                    }
                }
                sec_auditlog_write(msr, text, strlen(text));
            }
        }
    }

    apr_table_clear(msr->pattern_to_sanitize);

    /* AUDITLOG_PART_RESPONSE_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_RESPONSE_BODY) != NULL) {
        if (msr->resbody_data != NULL) {
            text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_RESPONSE_BODY);
            sec_auditlog_write(msr, text, strlen(text));
            sec_auditlog_write(msr, msr->resbody_data, msr->resbody_length);
            wrote_response_body = 1;
        }
    }

    /* AUDITLOG_PART_TRAILER */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_TRAILER) != NULL) {
        apr_time_t now = apr_time_now();
        text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_TRAILER);
        sec_auditlog_write(msr, text, strlen(text));

        /* Messages */
        for(i = 0; i < msr->alerts->nelts; i++) {
            text = apr_psprintf(msr->mp, "Message: %s\n", ((char **)msr->alerts->elts)[i]);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Apache error messages */
        for(i = 0; i < msr->error_messages->nelts; i++) {
            error_message_t *em = (((error_message_t **)msr->error_messages->elts)[i]);
            text = apr_psprintf(msr->mp, "Apache-Error: %s\n",
                format_error_log_message(msr->mp, em));
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Action */
        if (msr->was_intercepted) {
            text = apr_psprintf(msr->mp, "Action: Intercepted (phase %d)\n", msr->intercept_phase);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Apache-Handler */
#ifdef LOG_NO_HANDLER
	if (msr->txcfg->debuglog_level >= 9)
#endif
	if (msr->r->handler != NULL) {
            text = apr_psprintf(msr->mp, "Apache-Handler: %s\n", msr->r->handler);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Stopwatch; left in for compatibility reasons */
#ifdef LOG_NO_STOPWATCH
	if (msr->txcfg->debuglog_level >= 9) {
#endif
        text = apr_psprintf(msr->mp, "Stopwatch: %" APR_TIME_T_FMT " %" APR_TIME_T_FMT " (- - -)\n",
            msr->request_time, (now - msr->request_time));
        sec_auditlog_write(msr, text, strlen(text));

        /* Stopwatch2 */
        {
            char *perf_all = format_all_performance_variables(msr, msr->mp);

            text = apr_psprintf(msr->mp, "Stopwatch2: %" APR_TIME_T_FMT " %" APR_TIME_T_FMT
                "; %s\n", msr->request_time, (now - msr->request_time), perf_all);
            sec_auditlog_write(msr, text, strlen(text));
        }
#ifdef LOG_NO_STOPWATCH
        }
#endif

        /* Our response body does not contain chunks */
        /* ENH Only write this when the output was chunked. */
        /* ENH Add info when request body was decompressed, dechunked too. */
#ifdef LOG_NO_DECHUNK
	if (msr->txcfg->debuglog_level >= 9)
#endif
        if (wrote_response_body) {
            text = apr_psprintf(msr->mp, "Response-Body-Transformed: Dechunked\n");
            sec_auditlog_write(msr, text, strlen(text));
        }

#ifdef LOG_NO_SERVER_CONTEXT
        if (msr->txcfg->debuglog_level >= 9) {
#endif
        sec_auditlog_write_producer_header(msr);

        /* Server */
        if (msr->server_software != NULL) {
            text = apr_psprintf(msr->mp, "Server: %s\n", msr->server_software);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Sanitised arguments */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->arguments_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                text = apr_psprintf(msr->mp, "Sanitised-Args: ");
                sec_auditlog_write(msr, text, strlen(text));
            }

            for(i = 0; i < tarr->nelts; i++) {
                msc_arg *arg = (msc_arg *)telts[i].val;
                text = apr_psprintf(msr->mp, "%s\"%s\"%s", ((i == 0) ? "" : ", "),
                    log_escape(msr->mp, arg->name), ((i == (tarr->nelts - 1)) ? ".\n" : ""));
                sec_auditlog_write(msr, text, strlen(text));
            }
        }

        /* Sanitised request headers */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->request_headers_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                text = apr_psprintf(msr->mp, "Sanitised-Request-Headers: ");
                sec_auditlog_write(msr, text, strlen(text));
            }

            for(i = 0; i < tarr->nelts; i++) {
                text = apr_psprintf(msr->mp, "%s\"%s\"%s", ((i == 0) ? "" : ", "),
                    log_escape(msr->mp, telts[i].key), ((i == (tarr->nelts - 1)) ? ".\n" : ""));
                sec_auditlog_write(msr, text, strlen(text));
            }
        }

        /* Sanitised response headers */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->response_headers_to_sanitize);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                text = apr_psprintf(msr->mp, "Sanitised-Response-Headers: ");
                sec_auditlog_write(msr, text, strlen(text));
            }

            for(i = 0; i < tarr->nelts; i++) {
                text = apr_psprintf(msr->mp, "%s\"%s\"%s", ((i == 0) ? "" : ", "),
                    log_escape(msr->mp, telts[i].key), ((i == (tarr->nelts - 1)) ? ".\n" : ""));
                sec_auditlog_write(msr, text, strlen(text));
            }
        }
#ifdef LOG_NO_SERVER_CONTEXT
	}
#endif

	/* Web application info. */
        if ( ((msr->txcfg->webappid != NULL)&&(strcmp(msr->txcfg->webappid, "default") != 0))
            || (msr->sessionid != NULL) || (msr->userid != NULL))
        {
            text = apr_psprintf(msr->mp, "WebApp-Info: \"%s\" \"%s\" \"%s\"\n",
                msr->txcfg->webappid == NULL ? "-" : log_escape(msr->mp, msr->txcfg->webappid),
                msr->sessionid == NULL ? "-" : log_escape(msr->mp, msr->sessionid),
                msr->userid == NULL ? "-" : log_escape(msr->mp, msr->userid));
            sec_auditlog_write(msr, text, strlen(text));
        }

        if ( ((msr->txcfg->sensor_id != NULL)&&(strcmp(msr->txcfg->sensor_id, "default") != 0)))
        {
            text = apr_psprintf(msr->mp, "Sensor-Id: \"%s\"\n",
                msr->txcfg->sensor_id == NULL ? "-" : log_escape(msr->mp, msr->txcfg->sensor_id)),
            sec_auditlog_write(msr, text, strlen(text));
        }


        if (msr->txcfg->is_enabled > 0) {
            text = apr_psprintf(msr->mp, "Engine-Mode: \"%s\"\n",
                msr->txcfg->is_enabled == 1 ? "DETECTION_ONLY" : "ENABLED"),
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Rule performance time */
        if(msr->txcfg->max_rule_time > 0)   {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->perf_rules);
            telts = (const apr_table_entry_t*)tarr->elts;

            if (tarr->nelts > 0) {
                text = apr_psprintf(msr->mp, "Rules-Performance-Info: ");
                sec_auditlog_write(msr, text, strlen(text));
            }

            for(i = 0; i < tarr->nelts; i++) {
                text = apr_psprintf(msr->mp, "%s\"%s=%s\"%s", ((i == 0) ? "" : ", "),
                    log_escape(msr->mp, telts[i].key), log_escape(msr->mp, telts[i].val), ((i == (tarr->nelts - 1)) ? ".\n" : ""));
                sec_auditlog_write(msr, text, strlen(text));
            }
        }
    }

    /* AUDITLOG_PART_UPLOADS */
    if ((strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_UPLOADS) != NULL) && (msr->mpd != NULL)) {
        multipart_part **parts = NULL;
        unsigned int total_size = 0;
        int cfiles = 0;

        text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_UPLOADS);
        sec_auditlog_write(msr, text, strlen(text));

        parts = (multipart_part **)msr->mpd->parts->elts;
        for(cfiles = 0; cfiles < msr->mpd->parts->nelts; cfiles++) {
            if (parts[cfiles]->type == MULTIPART_FILE) {
                if(parts[cfiles]->filename != NULL) {
                    text = apr_psprintf(msr->mp, "%d,%u,\"%s\",\"%s\"\n", cfiles+1, parts[cfiles]->tmp_file_size, log_escape(msr->mp, parts[cfiles]->filename), log_escape(msr->mp, parts[cfiles]->content_type ? parts[cfiles]->content_type : "<Unknown ContentType>"));
                    sec_auditlog_write(msr, text, strlen(text));
                    total_size += parts[cfiles]->tmp_file_size;
                }
            }
        }
        text = apr_psprintf(msr->mp, "Total,%u\n", total_size);
        sec_auditlog_write(msr, text, strlen(text));
    }

    /* AUDITLOG_PART_MATCHEDRULES */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_MATCHEDRULES) != NULL) {
        text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_MATCHEDRULES);
        sec_auditlog_write(msr, text, strlen(text));

        /* Matched Rules */

        for(i = 0; i < msr->matched_rules->nelts; i++) {
            rule = ((msre_rule **)msr->matched_rules->elts)[i];
            if ((rule != NULL) && (rule->actionset != NULL) && rule->actionset->is_chained && (rule->chain_starter == NULL)) {
                text = apr_psprintf(msr->mp, "%s\n", rule->unparsed);
                sec_auditlog_write(msr, text, strlen(text));
                do {
                    if (rule->ruleset != NULL)   {

                        next_rule = return_chained_rule(rule,msr);

                        if (next_rule != NULL)  {

                            present = chained_is_matched(msr,next_rule);

                            if (present == 0)   {
                                text = apr_psprintf(msr->mp, "#%s\n",next_rule->unparsed);
                            } else  {
                                text = apr_psprintf(msr->mp, "%s\n",next_rule->unparsed);
                                i++;
                            }
                            sec_auditlog_write(msr, text, strlen(text));
                        }
                    }
                    rule = next_rule;
                } while (rule != NULL && rule->actionset != NULL && rule->actionset->is_chained);
                text = apr_psprintf(msr->mp, "\n");
                sec_auditlog_write(msr, text, strlen(text));
            } else  {
                if ((rule != NULL) && (rule->actionset != NULL) && !rule->actionset->is_chained && (rule->chain_starter == NULL)) {
                    text = apr_psprintf(msr->mp, "%s\n\n", rule->unparsed);
                    sec_auditlog_write(msr, text, strlen(text));
                }
            }
        }
    }
    /* AUDITLOG_PART_ENDMARKER */
    text = apr_psprintf(msr->mp, "\n--%s-%c--\n", msr->new_auditlog_boundary, AUDITLOG_PART_ENDMARKER);
    sec_auditlog_write(msr, text, strlen(text));

    /* Return here if we were writing to a serial log
     * as it does not need an index file.
     */
    if (msr->txcfg->auditlog_type != AUDITLOG_CONCURRENT) {
        sec_auditlog_write(msr, "\n", 1);

        /* Unlock the mutex we used to serialise access to the audit log file. */
        rc = apr_global_mutex_unlock(msr->modsecurity->auditlog_lock);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to unlock global mutex: %s",
                    get_apr_error(msr->mp, rc));
        }

        return;
    }

    /* From here on only concurrent-style processing. */

    /* File handle might already be closed after write failure. */
    if (msr->new_auditlog_fd) {
        apr_file_close(msr->new_auditlog_fd);
    }

    /* Write an entry to the index file */

    /* Calculate hash of the entry. */
    apr_md5_final(md5hash, &msr->new_auditlog_md5ctx);

    str2 = apr_psprintf(msr->mp, "%s %d %d md5:%s", msr->new_auditlog_filename, 0,
            msr->new_auditlog_size, bytes2hex(msr->mp, md5hash, 16));
    if (str2 == NULL) return;

    /* We do not want the index line to be longer than 3980 bytes. */
    limit = 3980;
    was_limited = 0;

    /* If we are logging to a pipe we need to observe and
     * obey the pipe atomic write limit - PIPE_BUF. For
     * more details see the discussion in sec_guardian_logger code.
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
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Audit Log: Writing %" APR_SIZE_T_FMT " bytes to primary concurrent index", nbytes);
    }
    apr_file_write_full(msr->txcfg->auditlog_fd, text, nbytes, &nbytes_written);

    /* Write to the secondary audit log if we have one */
    if (msr->txcfg->auditlog2_fd != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Audit Log: Writing %" APR_SIZE_T_FMT " bytes to secondary concurrent index", nbytes);
        }
        apr_file_write_full(msr->txcfg->auditlog2_fd, text, nbytes, &nbytes_written);
    }
}

/*
 * Handler for audit log writers.
 */
void sec_audit_logger(modsec_rec *msr) {
    #ifdef WITH_YAJL
    if (msr->txcfg->auditlog_format == AUDITLOGFORMAT_JSON) {
        sec_audit_logger_json(msr);
    } else {
    #endif
        sec_audit_logger_native(msr);
    #ifdef WITH_YAJL
    }
    #endif
}
