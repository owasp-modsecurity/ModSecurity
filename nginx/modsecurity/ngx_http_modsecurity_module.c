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

extern ngx_module_t ngx_http_modsecurity_module;

typedef struct {
    ngx_flag_t      enabled;
    char             *config_path;
    directory_config *config;
} ngx_http_modsecurity_loc_conf_t;

typedef struct {
    conn_rec *connection;
} ngx_http_modsecurity_ctx_t;


/*
** Module's registred function/handlers.
*/
static ngx_int_t    ngx_http_modsecurity_handler(ngx_http_request_t *r);
static ngx_int_t    ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t    ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle);
static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
//static ngx_int_t    ngx_http_read_request_body(ngx_http_request_t *req, ngx_http_client_body_handler_pt handler);
static char *ngx_http_modsecurity_set_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


/* command handled by the module */
static ngx_command_t  ngx_http_modsecurity_commands[] =  {
  { ngx_string("ModSecurityConfig"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
#ifdef zzz_
    ngx_http_modsecurity_set_config,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
#else
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_modsecurity_loc_conf_t, config_path),
#endif
    NULL },
  { ngx_string("ModSecurityEnabled"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_SIF_CONF
        |NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
    ngx_conf_set_flag_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_modsecurity_loc_conf_t, enabled),
    NULL },
  ngx_null_command
};

/*
** handlers for configuration phases of the module
*/

static ngx_http_module_t ngx_http_modsecurity_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_modsecurity_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_modsecurity_create_loc_conf, /* create location configuration */
    ngx_http_modsecurity_merge_loc_conf /* merge location configuration */
};


ngx_module_t ngx_http_modsecurity_module = {
    NGX_MODULE_V1,
    &ngx_http_modsecurity_module_ctx, /* module context */
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

    conf->enabled = NGX_CONF_UNSET;
    conf->config_path = NULL;
    conf->config = NULL;

    return conf;
}

/* merge loc conf */
static char *
ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, 
                  void *child)
{
    ngx_http_modsecurity_loc_conf_t  *prev = parent;
    ngx_http_modsecurity_loc_conf_t  *conf = child;

    ngx_conf_merge_value(conf->enabled, prev->enabled, 0);

    if (conf->config == NULL) {
        conf->config = prev->config;
    }

    if (conf->config_path == NULL) {
        conf->config_path = prev->config_path;
    }


//    ngx_conf_log_error(NGX_LOG_DEBUG_HTTP, cf, 0,
//                   "merging loc conf: %s", conf->config_path);

    return NGX_CONF_OK;
}

void
modsecLog(void *obj, int level, char *str)
{
    if (obj != NULL)
        ngx_log_error(NGX_LOG_INFO, (ngx_log_t *)obj, 0, "%s", str);
}

/*
** This function sets up handlers for PRE_ACCESS_PHASE,
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

    /* Register for pre access phase */
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRE_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_modsecurity_handler;

    return NGX_OK;
}

static ngx_int_t
ngx_http_modsecurity_init_process(ngx_cycle_t *cycle)
{
    cycle->log->log_level = NGX_LOG_INFO;

    modsecSetLogHook(cycle->log, modsecLog);

    modsecInit();
    modsecStartConfig();
    modsecFinalizeConfig();
    modsecInitProcess();

    return NGX_OK;
}

static void
ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle)
{
	// we are exiting process anyway and if the request was not finished properly
	// the pool cleanup function for ModSecurity might break the termination process
	//
    //modsecTerminate();
}

/* This is a temporary hack to make PCRE work with ModSecurity
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

/*
 ** Create payload handler for calling request body function
 */
void
ngx_http_dummy_payload_handler(ngx_http_request_t *req)
{
    ngx_http_finalize_request(req, NGX_DONE);
}


/*
 *  XXX: needs rewrite and testing
 ** If method is POST or PUT, read request body and put in req->request_body->bufs
 */
#ifdef zz
static ngx_int_t
ngx_http_read_request_body(ngx_http_request_t *req,  
                      ngx_http_client_body_handler_pt handler)
{
    // If has body request treat it
     ngx_int_t rc = 0;
     
     if (req->method == NGX_HTTP_POST || req->method==NGX_HTTP_PUT) {
        //calling request body function
        rc = ngx_http_read_client_request_body(req, ngx_http_dummy_payload_handler);
     }
     //If some error, returns rc
     if (rc == NGX_ERROR || rc >= NGX_HTTP_SPECIAL_RESPONSE) {
         return rc;
     }
     //Has the end of request body?
     if (rc == NGX_AGAIN) {
         return NGX_DONE;
     }

    return NGX_DECLINED;
}
#endif

