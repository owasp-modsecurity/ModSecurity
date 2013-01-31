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

#if defined(WITH_LUA)

#include "msc_lua.h"

#include "apr_strings.h"

typedef struct {
    apr_array_header_t *parts;
    apr_pool_t *pool;
} msc_lua_dumpw_t;

typedef struct {
    msc_script *script;
    int index;
} msc_lua_dumpr_t;

/**
 *
 */
static const char* dump_reader(lua_State* L, void* user_data, size_t* size) {
    msc_lua_dumpr_t *dumpr = (msc_lua_dumpr_t *)user_data;
    msc_script_part *part;

    /* Do we have more chunks to return? */
    if (dumpr->index == dumpr->script->parts->nelts) {
        return NULL;
    }

    /* Get one chunk. */
    part = ((msc_script_part **)dumpr->script->parts->elts)[dumpr->index];
    *size = part->len;

    dumpr->index++;

    return part->data;
}

/**
 *
 */
static int dump_writer(lua_State *L, const void* data, size_t len, void* user_data) {
    msc_lua_dumpw_t *dump = (msc_lua_dumpw_t *)user_data;
    msc_script_part *part;
    void *part_data;

    /* Allocate new part, copy the data into it. */
    part_data = apr_palloc(dump->pool, len);
    memcpy(part_data, data, len);
    part = apr_palloc(dump->pool, sizeof(msc_script_part));
    part->data = part_data;
    part->len = len;

    /* Then add it to the list of parsts. */
    *(const msc_script_part **)apr_array_push(dump->parts) = part;

    return 0;
}

/**
 *
 */
static int lua_restore(lua_State *L, msc_script *script) {
    msc_lua_dumpr_t dumpr;

    dumpr.script = script;
    dumpr.index = 0;

#if LUA_VERSION_NUM > 501
    return lua_load(L, dump_reader, &dumpr, script->name, NULL);
#else
    return lua_load(L, dump_reader, &dumpr, script->name);
#endif
}

/**
 *
 */
char *lua_compile(msc_script **script, const char *filename, apr_pool_t *pool) {
    lua_State *L = NULL;
    msc_lua_dumpw_t dump;

    /* Initialise state. */
#if LUA_VERSION_NUM > 501
    L = luaL_newstate();
#else
    L = lua_open();
#endif
    luaL_openlibs(L);

    /* Find script. */
    if (luaL_loadfile(L, filename)) {
        return apr_psprintf(pool, "ModSecurity: Failed to compile script %s: %s",
            filename, lua_tostring(L, -1));
    }

    /* Dump the script into binary form. */
    dump.pool = pool;
    dump.parts = apr_array_make(pool, 128, sizeof(msc_script_part *));

    lua_dump(L, dump_writer, &dump);

    (*script) = apr_pcalloc(pool, sizeof(msc_script));
    (*script)->name = filename;
    (*script)->parts = dump.parts;

    /* Destroy state. */
    lua_close(L);

    return NULL;
}

/**
 *
 */
static int l_log(lua_State *L) {
    modsec_rec *msr = NULL;
    const char *text;
    int level;

    /* Retrieve parameters. */
    level = luaL_checknumber(L, 1);
    text = luaL_checkstring(L, 2);

    /* Retrieve msr. */
    lua_getglobal(L, "__msr");
    msr = (modsec_rec *)lua_topointer(L, -1);

    /* Log message. */
    if (msr != NULL) {
        msr_log(msr, level, "%s", text);
    }

    return 0;
}

/**
 *
 */
static apr_array_header_t *resolve_tfns(lua_State *L, int idx, modsec_rec *msr, apr_pool_t *mp) {
    apr_array_header_t *tfn_arr = NULL;
    msre_tfn_metadata *tfn = NULL;
    char *name = NULL;

    tfn_arr = apr_array_make(mp, 25, sizeof(msre_tfn_metadata *));
    if (tfn_arr == NULL) return NULL;

    /* ENH: Why is this userdata and not none/nil when parameter not given? */
    if (lua_isuserdata(L, idx) || lua_isnoneornil(L, idx)) { /* No second parameter */
        return tfn_arr;
    } else if (lua_istable(L, idx)) { /* Is the second parameter an array? */
#if LUA_VERSION_NUM > 501
        int i, n = lua_rawlen(L, idx);
#else
        int i, n = lua_objlen(L, idx);
#endif

        for(i = 1; i <= n; i++) {
            lua_rawgeti(L, idx, i);
            name = (char *)luaL_checkstring(L, -1);

            /* A "none" means start over */
            if (strcmp("none", name) == 0) {
                tfn_arr->nelts = 0;
                continue;
            }

            tfn = msre_engine_tfn_resolve(msr->modsecurity->msre, name);
            if (tfn == NULL) {
                msr_log(msr, 1, "SecRuleScript: Invalid transformation function: %s", name);
            } else {
                *(msre_tfn_metadata **)apr_array_push(tfn_arr) = tfn;
            }
        }
    } else if (lua_isstring(L, idx)) { /* The second parameter may be a simple string? */
        name = (char *)luaL_checkstring(L, idx);

        /* A "none" means start over */
        if (strcmp("none", name) == 0) {
            tfn_arr->nelts = 0;
        }
        else {
            tfn = msre_engine_tfn_resolve(msr->modsecurity->msre, name);
            if (tfn == NULL) {
                msr_log(msr, 1, "SecRuleScript: Invalid transformation function: %s", name);
            } else {
                *(msre_tfn_metadata **)apr_array_push(tfn_arr) = tfn;
            }
        }
    } else {
        msr_log(msr, 1, "SecRuleScript: Transformation parameter must be a transformation name or array of transformation names, but found \"%s\" (type %d).", lua_typename(L, idx), lua_type(L, idx));
        return NULL;
    }

    return tfn_arr;
}

