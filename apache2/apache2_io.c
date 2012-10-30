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

#include <util_filter.h>

#include "modsecurity.h"
#include "apache2.h"
#include "msc_crypt.h"

/* -- Input filter -- */

#if 0
static void dummy_free_func(void *data) {}
#endif

/**
 * This request filter will forward the previously stored
 * request body further down the chain (most likely to the
 * processing module).
 */
apr_status_t input_filter(ap_filter_t *f, apr_bucket_brigade *bb_out,
        ap_input_mode_t mode, apr_read_type_e block, apr_off_t nbytes)
{
    modsec_rec *msr = (modsec_rec *)f->ctx;
    msc_data_chunk *chunk = NULL;
    apr_bucket *bucket;
    apr_status_t rc;
    char *my_error_msg = NULL;

    if (msr == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, f->r->server,
                "ModSecurity: Internal error in input filter: msr is null.");
        ap_remove_input_filter(f);
        return APR_EGENERAL;
    }

    /* Make sure we are using the current request */
    msr->r = f->r;

    if (msr->phase < PHASE_REQUEST_BODY) {
        msr_log(msr, 1, "Internal error: REQUEST_BODY phase incomplete for input filter in phase %d", msr->phase);
        return APR_EGENERAL;
    }

    if ((msr->if_status == IF_STATUS_COMPLETE)||(msr->if_status == IF_STATUS_NONE)) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Input forwarding already complete, skipping (f %pp, r %pp).", f, f->r);
        }
        ap_remove_input_filter(f);
        return ap_get_brigade(f->next, bb_out, mode, block, nbytes);
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Input filter: Forwarding input: mode=%d, block=%d, nbytes=%" APR_OFF_T_FMT
                " (f %pp, r %pp).", mode, block, nbytes, f, f->r);
    }

    if (msr->if_started_forwarding == 0) {
        msr->if_started_forwarding = 1;
        rc = modsecurity_request_body_retrieve_start(msr, &my_error_msg);
        if (rc == -1) {
            if (my_error_msg != NULL) {
                msr_log(msr, 1, "%s", my_error_msg);
            }
            return APR_EGENERAL;
        }
    }

    rc = modsecurity_request_body_retrieve(msr, &chunk, (unsigned int)nbytes, &my_error_msg);
    if (rc == -1) {
        if (my_error_msg != NULL) {
            msr_log(msr, 1, "%s", my_error_msg);
        }
        return APR_EGENERAL;
    }

    if (chunk && (!msr->txcfg->stream_inbody_inspection || (msr->txcfg->stream_inbody_inspection && msr->if_stream_changed == 0))) {
        /* Copy the data we received in the chunk */
        bucket = apr_bucket_heap_create(chunk->data, chunk->length, NULL,
                f->r->connection->bucket_alloc);

#if 0

        It would seem that we cannot prevent other filters in the chain
            from modifying data in-place. Hence we copy.

            if (chunk->is_permanent) {
                /* Do not make a copy of the data we received in the chunk. */
                bucket = apr_bucket_heap_create(chunk->data, chunk->length, dummy_free_func,
                        f->r->connection->bucket_alloc);
            } else {
                /* Copy the data we received in the chunk. */
                bucket = apr_bucket_heap_create(chunk->data, chunk->length, NULL,
                        f->r->connection->bucket_alloc);
            }

#endif

        if (bucket == NULL) return APR_EGENERAL;
        APR_BRIGADE_INSERT_TAIL(bb_out, bucket);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Forwarded %" APR_SIZE_T_FMT " bytes.", chunk->length);
        }
    } else if (msr->stream_input_data != NULL) {

        msr->if_stream_changed = 0;

        bucket = apr_bucket_heap_create(msr->stream_input_data, msr->stream_input_length, NULL,
                f->r->connection->bucket_alloc);

        if (msr->txcfg->stream_inbody_inspection)  {
            if(msr->stream_input_data != NULL) {
                free(msr->stream_input_data);
                msr->stream_input_data = NULL;
            }
        }

        if (bucket == NULL) return APR_EGENERAL;
        APR_BRIGADE_INSERT_TAIL(bb_out, bucket);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input stream filter: Forwarded %" APR_SIZE_T_FMT " bytes.", msr->stream_input_length);
        }

    }

    if (rc == 0) {
        modsecurity_request_body_retrieve_end(msr);

        bucket = apr_bucket_eos_create(f->r->connection->bucket_alloc);
        if (bucket == NULL) return APR_EGENERAL;
        APR_BRIGADE_INSERT_TAIL(bb_out, bucket);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Sent EOS.");
        }

        /* We're done */
        msr->if_status = IF_STATUS_COMPLETE;
        ap_remove_input_filter(f);

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Input forwarding complete.");
        }
    }

    return APR_SUCCESS;
}

