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

    if (msr == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, f->r->server,
            "ModSecurity: Internal error in input filter: msr is null.");
        ap_remove_input_filter(f);
        return APR_EGENERAL;
    }

    msr->r = f->r;

    if ((msr->if_status == IF_STATUS_COMPLETE)||(msr->if_status == IF_STATUS_NONE)) {
        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Input filter: Input forwarding already complete, skipping (f %x, r %x).", f, f->r);
        }
        ap_remove_input_filter(f);
        return ap_get_brigade(f->next, bb_out, mode, block, nbytes);
    }

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Input filter: Forwarding input: mode=%i, block=%i, nbytes=%" APR_OFF_T_FMT
            " (f %x, r %x).", mode, block, nbytes, f, f->r);
    }

    if (msr->if_started_forwarding == 0) {
        msr->if_started_forwarding = 1;
        rc = modsecurity_request_body_retrieve_start(msr);
        if (rc == -1) {
            // TODO err
            return APR_EGENERAL;
        }
    }

    rc = modsecurity_request_body_retrieve(msr, &chunk, (unsigned int)nbytes);
    if (rc == -1) {
        // TODO err
        return APR_EGENERAL;
    }

    if (chunk) {
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
            msr_log(msr, 4, "Input filter: Forwarded %lu bytes.", chunk->length);
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

    if (modsecurity_request_body_start(msr) < 0) {
        // TODO err
        return -1;
    }

    seen_eos = 0;
    bb_in = apr_brigade_create(msr->mp, r->connection->bucket_alloc);
    if (bb_in == NULL) return -1;    
    do {
        apr_status_t rc;

        rc = ap_get_brigade(r->input_filters, bb_in, AP_MODE_READBYTES, APR_BLOCK_READ, HUGE_STRING_LEN);
        if (rc != APR_SUCCESS) {
            /* NOTE Apache returns -3 here when the request is too large
             *      and APR_EGENERAL when the client disconnects.
             */
            switch(rc) {
                case APR_TIMEUP :
                    return -4;
                    break;
                case -3 :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: HTTP Error 413 - Request entity too large. (Most likely.)");
                    rc = -3;
                    break;
                case APR_EGENERAL :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: Client went away.");
                    rc = -2;
                    break;
                default :
                    *error_msg = apr_psprintf(msr->mp, "Error reading request body: %s", get_apr_error(msr->mp, rc));
                    rc = -1;
                    break;
            }

            if (*error_msg) msr_log(msr, 1, "%s", *error_msg);

            return rc;
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
                msr_log(msr, 1, "Input filter: Failed reading input / bucket (%i): %s",
                    rc, get_apr_error(msr->mp, rc));
                return -1;
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Input filter: Bucket type %s contains %i bytes.",
                    bucket->type->name, buflen);
            }

            /* Check request body limit (should only trigger on chunked requests). */
            if (msr->reqbody_length + buflen > (apr_size_t)msr->txcfg->reqbody_limit) {
                *error_msg = apr_psprintf(msr->mp, "Requests body is larger than the "
                    "configured limit (%lu).", msr->txcfg->reqbody_limit);
                return -5;
            }

            if (buflen != 0) {
                if (modsecurity_request_body_store(msr, buf, buflen) < 0) {
                    // TODO err
                    return -1;
                }

                msr->reqbody_length += buflen;
            }

            if (APR_BUCKET_IS_EOS(bucket)) {
                seen_eos = 1;
            }
        }

        apr_brigade_cleanup(bb_in);
    } while(!seen_eos);

    modsecurity_request_body_end(msr);

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Input filter: Completed receiving request body (length %lu).",
            msr->reqbody_length);
    }

    msr->if_status = IF_STATUS_WANTS_TO_RUN;

    return 1;
}


/* -- Output filter -- */

/**
 * Sends a brigade with an error bucket down the filter chain.
 */
