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

#include "apr_lib.h"
#include "ap_config.h"
#include "http_config.h"
#include "apr_buckets.h"


AP_DECLARE(apr_status_t) ap_get_brigade(ap_filter_t *next, 
                                        apr_bucket_brigade *bb, 
                                        ap_input_mode_t mode,
                                        apr_read_type_e block, 
                                        apr_off_t readbytes)
{
    if (next) {
        return next->frec->filter_func.in_func(next, bb, mode, block,
                                               readbytes);
    }

	return AP_NOBODY_READ;
}

AP_DECLARE(apr_status_t) ap_pass_brigade(ap_filter_t *next,
                                         apr_bucket_brigade *bb)
{
    if (next) {
        apr_bucket *e;
        if ((e = APR_BRIGADE_LAST(bb)) && APR_BUCKET_IS_EOS(e) && next->r) {
            /* This is only safe because HTTP_HEADER filter is always in
             * the filter stack.   This ensures that there is ALWAYS a
             * request-based filter that we can attach this to.  If the
             * HTTP_FILTER is removed, and another filter is not put in its
             * place, then handlers like mod_cgi, which attach their own
             * EOS bucket to the brigade will be broken, because we will
             * get two EOS buckets on the same request.
             */
            next->r->eos_sent = 1;

            /* remember the eos for internal redirects, too */
            if (next->r->prev) {
                request_rec *prev = next->r->prev;

                while (prev) {
                    prev->eos_sent = 1;
                    prev = prev->prev;
                }
            }
        }
        return next->frec->filter_func.out_func(next, bb);
    }
    return AP_NOBODY_WROTE;
}

AP_DECLARE(apr_status_t) ap_save_brigade(ap_filter_t *f,
                                         apr_bucket_brigade **saveto,
                                         apr_bucket_brigade **b, apr_pool_t *p)
{
    apr_bucket *e;
    apr_status_t rv, srv = APR_SUCCESS;

    /* If have never stored any data in the filter, then we had better
     * create an empty bucket brigade so that we can concat.
     */
    if (!(*saveto)) {
        *saveto = apr_brigade_create(p, f->c->bucket_alloc);
    }

    for (e = APR_BRIGADE_FIRST(*b);
         e != APR_BRIGADE_SENTINEL(*b);
         e = APR_BUCKET_NEXT(e))
    {
        rv = apr_bucket_setaside(e, p);

        /* If the bucket type does not implement setaside, then
         * (hopefully) morph it into a bucket type which does, and set
         * *that* aside... */
        if (rv == APR_ENOTIMPL) {
            const char *s;
            apr_size_t n;

            rv = apr_bucket_read(e, &s, &n, APR_BLOCK_READ);
            if (rv == APR_SUCCESS) {
                rv = apr_bucket_setaside(e, p);
            }
        }

        if (rv != APR_SUCCESS) {
            srv = rv;
            /* Return an error but still save the brigade if
             * ->setaside() is really not implemented. */
            if (rv != APR_ENOTIMPL) {
                return rv;
            }
        }
    }
    APR_BRIGADE_CONCAT(*saveto, *b);
    return srv;
}

static apr_status_t error_bucket_read(apr_bucket *b, const char **str,
                                      apr_size_t *len, apr_read_type_e block)
{
    *str = "Unknown error.";
    *len = strlen(*str);
    return APR_SUCCESS;
}

static void error_bucket_destroy(void *data)
{
    ap_bucket_error *h = data;

    if (apr_bucket_shared_destroy(h)) {
        apr_bucket_free(h);
    }
}

AP_DECLARE_DATA const apr_bucket_type_t ap_bucket_type_error = {
    "ERROR", 5, APR_BUCKET_METADATA,
    error_bucket_destroy,
    error_bucket_read,
    apr_bucket_setaside_notimpl,
    apr_bucket_split_notimpl,
    apr_bucket_shared_copy
};

AP_DECLARE(apr_bucket *) ap_bucket_error_make(apr_bucket *b, int error,
                                              const char *buf, apr_pool_t *p)
{
    ap_bucket_error *h;

    h = apr_bucket_alloc(sizeof(*h), b->list);
    h->status = error;
    h->data = (buf) ? apr_pstrdup(p, buf) : NULL;

    b = apr_bucket_shared_make(b, h, 0, 0);
    b->type = &ap_bucket_type_error;
    return b;
}

AP_DECLARE(apr_bucket *) ap_bucket_error_create(int error, const char *buf,
                                                apr_pool_t *p,
                                                apr_bucket_alloc_t *list)
{
    apr_bucket *b = apr_bucket_alloc(sizeof(*b), list);

    APR_BUCKET_INIT(b);
    b->free = apr_bucket_free;
    b->list = list;
    return ap_bucket_error_make(b, error, buf, p);
}