/**
 * Reads request body from a client.
 */
apr_status_t read_request_body(modsec_rec *msr, char **error_msg) {
    request_rec *r = msr->r;
    unsigned int seen_eos;
    apr_bucket_brigade *bb_in;
    apr_bucket *bucket;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (msr->reqbody_should_exist != 1) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: This request does not have a body.");
        }
        return 0;
    }

    if (msr->txcfg->reqbody_access != 1) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Request body access not enabled.");
        }
        return 0;
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Input filter: Reading request body.");
    }

    if (modsecurity_request_body_start(msr, error_msg) < 0) {
        return -1;
    }

    seen_eos = 0;
    bb_in = apr_brigade_create(msr->mp, r->connection->bucket_alloc);
    if (bb_in == NULL) return -1;
    do {
        apr_status_t rc;

        rc = ap_get_brigade(r->input_filters, bb_in, AP_MODE_READBYTES, APR_BLOCK_READ, HUGE_STRING_LEN);
        if (rc != APR_SUCCESS) {
            /* NOTE Apache returns AP_FILTER_ERROR here when the request is
             *      too large and APR_EGENERAL when the client disconnects.
             */
            switch(rc) {
                case APR_EOF :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: %s", get_apr_error(msr->mp, rc));
                    return -6;
                case APR_TIMEUP :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: %s", get_apr_error(msr->mp, rc));
                    return -4;
                case AP_FILTER_ERROR :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: HTTP Error 413 - Request entity too large. (Most likely.)");
                    return -3;
                case APR_EGENERAL :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: Client went away.");
                    return -2;
                default :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: %s", get_apr_error(msr->mp, rc));
                    return -1;
            }
        }

        /* Loop through the buckets in the brigade in order
         * to extract the size of the data available.
         */
        for(bucket = APR_BRIGADE_FIRST(bb_in);
                bucket != APR_BRIGADE_SENTINEL(bb_in);
                bucket = APR_BUCKET_NEXT(bucket))
        {
            const char *buf;
            apr_size_t buflen;

            rc = apr_bucket_read(bucket, &buf, &buflen, APR_BLOCK_READ);
            if (rc != APR_SUCCESS) {
                *error_msg = apr_psprintf(msr->mp, "Failed reading input / bucket (%d): %s", rc, get_apr_error(msr->mp, rc));
                return -1;
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Input filter: Bucket type %s contains %" APR_SIZE_T_FMT " bytes.",
                        bucket->type->name, buflen);
            }

            /* Check request body limit (should only trigger on chunked requests). */
            if (msr->reqbody_length + buflen > (apr_size_t)msr->txcfg->reqbody_limit) {
                if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT)) {
                    *error_msg = apr_psprintf(msr->mp, "Request body is larger than the "
                            "configured limit (%ld).", msr->txcfg->reqbody_limit);
                    return -5;
                } else if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)) {

                    *error_msg = apr_psprintf(msr->mp, "Request body is larger than the "
                            "configured limit (%ld).", msr->txcfg->reqbody_limit);

                } else if ((msr->txcfg->is_enabled == MODSEC_DETECTION_ONLY) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)){

                    *error_msg = apr_psprintf(msr->mp, "Request body is larger than the "
                            "configured limit (%ld).", msr->txcfg->reqbody_limit);

                } else if ((msr->txcfg->is_enabled == MODSEC_DETECTION_ONLY) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT)){

                    *error_msg = apr_psprintf(msr->mp, "Request body is larger than the "
                            "configured limit (%ld).", msr->txcfg->reqbody_limit);

                } else  {

                    *error_msg = apr_psprintf(msr->mp, "Request body is larger than the "
                            "configured limit (%ld).", msr->txcfg->reqbody_limit);

                    return -5;
                }
            }

            if (msr->txcfg->stream_inbody_inspection == 1)   {
                msr->stream_input_length+=buflen;
                modsecurity_request_body_to_stream(msr, buf, buflen, error_msg);
            }

            msr->reqbody_length += buflen;

            if (buflen != 0) {
                int rcbs = modsecurity_request_body_store(msr, buf, buflen, error_msg);
                if (rcbs < 0) {
                    if (rcbs == -5) {
                        if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT)) {
                            *error_msg = apr_psprintf(msr->mp, "Request body no files data length is larger than the "
                                    "configured limit (%ld).", msr->txcfg->reqbody_no_files_limit);
                            return -5;
                        } else if ((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)) {
                            *error_msg = apr_psprintf(msr->mp, "Request body no files data length is larger than the "
                                    "configured limit (%ld).", msr->txcfg->reqbody_no_files_limit);
                        } else if ((msr->txcfg->is_enabled == MODSEC_DETECTION_ONLY) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_PARTIAL)) {
                            *error_msg = apr_psprintf(msr->mp, "Request body no files data length is larger than the "
                                    "configured limit (%ld).", msr->txcfg->reqbody_no_files_limit);
                        } else {
                            *error_msg = apr_psprintf(msr->mp, "Request body no files data length is larger than the "
                                    "configured limit (%ld).", msr->txcfg->reqbody_no_files_limit);
                            return -5;
                        }
                    }

                    if((msr->txcfg->is_enabled == MODSEC_ENABLED) && (msr->txcfg->if_limit_action == REQUEST_BODY_LIMIT_ACTION_REJECT))
                        return -1;
                }

            }

            if (APR_BUCKET_IS_EOS(bucket)) {
                seen_eos = 1;
            }
        }

        apr_brigade_cleanup(bb_in);
    } while(!seen_eos);

    // TODO: Why ignore the return code here?
    modsecurity_request_body_end(msr, error_msg);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Input filter: Completed receiving request body (length %" APR_SIZE_T_FMT ").",
                msr->reqbody_length);
    }

    msr->if_status = IF_STATUS_WANTS_TO_RUN;

    return 1;
}


