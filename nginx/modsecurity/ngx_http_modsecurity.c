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

#undef CR
#undef LF
#undef CRLF

#include "api.h"

#define NOTE_NGINX_REQUEST_CTX "nginx-ctx"

extern ngx_module_t ngx_http_modsecurity;

struct ngx_http_modsecurity_ctx_s;

typedef ngx_int_t (*ngx_http_request_body_data_handler_pt)
    (struct ngx_http_modsecurity_ctx_s*, u_char *, u_char*);

typedef struct {
    ngx_uint_t                  enable;
    directory_config            *config;
    ngx_str_t                   url;
    ngx_http_complex_value_t    *url_cv;
} ngx_http_modsecurity_loc_conf_t;

typedef struct ngx_http_modsecurity_ctx_s {
    ngx_http_request_t  *r;
    conn_rec            *connection;
    request_rec         *req;
    ngx_chain_t         *chain;
    ngx_buf_t            buf;
    void               **loc_conf;
    unsigned             request_body_in_single_buf:1;
    unsigned             request_body_in_file_only:1;
} ngx_http_modsecurity_ctx_t;


/*
** Module's registred function/handlers.
*/
static ngx_int_t    ngx_http_modsecurity_handler(ngx_http_request_t *r);
static void ngx_http_modsecurity_request_body_handler(ngx_http_request_t *r);
//static ngx_int_t    ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t    ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle);
static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_modsecurity_set_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
apr_status_t    modsecurity_read_body_cb(request_rec *r, char *buf, unsigned int length,
                                        unsigned int *readcnt, int *is_eos);

static char *ngx_http_modsecurity_add_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_modsecurity_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_modsecurity_pass_to_backend(ngx_http_request_t *r);

static int ngx_http_modsecurity_drop_action(request_rec *r);

static void ngx_http_modsecurity_cleanup(void *data);

