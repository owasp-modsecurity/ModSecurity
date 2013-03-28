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

#include <ngx_http.h>
#include <apr_bucket_nginx.h>
#include <ngx_pool_context.h>

#undef CR
#undef LF
#undef CRLF

#include "api.h"

#define NOTE_NGINX_REQUEST_CTX "nginx-ctx"

typedef struct {
    ngx_flag_t                  enable;
    directory_config            *config;

    ngx_str_t                   *file;
    ngx_uint_t                   line;
} ngx_http_modsecurity_loc_conf_t;

typedef struct {
    ngx_http_request_t  *r;
    conn_rec            *connection;
    request_rec         *req;

    apr_bucket_brigade  *brigade;
    unsigned             complete;
} ngx_http_modsecurity_ctx_t;


/*
** Module's registred function/handlers.
*/
static ngx_int_t ngx_http_modsecurity_handler(ngx_http_request_t *r);
static void ngx_http_modsecurity_body_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_modsecurity_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_http_modsecurity_body_filter(ngx_http_request_t *r, ngx_chain_t *in);
static ngx_int_t ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle);
static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_modsecurity_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_modsecurity_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_http_modsecurity_ctx_t * ngx_http_modsecurity_create_ctx(ngx_http_request_t *r);
static int ngx_http_modsecurity_drop_action(request_rec *r);
static void ngx_http_modsecurity_cleanup(void *data);

static int ngx_http_modsecurity_save_headers_in_visitor(void *data, const char *key, const char *value);
static int ngx_http_modsecurity_save_headers_out_visitor(void *data, const char *key, const char *value);


