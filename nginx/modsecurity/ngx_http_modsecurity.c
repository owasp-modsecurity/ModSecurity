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

#include <apr_base64.h>

#undef CR
#undef LF
#undef CRLF

#include "api.h"
#ifdef WAF_JSON_LOGGING_ENABLE
#include <waf_log_util_external.h>
#endif

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

    int                  thread_running;
    int                  status_code;
} ngx_http_modsecurity_ctx_t;

#define STATUS_CODE_NOT_SET -1000

typedef struct {
    ngx_http_request_t *r;
} ngx_http_modsecurity_prevention_thread_ctx_t;

typedef struct {
    conn_rec            *connection;
    request_rec         *req;

    apr_bucket_brigade  *brigade;
} ngx_http_modsecurity_detection_thread_ctx_t;

/*
** Module's registred function/handlers.
*/
static ngx_int_t ngx_http_modsecurity_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_modsecurity_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void *ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char *ngx_http_modsecurity_config(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_modsecurity_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_http_modsecurity_ctx_t * ngx_http_modsecurity_create_ctx(ngx_http_request_t *r);
static int ngx_http_modsecurity_drop_action(request_rec *r);
static void ngx_http_modsecurity_terminate(ngx_cycle_t *cycle);
static void ngx_http_modsecurity_cleanup(void *data);

static ngx_str_t thread_pool_name = ngx_string("default");

ngx_thread_mutex_t mtx;


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
    ngx_http_modsecurity_terminate, /* exit process */
    ngx_http_modsecurity_terminate, /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_http_upstream_t ngx_http_modsecurity_upstream;

static inline char *
dup_ngx_str_to_apr(apr_pool_t *pool, ngx_str_t *src)
{
    return apr_pstrmemdup(pool, (char *)src->data, src->len);
}

/*
 * Helper to allocate a thread task using an Apache pool.
 * This is useful when the background task can outlive the Nginx request
 * it is initiated for.
 */
ngx_thread_task_t *
apr_thread_task_alloc(apr_pool_t *pool, size_t size)
{
    ngx_thread_task_t  *task;

    task = apr_palloc(pool, sizeof(ngx_thread_task_t) + size);
    if (task == NULL) {
        return NULL;
    }

    task->ctx = task + 1;

    return task;
}

static inline int
ngx_http_modsecurity_method_number(unsigned int nginx)
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

/*
 * Use APR pool for all allocations because they should not depend on Nginx request pool.
 * In case of detection mode, processing will take place entirely in background and may last
 * longer than the original request lives.
 */
static ngx_inline ngx_int_t
ngx_http_modsecurity_load_request(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t  *ctx;
    request_rec                 *req;
    size_t                       root;
    ngx_str_t                    path;
    ngx_uint_t                   port;
    struct sockaddr_in          *sin;
#if (NGX_HAVE_INET6)
    struct sockaddr_in6         *sin6;
#endif

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    req = ctx->req;

    /* request line */
    req->method = dup_ngx_str_to_apr(req->pool, &r->method_name);

    /* TODO: how to use ap_method_number_of ?
     * req->method_number = ap_method_number_of(req->method);
     */

    req->method_number = ngx_http_modsecurity_method_number(r->method);

    /* ngx_http_map_uri_to_path() allocates memory for terminating '\0' */
    if (ngx_http_map_uri_to_path(r, &path, &root, 0) == NULL) {
        return NGX_ERROR;
    }

    req->filename = dup_ngx_str_to_apr(req->pool, &path);
    req->path_info = req->filename;

    req->args = dup_ngx_str_to_apr(req->pool, &r->args);

    req->proto_num = r->http_major *1000 + r->http_minor;
    req->protocol = dup_ngx_str_to_apr(req->pool, &r->http_protocol);
    req->request_time = apr_time_make(r->start_sec, r->start_msec);
    req->the_request = dup_ngx_str_to_apr(req->pool, &r->request_line);

    req->unparsed_uri = dup_ngx_str_to_apr(req->pool, &r->unparsed_uri);
    req->uri = dup_ngx_str_to_apr(req->pool, &r->uri);

    req->parsed_uri.scheme = "http";

#if (NGX_HTTP_SSL)
    if (r->connection->ssl) {
        req->parsed_uri.scheme = "https";
    }
#endif

    req->parsed_uri.path = dup_ngx_str_to_apr(req->pool, &r->uri);
    req->parsed_uri.is_initialized = 1;

    switch (r->connection->local_sockaddr->sa_family) {

#if (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) r->connection->local_sockaddr;
        port = ntohs(sin6->sin6_port);
        break;
#endif

#if (NGX_HAVE_UNIX_DOMAIN)
    case AF_UNIX:
        port = 0;
        break;
#endif

    default: /* AF_INET */
        sin = (struct sockaddr_in *) r->connection->local_sockaddr;
        port = ntohs(sin->sin_port);
        break;
    }

    req->parsed_uri.port = port;
    req->parsed_uri.port_str = apr_palloc(req->pool, sizeof("65535"));
    (void) ngx_sprintf((u_char *)req->parsed_uri.port_str, "%ui%c", port, '\0');

    req->parsed_uri.query = r->args.len ? req->args : NULL;
    req->parsed_uri.dns_looked_up = 0;
    req->parsed_uri.dns_resolved = 0;

    req->parsed_uri.fragment = dup_ngx_str_to_apr(req->pool, &r->exten);

    req->hostname = dup_ngx_str_to_apr(req->pool, (ngx_str_t *)&ngx_cycle->hostname);

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

        const char *key = dup_ngx_str_to_apr(req->pool, &h[i].key);
        if (key == NULL) {
            return NGX_ERROR;
        }
        const char *value = dup_ngx_str_to_apr(req->pool, &h[i].value);
        if (value == NULL) {
            return NGX_ERROR;
        }
        apr_table_setn(req->headers_in, key, value);
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
        ctx->req->content_languages = apr_array_make(req->pool, 1, sizeof(const char *));

        *(const char **)apr_array_push(req->content_languages) = lang;
    }

    req->ap_auth_type = (char *)apr_table_get(req->headers_in, "Authorization");

    req->user = dup_ngx_str_to_apr(req->pool, &r->headers_in.user);



    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "ModSecurity: load headers in done");

    return NGX_OK;
}