/* command handled by the module */
static ngx_command_t  ngx_http_modsecurity_commands[] =  {
  { ngx_string("ModSecurityConfig"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_http_modsecurity_set_config,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  { ngx_string("ModSecurityEnabled"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
        |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
    ngx_http_modsecurity_add_handler,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  { ngx_string("ModSecurityPass"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
    ngx_http_modsecurity_pass,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  ngx_null_command
};

/*
** handlers for configuration phases of the module
*/

static ngx_http_module_t ngx_http_modsecurity_ctx = {
    NULL, /* preconfiguration */
//    ngx_http_modsecurity_init, /* postconfiguration */
    NULL,

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
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

/* create loc conf struct */
static void *
ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_modsecurity_loc_conf_t  *conf;

    conf = (ngx_http_modsecurity_loc_conf_t  *) ngx_pcalloc(cf->pool, sizeof(ngx_http_modsecurity_loc_conf_t));
    if (conf == NULL)
        return NULL;

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

    if (conf->config == NULL) {
        conf->config = prev->config;
    }

/*
    if (conf->config_path == NULL) {
        conf->config_path = prev->config_path;
    }
*/
    if ((conf->url.len == 0) && (conf->url_cv == NULL)) {
        conf->url = prev->url;
        conf->url_cv = prev->url_cv;
    }

    ngx_conf_merge_uint_value(conf->enable, prev->enable, 0);

    return NGX_CONF_OK;
}

void
modsecLog(void *obj, int level, char *str)
{
    if (obj != NULL)
        ngx_log_error(NGX_LOG_INFO, (ngx_log_t *)obj, 0, "%s", str);
}

#ifdef PROCESS_RESPONSE
/*
** This function sets up handlers for CONTENT_PHASE,
** XXX: not implemented yet
*/
static ngx_int_t 
ngx_http_modsecurity_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = (ngx_http_core_main_conf_t *) ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
    if (cmcf == NULL) {
        return NGX_ERROR;
    }

    /* Register for CONTENT phase */
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_modsecurity_content_handler;

    return NGX_OK;
}
#endif

static ngx_int_t
ngx_http_modsecurity_init_process(ngx_cycle_t *cycle)
{
    cycle->log->log_level = NGX_LOG_INFO;

    modsecSetLogHook(cycle->log, modsecLog);

    modsecSetDropAction(ngx_http_modsecurity_drop_action);
    modsecSetReadBody(modsecurity_read_body_cb);

    modsecInit();
    /* config was already parsed in master process */
//    modsecStartConfig();
//    modsecFinalizeConfig();
    modsecInitProcess();

    return NGX_OK;
}

static void
ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle)
{
    modsecTerminate();
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

char *
ConvertNgxStringToUTF8(ngx_str_t str, apr_pool_t *pool)
{
    char *t = (char *) apr_palloc(pool, str.len + 1);

    ngx_memcpy(t, str.data, str.len);
    t[str.len] = 0;

    return t;
}


static ngx_int_t
ngx_http_modsecurity_pass_to_backend(ngx_http_request_t *r)
{
    ngx_str_t                       uri;
    ngx_str_t                       args;
    ngx_uint_t                      flags;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_int_t rc;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                       "modSecurity: pass_to_backend");
    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    if (!cf) {
        return NGX_ERROR;
    }

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    if (ctx == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    if (cf->enable) {
        int status = modsecProcessRequest(ctx->req);

//        modsecFinishRequest(r);

        if (status != DECLINED) {
            ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity: status: %d, need action", status);

            ngx_http_clear_accept_ranges(r);
            ngx_http_clear_last_modified(r);
            ngx_http_clear_content_length(r);

            /* XXX: return correct status */
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
    }

    args = r->args; /* forward the query args */
    flags = 0;

    if (cf->url_cv) {
        /* complex value */
        if (ngx_http_complex_value(r, cf->url_cv, &uri) != NGX_OK) {
            ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: uri parsing error #2");
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        if (uri.len == 0) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "empty \"upload_pass\" (was: \"%V\")",
                          &cf->url_cv->value);

            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
    } else {
        /* simple value */
        uri = cf->url;
    }

    if (ngx_http_parse_unsafe_uri(r, &uri, &args, &flags) != NGX_OK) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: uri parsing error");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }


    if (uri.len != 0 && uri.data[0] == '/') {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: using internal redirect");
        rc = ngx_http_internal_redirect(r, &uri, &args);
    } else {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: using named location");
        rc = ngx_http_named_location(r, &uri);
    }
    return rc;
}

/*
** request body callback, passing body to mod security
*/
apr_status_t
modsecurity_read_body_cb(request_rec *r, char *outpos, unsigned int length,
                                        unsigned int *outlen, int *is_eos)
{
    size_t                          len, rest;
    ssize_t                         size;
    ngx_http_modsecurity_ctx_t     *ctx;

    ctx = (ngx_http_modsecurity_ctx_t *) apr_table_get(r->notes, NOTE_NGINX_REQUEST_CTX);
    if (ctx == NULL) {
        return APR_EINVAL;
    }
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, ctx->r->connection->log, 0, "modSecurity: read_body_cb");

    ngx_buf_t *buf = &ctx->buf;
    rest = length;
    *is_eos = 0;
    
    while (rest) {
        
        if (ngx_buf_size(buf) == 0) {
            if (ctx->chain == NULL) {
                *outlen = length - rest;
                *is_eos = 1;
                // END
                return APR_SUCCESS;
            }

            ngx_memcpy(buf, ctx->chain->buf, sizeof(ngx_buf_t));
            ctx->chain = ctx->chain->next;
        }
        
        len = (size_t) ngx_min((size_t)ngx_buf_size(buf), rest);
        
        if (ngx_buf_in_memory(buf)) {
            
            outpos = (char *) ngx_cpymem(outpos, buf->pos, len);
            rest -= len;
            buf->pos += len;
        } else if (buf->in_file) {
            
            size = ngx_read_file(buf->file, (u_char*)outpos, len, buf->file_pos);
            
            if (size < 0) {
                return NGX_ERROR;
            }
            outpos += size;
            rest -= size;
            buf->file_pos += size;
        } else {
            return -1;
        }
    }
    
    *outlen = length - rest;
    return APR_SUCCESS;
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
    ngx_http_core_loc_conf_t        *clcf, *lcf;
    ngx_http_modsecurity_ctx_t      *ctx;
    ngx_list_part_t                 *part;
    ngx_table_elt_t                 *h;
    ngx_uint_t                       i;
    ngx_int_t                        rc;
    void                           **loc_conf;
    ngx_pool_cleanup_t              *cln;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    /* Process only main request */
    if (r != r->main || r->internal) {
        return NGX_DECLINED;
    }

    cf = ngx_http_get_module_loc_conf(r, ngx_http_modsecurity);
    if (!cf) {
        return NGX_ERROR;
    }

    /*  XXX: temporary hack, nginx uses pcre as well and hijacks these two */
    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    if (ctx == NULL) {
        ctx = (ngx_http_modsecurity_ctx_t *) ngx_pcalloc(r->pool, sizeof(ngx_http_modsecurity_ctx_t));
        if (ctx == NULL) {
            ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "modSecurity: ctx memory allocation error");
            return NGX_ERROR;
        }
        ctx->r = r;
        ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);
    }

    /* do all modsecurity related work only if handler is enabled */
    if (cf->enable) {
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

        /* cf->config was set in master process??? */
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

            apr_table_setn(ctx->req->headers_in, ConvertNgxStringToUTF8(h[i].key, ctx->req->pool),
                            ConvertNgxStringToUTF8(h[i].value, ctx->req->pool));
        }

        /* XXX: if mod_uniqid enabled - use it's value */
        apr_table_setn(ctx->req->subprocess_env, "UNIQUE_ID", "12345");
        /* actually, we need ctx only for POST request body handling - don't like this part */
        apr_table_setn(ctx->req->notes, NOTE_NGINX_REQUEST_CTX, (const char *) ctx);

        /* add cleanup */
        cln = ngx_pool_cleanup_add(r->pool, 0);
        if (cln == NULL) {
            return NGX_ERROR;
        }
        cln->data = ctx;
        cln->handler = ngx_http_modsecurity_cleanup;
    }

//    r->keepalive = 0;
    if (r->method == NGX_HTTP_POST) {
        /* Processing POST request body, should we process PUT? */
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: method POST");
        
        clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);

        if (clcf == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        ctx->loc_conf = r->loc_conf;
        /* hijack loc_conf so that we can receive any body length
         *  TODO: nonblocking process & chuncked body
         */
        if (clcf->client_body_buffer_size < r->headers_in.content_length_n) {
            
            loc_conf = ngx_pcalloc(r->pool, sizeof(void *) * ngx_http_max_module);
            if (loc_conf == NULL) {
                return NGX_HTTP_INTERNAL_SERVER_ERROR;
            }

            lcf = ngx_pcalloc(r->pool, sizeof(ngx_http_core_loc_conf_t));
            if (lcf == NULL) {
                return NGX_HTTP_INTERNAL_SERVER_ERROR;
            }
    
            ngx_memcpy(loc_conf, r->loc_conf, sizeof(void *) * ngx_http_max_module);
            ngx_memcpy(lcf, clcf, sizeof(ngx_http_core_loc_conf_t));
            
            ctx->loc_conf = r->loc_conf;
            r->loc_conf = loc_conf;

            ngx_http_get_module_loc_conf(r, ngx_http_core_module) = lcf;
            clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
            clcf->client_body_buffer_size = r->headers_in.content_length_n;
        }
        
        ctx->request_body_in_single_buf = r->request_body_in_single_buf;
        ctx->request_body_in_file_only = r->request_body_in_file_only;
        r->request_body_in_single_buf = 1;
        r->request_body_in_file_only = 0;

        rc = ngx_http_read_client_request_body(r, ngx_http_modsecurity_request_body_handler);
        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }

        return NGX_DONE;

    } else {
        /* processing all the other methods */
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: method is not POST");
        
        rc = ngx_http_modsecurity_pass_to_backend(r);
    }

    return rc;
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

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    if (ctx == NULL 
            || r->request_body->bufs == NULL 
            || r->request_body->bufs->next != NULL) {
        ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    
    r->request_body_in_single_buf = ctx->request_body_in_single_buf;
    r->request_body_in_file_only = ctx->request_body_in_file_only;
    r->header_in = r->request_body->bufs->buf;
    ctx->chain = r->request_body->bufs;
    r->request_body = NULL;
    r->loc_conf = ctx->loc_conf;

    ngx_http_finalize_request(r, ngx_http_modsecurity_pass_to_backend(r));
    return;
}


static char *
ngx_http_modsecurity_set_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_modsecurity_loc_conf_t *mscf = conf;
    ngx_str_t       *value;
    char            *config_path;
    const char      *msg;

    value = cf->args->elts;

    if (cf->args->nelts == 0 || value[1].len == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                              "ModSecurity: config path required");
        return NGX_CONF_ERROR;
    }

    if (ngx_conf_full_name(cf->cycle, &value[1], 1) != NGX_OK) {
        return NGX_CONF_ERROR;
    }
    /*
    ** XXX: we need to check if file exists here
    ** b/c modsecurity standalone will segfault with non-existent file
    */
    config_path = (char *) ngx_pcalloc(cf->pool, value[1].len + 1);
    if (config_path == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                              "ModSecurity: config path memory allocation error");
        return NGX_CONF_ERROR;
    }
    ngx_memcpy(config_path, value[1].data, value[1].len);

    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    cf->log->log_level = NGX_LOG_INFO;

    modsecSetLogHook(cf->log, modsecLog);

    modsecInit();
    modsecStartConfig();

    mscf->config = modsecGetDefaultConfig();

    msg = modsecProcessConfig(mscf->config, config_path);
    if (msg != NULL) {
        ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "modSecurity: modsecProcessConfig() %s", msg);
        return NGX_CONF_ERROR;
    }

