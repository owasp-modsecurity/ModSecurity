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
#include "msc_logging.h"
#include "httpd.h"
#include "apr_strings.h"
#include "apr_global_mutex.h"
#include "msc_util.h"


/**
 * Write the supplied data to the audit log (if the FD is ready), update
 * the size counters, update the hash context.
 */
static int sec_auditlog_write(modsec_rec *msr, const char *data, unsigned int len) {
    apr_size_t nbytes_written, nbytes = len;
    apr_status_t rc;

    if ((msr->new_auditlog_fd == NULL)||(data == NULL)) return -1;

    rc = apr_file_write_full(msr->new_auditlog_fd, data, nbytes, &nbytes_written);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Audit log: Failed writing (requested %" APR_SIZE_T_FMT
            " bytes, written %" APR_SIZE_T_FMT ")", nbytes, nbytes_written);
        return -1;
    }

    /* Note the following will only take into account the actual
     * amount of bytes we've written.
     */
    msr->new_auditlog_size += nbytes_written;
    apr_md5_update(&msr->new_auditlog_md5ctx, data, nbytes_written);

    return rc;
}

/**
 * Construct a log line in the vcombinedus format (see below).
 */
char *construct_log_vcombinedus(modsec_rec *msr) {
    const char *local_user, *remote_user;
    const char *referer, *user_agent, *uniqueid;
    const char *sessionid;

    /* remote log name */
    if (msr->remote_user == NULL) remote_user = "-";
    else remote_user = msr->remote_user;

    /* authenticated user */
    if (msr->local_user == NULL) local_user = "-";
    else local_user = msr->local_user;

    /* unique id */
    uniqueid = msr->txid;
    if (uniqueid == NULL) uniqueid = "-";

    /* referer */
    referer = "-";
    /* Logging Referer is a waste of space.
    referer = (char *)apr_table_get(msr->request_headers, "Referer");
    if (referer == NULL) referer = "-";
    */

    /* user agent */
    user_agent = "-";
    /* Logging User-Agent is a waste of space too.
    user_agent = (char *)apr_table_get(msr->request_headers, "User-Agent");
    if (user_agent == NULL) user_agent = "-";
    */

    /* sessionid */
    sessionid = (msr->sessionid == NULL ? "-" : msr->sessionid);

    return apr_psprintf(msr->mp, "%s %s %s %s [%s] \"%s\" %i %" APR_OFF_T_FMT " \"%s\" \"%s\" %s \"%s\"",
        log_escape_nq(msr->mp, msr->hostname), msr->remote_addr, log_escape_nq(msr->mp, remote_user),
        log_escape_nq(msr->mp, local_user), current_logtime(msr->mp),
        ((msr->request_line == NULL) ? "" : log_escape(msr->mp, msr->request_line)),
        msr->response_status, msr->bytes_sent, log_escape(msr->mp, referer),
        log_escape(msr->mp, user_agent), log_escape(msr->mp, uniqueid), sessionid);
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
        msr_log(msr, 1, "GuardianLog: Atomic pipe write size too small: %i", PIPE_BUF);
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
            msr_log(msr, 1, "GuardianLog: Atomic pipe write size too small: %i.", PIPE_BUF);
            return NULL;
        }

        /* use what's left for the request line */
        if ((int)strlen(the_request) > limit) {
            the_request[limit] = '\0';
            msr_log(msr, 9, "GuardianLog: Reduced the_request to %i bytes.", limit);
        }
    } else {
        /* Yay! We have enough space! */
        *was_limited = 0;
    }

    return apr_psprintf(msr->mp, "%s %s %s %s [%s] \"%s\" %i %s \"%s\" \"%s\" %s \"%s\"",
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

    while((c = *t++) != '\0') {
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
    unsigned long data = rand();
    /* Do note that I tried using apr_generate_random_bytes but it turned
     * out to be terribly slow for some reason. Needs further investigation.
     */
    return bytes2hex(r->pool, (void *)&data, 4);
}

/**
 * Sanitises the request line by removing the parameters
 * that have been marked as sensitive.
 */
static void sanitise_request_line(modsec_rec *msr) {
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    int i;
    char *qspos;

    /* Locate the query string. */
    qspos = strstr(msr->request_line, "?");
    if (qspos == NULL) return;
    qspos++;

    /* Loop through the list of sensitive parameters. */
    tarr = apr_table_elts(msr->arguments_to_sanitise);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msc_arg *arg = (msc_arg *)telts[i].val;
        /* Only look at the parameters that appeared in the query string. */
        if (strcmp(arg->origin, "QUERY_STRING") == 0) {
            char *p;
            int j;

            /* Go to the beginning of the parameter. */
            p = qspos;
            j = arg->value_origin_offset;
            while((*p != '\0')&&(j--)) p++;
            if (*p == '\0') {
                msr_log(msr, 1, "Unable to sanitise variable \"%s\" at offset %i of QUERY_STRING"
                    "because the request line is too short.",
                    log_escape_ex(msr->mp, arg->name, arg->name_len),
                    arg->value_origin_offset);
                continue;
            }

            /* Write over the value. */
            j = arg->value_origin_len;
            while((*p != '\0')&&(j--)) {
                *p++ = '*';
            }
            if (*p == '\0') {
                msr_log(msr, 1, "Unable to sanitise variable \"%s\" at offset %i (size %i) "
                    "of QUERY_STRING because the request line is too short.",
                    log_escape_ex(msr->mp, arg->name, arg->name_len),
                    arg->value_origin_offset, arg->value_origin_len);
                continue;
            }
        }
    }
}

