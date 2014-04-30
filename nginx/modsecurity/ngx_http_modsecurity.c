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

#include <ngx_http.h>
#include <apr_bucket_nginx.h>
#include <ngx_pool_context.h>

#include <apr_base64.h>

/* Those are defined twice, lets keep it defined just once by `undef`
 * the first one.
 */
#undef CR
#undef LF
#undef CRLF

#include "api.h"

#define NOTE_NGINX_REQUEST_CTX "nginx-ctx"
#define TXID_SIZE 25

/*
 * If MOVE_REQUEST_CHAIN_TO_MODSEC is set, the chain will be moved into the
 * ModSecurity brigade, after be processed by ModSecurity, it needs to be moved
 * to nginx chains again, in order to be processed by other modules. It seems
 * that this is not working well whenever the request body is delivered into
 * chunks. Resulting in segfaults.
 *
 * If MOVE_REQUEST_CHAIN_TO_MODSEC is _not_ set, a copy of the request body
 * will be delivered to ModSecurity and after processed it will be released,
 * not modifying the nginx chain.
 *
 * The malfunctioning can be observer while running the following regression
 * tests:
 *  #15 - SecRequestBodyInMemoryLimit
 *  #16 - SecRequestBodyInMemoryLimit (greater)
 *  #19 - SecRequestBodyLimitAction ProcessPartial (multipart/greater - chunked)
 *  (from: regression/config/10-request-directives.t)
 */
/*
#define MOVE_REQUEST_CHAIN_TO_MODSEC
*/


#define tuxb

/*
 * If NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY is defined, this module will
 * not process the body and header filter, instead it will only process the
 * preaccess request handler. This is useful to debug the module and check if
 * reference counters (r->main->count) and connections are being handled as
 * expected. However, do not expect to have ModSecurity fully workable with
 * this variable set. Use only for debugging.
 *
 */
/*
#define NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
 */

typedef struct {
    ngx_flag_t enable;
    directory_config *config;

    ngx_str_t *file;
    ngx_uint_t line;
} ngx_http_modsecurity_loc_conf_t;

typedef struct {
    ngx_http_request_t *r;
    conn_rec *connection;
    request_rec *req;

    apr_bucket_brigade *brigade;

    unsigned complete:1;
    unsigned request_processed:1;
    unsigned waiting_more_body:1;
    unsigned body_requested:1;
} ngx_http_modsecurity_ctx_t;


/*
** Module's registred function/handlers.
*/
static ngx_int_t ngx_http_modsecurity_handler(ngx_http_request_t *r);
static void ngx_http_modsecurity_request_read(ngx_http_request_t *r);

#ifndef NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
static ngx_int_t ngx_http_modsecurity_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_http_modsecurity_body_filter(ngx_http_request_t *r,
        ngx_chain_t *in);
#endif
static ngx_int_t ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);

static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf,
        void *parent, void *child);

static char *ngx_http_modsecurity_config(ngx_conf_t *cf,
        ngx_command_t *cmd, void *conf);
static char *ngx_http_modsecurity_enable(ngx_conf_t *cf,
        ngx_command_t *cmd, void *conf);

static ngx_http_modsecurity_ctx_t
    *ngx_http_modsecurity_create_ctx(ngx_http_request_t *r);

