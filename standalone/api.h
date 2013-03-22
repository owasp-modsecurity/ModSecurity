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


#pragma once

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


#ifdef	__cplusplus
extern "C"
{
#endif

server_rec *modsecInit();
void modsecTerminate();

void modsecStartConfig();
directory_config *modsecGetDefaultConfig();
const char *modsecProcessConfig(directory_config *config, const char *file, const char *dir);
void modsecFinalizeConfig();

void modsecInitProcess();

conn_rec *modsecNewConnection();
void modsecProcessConnection(conn_rec *c);

request_rec *modsecNewRequest(conn_rec *connection, directory_config *config);

int modsecProcessRequestBody(request_rec *r);
int modsecProcessRequestHeaders(request_rec *r);

static inline int modsecProcessRequest(request_rec *r)    {
    int status;
    status = modsecProcessRequestHeaders(r);
    if (status != DECLINED) {
        return status;
    }

    return modsecProcessRequestBody(r);
}


int modsecProcessResponse(request_rec *r);
int modsecFinishRequest(request_rec *r);

void modsecSetLogHook(void *obj, void (*hook)(void *obj, int level, char *str));

#define NOTE_MSR_BRIGADE_REQUEST  "modsecurity-brigade-request"
#define NOTE_MSR_BRIGADE_RESPONSE "modsecurity-brigade-response"

static inline void 
modsecSetBodyBrigade(request_rec *r, apr_bucket_brigade *b) {
    apr_table_setn(r->notes, NOTE_MSR_BRIGADE_REQUEST, (char *)b);
};

static inline apr_bucket_brigade * 
modsecGetBodyBrigade(request_rec *r) {
    return (apr_bucket_brigade *)apr_table_get(r->notes, NOTE_MSR_BRIGADE_REQUEST);
};

static inline void 
modsecSetResponseBrigade(request_rec *r, apr_bucket_brigade *b) {
    apr_table_setn(r->notes, NOTE_MSR_BRIGADE_RESPONSE, (char *)b);
};

static inline apr_bucket_brigade * 
modsecGetResponseBrigade(request_rec *r) {
    return (apr_bucket_brigade *)apr_table_get(r->notes, NOTE_MSR_BRIGADE_RESPONSE);
};

void modsecSetReadBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos));
void modsecSetReadResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos));
void modsecSetWriteBody(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length));
void modsecSetWriteResponse(apr_status_t (*func)(request_rec *r, char *buf, unsigned int length));
void modsecSetDropAction(int (*func)(request_rec *r));

int modsecIsResponseBodyAccessEnabled(request_rec *r);
int modsecIsRequestBodyAccessEnabled(request_rec *r);

void modsecSetConfigForIISRequestBody(request_rec *r);

#ifdef __cplusplus
}
#endif
