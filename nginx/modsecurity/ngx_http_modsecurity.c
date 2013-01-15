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
} ngx_http_modsecurity_loc_conf_t;

typedef struct {
    ngx_http_request_t  *r;
    conn_rec            *connection;
    request_rec         *req;
} ngx_http_modsecurity_ctx_t;


/*
** Module's registred function/handlers.
*/
static ngx_int_t ngx_http_modsecurity_handler(ngx_http_request_t *r);
static void ngx_http_modsecurity_request_body_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle);
static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_modsecurity_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

apr_status_t modsecurity_read_body_cb(request_rec *r, char *buf, unsigned int length,
                                        unsigned int *readcnt, int *is_eos);
apr_status_t modsecurity_write_body_cb(request_rec *rec, char *buf, unsigned int length);

static ngx_http_modsecurity_ctx_t * ngx_http_modsecurity_create_ctx(ngx_http_request_t *r);
static int ngx_http_modsecurity_drop_action(request_rec *r);
static void ngx_http_modsecurity_cleanup(void *data);
char *ConvertNgxStringToUTF8(ngx_str_t str, apr_pool_t *pool);

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
    ngx_conf_set_flag_slot,
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

    return NGX_CONF_OK;
}

void
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

void *
modsec_pcre_malloc(size_t size)
{
    return apr_palloc(pool, size);
}

void
modsec_pcre_free(void *ptr)
{
}

static ngx_int_t
ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf)
{

    /*  XXX: temporary hack, nginx uses pcre as well and hijacks these two */
    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    modsecSetLogHook(cf->log, modsecLog);
    modsecSetDropAction(ngx_http_modsecurity_drop_action);

    modsecInit();
    modsecStartConfig();

    return NGX_OK;
}

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

#ifdef PROCESS_RESPONSE
    /*
    ** This function sets up handlers for CONTENT_PHASE,
    ** XXX: not implemented yet
    */

    /* Register for CONTENT phase ?? */
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_modsecurity_content_handler;
#endif

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


char *
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

apr_sockaddr_t *CopySockAddr(apr_pool_t *pool, struct sockaddr *pAddr) {
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
    ngx_http_core_loc_conf_t        *clcf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    /* Process only main request */
    if (r != r->main || r->internal) {
        return NGX_DECLINED;
    }

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);

    if (!cf->enable) {
        return NGX_DECLINED;
    }

    ctx = ngx_http_modsecurity_create_ctx(r);
    if (ctx == NULL) {
        return NGX_ERROR;
    }
    ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);
    
    if (r->method == NGX_HTTP_POST) {
        /* Processing POST request body, should we process PUT? */
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: method POST");
        
        clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);

        if (clcf == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        rc = ngx_http_read_client_request_body(r, ngx_http_modsecurity_request_body_handler);
        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }

        return NGX_DONE;

    } else {
        /* processing all the other methods */
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: method is not POST");
        
        rc = modsecProcessRequest(ctx->req);

        if (rc != DECLINED) {
            ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: status: %d, need action", rc);

            ngx_http_clear_accept_ranges(r);
            ngx_http_clear_last_modified(r);
            ngx_http_clear_content_length(r);

            /* Nginx and Apache share same response code  */
            if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
                return rc;
            }
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        } 
    }

    return NGX_DECLINED;
}
#if 0
static ngx_int_t
ngx_http_modsecurity_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    /* headers */
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_core_loc_conf_t        *clcf, *lcf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_int_t                        rc;
    void                           **loc_conf;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    /* Process only main request */
    if (r != r->main || r->internal) {
        return ngx_http_next_body_filter(r, in);
    }

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);

    if (!cf->enable) {
        return ngx_http_next_body_filter(r, in);
    }

    
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    if (ctx == NULL) {
        ctx = ngx_http_modsecurity_create_ctx(r);
        if (ctx == NULL) {
            r->filter_finalize = 1;
            return NGX_ERROR;
        }
        ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);
    }

    if (ctx->phase < RESPONSE) {
        ctx->chain_in = NULL;
        if (ngx_list_copy_to_apr_table(&r->headers_out.headers, 
                                    ctx->req->headers_out, 
                                    ctx->req->pool) != NGX_OK) {
            r->filter_finalize = 1;
            return NGX_ERROR;
        }
        ctx->phase = RESPONSE;
    }

    /* buffer chain */
    if (in != NULL) {
        ngx_chain_t  *cl, **ll;
        ll = &ctx->chain_in;
    
        for (cl = ctx->chain_in; cl; cl = cl->next) {
            ll = &cl->next;
        }

        while (in) {
            cl = ngx_alloc_chain_link(pool);
            if (cl == NULL) {
                r->filter_finalize = 1;
                return NGX_ERROR;
            }
            cl->buf = in->buf;
            in = in->next;
            cl->next = NULL;
            *ll = cl;
            ll = &cl->next;
        }

        return NGX_AGAIN;
    }

    /* ngx chain to apr brigade */
    

    /* process headers and buffered body  */
    rc = modsecProcessResponse(ctx->req);

    if (rc == DECLINED) {
        /* apr brigade to ngx chain */

        
        return ngx_http_next_body_filter(r, in);
    }
    
    if (rc > NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }
    
    r->filter_finalize = 1;
    return NGX_ERROR;
}
#endif