static int ngx_http_modsecurity_drop_action(request_rec *r);
static void ngx_http_modsecurity_terminate(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_cleanup(void *data);

static int ngx_http_modsecurity_save_headers_in_visitor(void *data,
        const char *key, const char *value);
static int ngx_http_modsecurity_save_headers_out_visitor(void *data,
        const char *key, const char *value);

/*
** This is a temporary hack to make PCRE work with ModSecurity
** nginx hijacks pcre_malloc and pcre_free, so we have to re-hijack them
*/
extern apr_pool_t *pool;
static void *
modsec_pcre_malloc(size_t size)
{
    return apr_palloc(pool, size);
}

static void
modsec_pcre_free(void *ptr)
{
}

static server_rec *modsec_server = NULL;

static ngx_command_t ngx_http_modsecurity_commands[] =  {
  { ngx_string("ModSecurityConfig"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_http_modsecurity_config,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  { ngx_string("ModSecurityEnabled"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
        |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
    ngx_http_modsecurity_enable,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_modsecurity_loc_conf_t, enable),
    NULL },
  ngx_null_command
};

static ngx_http_module_t ngx_http_modsecurity_ctx = {
    ngx_http_modsecurity_preconfiguration, /* preconfiguration */
    ngx_http_modsecurity_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_modsecurity_create_loc_conf, /* create location configuration */
    ngx_http_modsecurity_merge_loc_conf /* merge location configuration */
};

ngx_module_t ngx_http_modsecurity = {
    NGX_MODULE_V1,
    &ngx_http_modsecurity_ctx, /* module context */
    ngx_http_modsecurity_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    ngx_http_modsecurity_init_process, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    ngx_http_modsecurity_terminate, /* exit process */
    ngx_http_modsecurity_terminate, /* exit master */
    NGX_MODULE_V1_PADDING
};


#ifndef NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
static ngx_http_output_header_filter_pt ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt ngx_http_next_body_filter;
#endif
static ngx_http_upstream_t ngx_http_modsecurity_upstream;

static struct {
    char      *name;
    ngx_str_t  variable_name;
} special_headers_out[] = {
    {"Content-Type",      ngx_string("sent_http_content_type")  },
    {"Content-Length",    ngx_string("sent_http_content_length")},
    {"Location",          ngx_string("sent_http_location")},
    {"Last-Modified",     ngx_string("sent_http_last_modified")},
    {"Connection",        ngx_string("sent_http_connection")},
    {"Keep-Alive",        ngx_string("sent_http_keep_alive")},
    {"Transfer-Encoding", ngx_string("sent_http_transfer_encoding")},
    {"Cache-Control",     ngx_string("sent_http_cache_control")},
    {NULL,                ngx_null_string}
};

static inline u_char *
ngx_pstrdup0(ngx_pool_t *pool, ngx_str_t *src)
{
    u_char  *dst;

    dst = ngx_pnalloc(pool, src->len + 1);
    if (dst == NULL) {
       return NULL;
    }

    ngx_memcpy(dst, src->data, src->len);
    dst[src->len] = '\0';

    return dst;
}


/*
 * MultiplyDeBruijnBitPosition
 * http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightMultLookup
 */
static inline int
ngx_http_modsecurity_method_number(unsigned int nginx)
{
    static const int MultiplyDeBruijn[32] = {
        M_INVALID, /* 1 >> 0 */
        M_GET,
        M_INVALID, /* 1 >> 28 */
        M_GET,     /* NGX_HTTP_HEAD */
        M_INVALID, /* 1 >> 29 */
        M_PATCH,
        M_INVALID, /* 1 >> 24 */
        M_POST,
        M_INVALID, /* 1 >> 30 */
        M_INVALID, /* 1 >> 22 */
        M_INVALID, /* 1 >> 20 */
        M_TRACE,
        M_INVALID, /* 1 >> 25 */
        M_INVALID, /* 1 >> 17 */
        M_PUT,
        M_MOVE,
        M_INVALID, /* 1 >> 31 */
        M_INVALID, /* 1 >> 27 */
        M_UNLOCK,
        M_INVALID, /* 1 >> 23 */
        M_INVALID, /* 1 >> 21 */
        M_INVALID, /* 1 >> 19 */
        M_INVALID, /* 1 >> 16 */
        M_COPY,
        M_INVALID, /* 1 >> 26 */
        M_LOCK,
        M_INVALID, /* 1 >> 18 */
        M_MKCOL,
        M_PROPPATCH,
        M_DELETE,
        M_PROPFIND,
        M_OPTIONS
    };

    return MultiplyDeBruijn[((uint32_t)((nginx & -nginx) * 0x077CB531U)) >> 27];
}

static ngx_inline ngx_int_t
ngx_http_modsecurity_load_request(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t  *ctx;
    request_rec                 *req;
    ngx_str_t                    str;
    size_t                       root;
    ngx_str_t                    path;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    req = ctx->req;

    /* request line */
    req->method = (char *)ngx_pstrdup0(r->pool, &r->method_name);

    /* TODO: how to use ap_method_number_of ?
     * req->method_number = ap_method_number_of(req->method);
     */

    req->method_number = ngx_http_modsecurity_method_number(r->method);

    /* ngx_http_map_uri_to_path() allocates memory for terminating '\0' */
    if (ngx_http_map_uri_to_path(r, &path, &root, 0) == NULL) {
        return NGX_ERROR;
    }

    req->filename = (char *) path.data;
    req->path_info = req->filename;

    req->args = (char *)ngx_pstrdup0(r->pool, &r->args);

    req->proto_num = r->http_major *1000 + r->http_minor;
    req->protocol = (char *)ngx_pstrdup0(r->pool, &r->http_protocol);
    req->request_time = apr_time_make(r->start_sec, r->start_msec);
    req->the_request = (char *)ngx_pstrdup0(r->pool, &r->request_line);

    req->unparsed_uri = (char *)ngx_pstrdup0(r->pool, &r->unparsed_uri);
    req->uri = (char *)ngx_pstrdup0(r->pool, &r->uri);

    req->parsed_uri.scheme = "http";

#if (NGX_HTTP_SSL)
    if (r->connection->ssl) {
        req->parsed_uri.scheme = "https";
    }
#endif

    req->parsed_uri.path = (char *)ngx_pstrdup0(r->pool, &r->uri);
    req->parsed_uri.is_initialized = 1;

    str.data = r->port_start;
    str.len = r->port_end - r->port_start;
    req->parsed_uri.port = ngx_atoi(str.data, str.len);
    req->parsed_uri.port_str = (char *)ngx_pstrdup0(r->pool, &str);

    req->parsed_uri.query = r->args.len ? req->args : NULL;
    req->parsed_uri.dns_looked_up = 0;
    req->parsed_uri.dns_resolved = 0;

    /* req->parsed_uri.password = (char *)ngx_pstrdup0(r->pool, */
    /*  &r->headers_in.passwd);                                 */

    /* req->parsed_uri.user = (char *)ngx_pstrdup0(r->pool, */
    /*      &r->headers_in.user);                           */
    req->parsed_uri.fragment = (char *)ngx_pstrdup0(r->pool, &r->exten);

    req->hostname = (char *)ngx_pstrdup0(r->pool,
            (ngx_str_t *)&ngx_cycle->hostname);


    if (r->header_only) {
        req->header_only = r->header_only;
    }
    else {
        req->header_only = (r->method == NGX_HTTP_HEAD);
    }

    return NGX_OK;
}


/*
 * TODO: deal more headers.
 */

static ngx_inline ngx_int_t
ngx_http_modsecurity_load_headers_in(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t  *ctx;
    const char                  *lang;
    request_rec                 *req;
    ngx_list_part_t             *part;
    ngx_table_elt_t             *h;
    ngx_uint_t                   i;


    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    req = ctx->req;

    part = &r->headers_in.headers.part;
    h = part->elts;

    for (i = 0; ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL)
                break;

            part = part->next;
            h = part->elts;
            i = 0;
        }

        apr_table_setn(req->headers_in, (char *)h[i].key.data,
                (char *)h[i].value.data);
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "ModSecurity: load headers in: \"%V: %V\"",
                       &h[i].key, &h[i].value);
    }

    req->clength = r->headers_in.content_length_n;


    req->range = apr_table_get(req->headers_in, "Range");
    req->content_type = apr_table_get(req->headers_in, "Content-Type");
    req->content_encoding = apr_table_get(req->headers_in, "Content-Encoding");

    lang = apr_table_get(ctx->req->headers_in, "Content-Languages");
    if(lang != NULL)
    {
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1,
                sizeof(const char *));

        *(const char **)apr_array_push(ctx->req->content_languages) = lang;
    }

    req->ap_auth_type = (char *)apr_table_get(req->headers_in, "Authorization");

    req->user = (char *)ngx_pstrdup0(r->pool, &r->headers_in.user);

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: load headers in done");

    return NGX_OK;
}

