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

#define	FILTER_POOL	apr_hook_global_pool
#include "apr_hooks.h"

/*
** This macro returns true/false if a given filter should be inserted BEFORE
** another filter. This will happen when one of: 1) there isn't another
** filter; 2) that filter has a higher filter type (class); 3) that filter
** corresponds to a different request.
*/
#define INSERT_BEFORE(f, before_this) ((before_this) == NULL \
                           || (before_this)->frec->ftype > (f)->frec->ftype \
                           || (before_this)->r != (f)->r)


apr_table_t *ms_input_filters = NULL;
apr_table_t *ms_output_filters = NULL;

void init_filter_tables()
{
	if(ms_input_filters == NULL)
	{
		ms_input_filters = apr_table_make(FILTER_POOL, 10);
		ms_output_filters = apr_table_make(FILTER_POOL, 10);
	}
}

AP_DECLARE(ap_filter_rec_t *) ap_register_input_filter(const char *name,
                                          ap_in_filter_func filter_func,
                                          ap_init_filter_func filter_init,
                                          ap_filter_type ftype)
{
	ap_filter_rec_t *f;

	init_filter_tables();
	
	f = apr_palloc(FILTER_POOL, sizeof(ap_filter_rec_t));

	f->filter_func.in_func = filter_func;
	f->filter_init_func = filter_init;
	f->ftype = ftype;
	f->name = name;

	apr_table_setn(ms_input_filters, name, (const char *)f);

	return f;
}

AP_DECLARE(ap_filter_rec_t *) ap_register_output_filter(const char *name,
                                            ap_out_filter_func filter_func,
                                            ap_init_filter_func filter_init,
                                            ap_filter_type ftype)
{
	ap_filter_rec_t *f;
	
	init_filter_tables();
	
	f = apr_palloc(FILTER_POOL, sizeof(ap_filter_rec_t));

	f->filter_func.out_func = filter_func;
	f->filter_init_func = filter_init;
	f->ftype = ftype;
	f->name = name;

	apr_table_setn(ms_output_filters, name, (const char *)f);

	return f;
}

static ap_filter_t *add_any_filter_handle(ap_filter_rec_t *frec, void *ctx,
                                          request_rec *r, conn_rec *c,
                                          ap_filter_t **r_filters,
                                          ap_filter_t **p_filters,
                                          ap_filter_t **c_filters)
{
    apr_pool_t* p = r ? r->pool : c->pool;
    ap_filter_t *f = apr_palloc(p, sizeof(*f));
    ap_filter_t **outf;

    if (frec->ftype < AP_FTYPE_PROTOCOL) {
        if (r) {
            outf = r_filters;
        }
        else {
            ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL,
                      "a content filter was added without a request: %s", frec->name);
            return NULL;
        }
    }
    else if (frec->ftype < AP_FTYPE_CONNECTION) {
        if (r) {
            outf = p_filters;
        }
        else {
            ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL,
                         "a protocol filter was added without a request: %s", frec->name);
            return NULL;
        }
    }
    else {
        outf = c_filters;
    }

    f->frec = frec;
    f->ctx = ctx;
    f->r = r;
    f->c = c;
    f->next = NULL;

    if (INSERT_BEFORE(f, *outf)) {
        f->next = *outf;

        if (*outf) {
            ap_filter_t *first = NULL;

            if (r) {
                /* If we are adding our first non-connection filter,
                 * Then don't try to find the right location, it is
                 * automatically first.
                 */
                if (*r_filters != *c_filters) {
                    first = *r_filters;
                    while (first && (first->next != (*outf))) {
                        first = first->next;
                    }
                }
            }
            if (first && first != (*outf)) {
                first->next = f;
            }
        }
        *outf = f;
    }
    else {
        ap_filter_t *fscan = *outf;
        while (!INSERT_BEFORE(f, fscan->next))
            fscan = fscan->next;

        f->next = fscan->next;
        fscan->next = f;
    }

    if (frec->ftype < AP_FTYPE_CONNECTION && (*r_filters == *c_filters)) {
        *r_filters = *p_filters;
    }
    return f;
}

AP_DECLARE(ap_filter_t *) ap_add_input_filter(const char *name, void *ctx,
                                              request_rec *r, conn_rec *c)
{
	ap_filter_rec_t *f = (ap_filter_rec_t *)apr_table_get(ms_input_filters, name);

	if(f == NULL)
		return NULL;

	return add_any_filter_handle(f, ctx, r, c,
                          r ? &r->input_filters : NULL,
                          r ? &r->proto_input_filters : NULL, &c->input_filters);
}

AP_DECLARE(ap_filter_t *) ap_add_output_filter(const char *name, void *ctx, 
                                               request_rec *r, conn_rec *c)
{
	ap_filter_rec_t *f = (ap_filter_rec_t *)apr_table_get(ms_output_filters, name);

	if(f == NULL)
		return NULL;

    return add_any_filter_handle(f, ctx, r, c,
                          r ? &r->output_filters : NULL,
                          r ? &r->proto_output_filters : NULL, &c->output_filters);
}

static void remove_any_filter(ap_filter_t *f, ap_filter_t **r_filt, ap_filter_t **p_filt,
                              ap_filter_t **c_filt)
{
    ap_filter_t **curr = r_filt ? r_filt : c_filt;
    ap_filter_t *fscan = *curr;

    if (p_filt && *p_filt == f)
        *p_filt = (*p_filt)->next;

    if (*curr == f) {
        *curr = (*curr)->next;
        return;
    }

    while (fscan->next != f) {
        if (!(fscan = fscan->next)) {
            return;
        }
    }

    fscan->next = f->next;
}

AP_DECLARE(void) ap_remove_input_filter(ap_filter_t *f)
{
    remove_any_filter(f, f->r ? &f->r->input_filters : NULL,
                      f->r ? &f->r->proto_input_filters : NULL,
                      &f->c->input_filters);
}

AP_DECLARE(void) ap_remove_output_filter(ap_filter_t *f)
{
    remove_any_filter(f, f->r ? &f->r->output_filters : NULL,
                      f->r ? &f->r->proto_output_filters : NULL,
                      &f->c->output_filters);
}