/* -- Output filter -- */

/**
 * Examines the configuration and the response MIME type
 * in order to determine whether output buffering should
 * run or not.
 */
static int output_filter_should_run(modsec_rec *msr, request_rec *r) {
    char *content_type = NULL;

    /* Check configuration. */
    if (msr->txcfg->resbody_access != 1) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Output filter: Response body buffering is not enabled.");
        }

        return 0;
    }

    /* Check MIME type. */

    if ((msr->txcfg->of_mime_types == NULL)||(msr->txcfg->of_mime_types == NOT_SET_P)) {
        msr_log(msr, 1, "Output filter: MIME type structures corrupted (internal error).");
        return -1;
    }

    if (r->content_type != NULL) {
        char *p = NULL;

        content_type = apr_pstrdup(msr->mp, r->content_type);
        if (content_type == NULL) {
            msr_log(msr, 1, "Output filter: Failed to allocate memory for content type.");
            return -1;
        }

        /* Hide the character encoding information
         * if present. Sometimes the content type header
         * looks like this "text/html; charset=xyz" ...
         */
        p = strstr(content_type, ";");
        if (p != NULL) {
            *p = '\0';
        }

        strtolower_inplace((unsigned char *)content_type);

        if (strcmp(content_type, "text/html") == 0) {
            /* Useful information to have should we later
             * decide to do something with the HTML output.
             */
            msr->resbody_contains_html = 1;
        }
    } else {
        content_type = "null";
    }

    if (apr_table_get(msr->txcfg->of_mime_types, content_type) != NULL) return 1;

    msr_log(msr, 4, "Output filter: Not buffering response body for unconfigured MIME type \"%s\".", content_type);

    return 0;
}

/**
 * Initialises the output filter.
 */