/* command handled by the module */
static ngx_command_t  ngx_http_modsecurity_commands[] =  {
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

/*
** handlers for configuration phases of the module
*/

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
    ngx_http_modsecurity_exit_process, /* exit process */
    ngx_http_modsecurity_exit_process, /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;

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


static inline int ngx_http_modsecurity_method_number(unsigned int nginx)
{
    /*
     * http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightMultLookup
     */
    static const int MultiplyDeBruijnBitPosition[32] = {
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

    return MultiplyDeBruijnBitPosition[((uint32_t)((nginx & -nginx) * 0x077CB531U)) >> 27];
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

    req->parsed_uri.path = req->path_info;
    req->parsed_uri.is_initialized = 1;

    str.data = r->port_start;
    str.len = r->port_end - r->port_start;
    req->parsed_uri.port = ngx_atoi(str.data, str.len);
    req->parsed_uri.port_str = (char *)ngx_pstrdup0(r->pool, &str);

    req->parsed_uri.query = req->args;
    req->parsed_uri.dns_looked_up = 0;
    req->parsed_uri.dns_resolved = 0;

    // req->parsed_uri.password = (char *)ngx_pstrdup0(r->pool, &r->headers_in.passwd);
    // req->parsed_uri.user = (char *)ngx_pstrdup0(r->pool, &r->headers_in.user);
    req->parsed_uri.fragment = (char *)ngx_pstrdup0(r->pool, &r->exten);

    req->hostname = (char *)ngx_pstrdup0(r->pool, (ngx_str_t *)&ngx_cycle->hostname);

    req->header_only = r->header_only ? r->header_only : (r->method == NGX_HTTP_HEAD);

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

        apr_table_setn(req->headers_in, (char *)h[i].key.data, (char *)h[i].value.data);
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
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1, sizeof(const char *));

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


static int
ngx_http_modsecurity_save_headers_in_visitor(void *data, const char *key, const char *value)
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
    ngx_http_modsecurity_ctx_t    *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    modsecSetBodyBrigade(ctx->req, ctx->brigade);

    if (r->request_body == NULL || r->request_body->bufs == NULL) {

        return move_chain_to_brigade(NULL, ctx->brigade, r->pool, 1);
    }

    if (move_chain_to_brigade(r->request_body->bufs, ctx->brigade, r->pool, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    r->request_body = NULL;

    return NGX_OK;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_save_request_body(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t    *ctx;
    apr_off_t                      content_length;
    ngx_buf_t                     *buf;
    ngx_http_core_srv_conf_t      *cscf;
    size_t                         size;
    ngx_http_connection_t         *hc;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    apr_brigade_length(ctx->brigade, 0, &content_length);

    if (r->header_in->end - r->header_in->last >= content_length) {
        /* use r->header_in */

        if (ngx_buf_size(r->header_in)) {
            /* move to the end */
            ngx_memmove(r->header_in->pos + content_length,
                        r->header_in->pos,
                        ngx_buf_size(r->header_in));
        }

        if (apr_brigade_flatten(ctx->brigade,
                                (char *)r->header_in->pos,
                                (apr_size_t *)&content_length) != APR_SUCCESS) {
            return NGX_ERROR;
        }

        apr_brigade_cleanup(ctx->brigade);

        r->header_in->last += content_length;

        return NGX_OK;
    }

    if (ngx_buf_size(r->header_in)) {

        /*
         * ngx_http_set_keepalive will reuse r->header_in if
         * (r->header_in != c->buffer && r->header_in.last != r->header_in.end),
         * so we need this code block.
         * see ngx_http_set_keepalive, ngx_http_alloc_large_header_buffer
         */
        cscf = ngx_http_get_module_srv_conf(r, ngx_http_core_module);

        size = ngx_max(cscf->large_client_header_buffers.size,
                       (size_t)content_length + ngx_buf_size(r->header_in));

        hc = r->http_connection;

        if (hc->nfree && size == cscf->large_client_header_buffers.size) {

            buf = hc->free[--hc->nfree];

            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                           "ModSecurity: use http free large header buffer: %p %uz",
                           buf->pos, buf->end - buf->last);

        } else if (hc->nbusy < cscf->large_client_header_buffers.num) {

            if (hc->busy == NULL) {
                hc->busy = ngx_palloc(r->connection->pool,
                                      cscf->large_client_header_buffers.num * sizeof(ngx_buf_t *));
            }

            if (hc->busy == NULL) {
                return NGX_ERROR;
            } else {
                buf = ngx_create_temp_buf(r->connection->pool, size);
            }
        } else {
            /* TODO: how to deal this case ? */
            return NGX_ERROR;
        }

    } else {

        buf = ngx_create_temp_buf(r->pool, (size_t) content_length);
    }

    if (buf == NULL) {
        return NGX_ERROR;
    }

    if (apr_brigade_flatten(ctx->brigade, (char *)buf->pos,
                            (apr_size_t *)&content_length) != APR_SUCCESS) {
        return NGX_ERROR;
    }

    apr_brigade_cleanup(ctx->brigade);
    buf->last += content_length;

    ngx_memcpy(buf->last, r->header_in->pos, ngx_buf_size(r->header_in));
    buf->last += ngx_buf_size(r->header_in);

    r->header_in = buf;

    return NGX_OK;
}


static ngx_inline ngx_int_t
ngx_http_modsecurity_load_headers_out(ngx_http_request_t *r)
{

    ngx_http_modsecurity_ctx_t  *ctx;
    char                        *data;
    request_rec                 *req;
    u_char                      *content_type;
    ngx_uint_t                   content_type_len;
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
    req->status_line = (char *)ngx_pstrdup0(r->pool, &r->headers_out.status_line);

    if (r->headers_out.charset.len) {

        content_type_len = r->headers_out.content_type.len
                           + r->headers_out.charset.len
                           + ngx_strlen("; charset=") + 1;

        content_type = ngx_palloc(r->pool, content_type_len);

        if (content_type == NULL) {
            return NGX_ERROR;
        }

        ngx_snprintf(content_type, content_type_len,
                     "%V; charset=%V",
                     &r->headers_out.content_type,
                     &r->headers_out.charset);

        r->headers_out.content_type.data = content_type;
        r->headers_out.content_type.len = content_type_len;
    }

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

        apr_table_setn(req->headers_out, key, value);
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
    req->content_encoding = apr_table_get(ctx->req->headers_out, "Content-Encoding");

    data = (char *)apr_table_get(ctx->req->headers_out, "Content-Languages");

    if(data != NULL)
    {
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1, sizeof(const char *));
        *(const char **)apr_array_push(ctx->req->content_languages) = data;
    }

    /* req->chunked = r->chunked; may be useless */
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

    /* r->chunked = ctx->req->chunked; */

    ngx_http_clean_header(r);

    upstream = r->upstream;
    r->upstream = &ngx_http_modsecurity_upstream;

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
ngx_http_modsecurity_save_headers_out_visitor(void *data, const char *key, const char *value)
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

static ngx_int_t
ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf)
{
    server_rec *s;

    /*  XXX: temporary hack, nginx uses pcre as well and hijacks these two */
    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    modsecSetLogHook(cf->log, modsecLog);
    modsecSetDropAction(ngx_http_modsecurity_drop_action);

    /* TODO: server_rec per server conf */
    s = modsecInit();
    if (s == NULL) {
        return NGX_ERROR;
    }

    /* set host name */
    s->server_hostname = ngx_palloc(cf->pool, ngx_cycle->hostname.len + 1);
    if (s->server_hostname == NULL) {
        return NGX_ERROR;
    }
    ngx_memcpy(s->server_hostname, ngx_cycle->hostname.data, ngx_cycle->hostname.len);
    s->server_hostname[ ngx_cycle->hostname.len] = '\0';

    modsecStartConfig();
    return NGX_OK;
}



static ngx_int_t
ngx_http_modsecurity_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    modsecFinalizeConfig();

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    if (cmcf == NULL) {
        return NGX_ERROR;
    }

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_modsecurity_handler;

    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_modsecurity_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_modsecurity_body_filter;

    ngx_memzero(&ngx_http_modsecurity_upstream, sizeof(ngx_http_upstream_t));
    ngx_http_modsecurity_upstream.cacheable = 1;
    return NGX_OK;
}


static ngx_int_t
ngx_http_modsecurity_init_process(ngx_cycle_t *cycle)
{
    modsecSetLogHook(cycle->log, modsecLog);
    modsecInitProcess();
    return NGX_OK;
}

static void
ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle)
{
    modsecTerminate();
}


