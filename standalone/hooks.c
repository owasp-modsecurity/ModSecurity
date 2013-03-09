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


#define DECLARE_EXTERNAL_HOOK(ns,link,ret,name,args) \
ns##_HOOK_##name##_t *hookfn_##name = NULL; \
link##_DECLARE(void) ns##_hook_##name(ns##_HOOK_##name##_t *pf, \
                                      const char * const *aszPre, \
                                      const char * const *aszSucc, int nOrder) \
{ \
	hookfn_##name = pf; \
}

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