/**
 *
 */
static int l_getvar(lua_State *L) {
    char *varname = NULL, *param = NULL;
    modsec_rec *msr = NULL;
    msre_rule *rule = NULL;
    char *my_error_msg = NULL;
    char *p1 = NULL;
    apr_array_header_t *tfn_arr = NULL;
    msre_var *vx = NULL;
    msre_var *var;

    /* Retrieve parameters. */
    p1 = (char *)luaL_checkstring(L, 1);

    /* Retrieve msr. */
    lua_getglobal(L, "__msr");
    msr = (modsec_rec *)lua_topointer(L, -1);

    /* Retrieve rule. */
    lua_getglobal(L, "__rule");
    rule = (msre_rule *)lua_topointer(L, -1);

    /* Extract the variable name and its parameter from the script. */
    varname = apr_pstrdup(msr->msc_rule_mptmp, p1);
    param = strchr(varname, '.');
    if (param != NULL) {
        *param = '\0';
        param++;
    }

    /* Resolve variable. */
    var = msre_create_var_ex(msr->msc_rule_mptmp, msr->modsecurity->msre,
        varname, param, msr, &my_error_msg);

    if (var == NULL) {
        msr_log(msr, 1, "%s", my_error_msg);

        lua_pushnil(L);

        return 0;
    }

    /* Resolve transformation functions. */
    tfn_arr = resolve_tfns(L, 2, msr, msr->msc_rule_mptmp);

    /* Generate variable. */
    vx = generate_single_var(msr, var, tfn_arr, rule, msr->msc_rule_mptmp);
    if (vx == NULL) {
        lua_pushnil(L);

        return 0;
    }

    /* Return variable value. */
    lua_pushlstring(L, vx->value, vx->value_len);

    return 1;
}

/**
 *
 */
static int l_getvars(lua_State *L) {
    const apr_array_header_t *tarr;
    const apr_table_entry_t *telts;
    apr_table_t *vartable = NULL;
    apr_array_header_t *tfn_arr = NULL;
    char *varname = NULL, *param = NULL;
    modsec_rec *msr = NULL;
    msre_rule *rule = NULL;
    msre_var *vartemplate = NULL;
    char *my_error_msg = NULL;
    char *p1 = NULL;
    int i;

    /* Retrieve parameters. */
    p1 = (char *)luaL_checkstring(L, 1);

    /* Retrieve msr. */
    lua_getglobal(L, "__msr");
    msr = (modsec_rec *)lua_topointer(L, -1);

    /* Retrieve rule. */
    lua_getglobal(L, "__rule");
    rule = (msre_rule *)lua_topointer(L, -1);

    /* Extract the variable name and its parameter from the script. */
    varname = apr_pstrdup(msr->msc_rule_mptmp, p1);
    param = strchr(varname, '.');
    if (param != NULL) {
        *param = '\0';
        param++;
    }

    /* Resolve transformation functions. */
    tfn_arr = resolve_tfns(L, 2, msr, msr->msc_rule_mptmp);

    lua_newtable(L);

    /* Resolve variable. */
    vartemplate = msre_create_var_ex(msr->msc_rule_mptmp, msr->modsecurity->msre,
        varname, param, msr, &my_error_msg);

    if (vartemplate == NULL) {
        msr_log(msr, 1, "%s", my_error_msg);

        /* Returning empty table. */
        return 1;
    }

    vartable = generate_multi_var(msr, vartemplate, tfn_arr, rule, msr->msc_rule_mptmp);

    tarr = apr_table_elts(vartable);
    telts = (const apr_table_entry_t*)tarr->elts;
    for (i = 0; i < tarr->nelts; i++) {
        msre_var *var = (msre_var *)telts[i].val;

        lua_pushnumber(L, i + 1); /* Index is not zero-based. */

        lua_newtable(L); /* Per-parameter table. */

        lua_pushstring(L, "name");
        lua_pushlstring(L, var->name, strlen(var->name));
        lua_settable(L, -3);

        lua_pushstring(L, "value");
        lua_pushlstring(L, var->value, var->value_len);
        lua_settable(L, -3);

        lua_settable(L, -3); /* Push one parameter into the results table. */
    }

    return 1;
}