/*
** [ENTRY POINT] does : this function called by nginx from the request handler
*/
static ngx_int_t
ngx_http_modsecurity_handler(ngx_http_request_t *r)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);

    /* Process only main request */
    if (r != r->main || !cf->enable) {
        return NGX_DECLINED;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    if (r->internal) {
        /* we have already processed the request headers with previous loc conf */

        /* TODO: do we need update ctx and process headers again? */
        ctx = ngx_http_get_module_pool_ctx(r, ngx_http_modsecurity);

        if (ctx) {
            ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);
            return NGX_DECLINED;
        }

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: get internel request ctx failed");
    }

    ctx = ngx_http_modsecurity_create_ctx(r);
    if (ctx == NULL) {

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);

    if (ngx_http_set_pool_ctx(r, ctx, ngx_http_modsecurity) != NGX_OK) {
        return NGX_ERROR;
    }

    ngx_http_modsecurity_load_request(r);

    if (ngx_http_modsecurity_load_headers_in(r) != NGX_OK) {

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    /* processing request headers */
    rc = modsecProcessRequestHeaders(ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestHeaders %d", rc);

    if (rc == DECLINED) {

        if (modsecIsRequestBodyAccessEnabled(ctx->req)
                && r->method == NGX_HTTP_POST) {

            /* Processing POST request body, should we process PUT? */
            rc = ngx_http_read_client_request_body(r, ngx_http_modsecurity_body_handler);
            if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
                return rc;
            }

            return NGX_DONE;
        }
        /* other method */
        rc = modsecProcessRequestBody(ctx->req);
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestBody %d", rc);
    }

    if (rc != DECLINED) {

        /* Nginx and Apache share same response code  */
        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return rc;
    }

    /*
    if (ngx_http_modsecurity_save_headers_in(r) != NGX_OK) {

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    */

    return NGX_DECLINED;
}