static apr_status_t send_error_bucket(ap_filter_t *f, int status) {
    apr_bucket_brigade *brigade = NULL;
    apr_bucket *bucket = NULL;

    brigade = apr_brigade_create(f->r->pool, f->r->connection->bucket_alloc);
    if (brigade == NULL) return APR_EGENERAL;
    bucket = ap_bucket_error_create(status, NULL, f->r->pool, f->r->connection->bucket_alloc);
    if (bucket == NULL) return APR_EGENERAL;
    APR_BRIGADE_INSERT_TAIL(brigade, bucket);
    bucket = apr_bucket_eos_create(f->r->connection->bucket_alloc);
    if (bucket == NULL) return APR_EGENERAL;
    APR_BRIGADE_INSERT_TAIL(brigade, bucket);

    return ap_pass_brigade(f->next, brigade);
}

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
        msr_log(msr, 1, "Output filter: MIME type structures are corrupted (internal error).");
        return -1;
    }

    if (r->content_type != NULL) {
        char *p = NULL;

        content_type = apr_pstrdup(msr->mp, r->content_type);
        if (content_type == NULL) return -1;

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
    if (msr->of_brigade == NULL) return -1;
    msr->of_status = OF_STATUS_IN_PROGRESS;
    
    rc = output_filter_should_run(msr, r);
    if (rc < 0) return -1;
    if (rc == 0) return 0;

    /* Look up the Content-Length header to see if we know
     * the amount of data coming our way. If we do and if
     * it's too much we might want to stop processing right here.
     */
    s_content_length = apr_table_get(r->headers_out, "Content-Length");
    if (s_content_length == NULL) {
        /* Try this too, mod_cgi seems to put headers there */
        s_content_length = apr_table_get(r->err_headers_out, "Content-Length");
    }

    if (s_content_length != NULL) {
        long int len;

        len = strtol(s_content_length, NULL, 10);
        if ((len == LONG_MIN)||(len == LONG_MAX)||(len < 0)||(len >= 1073741824)) {
            msr_log(msr, 1, "Output filter: Invalid Content-Length: %s", log_escape_nq(r->pool, (char *)s_content_length));
            return -1;
        }

        if (len == 0) {
            if (msr->txcfg->debuglog_level >= 4) {
                msr_log(msr, 4, "Output filter: Skipping response since Content-Length is zero.");
            }
            return 0;
        }

        if (len > msr->txcfg->of_limit) {
            msr_log(msr, 1, "Output filter: Content-Length (%s) over the limit (%lu).", log_escape_nq(r->pool, (char *)s_content_length), msr->txcfg->of_limit);
            return -2;
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
    apr_bucket *bucket;
    apr_status_t rc;

    if (msr == NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, f->r->server,
            "ModSecurity: Internal Error: msr is null in output filter.");
        ap_remove_output_filter(f);
        return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
    }

    msr->r = r;

    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Output filter: Receiving output (f %x, r %x).", f, f->r);
    }
    
    /* Initialise on first invocation */
    if (msr->of_status == OF_STATUS_NOT_STARTED) {
        /* Update our context from the request structure. */
        msr->r = r;
        msr->response_status = r->status;
        msr->status_line = ((r->status_line != NULL)    
            ? r->status_line : ap_get_status_line(r->status));
        msr->response_protocol = get_response_protocol(r);
        msr->response_headers = apr_table_overlay(msr->mp, r->err_headers_out, r->headers_out);

        /* Process phase RESPONSE_HEADERS */        
        rc = modsecurity_process_phase(msr, PHASE_RESPONSE_HEADERS);
        if (rc < 0) { /* error */
            ap_remove_output_filter(f);
            return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
        }
        if (rc > 0) { /* transaction needs to be interrupted */
            int status = perform_interception(msr);
            if (status != DECLINED) { /* DECLINED means we allow-ed the request. */
                ap_remove_output_filter(f);
                return send_error_bucket(f, status);
            }
        }

        /* Decide whether to observe the response body. */
        rc = output_filter_init(msr, f, bb_in);
        switch(rc) {
            case -2 : /* response too large */
            case -1 : /* error */
                /* there's something wrong with this response */
                ap_remove_output_filter(f);
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_ERROR;
                return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
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

        if (msr->of_skipping == 0) {
            rc = apr_bucket_read(bucket, &buf, &buflen, APR_BLOCK_READ);
            if (rc != APR_SUCCESS) {   
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_ERROR;
                msr_log(msr, 1, "Output filter: Failed to read bucket (rc %i): %s",
                    rc, get_apr_error(r->pool, rc));
                ap_remove_output_filter(f);
                return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
            }

            if (msr->txcfg->debuglog_level >= 9) {
                msr_log(msr, 9, "Output filter: Bucket type %s contains %i bytes.",
                    bucket->type->name, buflen);
            }

            if (msr->resbody_length > (apr_size_t)msr->txcfg->of_limit) {
                msr_log(msr, 1, "Output filter: Response body too large (over limit of %lu, total length not known).",
                    msr->txcfg->of_limit);
                msr->of_status = OF_STATUS_COMPLETE;
                msr->resbody_status = RESBODY_STATUS_PARTIAL;
                ap_remove_output_filter(f);
                return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
            }

            msr->resbody_length += buflen;
        }

        if (APR_BUCKET_IS_EOS(bucket)) {
            msr->of_done_reading = 1;
        }
    }

    /* Add buckets in this brigade to the brigade
     * we have in the context, but only if we actually
     * want to keep the response body.
     */
    if (msr->of_skipping == 0) {
        ap_save_brigade(f, &msr->of_brigade, &bb_in, msr->mp);

        if (msr->of_done_reading == 0) {
            /* We are done for now. We will be called again with more data. */        
            return APR_SUCCESS;
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Output filter: Completed receiving response (length %lu).",
                msr->resbody_length);
        }
    } else {
        if (msr->of_done_reading == 0) {
            return ap_pass_brigade(f->next, bb_in);
        }

        if (msr->txcfg->debuglog_level >= 4) {
            msr_log(msr, 4, "Output filter: Completed receiving response.");
        }
    }

    /* We're not coming back here. */
    msr->of_status = OF_STATUS_COMPLETE;
    ap_remove_output_filter(f);

    if (msr->of_skipping == 0) {
        /* We've done with reading, it's time to inspect the data. */
        msr->resbody_status = RESBODY_STATUS_READ_BRIGADE;

        if (msr->resbody_length + 1 <= 0) {
            msr_log(msr, 1, "Output filter: Invalid response length: %lu", msr->resbody_length);
            return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
        }

        msr->resbody_data = apr_palloc(msr->mp, msr->resbody_length + 1); 
        if (msr->resbody_data == NULL) {
            msr_log(msr, 1, "Output filter: Response body data memory allocation failed. Asked for: %li",
                msr->resbody_length + 1);
            return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
        }

        // TODO Why does the function below take pointer to length? Will it modify it?

        rc = apr_brigade_flatten(msr->of_brigade, msr->resbody_data, &msr->resbody_length);
        if (rc != APR_SUCCESS) {
            msr_log(msr, 1, "Output filter: Failed to flatten brigade (%i): %s", rc,
                get_apr_error(r->pool, rc));
            return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
        }
        msr->resbody_data[msr->resbody_length] = '\0';
        msr->resbody_status = RESBODY_STATUS_READ;
    }

    /* Process phase RESPONSE_BODY */
    rc = modsecurity_process_phase(msr, PHASE_RESPONSE_BODY);
    if (rc < 0) {
        return send_error_bucket(f, HTTP_INTERNAL_SERVER_ERROR);
    }
    if (rc > 0) {
        int status = perform_interception(msr);
        if (status != DECLINED) { /* DECLINED means we allow-ed the request. */
            return send_error_bucket(f, status);
        }
    }

    if (msr->of_skipping == 0) {
        record_time_checkpoint(msr, 3);

        rc = ap_pass_brigade(f->next, msr->of_brigade);
        if (rc != APR_SUCCESS) {
            int log_level = 1;

            if (APR_STATUS_IS_ECONNRESET(rc)) {
                /* Message "Connection reset by peer" is common and not a sign
                 * of something unusual. Hence we don't want to make a big deal
                 * about it, logging at NOTICE level. Everything else we log
                 * at ERROR level.
                 */
                log_level = 3;
            }            

            msr_log(msr, log_level, "Output filter: Error while forwarding response data (%i): %s",
                    rc, get_apr_error(msr->mp, rc));

            return rc;
        }
    }
    
    /* Another job well done! */
    if (msr->txcfg->debuglog_level >= 4) {
        msr_log(msr, 4, "Output filter: Output forwarding complete.");
    }

    if (msr->of_skipping == 0) {
        return APR_SUCCESS;
    } else {
        return ap_pass_brigade(f->next, bb_in);
    }
}