static ngx_inline ngx_int_t
ngx_http_modsecurity_load_request_body(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t    *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    modsecSetBodyBrigade(ctx->req, ctx->brigade);

    if (r->request_body == NULL || r->request_body->bufs == NULL) {

        return copy_chain_to_brigade(NULL, ctx->brigade, 1);
    }

    if (copy_chain_to_brigade(r->request_body->bufs, ctx->brigade, 1) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_OK;
}

static ngx_inline ngx_int_t
ngx_http_modsecurity_status(int status)
{
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
    ngx_thread_mutex_lock(&mtx, NULL);
    void* m = apr_palloc(pool, size);
    ngx_thread_mutex_unlock(&mtx, NULL);
    return m;
}

static void
modsec_pcre_free(void *ptr)
{
}

static server_rec *modsec_server = NULL;

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
    modsec_server->server_hostname = ngx_palloc(cf->pool, ngx_cycle->hostname.len + 1);
    if (modsec_server->server_hostname == NULL) {
        return NGX_ERROR;
    }
    ngx_memcpy(modsec_server->server_hostname, ngx_cycle->hostname.data, ngx_cycle->hostname.len);
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

    ngx_memzero(&ngx_http_modsecurity_upstream, sizeof(ngx_http_upstream_t));
    ngx_http_modsecurity_upstream.cacheable = 1;

    ngx_thread_pool_t* thread_pool = ngx_thread_pool_add(cf, &thread_pool_name);
    if (thread_pool == NULL) {
        return NGX_ERROR;
    }

    if (ngx_thread_mutex_create(&mtx, cf->log) != NGX_OK) {
        return NGX_ERROR;
    }

    extern pthread_mutex_t msc_pregcomp_ex_mtx;
    pthread_mutex_init(&msc_pregcomp_ex_mtx, NULL);

#ifdef WAF_JSON_LOGGING_ENABLE
    init_appgw_rules_id_hash();
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


static void
ngx_http_modsecurity_prevention_thread_func(void *data, ngx_log_t *log)
{
    // Executed in a separate thread

    ngx_http_modsecurity_prevention_thread_ctx_t *thread_ctx = data;
    ngx_http_request_t *r = thread_ctx->r;

    ngx_http_modsecurity_ctx_t* mod_ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);

    // Load request to request rec
    if (ngx_http_modsecurity_load_request(r) != NGX_OK
        || ngx_http_modsecurity_load_headers_in(r) != NGX_OK) {

        mod_ctx->status_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
        return;
    }

    // Processing request headers
    ngx_int_t rc = modsecProcessRequestHeaders(mod_ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity process headers: status %d", rc);
    rc = ngx_http_modsecurity_status(rc);

    if (rc != NGX_DECLINED) {
        mod_ctx->status_code = rc;
        return;
    }

    if (modsecContextState(mod_ctx->req) == MODSEC_DISABLED) {
        mod_ctx->status_code = NGX_DECLINED;
        return;
    }

    if (modsecIsRequestBodyAccessEnabled(mod_ctx->req) && (r->headers_in.content_length || r->headers_in.chunked)) {
        if (ngx_http_modsecurity_load_request_body(r) != NGX_OK) {
            mod_ctx->status_code = NGX_HTTP_INTERNAL_SERVER_ERROR;
            return;
        }
    }

    // The name of modsecProcessRequestBody is a bit misleading. This function call is needed even to just process GET args.
    rc = modsecProcessRequestBody(mod_ctx->req);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "ModSecurity process body: status %d", rc);
    mod_ctx->status_code = ngx_http_modsecurity_status(rc);
}