static ngx_inline ngx_int_t
ngx_http_modsecurity_save_headers_in(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t  *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    /* clean up headers_in */
    ngx_memzero(&r->headers_in, sizeof(ngx_http_headers_in_t));
    if (ngx_list_init(&r->headers_in.headers, r->pool, 20,
                      sizeof(ngx_table_elt_t))
            != NGX_OK)
    {
        return NGX_ERROR;
    }


    if (ngx_array_init(&r->headers_in.cookies, r->pool, 2,
                       sizeof(ngx_table_elt_t *))
            != NGX_OK)
    {
        return NGX_ERROR;
    }

    r->headers_in.content_length_n = -1;
    r->headers_in.keep_alive_n = -1;

    r->headers_in.headers.part.nelts = 0;
    r->headers_in.headers.part.next = NULL;
    r->headers_in.headers.last = &r->headers_in.headers.part;

    /* shadow copy */
    if (apr_table_do(ngx_http_modsecurity_save_headers_in_visitor,
                     r, ctx->req->headers_in, NULL) == 0) {

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "ModSecurity: save headers in error");

        return NGX_ERROR;
    }

    if (r->headers_in.content_length) {
        r->headers_in.content_length_n =
            ngx_atoof(r->headers_in.content_length->value.data,
                      r->headers_in.content_length->value.len);

        if (r->headers_in.content_length_n == NGX_ERROR) {
            ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
                          "ModSecurity: invalid \"Content-Length\" header");
            return NGX_ERROR;
        }
    }

    if (r->headers_in.connection_type == NGX_HTTP_CONNECTION_KEEP_ALIVE) {
        if (r->headers_in.keep_alive) {
            r->headers_in.keep_alive_n =
                ngx_atotm(r->headers_in.keep_alive->value.data,
                          r->headers_in.keep_alive->value.len);
        }
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers in done");

    return NGX_OK;
}


static int ngx_http_modsecurity_save_headers_in_visitor(void *data,
        const char *key, const char *value)
{
    ngx_http_request_t         *r = data;
    ngx_table_elt_t            *h;
    ngx_http_header_t          *hh;
    ngx_http_core_main_conf_t  *cmcf;

    h = ngx_list_push(&r->headers_in.headers);
    if (h == NULL) {
        return 0;
    }

    h->key.data = (u_char *)key;
    h->key.len = ngx_strlen(key);

    h->value.data = (u_char *)value;
    h->value.len = ngx_strlen(value);

    h->lowcase_key = ngx_pnalloc(r->pool, h->key.len);

    if (h->lowcase_key == NULL) {
        return 0;
    }

    ngx_strlow(h->lowcase_key, h->key.data, h->key.len);

    h->hash = ngx_hash_key(h->lowcase_key, h->key.len);

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);

    hh = ngx_hash_find(&cmcf->headers_in_hash, h->hash,
                       h->lowcase_key, h->key.len);

    if (hh && hh->handler(r, h, hh->offset) != NGX_OK) {
        return 0;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers in: \"%V: %V\"",
                   &h->key, &h->value);

    return 1;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_load_request_body(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t *ctx;
    ngx_chain_t *chain;

    ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "ModSec: loading request body.");

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    modsecSetBodyBrigade(ctx->req, ctx->brigade);

    if (r->request_body == NULL) {
        chain = NULL;
    }
    else {
        chain = r->request_body->bufs;
    }


#ifdef MOVE_REQUEST_CHAIN_TO_MODSEC
    if (move_chain_to_brigade(chain, ctx->brigade, r->pool, 1) != NGX_OK) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: failed to move chain to brigade.");

        return NGX_ERROR;
    }

    r->request_body = NULL;
#else
    if (copy_chain_to_brigade(chain, ctx->brigade, r->pool, 1) != NGX_OK) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: failed to copy chain to brigade.");

        return NGX_ERROR;
    }
