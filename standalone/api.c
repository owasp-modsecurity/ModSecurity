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
#include <stdio.h>

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

#include "api.h"

extern void *modsecLogObj;
extern void (*modsecLogHook)(void *obj, int level, char *str);
extern int (*modsecDropAction)(request_rec *r);
apr_status_t (*modsecReadBody)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos);
apr_status_t (*modsecReadResponse)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos);
apr_status_t (*modsecWriteBody)(request_rec *r, char *buf, unsigned int length);
apr_status_t (*modsecWriteResponse)(request_rec *r, char *buf, unsigned int length);

extern const char *process_command_config(server_rec *s,
                                          void *mconfig,
                                          apr_pool_t *p,
                                          apr_pool_t *ptemp,
                                          const char *filename);

#define DECLARE_EXTERNAL_HOOK(ns,link,ret,name,args) \
extern ns##_HOOK_##name##_t *hookfn_##name;

#define DECLARE_HOOK(ret,name,args) \
    DECLARE_EXTERNAL_HOOK(ap,AP,ret,name,args)

DECLARE_HOOK(int,pre_config,(apr_pool_t *pconf,apr_pool_t *plog, apr_pool_t *ptemp))
DECLARE_HOOK(int,post_config,(apr_pool_t *pconf,apr_pool_t *plog, apr_pool_t *ptemp,server_rec *s))
DECLARE_HOOK(void,child_init,(apr_pool_t *pchild, server_rec *s))
DECLARE_HOOK(int,process_connection,(conn_rec *c))
DECLARE_HOOK(int,post_read_request,(request_rec *r))
DECLARE_HOOK(int,fixups,(request_rec *r))
DECLARE_HOOK(void, error_log, (const char *file, int line, int level,
                       apr_status_t status, const server_rec *s,
                       const request_rec *r, apr_pool_t *pool,
                       const char *errstr))
DECLARE_HOOK(int,log_transaction,(request_rec *r))
DECLARE_HOOK(void,insert_filter,(request_rec *r))
DECLARE_HOOK(void,insert_error_filter,(request_rec *r))

char *sa_name = "standalone";
server_rec *server;
apr_pool_t *pool = NULL;

apr_status_t ap_http_in_filter(ap_filter_t *f, apr_bucket_brigade *b,
                            ap_input_mode_t mode, apr_read_type_e block,
                            apr_off_t readbytes);
apr_status_t ap_http_out_filter(ap_filter_t *f, apr_bucket_brigade *b);

server_rec *modsecInit()    {
    apr_initialize();

    apr_pool_create(&pool, NULL);

    apr_hook_global_pool = pool;

    server = apr_palloc(pool, sizeof(server_rec));

    server->addrs = apr_palloc(pool, sizeof(server_addr_rec));
    server->addrs->host_addr = apr_palloc(pool, sizeof(apr_sockaddr_t));
    server->addrs->host_addr->addr_str_len = 16;
    server->addrs->host_addr->family = AF_INET;
    server->addrs->host_addr->hostname = sa_name;
#ifdef WIN32
    server->addrs->host_addr->ipaddr_len = sizeof(IN_ADDR);
#else
    server->addrs->host_addr->ipaddr_len = sizeof(struct in_addr);
#endif
    server->addrs->host_addr->ipaddr_ptr = &server->addrs->host_addr->sa.sin.sin_addr;
    server->addrs->host_addr->pool = pool;
    server->addrs->host_addr->port = 80;
#ifdef WIN32
    server->addrs->host_addr->sa.sin.sin_addr.S_un.S_addr = 0x0100007f;
#else
    server->addrs->host_addr->sa.sin.sin_addr.s_addr = 0x0100007f;
#endif
    server->addrs->host_addr->sa.sin.sin_family = AF_INET;
    server->addrs->host_addr->sa.sin.sin_port = 80;
    server->addrs->host_addr->salen = sizeof(server->addrs->host_addr->sa);
    server->addrs->host_addr->servname = sa_name;
    server->addrs->host_port = 80;
    server->error_fname = "error.log";
    server->error_log = NULL;
    server->limit_req_fields = 1024;
    server->limit_req_fieldsize = 1024;
    server->limit_req_line = 1024;
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    server->loglevel = APLOG_DEBUG;
#endif
    server->lookup_defaults = NULL;
    server->module_config = NULL;
    server->names = NULL;
#ifdef WIN32
    server->path = "c:\\inetpub\\wwwroot";
#else
    server->path = "/var/www";
#endif
    server->pathlen = strlen(server->path);
    server->port = 80;
    server->process = apr_palloc(pool, sizeof(process_rec));
    server->process->argc = 1;
    server->process->argv = &sa_name;
    server->process->pconf = pool;
    server->process->pool = pool;
    server->process->short_name = sa_name;
    server->server_admin = sa_name;
    server->server_hostname = sa_name;
    server->server_scheme = "";
    server->timeout = 60 * 1000000;// 60 seconds
    server->wild_names = NULL;
    server->is_virtual = 0;

    ap_server_config_defines = apr_array_make(pool, 1, sizeof(char *));

    // here we should add scoreboard handling for multiple processes and threads
    //
    ap_scoreboard_image = (scoreboard *)apr_palloc(pool, sizeof(scoreboard));

    memset(ap_scoreboard_image, 0, sizeof(scoreboard));

    // ----------

    security2_module.module_index = 0;

    security2_module.register_hooks(pool);

    ap_register_input_filter("HTTP_IN", ap_http_in_filter, NULL, AP_FTYPE_RESOURCE);
    ap_register_output_filter("HTTP_OUT", ap_http_out_filter, NULL, AP_FTYPE_CONTENT_SET);

    return server;
}

