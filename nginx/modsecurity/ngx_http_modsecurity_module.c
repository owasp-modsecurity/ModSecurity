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

typedef struct
{
  ngx_log_t	*log;
} ngx_http_modsecurity_main_conf_t;

typedef struct
{
  ngx_int_t	request_processed;
  ngx_int_t	request_blocked;
  ngx_int_t	error;
  ngx_flag_t	enabled;
  ngx_str_t	config_path;
  directory_config *config;
} ngx_http_modsecurity_loc_conf_t;

/*
** Module's registred function/handlers.
*/
static ngx_int_t	ngx_http_modsecurity_access_handler(ngx_http_request_t *r);
static ngx_int_t	ngx_http_modsecurity_init(ngx_conf_t *cf);
static ngx_int_t	ngx_http_modsecurity_init_process(ngx_cycle_t *cycle);
static void		ngx_http_modsecurity_exit_process(ngx_cycle_t *cycle);
static void		*ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf);
static char		*ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, 
						       void *parent,
						       void *child);
static ngx_int_t        ngx_http_read_request_body(ngx_http_request_t *req,  
                                                        ngx_http_client_body_handler_pt handler);
void			*ngx_http_modsecurity_create_main_conf(ngx_conf_t *cf);


/* command handled by the module */
static ngx_command_t  ngx_http_modsecurity_commands[] =  {
  { ngx_string("ModSecurityConfig"),
    NGX_HTTP_LOC_CONF|NGX_CONF_1MORE,
    ngx_conf_set_str_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_modsecurity_loc_conf_t, config_path),
    NULL },
  { ngx_string("ModSecurityEnabled"),
    NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
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
  ngx_http_modsecurity_create_main_conf, /* create main configuration */
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

#define DEFAULT_MAX_LOC_T	10 

void *
ngx_http_modsecurity_create_main_conf(ngx_conf_t *cf) 
{
  ngx_http_modsecurity_main_conf_t	*mc;
  
  mc = ngx_pcalloc(cf->pool, sizeof(ngx_http_modsecurity_main_conf_t));
  if (!mc)
    return (NGX_CONF_ERROR);

  return (mc);
}


/* create log conf struct */
static void *
ngx_http_modsecurity_create_loc_conf(ngx_conf_t *cf)
{
  ngx_http_modsecurity_loc_conf_t  *conf;
  
  conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_modsecurity_loc_conf_t));
  if (conf == NULL)
    return NULL;
    
  conf->enabled = NGX_CONF_UNSET;

  return (conf);
}

/* merge loc conf */
static char *
ngx_http_modsecurity_merge_loc_conf(ngx_conf_t *cf, void *parent, 
			      void *child)
{
  ngx_http_modsecurity_loc_conf_t  *prev = parent;
  ngx_http_modsecurity_loc_conf_t  *conf = child;

  if(conf->config_path.len == 0)
    conf->config_path = prev->config_path;
    
  ngx_conf_merge_value(conf->enabled, prev->enabled, 0);

  return NGX_CONF_OK;
}

void modsecLog(void *obj, int level, char *str)
{
  if(obj != NULL)
    ngx_log_error(NGX_LOG_INFO, (ngx_log_t *)obj, 0, str);
}

/*
** This function sets up handlers for ACCESS_PHASE,
*/
static ngx_int_t 
ngx_http_modsecurity_init(ngx_conf_t *cf)
{
  ngx_http_handler_pt *h;
  ngx_http_core_main_conf_t *cmcf;
  ngx_http_modsecurity_main_conf_t *main_cf;
//  ngx_http_modsecurity_loc_conf_t **loc_cf;
//  unsigned int 				i;

  cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
  main_cf = ngx_http_conf_get_module_main_conf(cf, ngx_http_modsecurity_module);
  if (cmcf == NULL || 
      main_cf == NULL)
    return (NGX_ERROR);

  /* Register for access phase */
  //h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
  h = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);
  if (h == NULL) 
    return (NGX_ERROR);
  *h = ngx_http_modsecurity_access_handler;
  
  return (NGX_OK);
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
  modsecTerminate();
}

// This is a temporary hack to make PCRE work with ModSecurity
// nginx hijacks pcre_malloc and pcre_free, so we have to re-hijack them
//

extern apr_pool_t *pool;

void *modsec_pcre_malloc(size_t size)
{
  return apr_palloc(pool, size);
}

