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

#if defined(WITH_LUA)

#ifndef _MSC_LUA_H_
#define _MSC_LUA_H_

typedef struct msc_script msc_script;
typedef struct msc_script_part msc_script_part;

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "apr_general.h"
#include "apr_tables.h"
#include "modsecurity.h"

struct msc_script {
    const char *name;
    apr_array_header_t *parts;
};

struct msc_script_part {
    const void *data;
    size_t len;
};

char DSOLOCAL *lua_compile(msc_script **script, const char *filename, apr_pool_t *pool);

int DSOLOCAL lua_execute(msc_script *script, char *param, modsec_rec *msr, msre_rule *rule, char **error_msg);

apr_status_t DSOLOCAL  msre_action_setvar_execute(modsec_rec *r, apr_pool_t *, msre_rule *, char *, char *);

#endif

#endif /* WITH_LUA */