apr_status_t ap_http_in_filter(ap_filter_t *f, apr_bucket_brigade *bb_out,
        ap_input_mode_t mode, apr_read_type_e block,
        apr_off_t readbytes)    {
    char *tmp = NULL;
    apr_bucket *e = NULL;
    unsigned int readcnt = 0;
    int is_eos = 0;
    apr_bucket_brigade *bb_in;
    apr_bucket *after;
    apr_status_t rv;

    bb_in = modsecGetBodyBrigade(f->r);

    /* use request brigade */
    if (bb_in != NULL) {
        if (!APR_BUCKET_IS_EOS(APR_BRIGADE_LAST(bb_in))) {
            e = apr_bucket_eos_create(f->c->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb_in, e);
        }

        rv = apr_brigade_partition(bb_in, readbytes, &after);
        if (rv != APR_SUCCESS && rv != APR_INCOMPLETE) {
            return rv;
        }
        
        for (e = APR_BRIGADE_FIRST(bb_in); e != after; e = APR_BRIGADE_FIRST(bb_in)) {
            APR_BUCKET_REMOVE(e);
            APR_BRIGADE_INSERT_TAIL(bb_out, e);
        }

        return APR_SUCCESS;
    }

    /* call the callback */
    if(modsecReadBody != NULL) {

        tmp = (char *)apr_palloc(f->r->pool, readbytes);
        modsecReadBody(f->r, tmp, readbytes, &readcnt, &is_eos);

        e = apr_bucket_pool_create(tmp, readcnt, f->r->pool, f->c->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb_out, e);

        if(is_eos)  {
            e = apr_bucket_eos_create(f->c->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb_out, e);
        }
        return APR_SUCCESS;
    }

    /* cannot read request body */
    e = apr_bucket_eos_create(f->c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb_out, e);

    return APR_SUCCESS;
}

apr_status_t ap_http_out_filter(ap_filter_t *f, apr_bucket_brigade *b)  {
    modsec_rec *msr = (modsec_rec *)f->ctx;
    apr_status_t rc;
    apr_bucket_brigade *bb_out;
    
    bb_out = modsecGetResponseBrigade(f->r);


    if (bb_out) {
        APR_BRIGADE_CONCAT(bb_out, b);
        return APR_SUCCESS;
    }

    // is there a way to tell whether the response body was modified or not?
    //
    if((msr->txcfg->content_injection_enabled || msr->content_prepend_len != 0 || msr->content_append_len != 0)
            && msr->txcfg->resbody_access)   {

        if (modsecWriteResponse != NULL) {
            char *data = NULL;
            apr_size_t length;

            rc = apr_brigade_pflatten(msr->of_brigade, &data, &length, msr->mp);

            if (rc != APR_SUCCESS) {
                msr_log(msr, 1, "Output filter: Failed to flatten brigade (%d): %s", rc,
                        get_apr_error(msr->mp, rc));
                return -1;
            }

            /* TODO: return ?*/
            modsecWriteResponse(msr->r, data, msr->stream_output_length);
        }
    }

    return APR_SUCCESS;
}

