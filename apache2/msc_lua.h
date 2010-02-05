/*
 * ModSecurity for Apache 2.x, http://www.modsecurity.org/
 * Copyright (c) 2004-2010 Breach Security, Inc. (http://www.breach.com/)
 *
 * This product is released under the terms of the General Public Licence,
 * version 2 (GPLv2). Please refer to the file LICENSE (included with this
 * distribution) which contains the complete text of the licence.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the exception in
 * file MODSECURITY_LICENSING_EXCEPTION in the directory of this software
 * distribution.
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Breach Security, Inc.
 * directly using the email address support@breach.com.
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