#endif

    return NGX_OK;
}

static ngx_inline ngx_int_t
ngx_http_modsecurity_save_request_body(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t *ctx;
#ifdef MOVE_REQUEST_CHAIN_TO_MODSEC
    apr_off_t content_length;
    ngx_buf_t *buf;
#endif

   ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

#ifdef MOVE_REQUEST_CHAIN_TO_MODSEC
    apr_brigade_length(ctx->brigade, 0, &content_length);

    buf = ngx_create_temp_buf(ctx->r->pool, (size_t) content_length);
    apr_brigade_flatten(ctx->brigade, (char *)buf->pos,
            (apr_size_t *)&content_length);
    apr_brigade_cleanup(ctx->brigade);
    buf->last += content_length;
    r->header_in = buf;

    if (r->headers_in.content_length) {
        ngx_str_t *str = NULL;

        str = &r->headers_in.content_length->value;
        str->data = ngx_palloc(r->pool, NGX_OFF_T_LEN);
        if (str->data == NULL) {
            ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
            return NGX_OK;
        }
        str->len = ngx_snprintf(str->data, NGX_OFF_T_LEN, "%O",
                content_length) - str->data;

    }

    r->headers_in.content_length_n = content_length;

    ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: Content length: %O, Content length n: %O", content_length,
            r->headers_in.content_length_n);
#else
    apr_brigade_cleanup(ctx->brigade);
#endif

    return NGX_OK;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_load_headers_out(ngx_http_request_t *r)
{

    ngx_http_modsecurity_ctx_t  *ctx;
    char                        *data;
    request_rec                 *req;
    ngx_http_variable_value_t   *vv;
    ngx_list_part_t             *part;
    ngx_table_elt_t             *h;
    ngx_uint_t                   i;
    char                        *key, *value;
    u_char                      *buf = NULL;
    size_t                       size = 0;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    req = ctx->req;

    req->status = r->headers_out.status;
    req->status_line = (char *)ngx_pstrdup0(r->pool,
            &r->headers_out.status_line);

    /* deep copy */
    part = &r->headers_out.headers.part;
    h = part->elts;

    for (i = 0; ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL)
                break;

            part = part->next;
            h = part->elts;
            i = 0;
        }
        size += h[i].key.len + h[i].value.len + 2;

        buf = ngx_palloc(r->pool, size);

        if (buf == NULL) {
            return NGX_ERROR;
        }

        key = (char *)buf;
        buf = ngx_cpymem(buf, h[i].key.data, h[i].key.len);
        *buf++ = '\0';

        value = (char *)buf;
        buf = ngx_cpymem(buf, h[i].value.data, h[i].value.len);
        *buf++ = '\0';

        apr_table_addn(req->headers_out, key, value);
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "ModSecurity: load headers out: \"%V: %V\"",
                       &h[i].key, &h[i].value);

    }

    for (i = 0; special_headers_out[i].name; i++) {

        vv = ngx_http_get_variable(r, &special_headers_out[i].variable_name,
                ngx_hash_key(special_headers_out[i].variable_name.data,
                        special_headers_out[i].variable_name.len));

        if (vv && !vv->not_found) {

            data = ngx_palloc(r->pool, vv->len + 1);
            if (data == NULL) {
                return NGX_ERROR;
            }

            ngx_memcpy(data,vv->data, vv->len);
            data[vv->len] = '\0';

            apr_table_setn(req->headers_out, special_headers_out[i].name, data);
            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "ModSecurity: load headers out: \"%s: %s\"",
                           special_headers_out[i].name, data);
        }
    }

    req->content_type = apr_table_get(ctx->req->headers_out, "Content-Type");
    req->content_encoding = apr_table_get(ctx->req->headers_out,
            "Content-Encoding");

    data = (char *)apr_table_get(ctx->req->headers_out, "Content-Languages");

    if(data != NULL)
    {
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1,
                sizeof(const char *));
        *(const char **)apr_array_push(ctx->req->content_languages) = data;
    }

    req->chunked = r->chunked;
    req->clength = r->headers_out.content_length_n;
    req->mtime = apr_time_make(r->headers_out.last_modified_time, 0);

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: load headers out done");

    return NGX_OK;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_save_headers_out(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_http_upstream_t             *upstream;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    r->chunked = ctx->req->chunked;

    ngx_http_clean_header(r);

    upstream = r->upstream;
    r->upstream = &ngx_http_modsecurity_upstream;

    /* case SecServerSignature was used, the "Server: ..." header is added
     * here, overwriting the default header supplied by nginx.
     */
    if (modsecIsServerSignatureAvailale() != NULL) {
        apr_table_add(ctx->req->headers_out, "Server",
                modsecIsServerSignatureAvailale());
    }

    if (apr_table_do(ngx_http_modsecurity_save_headers_out_visitor,
                     r, ctx->req->headers_out, NULL) == 0) {

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "ModSecurity: save headers out error");

        return NGX_ERROR;
    }

    r->upstream = upstream;

    r->headers_out.status = ctx->req->status;
    r->headers_out.status_line.data = (u_char *)ctx->req->status_line;
    r->headers_out.status_line.len = ctx->req->status_line ?
                                     ngx_strlen(ctx->req->status_line) : 0;

    r->headers_out.content_length_n = ctx->req->clength;
    r->headers_out.last_modified_time = apr_time_sec(ctx->req->mtime);

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers out done");

    return NGX_OK;
}