void modsecTerminate()  {
    apr_pool_destroy(pool);
    pool = NULL;
    apr_terminate();
}

void modsecStartConfig()    {
    apr_pool_t *ptemp = NULL;

    apr_pool_create(&ptemp, pool);

    hookfn_pre_config(pool, pool, ptemp);

    apr_pool_destroy(ptemp);
}

directory_config *modsecGetDefaultConfig()  {
    return (directory_config *)security2_module.create_dir_config(pool, NULL);
}

const char *modsecProcessConfig(directory_config *config, const char *file, const char *dir)  {
    apr_pool_t *ptemp = NULL;
    const char *err;
	apr_status_t status;
	const char *rootpath, *incpath;

	if(dir == NULL || strlen(dir) == 0)
#ifdef	WIN32
		dir = "\\";
#else
		dir = "/";
#endif

	incpath = file;

	/* locate the start of the directories proper */
	status = apr_filepath_root(&rootpath, &incpath, APR_FILEPATH_TRUENAME | APR_FILEPATH_NATIVE, pool);

	/* we allow APR_SUCCESS and APR_EINCOMPLETE */
	if (APR_ERELATIVE == status) {
		int li = strlen(dir) - 1;

		if(dir[li] != '/' && dir[li] != '\\')
#ifdef	WIN32
			file = apr_pstrcat(pool, dir, "\\", file, NULL);
#else
			file = apr_pstrcat(pool, dir, "/", file, NULL);
#endif
		else
			file = apr_pstrcat(pool, dir, file, NULL);
	}
	else if (APR_EBADPATH == status) {
		return apr_pstrcat(pool, "Config file has a bad path, ", file, NULL);
	}

	apr_pool_create(&ptemp, pool);

    err = process_command_config(server, config, pool, ptemp, file);

    apr_pool_destroy(ptemp);

    return err;
}

void modsecFinalizeConfig() {
    apr_pool_t *ptemp = NULL;

    apr_pool_create(&ptemp, pool);

    hookfn_post_config(pool, pool, ptemp, server);
    hookfn_post_config(pool, pool, ptemp, server);

    apr_pool_destroy(ptemp);
}

void modsecInitProcess()    {
    hookfn_child_init(pool, server);
}

conn_rec *modsecNewConnection() {
    conn_rec *c;
    apr_pool_t *pc = NULL;

    apr_pool_create(&pc, pool);

    c = apr_pcalloc(pc, sizeof(conn_rec));

    c->base_server = server;
    c->id = 1;
    c->local_addr = server->addrs->host_addr;
    c->local_host = sa_name;
    c->local_ip = "127.0.0.1";
    c->pool = pc;
    c->remote_host = sa_name;
#if AP_SERVER_MAJORVERSION_NUMBER > 1 && AP_SERVER_MINORVERSION_NUMBER < 3
    c->remote_ip = "127.0.0.1";
    c->remote_addr = server->addrs->host_addr;
#else
    c->client_ip = "127.0.0.1";
    c->client_addr = server->addrs->host_addr;
#endif
    c->input_filters = NULL;
    c->output_filters = NULL;
    c->bucket_alloc = apr_bucket_alloc_create(pc);

    return c;
}

void modsecProcessConnection(conn_rec *c)   {
    hookfn_process_connection(c);
}