static ngx_http_modsecurity_ctx_t *
ngx_http_modsecurity_create_ctx(ngx_http_request_t *r)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_pool_cleanup_t              *cln;
    ngx_http_modsecurity_ctx_t      *ctx;

    cln = ngx_pool_cleanup_add(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
    if (cln == NULL) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "modSecurity: ctx memory allocation error");
        return NULL;
    }
    cln->handler = ngx_http_modsecurity_cleanup;
    ngx_memzero(cln->data, sizeof(ngx_http_modsecurity_ctx_t));
    
    ctx = cln->data;
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
    /* actually, we need ctx only for POST request body handling - don't like this part */
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


static void
ngx_http_modsecurity_request_body_handler(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t    *ctx;
    apr_bucket_brigade            *bb;
    ngx_int_t                      rc;
    apr_off_t                     len;
    ngx_str_t                    *str;

    rc = NGX_DONE;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (ctx == NULL 
            || r->request_body->bufs == NULL ) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    bb = ngx_chain_to_apr_brigade(r->request_body->bufs, 
                                  ctx->req->pool, 
                                  ctx->req->connection->bucket_alloc);
    r->request_body = NULL;
    if (bb == NULL) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    modsecSetBodyBrigade(ctx->req, bb);
    rc = modsecProcessRequest(ctx->req);

    if (rc != DECLINED) {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: status: %d, need action", rc);
    
        ngx_http_clear_accept_ranges(r);
        ngx_http_clear_last_modified(r);
        ngx_http_clear_content_length(r);
    
        /* Nginx and Apache share same response code  */
        if (rc < NGX_HTTP_SPECIAL_RESPONSE || rc >= 600) {
            rc = NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return ngx_http_finalize_request(r, rc);
    }
    bb = modsecGetBodyBrigade(ctx->req);

    if (apr_brigade_length(bb, 1, &len) != APR_SUCCESS) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    ngx_buf_t *buf = ngx_create_temp_buf(ctx->r->pool, (size_t) len);
    if (buf == NULL){
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }
    
    if (apr_brigade_flatten(bb, (char *)buf->pos, (apr_size_t *)&len) != APR_SUCCESS) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    apr_brigade_cleanup(bb);
    
    buf->last += len;
    r->header_in = buf;
    r->headers_in.content_length_n = len;

    /* set  headers_in.content_length */
    str = &r->headers_in.content_length->value;
    str->data = ngx_palloc(r->pool, NGX_OFF_T_LEN);
    if (str->data == NULL) {
        return ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    }

    str->len = ngx_snprintf(str->data, NGX_OFF_T_LEN, "%O", len) - str->data;

    r->phase_handler++;
    ngx_http_core_run_phases(r);
    ngx_http_finalize_request(r, NGX_DONE);
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

    msg = modsecProcessConfig(mscf->config, (const char *)value[1].data);
    if (msg != NULL) {
        ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "modSecurity: modsecProcessConfig() %s", msg);
        return NGX_CONF_ERROR;
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