static apr_status_t output_filter_init(modsec_rec *msr, ap_filter_t *f,
        apr_bucket_brigade *bb_in)
{
    request_rec *r = f->r;
    const char *s_content_length = NULL;
    apr_status_t rc;

    msr->of_brigade = apr_brigade_create(msr->mp, f->c->bucket_alloc);
    if (msr->of_brigade == NULL) {
        msr_log(msr, 1, "Output filter: Failed to create brigade.");
        return -1;
    }
    msr->of_status = OF_STATUS_IN_PROGRESS;

    rc = output_filter_should_run(msr, r);
    if (rc < 0) return -1; /* output_filter_should_run() generates error msg */
    if (rc == 0) return 0;

    /* Do not check the output limit if we are willing to
     * process partial response bodies.
     */

    if (msr->txcfg->of_limit_action == RESPONSE_BODY_LIMIT_ACTION_PARTIAL) {
        return 1;
    }

    /* Look up the Content-Length header to see if we know
     * the amount of data coming our way. If we do and if
     * it's too much we might want to stop processing right here.
     */
    s_content_length = apr_table_get(r->headers_out, "Content-Length");
    if (s_content_length == NULL) {
        /* Try this too, mod_cgi seems to put headers there. */
        s_content_length = apr_table_get(r->err_headers_out, "Content-Length");
    }

    if (s_content_length != NULL) {
        long int len;

        len = strtol(s_content_length, NULL, 10);
        if ((len == LONG_MIN)||(len == LONG_MAX)||(len < 0)||(len >= 1073741824)) {
            msr_log(msr, 1, "Output filter: Invalid Content-Length: %s", log_escape_nq(r->pool,
                        (char *)s_content_length));
            return -1; /* Invalid. */
        }

        if (len == 0) {
            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Output filter: Skipping response since Content-Length is zero.");
            }

            return 0;
        }

        if (len > msr->txcfg->of_limit) {
            msr_log(msr, 1, "Output filter: Content-Length (%s) over the limit (%ld).",
                    log_escape_nq(r->pool, (char *)s_content_length), msr->txcfg->of_limit);
            msr->outbound_error = 1;
            return -2; /* Over the limit. */
        }
    }

    return 1;
}

/**
 * Send the accumulated content down the filter stream
 * and to the client.
 */
static apr_status_t send_of_brigade(modsec_rec *msr, ap_filter_t *f) {
    apr_status_t rc;

    rc = ap_pass_brigade(f->next, msr->of_brigade);
    if (rc != APR_SUCCESS) {
        /* TODO: These need to move to flags in 2.6.  For now log them
         * at level 4 so that they are not confusing users.
         */
        int log_level = 4;

        if (msr->txcfg->debuglog_level >= log_level) {
            switch(rc) {
                case AP_NOBODY_WROTE :
                    msr_log(msr, log_level, "Output filter: Error while forwarding response data (%d): No data", rc);
                    break;
                case AP_FILTER_ERROR :
                    /* Look like this is caused by the error
                     * already being handled, so we should ignore it
                     *
                     msr_log(msr, log_level, "Output filter: Error while forwarding response data (%d): Filter error", rc);
                     */
                    break;
                default :
                    msr_log(msr, log_level, "Output filter: Error while forwarding response data (%d): %s",
                            rc, get_apr_error(msr->mp, rc));
                    break;
            }
        }

        return rc;
    }

    return APR_SUCCESS;
}

/** \brief Inject data into brigade
 *
 * \param msr ModSecurity transation resource
 * \param ap_filter_t Apache filter
 *
 */
static void inject_content_to_of_brigade(modsec_rec *msr, ap_filter_t *f) {
    apr_bucket *b;

    if (msr->txcfg->content_injection_enabled && msr->stream_output_data != NULL) {
        apr_bucket *bucket_ci = NULL;

        bucket_ci = apr_bucket_heap_create(msr->stream_output_data,
                msr->stream_output_length, NULL, f->r->connection->bucket_alloc);

        for (b = APR_BRIGADE_FIRST(msr->of_brigade); b != APR_BRIGADE_SENTINEL(msr->of_brigade); b = APR_BUCKET_NEXT(b))  {
            if(!APR_BUCKET_IS_METADATA(b))
                apr_bucket_delete(b);
        }

        APR_BRIGADE_INSERT_HEAD(msr->of_brigade, bucket_ci);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Content Injection: Data reinjected bytes [%"APR_SIZE_T_FMT"]",msr->stream_output_length);
        }

    }
}