request_rec *modsecNewRequest(conn_rec *connection, directory_config *config)   {
    request_rec *r;
    apr_pool_t *pr = NULL;

    apr_pool_create(&pr, connection->pool);

    r = apr_pcalloc(pr, sizeof(request_rec));

    r->connection = connection;
    r->server = server;
    r->pool = pr;
    r->main = NULL;
    r->next = NULL;
    r->notes = apr_table_make(pr, 10);
    r->per_dir_config = apr_palloc(pr, sizeof(void *));
    ((void **)r->per_dir_config)[0] = config;
    r->prev = NULL;
    r->subprocess_env = apr_table_make(pr, 10);
    apr_table_setn(r->subprocess_env, "UNIQUE_ID", "unique_id");
    r->user = NULL;

    r->headers_in = apr_table_make(pr, 10);
    r->headers_out = apr_table_make(pr, 10);
    r->err_headers_out = apr_table_make(pr, 10);
    //apr_table_setn(r->headers_in, "Host", "www.google.com");
    //apr_table_setn(r->headers_in, "", "");

    r->the_request = "GET /../../index.html HTTP/1.1";
    r->method = "GET";
    r->method_number = M_GET;
    r->protocol = "HTTP/1.1";
    r->uri = "http://www.google.com/../../index.html";
    r->args = "";
    r->filename = "/../../index.html";
    r->handler = "IIS";

    r->parsed_uri.scheme = "http";
    r->parsed_uri.path = "/../../index.html";
    r->parsed_uri.hostname = "www.google.com";
    r->parsed_uri.is_initialized = 1;
    r->parsed_uri.port = 1234;
    r->parsed_uri.port_str = "1234";
    r->parsed_uri.query = "";
    r->parsed_uri.dns_looked_up = 0;
    r->parsed_uri.dns_resolved = 0;
    r->parsed_uri.password = NULL;
    r->parsed_uri.user = NULL;
    r->parsed_uri.fragment = "";

    r->input_filters = NULL;
    r->output_filters = NULL;

    return r;
}

static modsec_rec *retrieve_msr(request_rec *r) {
    modsec_rec *msr = NULL;
    request_rec *rx = NULL;

    /* Look in the current request first. */
    msr = (modsec_rec *)apr_table_get(r->notes, NOTE_MSR);
    if (msr != NULL) {
        msr->r = r;
        return msr;
    }

    /* If this is a subrequest then look in the main request. */
    if (r->main != NULL) {
        msr = (modsec_rec *)apr_table_get(r->main->notes, NOTE_MSR);
        if (msr != NULL) {
            msr->r = r;
            return msr;
        }
    }

    /* If the request was redirected then look in the previous requests. */
    rx = r->prev;
    while(rx != NULL) {
        msr = (modsec_rec *)apr_table_get(rx->notes, NOTE_MSR);
        if (msr != NULL) {
            msr->r = r;
            return msr;
        }
        rx = rx->prev;
    }

    return NULL;
}

int modsecProcessRequestHeaders(request_rec *r) {
    return hookfn_post_read_request(r);
}

int modsecProcessRequestBody(request_rec *r) {
    int status = DECLINED;
    modsec_rec *msr = NULL;

    ap_filter_t *f = ap_add_input_filter("HTTP_IN", NULL, r, r->connection);
    apr_bucket_brigade* bb_out;

    status = hookfn_fixups(r);

    ap_remove_input_filter(f);

	if (status != DECLINED) {
		return status;
	}

    hookfn_insert_filter(r);

    /* Find the transaction context first. */
    msr = retrieve_msr(r);

    if (msr == NULL)
        return status;

    bb_out = modsecGetBodyBrigade(r);
    if (bb_out) {
        (void) apr_brigade_cleanup(bb_out);
        status = ap_get_brigade(r->input_filters, bb_out, AP_MODE_READBYTES, APR_BLOCK_READ, -1);
        if (status == APR_SUCCESS) {
            return DECLINED;
        }
        return status;
    }

    if(msr->stream_input_data != NULL && modsecWriteBody != NULL)
    {
        // target is responsible for copying the data into correctly managed buffer
        //
        modsecWriteBody(r, msr->stream_input_data, msr->stream_input_length);

        free(msr->stream_input_data);

        msr->stream_input_data = NULL;
    }

    // leftover code possibly for future use
    //
    //if(r->input_filters != NULL && r->input_filters->frec->filter_init_func != NULL)
    //r->input_filters->frec->filter_init_func(r->input_filters);
    //if(r->input_filters != NULL && r->input_filters->frec->filter_func.in_func != NULL)
    //r->input_filters->frec->filter_func.in_func(r->input_filters, NULL, 0, 0, 0);

    return status;
}

void modsecSetConfigForIISRequestBody(request_rec *r)
{
    modsec_rec *msr = retrieve_msr(r);

    if(msr == NULL || msr->txcfg == NULL)
        return;

    if(msr->txcfg->reqbody_access)
        msr->txcfg->stream_inbody_inspection = 1;
}