void modsec_pcre_free(void *ptr)
{
}

char *ConvertNgxStringToUTF8(ngx_str_t str, apr_pool_t *pool)
{
  char *t = (char *)apr_palloc(pool, str.len + 1);

  memcpy(t, str.data, str.len);  
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
 ** If method is POST or PUT, read request body and put in req->request_body->bufs
 */

static ngx_int_t
ngx_http_read_request_body(ngx_http_request_t *req,  
                      ngx_http_client_body_handler_pt handler)
{
// If has body request treat it
     ngx_int_t rc = 0;
     
     if(req->method == NGX_HTTP_POST || req->method==NGX_HTTP_PUT)
     {
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

/*
** [ENTRY POINT] does : this is the function called by nginx : 
*/
static ngx_int_t ngx_http_modsecurity_access_handler(ngx_http_request_t *req)
{
/*  ngx_http_request_ctx_t	*ctx;
  ngx_int_t			rc;
  ngx_http_core_loc_conf_t  *clcf;
  struct tms		 tmsstart, tmsend;
  clock_t		 start, end;
  
  ctx = ngx_http_get_module_ctx(r, ngx_http_modsecurity_module);

  clcf = ngx_http_get_module_loc_conf(r, ngx_http_core_module);
  
  if (ctx && ctx->over)
    return (NGX_DECLINED);
  if (ctx && ctx->wait_for_body) {
    return (NGX_DONE);
  }
  // the module is not enabled here 
  //if (!cf->enabled || cf->force_disabled)
  //  return (NGX_DECLINED);
  // don't process internal requests. 
  //if (r->internal) {
  //  return (NGX_DECLINED);
  }

      //nothing:      return (NGX_OK);
      //redirect : return (NGX_HTTP_OK);
*/
  ngx_http_modsecurity_loc_conf_t	*cf;
  conn_rec *c;
  request_rec *r;
  
  cf = ngx_http_get_module_loc_conf(req, ngx_http_modsecurity_module);

  if (!cf)
    return (NGX_ERROR);
    
  if(!cf->enabled)
    return (NGX_DECLINED);

  // temporary hack, nginx uses pcre as well and hijacks these two
  //
  pcre_malloc = modsec_pcre_malloc;
  pcre_free = modsec_pcre_free;

  if(cf->config == NULL)
  {
    cf->config = modsecGetDefaultConfig();

    if(cf->config_path.len != 0)
    {
      char *path = ngx_pcalloc(req->pool, cf->config_path.len+1);
      
      memcpy(path, cf->config_path.data, cf->config_path.len);
      
      const char *msg = modsecProcessConfig(cf->config, path);
  
      if(msg != NULL)
        ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, msg);
    }
  }

  //if(r->connection->requests == 0)
  //{
    c = modsecNewConnection();

    modsecProcessConnection(c);
  //}

  r = modsecNewRequest(c, cf->config);

  r->request_time = apr_time_now();

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

  ngx_list_part_t *part = &req->headers_in.headers.part;
  ngx_table_elt_t *h = part->elts;
  u_int i;

  for(i = 0; ; i++)
  {
    if(i >= part->nelts)
    {
      if(part->next == NULL)
        break;
      
      part = part->next;
      h = part->elts;
      i = 0;
    }

    apr_table_setn(r->headers_in, ConvertNgxStringToUTF8(h[i].key, r->pool),
      ConvertNgxStringToUTF8(h[i].value, r->pool));
  }
  
  apr_table_setn(r->subprocess_env, "UNIQUE_ID", "12345");

  ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, r->uri);

  int status = modsecProcessRequest(r);
  ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "status: %d", status);

  ngx_http_read_request_body(req, ngx_http_dummy_payload_handler); 

  if(req->headers_in.content_length)
  {
    ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "request body: %s", req->request_body->bufs); 
  } else {
    ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "request body: ");
  }     

  if(status == DECLINED)
  { 
    // this function would work here, but it is only internal
    //ngx_http_close_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    return (NGX_DECLINED);
    
    // If DECLINED, finalize connection (sent FIN) and return HTTP 500
    ngx_log_error(NGX_LOG_INFO, req->connection->log, 0, "Invalid Request");
    ngx_http_finalize_request(req, NGX_HTTP_INTERNAL_SERVER_ERROR);
    return NGX_HTTP_INTERNAL_SERVER_ERROR;
  }

    return status;
}