static void
ngx_http_modsecurity_body_handler(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t    *ctx;
    ngx_int_t                      rc;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: body handler");

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (ngx_http_modsecurity_load_request_body(r) != NGX_OK) {

        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    rc = modsecProcessRequestBody(ctx->req);

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestBody %d", rc);

    if (rc != DECLINED) {
        /* Nginx and Apache share same response code  */
        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            rc = NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return ngx_http_finalize_request(r, rc);
    }

    if (ngx_http_modsecurity_save_request_body(r) != NGX_OK
            || ngx_http_modsecurity_save_headers_in(r) != NGX_OK ) {

        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    r->phase_handler++;
    ngx_http_core_run_phases(r);
    ngx_http_finalize_request(r, NGX_DONE);
}


static ngx_int_t
ngx_http_modsecurity_header_filter(ngx_http_request_t *r) {
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (r != r->main || !cf->enable || ctx->complete) {
        return ngx_http_next_header_filter(r);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: header filter");

    if (r->method == NGX_HTTP_HEAD || r->header_only) {

        ctx->complete = 1;

        if (ngx_http_modsecurity_load_headers_in(r) != NGX_OK
                || ngx_http_modsecurity_load_headers_out(r) != NGX_OK) {

            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        rc = modsecProcessResponse(ctx->req);
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessResponse %d", rc);

        if (rc == DECLINED || rc == APR_SUCCESS) {

            if (ngx_http_modsecurity_save_headers_in(r) != NGX_OK
                    || ngx_http_modsecurity_save_headers_out(r) != NGX_OK) {
                return NGX_HTTP_INTERNAL_SERVER_ERROR;
            }

            return ngx_http_next_header_filter(r);
        }

        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            rc = NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        return rc;
    }

    return NGX_OK;
}


static ngx_int_t
ngx_http_modsecurity_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;
    apr_off_t                        content_length;

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (r != r->main || !cf->enable || ctx->complete) {
        return ngx_http_next_body_filter(r, in);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: body filter");

    if (in == NULL) {
        return NGX_AGAIN;
    }

    rc = move_chain_to_brigade(in, ctx->brigade, r->pool, 0);
    if (rc != NGX_OK)  {
        return rc;
    }

    /* last buf has been saved */

    ctx->complete = 1;
    modsecSetResponseBrigade(ctx->req, ctx->brigade);

    // TODO: do we need reload headers_in ?
    //
    if (ngx_http_modsecurity_load_headers_in(r) != NGX_OK
            || ngx_http_modsecurity_load_headers_out(r) != NGX_OK) {

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    rc = modsecProcessResponse(ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessResponse %d", rc);

    if (rc == DECLINED || rc == APR_SUCCESS) {

        in = NULL;

        apr_brigade_length(ctx->brigade, 0, &content_length);

        rc = move_brigade_to_chain(ctx->brigade, &in, r->pool);
        if (rc == NGX_ERROR) {
            return NGX_ERROR;
        }

        if (ngx_http_modsecurity_save_headers_in(r) != NGX_OK
                ||ngx_http_modsecurity_save_headers_out(r) != NGX_OK) {

            return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity, NGX_HTTP_INTERNAL_SERVER_ERROR);
        }

        if (r->headers_out.content_length_n != -1) {

            r->headers_out.content_length_n = content_length;
            r->headers_out.content_length = NULL; /* header filter will set this */
        }

        rc = ngx_http_next_header_filter(r);

        if (rc == NGX_ERROR || rc > NGX_OK) {
            return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity, rc);
        }

        return ngx_http_next_body_filter(r, in);
    }

    if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
        rc = NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    return ngx_http_filter_finalize_request(r, &ngx_http_modsecurity, rc);
}


static ngx_http_modsecurity_ctx_t *
ngx_http_modsecurity_create_ctx(ngx_http_request_t *r)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_pool_cleanup_t              *cln;
    ngx_http_modsecurity_ctx_t      *ctx;
    apr_sockaddr_t                  *asa;
    struct sockaddr_in              *sin;
#if (NGX_HAVE_INET6)
    struct sockaddr_in6             *sin6;
#endif


    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
    if (ctx == NULL) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "modSecurity: ctx memory allocation error");
        return NULL;
    }
    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
    if (cln == NULL) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "modSecurity: ctx memory allocation error");
        return NULL;
    }
    cln->handler = ngx_http_modsecurity_cleanup;
    cln->data = ctx;

    ctx->r = r;

    if (r->connection->requests == 0 || ctx->connection == NULL) {

        /* TODO: set server_rec, why igonre return value? */
        ctx->connection = modsecNewConnection();

        /* fill apr_sockaddr_t */
        asa = ngx_palloc(r->pool, sizeof(apr_sockaddr_t));
        asa->pool = ctx->connection->pool;
        asa->hostname = (char *)ngx_pstrdup0(r->pool, &r->connection->addr_text);
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
    apr_table_setn(ctx->req->subprocess_env, "UNIQUE_ID", "12345");

    ctx->brigade = apr_brigade_create(ctx->req->pool, ctx->req->connection->bucket_alloc);

    if (ctx->brigade == NULL) {
        return NULL;
    }

    return ctx;
}

static void
ngx_http_modsecurity_cleanup(void *data)
{
    ngx_http_modsecurity_ctx_t    *ctx = data;

    if (ctx->req != NULL) {
        (void) modsecFinishRequest(ctx->req);
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
        ngx_log_error(NGX_LOG_EMERG, cf->log, 0, "ModSecurityConfig in %s:%ui: %s",
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
    ctx = (ngx_http_modsecurity_ctx_t *) apr_table_get(r->notes, NOTE_NGINX_REQUEST_CTX);

    if (ctx == NULL) {
        return -1;
    }
    ctx->r->connection->error = 1;
    return 0;
}