int modsecIsRequestBodyAccessEnabled(request_rec *r)
{
    modsec_rec *msr = retrieve_msr(r);

    if(msr == NULL || msr->txcfg == NULL)
        return 0;

    return msr->txcfg->reqbody_access;
}

int modsecIsResponseBodyAccessEnabled(request_rec *r)
{
    modsec_rec *msr = retrieve_msr(r);

    if(msr == NULL || msr->txcfg == NULL)
        return 0;

    return msr->txcfg->resbody_access;
}

int modsecProcessResponse(request_rec *r)   {
    int status = DECLINED;

    if(r->output_filters != NULL)   {
        modsec_rec *msr = (modsec_rec *)r->output_filters->ctx;
        char buf[8192];
        char *tmp = NULL;
        apr_bucket *e = NULL;
        unsigned int readcnt = 0;
        int is_eos = 0;
        ap_filter_t *f = NULL;
        apr_bucket_brigade *bb_in, *bb = NULL;

        if (msr == NULL) {
            ap_log_error(APLOG_MARK, APLOG_ERR | APLOG_NOERRNO, 0, r->server,
                    "ModSecurity: Internal Error: msr is null in output filter.");
            ap_remove_output_filter(r->output_filters);
            return send_error_bucket(msr, r->output_filters, HTTP_INTERNAL_SERVER_ERROR);
        }

        bb = apr_brigade_create(msr->mp, r->connection->bucket_alloc);

        if (bb == NULL) {
            msr_log(msr, 1, "Process response: Failed to create brigade.");
            return APR_EGENERAL;
        }

        msr->r = r;
        
        bb_in = modsecGetResponseBrigade(r);

        if (bb_in != NULL) {
            APR_BRIGADE_CONCAT(bb, bb_in);
            if (!APR_BUCKET_IS_EOS(APR_BRIGADE_LAST(bb))) {
                e = apr_bucket_eos_create(bb->bucket_alloc);
                APR_BRIGADE_INSERT_TAIL(bb, e);
            }
        } else if (modsecReadResponse != NULL) {
            while(!is_eos)  {
                modsecReadResponse(r, buf, 8192, &readcnt, &is_eos);

                if(readcnt > 0) {
                    tmp = (char *)apr_palloc(r->pool, readcnt);
                    memcpy(tmp, buf, readcnt);
                    e = apr_bucket_pool_create(tmp, readcnt, r->pool, r->connection->bucket_alloc);
                    APR_BRIGADE_INSERT_TAIL(bb, e);
                }
            }

            e = apr_bucket_eos_create(r->connection->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb, e);
        } else {
            /* cannot read response body process header only */

            e = apr_bucket_eos_create(r->connection->bucket_alloc);
            APR_BRIGADE_INSERT_TAIL(bb, e);
        }
        
        f = ap_add_output_filter("HTTP_OUT", msr, r, r->connection);
        status = ap_pass_brigade(r->output_filters, bb);
        ap_remove_output_filter(f);
        if(status > 0
                && msr->intercept_actionset->intercept_status != 0)  {
            status =  msr->intercept_actionset->intercept_status;
        }
        return status;
    }

    return status;
}

int modsecFinishRequest(request_rec *r) {
    // run output filter
    //if(r->output_filters != NULL && r->output_filters->frec->filter_init_func != NULL)
    //r->output_filters->frec->filter_init_func(r->output_filters);

    hookfn_log_transaction(r);

    // make sure you cleanup before calling apr_terminate()
    // otherwise double-free might occur, because of the request body pool cleanup function
    //
    apr_pool_destroy(r->connection->pool);

    return DECLINED;
}

void modsecSetLogHook(void *obj, void (*hook)(void *obj, int level, char *str)) {
    modsecLogObj = obj;
    modsecLogHook = hook;
}

void modsecSetReadBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos))    {
    modsecReadBody = func;
}

void modsecSetReadResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos))    {
    modsecReadResponse = func;
}

void modsecSetWriteBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length))   {
    modsecWriteBody = func;
}

void modsecSetWriteResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length))   {
    modsecWriteResponse = func;
}

void modsecSetDropAction(int (*func)(request_rec *r)) {
    modsecDropAction = func;
}