static int
ngx_http_modsecurity_save_headers_out_visitor(void *data,
        const char *key, const char *value)
{
    ngx_http_request_t             *r = data;
    ngx_table_elt_t                *h, he;
    ngx_http_upstream_header_t     *hh;
    ngx_http_upstream_main_conf_t  *umcf;

    umcf = ngx_http_get_module_main_conf(r, ngx_http_upstream_module);

    h = &he;

    h->key.data = (u_char *)key;
    h->key.len = ngx_strlen(key);

    h->value.data = (u_char *)value;
    h->value.len = ngx_strlen(value);

    h->lowcase_key = ngx_palloc(r->pool, h->key.len);
    if (h->lowcase_key == NULL) {
        return 0;
    }

    ngx_strlow(h->lowcase_key, h->key.data, h->key.len);

    h->hash = ngx_hash_key(h->lowcase_key, h->key.len);

    hh = ngx_hash_find(&umcf->headers_in_hash, h->hash,
                       h->lowcase_key, h->key.len);

    if (hh) {
        /* copy all */
        if (hh->copy_handler(r, h, hh->conf) != NGX_OK) {
            return 0;
        }
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers out: \"%V: %V\"",
                   &h->key, &h->value);

    return 1;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_status(ngx_http_request_t *r, int status)
{
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSecurity: status %d", status);

    if (status == DECLINED || status == APR_SUCCESS) {
        return NGX_DECLINED;
    }

    /* nginx known status */
    if ( (status >= 300 && status < 308)       /* 3XX */
            || (status >= 400 && status < 417) /* 4XX */
            || (status >= 500 && status < 508) /* 5XX */
            || (status == NGX_HTTP_CREATED || status == NGX_HTTP_NO_CONTENT) ) {

        return status;
    }

    return NGX_HTTP_INTERNAL_SERVER_ERROR;
}


/*
 * @brief Process request whenever `request header` and `body` are ready for.
 *
 * This function provides calls ModSecurity standalone library in the necessary
 * order to analisys: `request headers` and `request body`.
 *
 * @note ModSecurity state can be changed by ctl:ruleEngine=off
 *
 * @param r pointer to ngx_http_request_t.
 *
 */
ngx_int_t
ngx_http_modsecurity_process_request(ngx_http_request_t *r)
{
    ngx_int_t rc = 0;
    ngx_http_modsecurity_ctx_t *ctx;
    int load_request_body = 0;

    ctx = ngx_http_get_module_pool_ctx(r, ngx_http_modsecurity);

    ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "ModSec: starting to process the request...");

    if (ngx_http_modsecurity_load_request(r) != NGX_OK) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
               "ModSec: failed while loading the request.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if (ngx_http_modsecurity_load_headers_in(r) != NGX_OK) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
               "ModSec: failed while loading the headers.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    rc = modsecProcessRequestHeaders(ctx->req);
    rc = ngx_http_modsecurity_status(r, rc);
    if (rc != NGX_DECLINED) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
               "ModSec: failed while processing the request headers");
        return rc;
    }

    /* Here we check if ModSecurity is enabled or disabled again.
     * This is a kind of hack. ModSecurity can be disable by:
     * ctl:ruleEngine=off, which shouldn't be a problem. However, due to
     * the way that standalone is handling this situation this lead us to
     * a problem that may end up by freezing the connection. As we don't
     * want that we verify if it was disabled prior to load the request body.
     *
     * Changes on standalone should be coordenated with others like IIS, so
     * leaving this dirty hack here.
     */
    if (r->method == NGX_HTTP_POST &&
        modsecIsRequestBodyAccessEnabled(ctx->req) &&
        modsecContextState(ctx->req) != MODSEC_DISABLED) {

        load_request_body = 1;
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: request body will be processed...");
    }

    if (load_request_body == 1) {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: loading request body...");

        rc = ngx_http_modsecurity_load_request_body(r);
        if (rc != NGX_OK)
        {
            ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                "ModSec: failed while loading the request body.");
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }


    }

    ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "ModSec: processing request body...");
    rc = modsecProcessRequestBody(ctx->req);
    rc = ngx_http_modsecurity_status(r, rc);
    if (rc != NGX_DECLINED)
    {
        ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: finalizing the connection after process the " \
            "request body.");

        return rc;
    }
    if (load_request_body == 1) {
        if (ngx_http_modsecurity_save_request_body(r) != NGX_OK)
        {
            ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                "ModSec: failed while saving the request body");
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        if (ngx_http_modsecurity_save_headers_in(r) != NGX_OK)
        {
            ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                "ModSec: failed while saving the headers in");
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
    }
    return NGX_OK;
}



/* create loc conf struct */
static void *
ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_modsecurity_loc_conf_t  *conf;

    conf = (ngx_http_modsecurity_loc_conf_t  *)
           ngx_palloc(cf->pool, sizeof(ngx_http_modsecurity_loc_conf_t));
    if (conf == NULL)
        return NULL;

    conf->config = NGX_CONF_UNSET_PTR;
    conf->enable = NGX_CONF_UNSET;

    return conf;
}