static void
ngx_http_modsecurity_prevention_thread_completion(ngx_event_t *ev)
{
    // executed in nginx event loop after thread task is done, in order to pick up and continue processing request

    ngx_http_request_t  *r;

    r = ev->data;
    r->main->blocked--;
    r->aio = 0;

    ngx_http_modsecurity_ctx_t *ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    ctx->thread_running = 0;

    ngx_http_core_run_phases(r);
}


static void
ngx_http_modsecurity_detection_thread_func(void *data, ngx_log_t *log)
{
    // Executed in a separate thread
    ngx_http_modsecurity_detection_thread_ctx_t *thread_ctx = data;

    // Processing request headers
    ngx_int_t rc = ngx_http_modsecurity_status(modsecProcessRequestHeaders(thread_ctx->req));
    if (rc != NGX_DECLINED) {
        return;
    }

    if (modsecContextState(thread_ctx->req) == MODSEC_DISABLED) {
        return;
    }

    // The name of modsecProcessRequestBody is a bit misleading. This function call is needed even to just process GET args.
    modsecProcessRequestBody(thread_ctx->req);
}


static void
ngx_http_modsecurity_detection_thread_completion(ngx_event_t *ev)
{
    // executed in nginx event loop after thread task is done, in order to pick up and continue processing request
    ngx_http_modsecurity_detection_thread_ctx_t *ctx = ev->data;

    request_rec *req = ctx->req;
    conn_rec *connection = ctx->connection;

    if (req != NULL) {
        modsecFinishRequest(req);
    }
    if (connection != NULL) {
        modsecFinishConnection(connection);
    }
}


static ngx_int_t
ngx_http_modsecurity_prevention_task_offload(ngx_http_request_t *r)
{
    ngx_http_modsecurity_prevention_thread_ctx_t *thread_ctx;
    ngx_thread_task_t *task;

    task = ngx_thread_task_alloc(r->pool, sizeof(ngx_http_modsecurity_prevention_thread_ctx_t));
    if (task == NULL) {
        return NGX_ERROR;
    }

    thread_ctx = task->ctx;
    thread_ctx->r = r;

    task->handler = ngx_http_modsecurity_prevention_thread_func;
    task->event.handler = ngx_http_modsecurity_prevention_thread_completion;
    task->event.data = r;

    ngx_thread_pool_t* thread_pool = ngx_thread_pool_get((ngx_cycle_t *)ngx_cycle, &thread_pool_name);
    if (ngx_thread_task_post(thread_pool, task) != NGX_OK) {
        return NGX_ERROR;
    }

    r->main->blocked++;
    r->aio = 1;

    return NGX_OK;
}