/*
** [ENTRY POINT] does : this function called by nginx from the request handler
*/
static ngx_int_t
ngx_http_modsecurity_handler(ngx_http_request_t *req)
{
    ngx_http_modsecurity_loc_conf_t *cf;
    ngx_http_modsecurity_ctx_t *ctx;
    request_rec *r;
    ngx_list_part_t *part;
    ngx_table_elt_t *h;
    ngx_uint_t  i;
    int status;
    conn_rec *connection;
    const char *msg;

    /* Process only main request */
    if (req != req->main || req->internal) {
        return NGX_DECLINED;
    }

    cf = ngx_http_get_module_loc_conf(req, ngx_http_modsecurity_module);
    if (!cf) {
        return NGX_ERROR;
    }

    if (!cf->enabled) {
        return NGX_DECLINED;
    }

    /*  XXX: temporary hack, nginx uses pcre as well and hijacks these two */
    pcre_malloc = modsec_pcre_malloc;
    pcre_free = modsec_pcre_free;

    ctx = ngx_http_get_module_ctx(req, ngx_http_modsecurity_module);
    if (ctx == NULL) {
        ctx = (ngx_http_modsecurity_ctx_t *) ngx_pcalloc(req->pool, sizeof(ngx_http_modsecurity_ctx_t));
        if (ctx == NULL) {
            ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "ModSecurity: ctx memory allocation error");
            return NGX_ERROR;
        }
        ngx_http_set_ctx(req, ctx, ngx_http_modsecurity_module);
    }


    if (cf->config == NULL) {
        cf->config = modsecGetDefaultConfig();

        msg = modsecProcessConfig(cf->config, cf->config_path);
        if (msg != NULL) {
            ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "ModSecurity: modsecProcessConfig() %s", msg);
            return NGX_ERROR;
        }
    }

    if (req->connection->requests == 0 || ctx->connection == NULL) {
        ctx->connection = modsecNewConnection();
        modsecProcessConnection(ctx->connection);
    }

    r = modsecNewRequest(ctx->connection, cf->config);
    r->request_time = apr_time_now();
    r->method = ConvertNgxStringToUTF8(req->method_name, r->pool);
    r->path_info = ConvertNgxStringToUTF8(req->unparsed_uri, r->pool);
    r->unparsed_uri = ConvertNgxStringToUTF8(req->unparsed_uri, r->pool);
    r->uri = r->unparsed_uri;
    r->the_request = ConvertNgxStringToUTF8(req->request_line, r->pool);
    r->args = ConvertNgxStringToUTF8(req->args, r->pool);
    r->filename = r->path_info;

    r->parsed_uri.scheme = "http";
    r->parsed_uri.path = r->path_info;
    r->parsed_uri.is_initialized = 1;
    r->parsed_uri.port = 80;
    r->parsed_uri.port_str = "80";
    r->parsed_uri.query = r->args;
    r->parsed_uri.dns_looked_up = 0;
    r->parsed_uri.dns_resolved = 0;
    r->parsed_uri.password = NULL;
    r->parsed_uri.user = NULL;
    r->parsed_uri.fragment = ConvertNgxStringToUTF8(req->exten, r->pool);

    part = &req->headers_in.headers.part;
    h = part->elts;

    for (i = 0; ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL)
                break;

            part = part->next;
            h = part->elts;
            i = 0;
        }

        apr_table_setn(r->headers_in, ConvertNgxStringToUTF8(h[i].key, r->pool),
                        ConvertNgxStringToUTF8(h[i].value, r->pool));
    }

    apr_table_setn(r->subprocess_env, "UNIQUE_ID", "12345");

/*
    ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "ModSecurity: %s", r->uri);
*/
    /* XXX: need correct request body handler */
/*
    ngx_http_read_request_body(req, ngx_http_dummy_payload_handler);

    if (req->headers_in.content_length) {
        ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "request body: %s", req->request_body->bufs); 
    } else {
        ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "request body: ");
    }
*/

    status = modsecProcessRequest(r);

    modsecFinishRequest(r);

    if (status != DECLINED) {
        ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "ModSecurity: status: %d", status);

        /* XXX: not implemented in standalone */
        /*
        ngx_http_clear_accept_ranges(req);
        ngx_http_clear_last_modified(req);
        ngx_http_clear_content_length(req);

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
        */
        return NGX_DECLINED;
    }

    return NGX_DECLINED;
}

static char *
ngx_http_modsecurity_set_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_modsecurity_loc_conf_t *ucf = conf;
    ngx_str_t  *value;

    value = cf->args->elts;

    if (cf->args->nelts == 0 || value[1].len == 0) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                              "ModSecurity: config path required");
        return NGX_CONF_ERROR;
    }

    /* not sure if we have to copy it in a buffed or use directly */
    /* XXX: need to check if path is absolute or relative and exists */
    ucf->config_path = (char *) ngx_pcalloc(cf->pool, value[1].len + 1);
    if (ucf->config_path == NULL) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                              "ModSecurity: config path memory allocation error");
        return NGX_CONF_ERROR;
    }
    ngx_memcpy(ucf->config_path, value[1].data, value[1].len);

    return NGX_CONF_OK;
}
