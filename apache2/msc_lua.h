/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2008 Breach Security, Inc. (http://www.breach.com/)
 *
 * You should have received a copy of the licence along with this
 * program (stored in the file "LICENSE"). If the file is missing,
 * or if you have any other questions related to the licence, please
 * write to Breach Security, Inc. at support@breach.com.
 *
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

#endif

#endif /* WITH_LUA */