static ngx_int_t
ngx_http_modsecurity_detection_task_offload(ngx_http_request_t *r)
{
    ngx_http_modsecurity_ctx_t *ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    request_rec *req = ctx->req;

    // Load request to request rec
    if (ngx_http_modsecurity_load_request(r) != NGX_OK ||
            ngx_http_modsecurity_load_headers_in(r) != NGX_OK) {
        return NGX_ERROR;
    }

    if (modsecIsRequestBodyAccessEnabled(req) && (r->headers_in.content_length || r->headers_in.chunked)) {
        if (ngx_http_modsecurity_load_request_body(r) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    ngx_thread_task_t *task = apr_thread_task_alloc(req->pool, sizeof(ngx_http_modsecurity_detection_thread_ctx_t));
    if (task == NULL) {
        return NGX_ERROR;
    }

    ngx_http_modsecurity_detection_thread_ctx_t *thread_ctx = task->ctx;
    thread_ctx->connection = ctx->connection;
    ctx->connection = NULL;
    thread_ctx->req = ctx->req;
    ctx->req = NULL;
    thread_ctx->brigade = ctx->brigade;
    ctx->brigade = NULL;

    task->handler = ngx_http_modsecurity_detection_thread_func;
    task->event.handler = ngx_http_modsecurity_detection_thread_completion;
    task->event.data = thread_ctx;

    ngx_thread_pool_t* thread_pool = ngx_thread_pool_get((ngx_cycle_t *)ngx_cycle, &thread_pool_name);
    if (ngx_thread_task_post(thread_pool, task) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_OK;
}

static void
ngx_http_modsecurity_body_handler(ngx_http_request_t *r)
{
    r->main->count--;
    ngx_http_core_run_phases(r);
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

    // Read body if not yet read
    if (!r->request_body) {
        rc = ngx_http_read_client_request_body(r, ngx_http_modsecurity_body_handler);

        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }

        return NGX_DONE;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "modSecurity: handler");

    // Get module request context or create if not yet created
    ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity);
    if (ctx == NULL) {
        ctx = ngx_http_modsecurity_create_ctx(r);
        if (ctx == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        ngx_http_set_ctx(r, ctx, ngx_http_modsecurity);
    }

    // Sometimes Nginx calls ngx_http_modsecurity_handler multiple times for the same request, after a worker thread has already been started. This is to guard against it.
    if (ctx->thread_running) {
        return NGX_DONE;
    }

    if (ctx->status_code != STATUS_CODE_NOT_SET) {
        if (ctx->status_code > 0) {
            return ctx->status_code;
        }

        // Request must be routed to the next handler
        return NGX_DECLINED;
    }

    /* When in detection mode, don't wait for background task to complete because it won't affect the request status */
    if (cf->config->is_enabled == MODSEC_DETECTION_ONLY) {
        if (ngx_http_modsecurity_detection_task_offload(r) != NGX_OK) {
            return NGX_ERROR;
        }

        return NGX_DECLINED;
    }

    ctx->thread_running = 1;
    if (ngx_http_modsecurity_prevention_task_offload(r) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_DONE;
}


#define TXID_SIZE 25

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
        asa = apr_palloc(ctx->connection->pool, sizeof(apr_sockaddr_t));
        asa->pool = ctx->connection->pool;
        asa->hostname = dup_ngx_str_to_apr(asa->pool, &r->connection->addr_text);
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

    if (cf->config->is_enabled != MODSEC_DETECTION_ONLY) {
        apr_table_setn(ctx->req->notes, NOTE_NGINX_REQUEST_CTX, (const char *)ctx);
    }
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

    apr_table_setn(ctx->req->subprocess_env, "UNIQUE_ID", apr_psprintf(ctx->req->pool, "%s", salt));

    ctx->brigade = apr_brigade_create(ctx->req->pool, ctx->req->connection->bucket_alloc);

    if (ctx->brigade == NULL) {
        return NULL;
    }

    ctx->status_code = STATUS_CODE_NOT_SET;

    return ctx;
}

    static void
ngx_http_modsecurity_cleanup(void *data)
{
    ngx_http_modsecurity_ctx_t    *ctx = data;

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