/**
 *
 */
static void prepend_content_to_of_brigade(modsec_rec *msr, ap_filter_t *f) {
    if ((msr->txcfg->content_injection_enabled) && (msr->content_prepend) && (!msr->of_skipping)) {
        apr_bucket *bucket_ci = NULL;

        bucket_ci = apr_bucket_heap_create(msr->content_prepend,
                msr->content_prepend_len, NULL, f->r->connection->bucket_alloc);
        APR_BRIGADE_INSERT_HEAD(msr->of_brigade, bucket_ci);

        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Content Injection (b): Added content to top: %s",
                    log_escape_nq_ex(msr->mp, msr->content_prepend, msr->content_prepend_len));
        }
    }
}

/**
 *
 */
static int flatten_response_body(modsec_rec *msr) {
    apr_status_t rc;

    msr->resbody_status = RESBODY_STATUS_READ_BRIGADE;

    if (msr->resbody_length + 1 <= 0) {
        msr_log(msr, 1, "Output filter: Invalid response length: %" APR_SIZE_T_FMT, msr->resbody_length);
        return -1;
    }

    msr->resbody_data = apr_palloc(msr->mp, msr->resbody_length + 1);
    if (msr->resbody_data == NULL) {
        msr_log(msr, 1, "Output filter: Response body data memory allocation failed. Asked for: %" APR_SIZE_T_FMT,
                msr->resbody_length + 1);
        return -1;
    }

    rc = apr_brigade_flatten(msr->of_brigade, msr->resbody_data, &msr->resbody_length);
    if (rc != APR_SUCCESS) {
        msr_log(msr, 1, "Output filter: Failed to flatten brigade (%d): %s", rc,
                get_apr_error(msr->mp, rc));
        return -1;
    }

    msr->resbody_data[msr->resbody_length] = '\0';
    msr->resbody_status = RESBODY_STATUS_READ;

    if (msr->txcfg->stream_outbody_inspection && msr->txcfg->hash_is_enabled == HASH_DISABLED)  {

        msr->stream_output_length = msr->resbody_length;

        if (msr->stream_output_data == NULL) {
            msr_log(msr, 1, "Output filter: Stream Response body data memory allocation failed. Asked for: %" APR_SIZE_T_FMT,
                    msr->stream_output_length + 1);
            return -1;
        }

        memset(msr->stream_output_data, 0, msr->stream_output_length+1);
        strncpy(msr->stream_output_data, msr->resbody_data, msr->stream_output_length);
        msr->stream_output_data[msr->stream_output_length] = '\0';
    } else if (msr->txcfg->stream_outbody_inspection && msr->txcfg->hash_is_enabled == HASH_ENABLED)    {
        int retval = 0;
        apr_time_t time1 = apr_time_now();

        retval = init_response_body_html_parser(msr);

        if(retval == 1) {
            retval = hash_response_body_links(msr);
            if(retval > 0) {
                retval = inject_hashed_response_body(msr, retval);
                if (msr->txcfg->debuglog_level >= 4) {
                    msr_log(msr, 4, "Hash completed in %" APR_TIME_T_FMT " usec.", (apr_time_now() - time1));
                }

            }
        }

        if(msr->of_stream_changed == 0) {
            msr->stream_output_length = msr->resbody_length;

            if (msr->stream_output_data == NULL) {
                msr_log(msr, 1, "Output filter: Stream Response body data memory allocation failed. Asked for: %" APR_SIZE_T_FMT,
                        msr->stream_output_length + 1);
                return -1;
            }

            memset(msr->stream_output_data, 0, msr->stream_output_length+1);
            strncpy(msr->stream_output_data, msr->resbody_data, msr->stream_output_length);
            msr->stream_output_data[msr->stream_output_length] = '\0';
        }
    }

    return 1;
}

/**
 * Output filter.
 */