//      modsecFinalizeConfig();

    return NGX_CONF_OK;
}

static char *
ngx_http_modsecurity_add_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t          *clcf;
    ngx_str_t                         *value;
    ngx_http_modsecurity_loc_conf_t   *mscf = conf;

    value = cf->args->elts;

    if (value[1].len == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "empty value in \"%V\" directive",
                           &cmd->name);

        return NGX_CONF_ERROR;
    }

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_modsecurity_handler;

    if (ngx_strcmp(value[1].data, "off") == 0) {
        mscf->enable = 0;
        return NGX_CONF_OK;
    }
    if (ngx_strcmp(value[1].data, "on") == 0) {
        mscf->enable = 1;
        return NGX_CONF_OK;
    }

    return NGX_CONF_OK;
}

static char *
ngx_http_modsecurity_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_modsecurity_loc_conf_t     *mscf = conf;
    ngx_str_t                           *value;
    ngx_http_compile_complex_value_t    ccv;

    if ((mscf->url.len != 0) || (mscf->url_cv != NULL)) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (value[1].len == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "empty value in \"%V\" directive",
                           &cmd->name);

        return NGX_CONF_ERROR;
    }

    if (ngx_http_script_variables_count(&value[1])) {
        /* complex value */
        mscf->url_cv = ngx_palloc(cf->pool, sizeof(ngx_http_complex_value_t));
        if (mscf->url_cv == NULL) {
            return NGX_CONF_ERROR;
        }

        ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

        ccv.cf = cf;
        ccv.value = &value[1];
        ccv.complex_value = mscf->url_cv;

        if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    } else {
        /* simple value */
        mscf->url = value[1];
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