/* merge loc conf */
static char *
ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent,
                                    void *child)
{
    ngx_http_modsecurity_loc_conf_t  *prev = parent;
    ngx_http_modsecurity_loc_conf_t  *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);
    ngx_conf_merge_ptr_value(conf->config, prev->config, NULL);

    if (conf->enable && conf->config == NULL) {
        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                      "\"ModSecurityEnabled\" in %V:%ui is set to \"on\""
                      " while directive \"ModSecurityConfig\" is not found"
                      " in the same location",
                      conf->file, conf->line);
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

static void
modsecLog(void *obj, int level, char *str)
{
    if (obj != NULL) {
        level = (level & APLOG_LEVELMASK) + NGX_LOG_EMERG - APLOG_EMERG;
        if (level > NGX_LOG_DEBUG) {
            level = NGX_LOG_DEBUG;
        }
        ngx_log_error((ngx_uint_t)level, (ngx_log_t *)obj, 0, "%s", str);
    }
}


static ngx_int_t
ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf)
{
    /*  XXX: temporary hack, nginx uses pcre as well and hijacks these two */
    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    modsecSetLogHook(cf->log, modsecLog);
    modsecSetDropAction(ngx_http_modsecurity_drop_action);

    /* TODO: server_rec per server conf */
    modsec_server = modsecInit();
    if (modsec_server == NULL) {
        return NGX_ERROR;
    }

    /* set host name */
    modsec_server->server_hostname = ngx_palloc(cf->pool,
            ngx_cycle->hostname.len + 1);
    if (modsec_server->server_hostname == NULL) {
        return NGX_ERROR;
    }
    ngx_memcpy(modsec_server->server_hostname, ngx_cycle->hostname.data,
            ngx_cycle->hostname.len);
    modsec_server->server_hostname[ ngx_cycle->hostname.len] = '\0';

    modsecStartConfig();
    return NGX_OK;
}


static void
ngx_http_modsecurity_terminate(ngx_cycle_t *cycle)
{
    if (modsec_server) {
        modsecTerminate();
        modsec_server = NULL;
    }
}

static ngx_int_t
ngx_http_modsecurity_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    modsecFinalizeConfig();

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);


    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_modsecurity_handler;

#ifndef NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_modsecurity_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_modsecurity_body_filter;
#endif

    ngx_memzero(&ngx_http_modsecurity_upstream, sizeof(ngx_http_upstream_t));
#ifdef MODSEC_UPSTREAM_CACHABLE
    ngx_http_modsecurity_upstream.cacheable = 1;
#endif
    return NGX_OK;
}

static ngx_int_t
ngx_http_modsecurity_init_process(ngx_cycle_t *cycle)
{
    /* must set log hook here cf->log maybe changed */
    modsecSetLogHook(cycle->log, modsecLog);
    modsecInitProcess();
    return NGX_OK;
}

static ngx_int_t
ngx_http_modsecurity_handler(ngx_http_request_t *r) {
    ngx_int_t rc = NULL;
    ngx_http_modsecurity_ctx_t *ctx = NULL;
    ngx_http_modsecurity_loc_conf_t *cf = NULL;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: Catching a new access phase handler. Count: %d",
            r->main->count);

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);

    /* Process only main request */
    if (r != r->main || !cf->enable) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: ModSecurity is not enabled or not a main request.");

        return NGX_DECLINED;
    }

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: Recovering ctx: %p", ctx);

    if (ctx == NULL) {
        ctx = ngx_http_modsecurity_create_ctx(r);

        ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);

        if (ngx_http_set_pool_ctx(r, ctx, ngx_http_modsecurity) != NGX_OK) {
            return NGX_ERROR;
        }


    }

    if (ctx == NULL) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: ctx is null, nothing we can do, returning.");

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: ctx is now: %p / count: %d", ctx, r->main->count);

    if (modsecContextState(ctx->req) == MODSEC_DISABLED) {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: ModSecurity was disabled, returning....", ctx);

        return NGX_DECLINED;
    }
    if (ctx->waiting_more_body == 1) {
       ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
               "ModSec: waiting for more data before proceed. / count: %d",
               r->main->count);

        return NGX_DONE;
    }

    if (ctx->body_requested == 0) {
       ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: asking for the request body, if any.");

        ctx->body_requested = 1;
        r->request_body_in_single_buf = 1;

        rc = ngx_http_read_client_request_body(r,
            ngx_http_modsecurity_request_read);

        if (rc == NGX_ERROR || rc >= NGX_HTTP_SPECIAL_RESPONSE) {
           return rc;
        }

        if (rc == NGX_AGAIN) {
            ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                "ModSec: nginx is asking us to wait for more data.");
            ctx->waiting_more_body = 1;
            return NGX_DONE;
        }
    }

    if (ctx->waiting_more_body == 0 && ctx->request_processed == 0) {

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "ModSec: request is ready to be processed.");
        rc = ngx_http_modsecurity_process_request(r);
        ctx->request_processed = 1;

        if (rc == NGX_ERROR || rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                "ModSec: returning a special response after process " \
                "a request: %d", rc);

           return rc;
        }


    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "ModSec: returning NGX_DECLINED." );

    return NGX_DECLINED;
}

/* post read callback for rewrite and access phases */
void
ngx_http_modsecurity_request_read(ngx_http_request_t *r)
{

    ngx_http_modsecurity_ctx_t *ctx;

    r->read_event_handler = ngx_http_request_empty_handler;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "ModSec: reading request body...");

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    r->main->count--;
    if (ctx->waiting_more_body) {
        ctx->waiting_more_body = 0;
        ngx_http_core_run_phases(r);
    }

}

