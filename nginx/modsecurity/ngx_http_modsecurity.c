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

#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_event.h>
#include <ngx_http_core_module.h>
#include <ctype.h>
#include <sys/times.h>
#include <apr_bucket_nginx.h>
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
static char *ConvertNgxStringToUTF8(ngx_str_t str, apr_pool_t *pool);

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

static inline ngx_int_t 
ngx_list_copy_to_apr_table(ngx_list_t *list, apr_table_t *table, apr_pool_t *pool) {
    ngx_list_part_t                 *part;
    ngx_table_elt_t                 *h;
    ngx_uint_t                        i;
    char                            *key, *value;
    
    part = &list->part;
    h = part->elts;

    for (i = 0; ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL)
                break;

            part = part->next;
            h = part->elts;
            i = 0;
        }

        key = ConvertNgxStringToUTF8(h[i].key, pool);
        if (key == NULL) {
            return NGX_ERROR;
        }
        
        value = ConvertNgxStringToUTF8(h[i].value, pool);
        if (value == NULL) {
            return NGX_ERROR;
        }

        apr_table_setn(table, key, value);
    }
    return NGX_OK;    
}

/* create loc conf struct */
static void *
ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_modsecurity_loc_conf_t  *conf;

    conf = (ngx_http_modsecurity_loc_conf_t  *) ngx_palloc(cf->pool, sizeof(ngx_http_modsecurity_loc_conf_t));
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

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;

static ngx_int_t 
ngx_http_modsecurity_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    modsecFinalizeConfig();

    cmcf = (ngx_http_core_main_conf_t *) ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
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


static char *
ConvertNgxStringToUTF8(ngx_str_t str, apr_pool_t *pool)
{
    char *t = (char *) apr_palloc(pool, str.len + 1);
    
    if (!t) {
        return NULL;
    }

    ngx_memcpy(t, str.data, str.len);
    t[str.len] = 0;

    return t;
}

static apr_sockaddr_t *CopySockAddr(apr_pool_t *pool, struct sockaddr *pAddr) {
    apr_sockaddr_t *addr = (apr_sockaddr_t *)apr_palloc(pool, sizeof(apr_sockaddr_t));
    int adrlen = 16, iplen = 4;

    if(pAddr->sa_family == AF_INET6) {
        adrlen = 46;
        iplen = 16;
    }

    addr->addr_str_len = adrlen;
    addr->family = pAddr->sa_family;

    addr->hostname = "unknown";
#ifdef WIN32
    addr->ipaddr_len = sizeof(IN_ADDR);
#else
    addr->ipaddr_len = sizeof(struct in_addr);
#endif
    addr->ipaddr_ptr = &addr->sa.sin.sin_addr;
    addr->pool = pool;
    addr->port = 80;
#ifdef WIN32
    memcpy(&addr->sa.sin.sin_addr.S_un.S_addr, pAddr->sa_data, iplen);
#else
    memcpy(&addr->sa.sin.sin_addr.s_addr, pAddr->sa_data, iplen);
#endif
    addr->sa.sin.sin_family = pAddr->sa_family;
    addr->sa.sin.sin_port = 80;
    addr->salen = sizeof(addr->sa);
    addr->servname = addr->hostname;

    return addr;
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
    if (r != r->main || r->internal || !cf->enable) {
        return NGX_DECLINED;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    ctx = ngx_http_modsecurity_create_ctx(r);
    if (ctx == NULL) {
        return NGX_ERROR;
    }
    ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);

    /* processing request headers */
    rc = modsecProcessRequestHeaders(ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestHeaders %d", rc);
    if (rc == DECLINED) {
        if (r->method == NGX_HTTP_POST) {
            /* Processing POST request body, should we process PUT? */
            rc = ngx_http_read_client_request_body(r, ngx_http_modsecurity_body_handler);
            if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
                return rc;
            }

            return NGX_DONE;
        }
        /* other method */
        if(modsecIsRequestBodyAccessEnabled(ctx->req))  {
            rc = modsecProcessRequestBody(ctx->req);
            ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestBody %d", rc);
        }
    }

    if (rc != DECLINED) {

        /* Nginx and Apache share same response code  */
        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return rc;
    }

    return NGX_DECLINED;
}