apr_status_t output_filter(ap_filter_t *f, apr_bucket_brigade *bb_in) {
    request_rec *r = f->r;
    modsec_rec *msr = (modsec_rec *)f->ctx;
    apr_bucket *bucket = NULL, *eos_bucket = NULL;
    apr_status_t rc;
    int start_skipping = 0;

    /* Do we have the context? */
    if (msr == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, f->r->server,
                "ModSecurity: Internal Error: msr is null in output filter.");
        ap_remove_output_filter(f);
        return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
    }

    msr->r = r;

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Output filter: Receiving output (f %pp, r %pp).", f, f->r);
    }

    /* Put back the Accept-Encoding and TE request headers
     * if they were removed from the request.
     */
    if (msr->txcfg->disable_backend_compression) {
        char *ae = (char *)apr_table_get(msr->request_headers, "Accept-Encoding");
        char *te = (char *)apr_table_get(msr->request_headers, "TE");

        if ((ae != NULL)&&(apr_table_get(f->r->headers_in, "Accept-Encoding") == NULL)) {
            apr_table_add(f->r->headers_in, "Accept-Encoding", ae);
        }

        if ((te != NULL)&&(apr_table_get(f->r->headers_in, "TE") == NULL)) {
            apr_table_add(f->r->headers_in, "TE", te);
        }
    }

    /* Initialise on first invocation */
    if (msr->of_status == OF_STATUS_NOT_STARTED) {
        /* Update our context from the request structure. */
        msr->r = r;
        msr->response_status = r->status;
        msr->status_line = ((r->status_line != NULL)
                ? r->status_line : ap_get_status_line(r->status));
        msr->response_protocol = get_response_protocol(r);

        if(msr->txcfg->crypto_hash_location_rx == 1 || msr->txcfg->crypto_hash_location_pm == 1)
            rc = modify_response_header(msr);

        msr->response_headers = apr_table_overlay(msr->mp, r->err_headers_out, r->headers_out);

        /* Process phase RESPONSE_HEADERS */
        rc = modsecurity_process_phase(msr, PHASE_RESPONSE_HEADERS);
        if (rc < 0) { /* error */
            ap_remove_output_filter(f);
            return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
        }

        if (rc > 0) { /* transaction needs to be interrupted */
            int status = perform_interception(msr);
            if (status != DECLINED) { /* DECLINED means we allow-ed the request. */
                ap_remove_output_filter(f);
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_ERROR;
                return send_error_bucket(msr, f, status);
            }
        }

        msr->outbound_error = 0;
        /* Decide whether to observe the response body. */
        rc = output_filter_init(msr, f, bb_in);
        switch(rc) {
            case -2 : /* response too large */
            case -1 : /* error */
                /* there's something wrong with this response */
                ap_remove_output_filter(f);
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_ERROR;
                return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
            case 0 :
                /* We do not want to observe this response body
                 * but we need to remain attached to observe
                 * when it is completed so that we can run
                 * the RESPONSE_BODY phase.
                 */
                msr->of_skipping = 1;
                msr->resbody_status = RESBODY_STATUS_NOT_READ;
                break;
            default :
                /* Continue (observe the response body). */
                break;
        }

        /* If injecting content unset headers now. */
        if (msr->txcfg->content_injection_enabled == 0) {
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Content Injection: Not enabled.");
            }
        } else {
            if ((msr->content_prepend) || (msr->content_append)) {
                apr_table_unset(msr->r->headers_out, "Content-Length");
                apr_table_unset(msr->r->headers_out, "Last-Modified");
                apr_table_unset(msr->r->headers_out, "ETag");
                apr_table_unset(msr->r->headers_out, "Expires");

                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Content Injection: Removing headers (C-L, L-M, Etag, Expires).");
                }
            } else {
                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Content Injection: Nothing to inject.");
                }
            }
        }

        /* Content injection (prepend & non-buffering). */
        if ((msr->txcfg->content_injection_enabled) && (msr->content_prepend) && (msr->of_skipping)) {
            apr_bucket *bucket_ci = apr_bucket_heap_create(msr->content_prepend,
                    msr->content_prepend_len, NULL, f->r->connection->bucket_alloc);
            APR_BRIGADE_INSERT_HEAD(bb_in, bucket_ci);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Content Injection (nb): Added content to top: %s",
                        log_escape_nq_ex(msr->mp, msr->content_prepend, msr->content_prepend_len));
            }
        }
    } else
        if (msr->of_status == OF_STATUS_COMPLETE) {
            msr_log(msr, 1, "Output filter: Internal error: output filtering complete yet filter was invoked.");
            ap_remove_output_filter(f);
            return APR_EGENERAL;
        }


    /* Loop through the buckets in the brigade in order
     * to extract the size of the data available.
     */
    for(bucket = APR_BRIGADE_FIRST(bb_in);
            bucket != APR_BRIGADE_SENTINEL(bb_in);
            bucket = APR_BUCKET_NEXT(bucket)) {
        const char *buf;
        apr_size_t buflen;

        /* Look into response data if configured to do so,
         * unless we've already processed a partial response.
         */
        if ((msr->of_skipping == 0)&&(!msr->of_partial)) { /* Observe the response data. */
            /* Retrieve data from the bucket. */
            rc = apr_bucket_read(bucket, &buf, &buflen, APR_BLOCK_READ);
            if (rc != APR_SUCCESS) {
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_ERROR;

                msr_log(msr, 1, "Output filter: Failed to read bucket (rc %d): %s",
                        rc, get_apr_error(r->pool, rc));

                ap_remove_output_filter(f);
                return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Output filter: Bucket type %s contains %" APR_SIZE_T_FMT " bytes.",
                        bucket->type->name, buflen);
            }

            /* Check the response size. */
            if (msr->resbody_length > (apr_size_t)msr->txcfg->of_limit) {
                /* The size of the response is larger than what we're
                 * ready to accept. We need to decide what we want to do
                 * about it.
                 */
                msr->outbound_error = 1;
                if (msr->txcfg->of_limit_action == RESPONSE_BODY_LIMIT_ACTION_REJECT) {
                    /* Reject response. */
                    msr_log(msr, 1, "Output filter: Response body too large (over limit of %ld, "
                            "total not specified).", msr->txcfg->of_limit);

                    msr->of_status = OF_STATUS_COMPLETE;
                    msr->resbody_status = RESBODY_STATUS_PARTIAL;

                    ap_remove_output_filter(f);
                    return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
                } else {
                    /* Process partial response. */
                    start_skipping = 1;
                    msr->resbody_length = msr->txcfg->of_limit;

                    if (msr->txcfg->debuglog_level >= 4) {
                        msr_log(msr, 4, "Output filter: Processing partial response body (limit %ld)",
                                msr->txcfg->of_limit);
                    }
                }
            } else {
                msr->resbody_length += buflen;
            }
        }

        /* Have we reached the end of the response? */
        if (APR_BUCKET_IS_EOS(bucket)) {
            eos_bucket = bucket;

            /* Inject content (append & non-buffering). */
            if ((msr->txcfg->content_injection_enabled) && (msr->content_append)
                    && (msr->of_skipping || msr->of_partial || start_skipping))
            {
                apr_bucket *bucket_ci = NULL;

                bucket_ci = apr_bucket_heap_create(msr->content_append,
                        msr->content_append_len, NULL, f->r->connection->bucket_alloc);
                APR_BUCKET_INSERT_BEFORE(bucket, bucket_ci);

                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "Content-Injection (nb): Added content to bottom: %s",
                            log_escape_nq_ex(msr->mp, msr->content_append, msr->content_append_len));
                }
            }

            msr->of_done_reading = 1;
        }
    }

    /* Add buckets in this brigade to the brigade
     * we have in the context, but only if we actually
     * want to keep the response body.
     */
    if ((msr->of_skipping == 0)&&(msr->of_partial == 0)) {
        ap_save_brigade(f, &msr->of_brigade, &bb_in, msr->mp);

        /* Do we need to process a partial response? */
        if (start_skipping) {

            if (msr->txcfg->stream_outbody_inspection)  {
                if(msr->stream_output_data != NULL) {
                    free(msr->stream_output_data);
                    msr->stream_output_data = NULL;
                }

                msr->stream_output_data = (char *)malloc(msr->resbody_length+1);
            }

            if (flatten_response_body(msr) < 0) {
                if (msr->txcfg->stream_outbody_inspection)  {
                    if(msr->stream_output_data != NULL) {
                        free(msr->stream_output_data);
                        msr->stream_output_data = NULL;
                    }
                }

                ap_remove_output_filter(f);
                return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
            }

            /* Process phase RESPONSE_BODY */
            rc = modsecurity_process_phase(msr, PHASE_RESPONSE_BODY);
            if (rc < 0) {
                ap_remove_output_filter(f);
                return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
            }
            if (rc > 0) {
                int status = perform_interception(msr);
                if (status != DECLINED) { /* DECLINED means we allow-ed the request. */
                    ap_remove_output_filter(f);
                    return send_error_bucket(msr, f, status);
                }
            }

            /* Prepend content as necessary. */
            prepend_content_to_of_brigade(msr, f);

            if ((rc = send_of_brigade(msr, f)) != APR_SUCCESS) {
                return rc;
            }

            msr->of_partial = 1;
        }

        if (msr->of_done_reading == 0) {
            /* We are done for now. We will be called again with more data. */
            return APR_SUCCESS;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Output filter: Completed receiving response body (buffered %s - %" APR_SIZE_T_FMT " bytes).",
                    (msr->of_partial ? "partial" : "full"), msr->resbody_length);
        }
    } else { /* Not looking at response data. */
        if (msr->of_done_reading == 0) {
            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Output filter: Sending input brigade directly.");
            }

            return ap_pass_brigade(f->next, bb_in);
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Output filter: Completed receiving response body (non-buffering).");
        }
    }

    /* We've done our thing; remove us from the filter list. */
    msr->of_status = OF_STATUS_COMPLETE;
    ap_remove_output_filter(f);

    /* Process phase RESPONSE_BODY, but
     * only if it hasn't been processed already.
     */
    if (msr->phase < PHASE_RESPONSE_BODY) {

        if (msr->txcfg->stream_outbody_inspection)  {
            if(msr->stream_output_data != NULL) {
                free(msr->stream_output_data);
                msr->stream_output_data = NULL;
            }

            msr->stream_output_data = (char *)malloc(msr->resbody_length+1);
        }

        if (flatten_response_body(msr) < 0) {
            if (msr->txcfg->stream_outbody_inspection)  {
                if(msr->stream_output_data != NULL) {
                    free(msr->stream_output_data);
                    msr->stream_output_data = NULL;
                }
            }

            ap_remove_output_filter(f);
            return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
        }

        rc = modsecurity_process_phase(msr, PHASE_RESPONSE_BODY);
        if (rc < 0) {
            ap_remove_output_filter(f);
            return send_error_bucket(msr, f, HTTP_INTERNAL_SERVER_ERROR);
        }
        if (rc > 0) {
            int status = perform_interception(msr);
            if (status != DECLINED) { /* DECLINED means we allow-ed the request. */
                ap_remove_output_filter(f);
                return send_error_bucket(msr, f, status);
            }
        }
    }

    /* Now send data down the filter stream
     * (full-buffering only).
     */
    if ((msr->of_skipping == 0)&&(!msr->of_partial)) {
        if(msr->of_stream_changed == 1) {
            inject_content_to_of_brigade(msr,f);
            msr->of_stream_changed = 0;
        }

        if (msr->txcfg->stream_outbody_inspection)  {
            if(msr->stream_output_data != NULL) {
                free(msr->stream_output_data);
                msr->stream_output_data = NULL;
            }
        }

        prepend_content_to_of_brigade(msr, f);

        /* Inject content into response (append & buffering). */
        if ((msr->txcfg->content_injection_enabled) && (msr->content_append)) {
            apr_bucket *bucket_ci = NULL;

            bucket_ci = apr_bucket_heap_create(msr->content_append,
                    msr->content_append_len, NULL, f->r->connection->bucket_alloc);
            APR_BUCKET_INSERT_BEFORE(eos_bucket, bucket_ci);

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Content-Injection (b): Added content to bottom: %s",
                        log_escape_nq_ex(msr->mp, msr->content_append, msr->content_append_len));
            }
        }

        /* Send data down the filter stream. */
        if ((rc = send_of_brigade(msr, f)) != APR_SUCCESS) {
            return rc;
        }
    }

    /* Another job well done! */
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Output filter: Output forwarding complete.");
    }

    if ((msr->of_skipping == 0)&&(msr->of_partial == 0)) {
        return APR_SUCCESS;
    } else {
        if (msr->txcfg->debuglog_level >= 9) {
            msr_log(msr, 9, "Output filter: Sending input brigade directly.");
        }

        return ap_pass_brigade(f->next, bb_in);
    }
}