#ifndef NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
static ngx_int_t
ngx_http_modsecurity_header_filter(ngx_http_request_t *r) {
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    const char                      *location;
    ngx_table_elt_t                 *h;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    /* already processed, checking redirect action. */
    if (ctx && ctx->complete
               && r->err_status >= NGX_HTTP_MOVED_PERMANENTLY
               && r->err_status < 308) {

        /* 3XX load redirect location header so that we can do redirect */
        /* in phase 3,4 */
        location = apr_table_get(ctx->req->headers_out, "Location");

        if (location == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        h = ngx_list_push(&r->headers_out.headers);
        if (h == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        h->hash = 1;
        h->key.data = (u_char *)"Location";
        h->key.len = ngx_strlen("Location");
        h->value.data = (u_char *)location;
        h->value.len = ngx_strlen(location);

        return ngx_http_next_header_filter(r);
    }

    if (r != r->main || !cf->enable || ctx == NULL ||ctx->complete) {
        return ngx_http_next_header_filter(r);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "modSecurity: header filter");

    r->filter_need_in_memory = 1;
    return NGX_OK;
}
#endif
#ifndef NGX_HTTP_MODSECURITY_PREACCESS_HANDLER_ONLY
static ngx_int_t
ngx_http_modsecurity_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;
    apr_off_t                        content_length;
    ngx_chain_t                     *cl, *out;
    ngx_int_t                        last_buf = 0;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (r != r->main || !cf->enable || ctx == NULL || ctx->complete) {
        return ngx_http_next_body_filter(r, in);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
            "modSecurity: body filter");

    for (cl = in; cl; cl = cl->next) {
        apr_bucket  *e;
        ngx_buf_t   *buf = cl->buf;
        apr_bucket_brigade  *bb = ctx->brigade;
        off_t size = ngx_buf_size(buf);
        if (size) {
            char *data = apr_pmemdup(bb->p, buf->pos, size);
            if (data == NULL) {
                return ngx_http_filter_finalize_request(r,
                         &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
            }
            e = apr_bucket_pool_create(data , size, bb->p, bb->bucket_alloc);
            if (e == NULL) {
                return ngx_http_filter_finalize_request(r,
                         &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
            }
            APR_BRIGADE_INSERT_TAIL(bb, e);
        }

        if (buf->last_buf) {
            last_buf = 1;
            buf->last_buf = 0;
            e = apr_bucket_eos_create(bb->bucket_alloc);
            if (e == NULL) {
                return ngx_http_filter_finalize_request(r,
                         &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
            }
            APR_BRIGADE_INSERT_TAIL(bb, e);
            break;
        }

        buf->pos = buf->last;
    }

    if (!last_buf) {
        return NGX_AGAIN;
    }

    /* last buf has been saved */
    ctx->complete = 1;
    modsecSetResponseBrigade(ctx->req, ctx->brigade);

    if (ngx_http_modsecurity_load_headers_in(r) != NGX_OK
            || ngx_http_modsecurity_load_headers_out(r) != NGX_OK) {

        return ngx_http_filter_finalize_request(r,
                 &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    rc = ngx_http_modsecurity_status(r, modsecProcessResponse(ctx->req));

    if (rc != NGX_DECLINED) {
        return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity, rc);
    }

    apr_brigade_length(ctx->brigade, 0, &content_length);

    rc = move_brigade_to_chain(ctx->brigade, &out, r->pool);
    if (rc == NGX_ERROR) {
           ngx_log_debug(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSec: Problems moving brigade to chain");

        return ngx_http_filter_finalize_request(r,
                 &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers out done: before");


    if (ngx_http_modsecurity_save_headers_in(r) != NGX_OK
            ||ngx_http_modsecurity_save_headers_out(r) != NGX_OK) {

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers out done _inside_");


        return ngx_http_filter_finalize_request(r,
                 &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: save headers out done: done");


    if (r->headers_out.content_length_n != -1) {

        r->headers_out.content_length_n = content_length;
        r->headers_out.content_length = NULL; /* header filter will set this */
    }

    r->header_sent = 0;

    rc = ngx_http_next_header_filter(r);
    if (rc == NGX_ERROR || rc > NGX_OK) {
       ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
           "ModSecurity: ngx_http_next_header_filter not NGX_OK.");

        return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity, rc);
    }

    return ngx_http_next_body_filter(r, out);

    return NGX_OK;
}
#endif

static ngx_http_modsecurity_ctx_t *
ngx_http_modsecurity_create_ctx(ngx_http_request_t *r)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_pool_cleanup_t              *cln;
    ngx_http_modsecurity_ctx_t      *ctx;
    apr_sockaddr_t                  *asa;
    struct sockaddr_in              *sin;
    char *txid;
    unsigned char salt[TXID_SIZE];
    int i;
#if (NGX_HAVE_INET6)
    struct sockaddr_in6             *sin6;
#endif


    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
    if (ctx == NULL) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
                "modSecurity: ctx memory allocation error");
        return NULL;
    }

    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
    if (cln == NULL) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
                "modSecurity: ctx memory allocation error");
        return NULL;
    }
    cln->handler = ngx_http_modsecurity_cleanup;
    cln->data = ctx;

    ctx->r = r;

    /*

    Using pcalloc already set to zero.

    ctx->request_processed = 0;
    ctx->waiting_more_body = 0;
    ctx->body_requested = 0;
    */

    if (r->connection->requests == 0 || ctx->connection == NULL) {

        /* TODO: set server_rec, why igonre return value? */
        ctx->connection = modsecNewConnection();

        /* fill apr_sockaddr_t */
        asa = ngx_palloc(r->pool, sizeof(apr_sockaddr_t));
        asa->pool = ctx->connection->pool;
        asa->hostname = (char *)ngx_pstrdup0(r->pool,
                &r->connection->addr_text);
        asa->servname = asa->hostname;
        asa->next = NULL;
        asa->salen = r->connection->socklen;
        ngx_memcpy(&asa->sa, r->connection->sockaddr, asa->salen);

        asa->family = ((struct sockaddr *)&asa->sa)->sa_family;
        switch ( asa->family) {

#if (NGX_HAVE_INET6)
        case AF_INET6:
            sin6 = (struct sockaddr_in6 *)&asa->sa;
            asa->ipaddr_ptr = &sin6->sin6_addr;
            asa->ipaddr_len = sizeof(sin6->sin6_addr);
            asa->port = ntohs(sin6->sin6_port);
            asa->addr_str_len = NGX_INET6_ADDRSTRLEN + 1;
            break;
#endif

        default: /* AF_INET */
            sin = (struct sockaddr_in *) &asa->sa;
            asa->ipaddr_ptr = &sin->sin_addr;
            asa->ipaddr_len = sizeof(sin->sin_addr);
            asa->port = ntohs(sin->sin_port);
            asa->addr_str_len = NGX_INET_ADDRSTRLEN + 1;
            break;
        }


#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
        ctx->connection->remote_addr = asa;
        ctx->connection->remote_ip = asa->hostname;
#else
        ctx->connection->client_addr = asa;
        ctx->connection->client_ip = asa->hostname;
#endif
        ctx->connection->remote_host = NULL;
        modsecProcessConnection(ctx->connection);
    }

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);

    ctx->req = modsecNewRequest(ctx->connection, cf->config);

    apr_table_setn(ctx->req->notes, NOTE_NGINX_REQUEST_CTX, (const char *) ctx);
    apr_generate_random_bytes(salt, TXID_SIZE);

    txid = apr_pcalloc (ctx->req->pool, TXID_SIZE);
    apr_base64_encode (txid, (const char*)salt, TXID_SIZE);

    for(i=0;i<TXID_SIZE;i++)        {
        if((salt[i] >= 0x30) && (salt[i] <= 0x39))      {}
        else if((salt[i] >= 0x40) && (salt[i] <= 0x5A)) {}
        else if((salt[i] >= 0x61) && (salt[i] <= 0x7A)) {}
        else {
            if((i%2)==0)
                salt[i] = 0x41;
            else
                salt[i] = 0x63;
        }
    }

    salt[TXID_SIZE-1] = '\0';

    apr_table_setn(ctx->req->subprocess_env, "UNIQUE_ID",
            apr_psprintf(ctx->req->pool, "%s", salt));

    ctx->brigade = apr_brigade_create(ctx->req->pool,
            ctx->req->connection->bucket_alloc);

    if (ctx->brigade == NULL) {
        return NULL;
    }
    return ctx;
}

static void
ngx_http_modsecurity_cleanup(void *data)
{
    ngx_http_modsecurity_ctx_t *ctx = data;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, ctx->r->connection->log, 0,
            "ModSec: Cleaning up: %p ", data);

    if (ctx->req != NULL) {
        (void) modsecFinishRequest(ctx->req);
    }
    if (ctx->connection != NULL) {
        (void) modsecFinishConnection(ctx->connection);
    }

}

    static char *
ngx_http_modsecurity_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_modsecurity_loc_conf_t *mscf = conf;
    ngx_str_t       *value;
    const char      *msg;

    if (mscf->config != NGX_CONF_UNSET_PTR) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (ngx_conf_full_name(cf->cycle, &value[1], 1) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    mscf->config = modsecGetDefaultConfig();

    if (mscf->config == NULL) {
        return NGX_CONF_ERROR;
    }

    msg = modsecProcessConfig(mscf->config, (const char *)value[1].data, NULL);
    if (msg != NULL) {
        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                "ModSecurityConfig in %s:%ui: %s",
                cf->conf_file->file.name.data, cf->conf_file->line, msg);
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

    static char *
ngx_http_modsecurity_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_modsecurity_loc_conf_t *mscf = conf;
    char                            *rc;

    rc = ngx_conf_set_flag_slot(cf, cmd, conf);
    if (rc != NGX_CONF_OK) {
        return rc;
    }
    if (mscf->enable) {
        mscf->file = &cf->conf_file->file.name;
        mscf->line = cf->conf_file->line;
    }
    return NGX_CONF_OK;
}

static int
ngx_http_modsecurity_drop_action(request_rec *r)
{
    ngx_http_modsecurity_ctx_t     *ctx;
    ctx = (ngx_http_modsecurity_ctx_t *) apr_table_get(r->notes,
            NOTE_NGINX_REQUEST_CTX);

    if (ctx == NULL) {
        return -1;
    }

    ctx->r->connection->error = 1;
    return 0;
}