/**
 * Produce an audit log entry.
 */
void sec_audit_logger(modsec_rec *msr) {
    const apr_array_header_t *arr = NULL;
    apr_table_entry_t *te = NULL;
    char *str1 = NULL, *str2 = NULL, *text = NULL;
    apr_size_t nbytes, nbytes_written;
    unsigned char md5hash[APR_MD5_DIGESTSIZE];
    int was_limited = 0;
    int wrote_response_body = 0;
    char *entry_filename, *entry_basename;
    apr_status_t rc;
    int i, limit;

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
         * TODO Actually there is something we can do! We will make
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
        rc = apr_dir_make_recursive(entry_basename, CREATEMODE_DIR, msr->mp);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Audit log: Failed to create subdirectories: %s (%s)",
                entry_basename, get_apr_error(msr->mp, rc));
            return;
        }

        rc = apr_file_open(&msr->new_auditlog_fd, entry_filename,
            APR_WRITE | APR_TRUNCATE | APR_CREATE | APR_BINARY | APR_FILE_NOCLEANUP,
            CREATEMODE, msr->mp);
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

    text = apr_psprintf(msr->mp, "--%s-A--\n", msr->new_auditlog_boundary);
    sec_auditlog_write(msr, text, strlen(text));

    /* Format: time transaction_id remote_addr remote_port local_addr local_port */

    text = apr_psprintf(msr->mp, "[%s] %s %s %i %s %i",
        current_logtime(msr->mp), msr->txid, msr->remote_addr, msr->remote_port,
        msr->local_addr, msr->local_port);
    sec_auditlog_write(msr, text, strlen(text));


    /* AUDITLOG_PART_REQUEST_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_REQUEST_HEADERS) != NULL) {
        text = apr_psprintf(msr->mp, "\n--%s-B--\n", msr->new_auditlog_boundary);
        sec_auditlog_write(msr, text, strlen(text));

        sanitise_request_line(msr);

        sec_auditlog_write(msr, msr->request_line, strlen(msr->request_line));
        sec_auditlog_write(msr, "\n", 1);

        arr = apr_table_elts(msr->request_headers);
        te = (apr_table_entry_t *)arr->elts;
        for (i = 0; i < arr->nelts; i++) {
            text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
            /* Do we need to sanitise this request header? */
            if (apr_table_get(msr->request_headers_to_sanitise, te[i].key) != NULL) {
                /* Yes, sanitise it. */
                memset(text + strlen(te[i].key) + 2, '*', strlen(te[i].val));
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
            int sanitise = 0; /* IMP1 Use constants for "sanitise" values. */

            sorted_args = apr_array_make(msr->mp, 25, sizeof(const msc_arg *));

            /* First we need to sort the arguments that need to be
             * sanitised in descending order (we are using a stack structure
             * to store then so the order will be ascending when we start
             * popping them out). This is because we will
             * be reading the request body sequentially and must
             * sanitise it as we go.
             */

            for(;;) {
                nextarg = NULL;

                /* Find the next largest offset (excluding
                 * the ones we've used up already).
                 */
                tarr = apr_table_elts(msr->arguments_to_sanitise);
                telts = (const apr_table_entry_t*)tarr->elts;
                for(i = 0; i < tarr->nelts; i++) {
                    msc_arg *arg = (msc_arg *)telts[i].val;
                    if (strcmp(arg->origin, "BODY") != 0) continue;

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

                sanitise = 2; /* Means time to pop the next argument out. */
                last_offset = offset;
                offset = 0;
                { /* IMP1 Fix this ugly bit here. */
                    msc_arg **x = apr_array_push(sorted_args);
                    *x = nextarg;
                }
            }

            /* Now start retrieving the body chunk by chunk and
             * sanitise data in pieces.
             */

            rc = modsecurity_request_body_retrieve_start(msr);
            if (rc < 0) {
                msr_log(msr, 1, "Audit log: Failed retrieving request body.");
            } else {
                msc_data_chunk *chunk = NULL;
                unsigned int chunk_offset = 0;
                unsigned int sanitise_offset = 0;
                unsigned int sanitise_length = 0;

                text = apr_psprintf(msr->mp, "\n--%s-C--\n", msr->new_auditlog_boundary);
                sec_auditlog_write(msr, text, strlen(text));

                for(;;) {
                    rc = modsecurity_request_body_retrieve(msr, &chunk, -1);
                    if (chunk != NULL) {
                        /* Anything greater than 1 means we have more data to sanitise. */
                        while (sanitise > 1) {
                            msc_arg **arg = NULL;

                            if (sanitise == 2) {
                                /* Get the next argument from the stack. */
                                arg = (msc_arg **)apr_array_pop(sorted_args);
                                if (arg == NULL) sanitise = 0; /* We're done sanitising. */
                                else {
                                    /* Continue with sanitation to process the
                                     * retrieved argument.
                                     */
                                    sanitise = 1;
                                    sanitise_offset = (*arg)->value_origin_offset;
                                    sanitise_length = (*arg)->value_origin_len;
                                }
                            }

                            if (sanitise) {
                                /* Check if the data we want to sanitise is
                                 * stored in the current chunk.
                                 */
                                if (chunk_offset + chunk->length > sanitise_offset) {
                                    unsigned int soff; /* data offset within chunk */
                                    unsigned int len;  /* amount in this chunk to sanitise */

                                    soff = sanitise_offset - chunk_offset;
                                       
                                    if (soff + sanitise_length <= chunk->length) {
                                        /* The entire argument resides in the current chunk. */
                                        len = sanitise_length;
                                        sanitise = 2; /* Get another parameter to sanitise. */
                                    } else {
                                        /* Some work to do here but we'll need to seek
                                         * another chunk.
                                         */
                                        len = chunk->length - soff;
                                        sanitise_offset += len;
                                        sanitise_length -= len;
                                        sanitise = 1; /* It's OK to go to the next chunk. */
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

                        /* Write the sanitised chunk to the log
                         * and advance to the next chunk. */
                        sec_auditlog_write(msr, chunk->data, chunk->length);
                        chunk_offset += chunk->length;
                    }

                    if (rc <= 0) break;
                }

                modsecurity_request_body_retrieve_end(msr);
            }
        }
    }

    /* AUDITLOG_PART_FAKE_REQUEST_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_FAKE_REQUEST_BODY) != NULL) {
        if ((msr->msc_reqbody_read)&&(msr->mpd != NULL)) {
            char *buffer = NULL;

            buffer = multipart_reconstruct_urlencoded_body_sanitise(msr);
            if (buffer == NULL) {
                msr_log(msr, 1, "Audit log: Failed to reconstruct request body.");
            } else {
                text = apr_psprintf(msr->mp, "\n--%s-I--\n", msr->new_auditlog_boundary);
                sec_auditlog_write(msr, text, strlen(text));
                sec_auditlog_write(msr, buffer, strlen(buffer));
            }
        }
    }

    /* AUDITLOG_PART_A_RESPONSE_HEADERS */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_A_RESPONSE_HEADERS) != NULL) {
        text = apr_psprintf(msr->mp, "\n--%s-F--\n", msr->new_auditlog_boundary);
        sec_auditlog_write(msr, text, strlen(text));

        /* There are no response headers (or the status line) in HTTP 0.9 */
        if (msr->response_headers_sent) {
            if (msr->status_line != NULL) {
                text = apr_psprintf(msr->mp, "%s %s\n", msr->response_protocol,
                    msr->status_line);
            } else {
                text = apr_psprintf(msr->mp, "%s %i\n", msr->response_protocol,
                    msr->response_status);
            }
            sec_auditlog_write(msr, text, strlen(text));

            /* Output headers */

            arr = apr_table_elts(msr->response_headers);
            te = (apr_table_entry_t *)arr->elts;
            for (i = 0; i < arr->nelts; i++) {
                text = apr_psprintf(msr->mp, "%s: %s\n", te[i].key, te[i].val);
                /* Do we need to sanitise this response header? */
                if (apr_table_get(msr->response_headers_to_sanitise, te[i].key) != NULL) {
                    /* Yes, sanitise it. */
                    memset(text + strlen(te[i].key) + 2, '*', strlen(te[i].val));
                }
                sec_auditlog_write(msr, text, strlen(text));
            }
        }
    }
    
    /* AUDITLOG_PART_RESPONSE_BODY */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_RESPONSE_BODY) != NULL) {
        if (msr->resbody_data != NULL) {
            text = apr_psprintf(msr->mp, "\n--%s-E--\n", msr->new_auditlog_boundary);
            sec_auditlog_write(msr, text, strlen(text));
            sec_auditlog_write(msr, msr->resbody_data, msr->resbody_length);
            wrote_response_body = 1;
        }
    }

    /* AUDITLOG_PART_TRAILER */

    if (strchr(msr->txcfg->auditlog_parts, AUDITLOG_PART_TRAILER) != NULL) {
        apr_time_t now = apr_time_now();

        text = apr_psprintf(msr->mp, "\n--%s-H--\n", msr->new_auditlog_boundary);
        sec_auditlog_write(msr, text, strlen(text));

        /* Messages */
        for(i = 0; i < msr->alerts->nelts; i++) {
            text = apr_psprintf(msr->mp, "Message: %s\n", ((char **)msr->alerts->elts)[i]);
            sec_auditlog_write(msr, text, strlen(text));
        }
    
        /* Apache error messages */
        for(i = 0; i < msr->error_messages->nelts; i++) {
            error_message *em = (((error_message**)msr->error_messages->elts)[i]);
            text = apr_psprintf(msr->mp, "Apache-Error: %s\n",
                format_error_log_message(msr->mp, em));
            sec_auditlog_write(msr, text, strlen(text));
        }
        
        /* Action */
        if (msr->was_intercepted) {
            text = apr_psprintf(msr->mp, "Action: Intercepted (phase %i)\n", msr->intercept_phase);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Apache-Handler */
        if (msr->r->handler != NULL) {
            text = apr_psprintf(msr->mp, "Apache-Handler: %s\n", msr->r->handler);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Processing times */
        if (msr->time_checkpoint_1 == 0) {
            text = apr_psprintf(msr->mp, "Stopwatch: %" APR_TIME_T_FMT " %" APR_TIME_T_FMT
                " (- - -)\n", (msr->request_time), (now - msr->request_time));
        } else {
            char sw_str2[101] = "-";
            char sw_str3[101] = "-";

            if (msr->time_checkpoint_2 != 0) {
                apr_snprintf(sw_str2, sizeof(sw_str2), "%" APR_TIME_T_FMT,
                    (msr->time_checkpoint_2 - msr->request_time));
            }

            if (msr->time_checkpoint_3 != 0) {
                apr_snprintf(sw_str3, sizeof(sw_str3), "%" APR_TIME_T_FMT,
                    (msr->time_checkpoint_3 - msr->request_time));
            }

            text = apr_psprintf(msr->mp, "Stopwatch: %" APR_TIME_T_FMT
                " %" APR_TIME_T_FMT " (%" APR_TIME_T_FMT
                "%s %s %s)\n",
                (msr->request_time), (now - msr->request_time),
                (msr->time_checkpoint_1 - msr->request_time),
                ((msr->msc_reqbody_read == 0) ? "" : "*"),
                sw_str2, sw_str3
                );
        }

        sec_auditlog_write(msr, text, strlen(text));
        
        /* Our response body does not contain chunks */
        /* ENH Only write this when the output was chunked. */
        /* ENH Add info when request body was decompressed, dechunked too. */
        if (wrote_response_body) {
            text = apr_psprintf(msr->mp, "Response-Body-Transformed: Dechunked\n");
            sec_auditlog_write(msr, text, strlen(text));
        }
        
        /* Producer */
        text = apr_psprintf(msr->mp, "Producer: %s\n", MODULE_NAME_FULL);
        sec_auditlog_write(msr, text, strlen(text));
        
        /* Server */
        if (msr->server_software != NULL) {
            text = apr_psprintf(msr->mp, "Server: %s\n", msr->server_software);
            sec_auditlog_write(msr, text, strlen(text));
        }

        /* Sanitised arguments */
        {
            const apr_array_header_t *tarr;
            const apr_table_entry_t *telts;

            tarr = apr_table_elts(msr->arguments_to_sanitise);
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

            tarr = apr_table_elts(msr->request_headers_to_sanitise);
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

            tarr = apr_table_elts(msr->response_headers_to_sanitise);
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
    }


    /* AUDITLOG_PART_ENDMARKER */

    text = apr_psprintf(msr->mp, "\n--%s-Z--\n", msr->new_auditlog_boundary);
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
    
    apr_file_close(msr->new_auditlog_fd);

    /* Write an entry to the index file */

    /* Calculate hash of the entry. */
    apr_md5_final(md5hash, &msr->new_auditlog_md5ctx);

    str2 = apr_psprintf(msr->mp, "%s %i %i md5:%s", msr->new_auditlog_filename, 0,
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
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Audit Log: Writing %d bytes to primary concurrent index", nbytes);
    }
    apr_file_write_full(msr->txcfg->auditlog_fd, text, nbytes, &nbytes_written);

    /* Write to the secondary audit log if we have one */
    if (msr->txcfg->auditlog2_fd != NULL) {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Audit Log: Writing %d bytes to secondary concurrent index", nbytes);
        }
        apr_file_write_full(msr->txcfg->auditlog2_fd, text, nbytes, &nbytes_written);
    }
}