static void
ngx_http_modsecurity_body_handler(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t    *ctx = NULL;
    ngx_int_t                      rc = DECLINED;
    apr_off_t                      content_length;
    ngx_str_t                     *str = NULL;
    ngx_buf_t                     *buf = NULL;

    if (r->request_body == NULL || r->request_body->bufs == NULL) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: request body empty");
        r->phase_handler++;
        ngx_http_core_run_phases(r);
        ngx_http_finalize_request(r, NGX_DONE);
        return;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: body handler");

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    rc = move_chain_to_brigade(r->request_body->bufs, ctx->brigade, r->pool);
    if (rc == NGX_ERROR) {
         return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    rc = DECLINED;
    r->request_body = NULL;

    modsecSetBodyBrigade(ctx->req, ctx->brigade);

    if(modsecIsRequestBodyAccessEnabled(ctx->req))  {
        rc = modsecProcessRequestBody(ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessRequestBody %d", rc);
    }

    if (rc != DECLINED) {
        /* Nginx and Apache share same response code  */
        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            rc = NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return ngx_http_finalize_request(r, rc);
    }

    apr_brigade_length(ctx->brigade, 0, &content_length);
    buf = ngx_create_temp_buf(ctx->r->pool, (size_t) content_length);
    if (buf == NULL){
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }
    if (apr_brigade_flatten(ctx->brigade, (char *)buf->pos, (apr_size_t *)&content_length) != APR_SUCCESS) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    apr_brigade_cleanup(ctx->brigade);
    buf->last += content_length;
    r->header_in = buf;

    if (r->headers_in.content_length) {
        str = &r->headers_in.content_length->value;
        str->data = ngx_palloc(r->pool, NGX_OFF_T_LEN);
        if (str->data == NULL) {
            return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
        }
        str->len = ngx_snprintf(str->data, NGX_OFF_T_LEN, "%O", content_length) - str->data;
    }

    r->headers_in.content_length_n = content_length;
    r->phase_handler++;
    ngx_http_core_run_phases(r);
    ngx_http_finalize_request(r, NGX_DONE);
}


static ngx_int_t
ngx_http_modsecurity_header_filter(ngx_http_request_t *r) {
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t      *ctx;
    const char                      *lang;
    ngx_int_t                        rc;
    
    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    
    if (r != r->main || r->internal || !cf->enable || ctx->complete) {
        return ngx_http_next_header_filter(r);
    }
    
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: header filter");
    
    /* copy headers_out */
    if (ngx_list_copy_to_apr_table(&r->headers_out.headers, 
                                ctx->req->headers_out, 
                                ctx->req->pool) != NGX_OK) {

        return NGX_ERROR;
    }

    ctx->req->content_type = ConvertNgxStringToUTF8(r->headers_out.content_type, ctx->req->pool);
    ctx->req->content_encoding = apr_table_get(ctx->req->headers_out, "Content-Encoding");
    lang = apr_table_get(ctx->req->headers_out, "Content-Languages");

    if(lang != NULL)
    {
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1, sizeof(const char *));
        *(const char **)apr_array_push(ctx->req->content_languages) = lang;
    }

    if (r->method == NGX_HTTP_HEAD || r->header_only) {
        
        ctx->complete = 1;
        rc = modsecProcessResponse(ctx->req);
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessResponse %d", rc);
        if (rc == DECLINED || rc == APR_SUCCESS) {
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

    if (r != r->main || r->internal || !cf->enable || ctx->complete) {
        return ngx_http_next_body_filter(r, in);
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: body filter");

    if (in == NULL) {
        return NGX_AGAIN;
    }

    rc = move_chain_to_brigade(in, ctx->brigade, r->pool);
    if (rc != NGX_OK)  {
        return rc;
    }

    /* last buf has been saved */

    ctx->complete = 1;
    modsecSetResponseBrigade(ctx->req, ctx->brigade);

    rc = modsecProcessResponse(ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: modsecProcessResponse %d", rc);

    if (rc == DECLINED || rc == APR_SUCCESS) {

        in = NULL;

        apr_brigade_length(ctx->brigade, 0, &content_length);
        
        rc = move_brigade_to_chain(ctx->brigade, &in, ctx->r->pool);
        if (rc == NGX_ERROR) {
            return NGX_ERROR;
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
    const char                      *lang;

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
        ctx->connection = modsecNewConnection();
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
        ctx->connection->remote_addr = CopySockAddr(ctx->connection->pool, r->connection->sockaddr);
        ctx->connection->remote_ip = ConvertNgxStringToUTF8(r->connection->addr_text, ctx->connection->pool);
#else
        ctx->connection->client_addr = CopySockAddr(ctx->connection->pool, r->connection->sockaddr);
        ctx->connection->client_ip = ConvertNgxStringToUTF8(r->connection->addr_text, ctx->connection->pool);
#endif
        ctx->connection->remote_host = NULL;
        modsecProcessConnection(ctx->connection);
    }

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    ctx->req = modsecNewRequest(ctx->connection, cf->config);
    ctx->req->request_time = apr_time_now();
    ctx->req->method = ConvertNgxStringToUTF8(r->method_name, ctx->req->pool);
    ctx->req->path_info = ConvertNgxStringToUTF8(r->unparsed_uri, ctx->req->pool);
    ctx->req->unparsed_uri = ConvertNgxStringToUTF8(r->unparsed_uri, ctx->req->pool);
    ctx->req->uri = ctx->req->unparsed_uri;
    ctx->req->the_request = ConvertNgxStringToUTF8(r->request_line, ctx->req->pool);
    ctx->req->args = ConvertNgxStringToUTF8(r->args, ctx->req->pool);
    ctx->req->filename = ctx->req->path_info;

    ctx->req->parsed_uri.scheme = "http";
    ctx->req->parsed_uri.path = ctx->req->path_info;
    ctx->req->parsed_uri.is_initialized = 1;
    ctx->req->parsed_uri.port = 80;
    ctx->req->parsed_uri.port_str = "80";
    ctx->req->parsed_uri.query = ctx->req->args;
    ctx->req->parsed_uri.dns_looked_up = 0;
    ctx->req->parsed_uri.dns_resolved = 0;
    ctx->req->parsed_uri.password = NULL;
    ctx->req->parsed_uri.user = NULL;
    ctx->req->parsed_uri.fragment = ConvertNgxStringToUTF8(r->exten, ctx->req->pool);

    if (ngx_list_copy_to_apr_table(&r->headers_in.headers, 
                                    ctx->req->headers_in, 
                                    ctx->req->pool) != NGX_OK) {
        return NULL;
    }

    /* XXX: if mod_uniqid enabled - use it's value */
    apr_table_setn(ctx->req->subprocess_env, "UNIQUE_ID", "12345");

    ctx->req->content_encoding = apr_table_get(ctx->req->headers_in, "Content-Encoding");
    ctx->req->content_type = apr_table_get(ctx->req->headers_in, "Content-Type");

    lang = apr_table_get(ctx->req->headers_in, "Content-Languages");
    if(lang != NULL)
    {
        ctx->req->content_languages = apr_array_make(ctx->req->pool, 1, sizeof(const char *));

        *(const char **)apr_array_push(ctx->req->content_languages) = lang;
    }

    ctx->brigade = apr_brigade_create(ctx->req->pool, ctx->req->connection->bucket_alloc);

    if (ctx->brigade == NULL) {
        return NULL;
    }

    apr_table_setn(ctx->req->notes, NOTE_NGINX_REQUEST_CTX, (const char *) ctx);

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