/*
* \brief New setvar function for Lua API. Users can put back
* data in modsecurity core via new variables
*
* \param L Pointer to Lua state
*
* \retval -1 On failure
* \retval 0 On Collection failure
* \retval 1 On Success
*/
static int l_setvar(lua_State *L) {
    modsec_rec *msr = NULL;
    msre_rule *rule = NULL;
    const char *var_value = NULL;
    const char *var_name = NULL;
    int nargs = lua_gettop(L);
    char *chr = NULL;

    lua_getglobal(L, "__msr");
    msr = (modsec_rec *)lua_topointer(L, -1);

    lua_getglobal(L, "__rule");
    rule = (msre_rule *)lua_topointer(L, -1);

    if(nargs != 2)  {
        msr_log(msr, 8, "m.setvar: Failed m.setvar funtion must has 2 arguments");
        return -1;
    }
    var_value = luaL_checkstring (L, 2);
    var_name = luaL_checkstring (L, 1);

    lua_pop(L,2);

    if(var_value == NULL || var_name == NULL)
        return -1;

    chr = strchr((char *)var_name,0x2e);

    if(chr == NULL) {
        msr_log(msr, 8, "m.setvar: Must specify a collection using dot character - ie m.setvar(tx.myvar,mydata)");
        return -1;
    }

    return msre_action_setvar_execute(msr,msr->msc_rule_mptmp,rule,(char *)var_name,(char *)var_value);
}

static const struct luaL_Reg mylib[] = {
    { "log", l_log },
    { "getvar", l_getvar },
    { "getvars", l_getvars },
    { "setvar", l_setvar },
    { NULL, NULL }
};

/**
 *
 */
int lua_execute(msc_script *script, char *param, modsec_rec *msr, msre_rule *rule, char **error_msg) {
    apr_time_t time_before;
    lua_State *L = NULL;
    int rc = 0;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (msr->txcfg->debuglog_level >= 8) {
        msr_log(msr, 8, "Lua: Executing script: %s", script->name);
    }

    time_before = apr_time_now();

#ifdef CACHE_LUA
    L = msr->L;
    rc = lua_gettop(L);
    if(rc)
        lua_pop(L, rc);
#else
    /* Create new state. */
#if LUA_VERSION_NUM > 501
    L = luaL_newstate();
#else
    L = lua_open();
#endif
    luaL_openlibs(L);
#endif

    if(L == NULL)
        return -1;

    /* Associate msr with the state. */
    lua_pushlightuserdata(L, (void *)msr);
    lua_setglobal(L, "__msr");

    /* Associate rule with the state. */
    if (rule != NULL) {
        lua_pushlightuserdata(L, (void *)rule);
        lua_setglobal(L, "__rule");
    }

    /* Register functions. */
#if LUA_VERSION_NUM > 501
    luaL_setfuncs(L,mylib,0);
    lua_setglobal(L,"m");
#else
    luaL_register(L, "m", mylib);
#endif

    rc = lua_restore(L, script);
    if (rc) {
        *error_msg = apr_psprintf(msr->mp, "Lua: Failed to restore script with %i.", rc);
        return -1;
    }

    /* Execute the chunk so that the functions are defined. */
    lua_pcall(L, 0, 0, 0);

    /* Execute main() */
    lua_getglobal(L, "main");

    /* Put the parameter on the stack. */
    if (param != NULL) {
        lua_pushlstring(L, param, strlen(param));
    }

    if (lua_pcall(L, ((param != NULL) ? 1 : 0), 1, 0)) {
        *error_msg = apr_psprintf(msr->mp, "Lua: Script execution failed: %s", lua_tostring(L, -1));

        if (msr->txcfg->debuglog_level >= 8) {
            msr_log(msr, 8, "Lua: Script execution failed: %s", lua_tostring(L, -1));
        }

        return -1;
    }

    /* Get the response from the script. */
    *error_msg = (char *)lua_tostring(L, -1);
    if (*error_msg != NULL) {
        *error_msg = apr_pstrdup(msr->mp, *error_msg);
    }

    /* Destroy state. */
    lua_pop(L, 1);
#ifndef CACHE_LUA
    lua_close(L);
#endif

    /* Returns status code to caller. */
    if (msr->txcfg->debuglog_level >= 8) {
        msr_log(msr, 8, "Lua: Script completed in %" APR_TIME_T_FMT " usec, returning: %s.",
            (apr_time_now() - time_before), *error_msg);
    }

    return ((*error_msg != NULL) ? RULE_MATCH : RULE_NO_MATCH);
}

#endif /* WITH_LUA */
