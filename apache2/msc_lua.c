
#include "msc_lua.h"


#ifdef WITH_LUA

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

    /* Do we have more chunks to return? */
    if (dumpr->index == dumpr->script->parts->nelts) {
        return NULL;
    }

    /* Get one chunk. */
    msc_script_part *part = ((msc_script_part **)dumpr->script->parts->elts)[dumpr->index];
    *size = part->len;
    
    dumpr->index++;

    return part->data;
}

/**
 *
 */
static int dump_writer(lua_State *L, const void* data, size_t len, void* user_data) {
    msc_lua_dumpw_t *dump = (msc_lua_dumpw_t *)user_data;

    /* Allocate new part, copy the data into it. */
    msc_script_part *part = apr_palloc(dump->pool, sizeof(msc_script_part));
    part->data = apr_palloc(dump->pool, len);
    part->len = len;
    memcpy((void *)part->data, data, len);

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

    return lua_load(L, dump_reader, &dumpr, script->name);
}

/**
 *
 */
char *lua_compile(msc_script **script, const char *filename, apr_pool_t *pool) {
    lua_State *L = NULL;
    msc_lua_dumpw_t dump;

    /* Initialise state. */
    L = lua_open();
    luaL_openlibs(L);

    /* Find script. */
    if (luaL_loadfile(L, filename)) {
        return apr_psprintf(pool, "ModSecurity: Failed to open script %s: %s",
            filename, lua_tostring(L, -1));
    }

    /* Compile script. */
    lua_pcall(L, 0, 0, 0);

    /* Find the execution entry point. */
    lua_getglobal(L, "main");

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
        msr_log(msr, level, text);
    }

    return 0;
}

/**
 *
 */
static int l_getvar(lua_State *L) {
    const char *varname = NULL;
    modsec_rec *msr = NULL;
    char *my_error_msg = NULL;

    /* Retrieve parameters. */
    varname = luaL_checkstring(L, 1);

    /* Retrieve msr. */
    lua_getglobal(L, "__msr");
    msr = (modsec_rec *)lua_topointer(L, -1);

    /* Resolve variable $varname. */
    msre_var *var = msre_create_var_ex(msr->msc_rule_mptmp, msr->modsecurity->msre,
        varname, NULL, msr, &my_error_msg);

    if (var == NULL) {
        msr_log(msr, 1, "SecRuleScript: Failed to resolve variable: %s", varname);
        return 0;
    } else {
        msre_var *vx = generate_single_var(msr, var, NULL, msr->msc_rule_mptmp);
        if (vx != NULL) {
            /* Transform the variable if a list of transformation
             * functions has been supplied.
            */
            if (lua_istable(L, 2)) { /* Is the second parameter an array? */
                int i, n = lua_objlen(L, 2);

                /* Make a copy so that we don't ruin the original value. */
                vx->value = apr_pstrmemdup(msr->msc_rule_mptmp, vx->value, vx->value_len);

                for(i = 1; i <= n; i++) {
                    msre_tfn_metadata *tfn = NULL;
                    char *name = NULL;
                    int rc = 0;

                    lua_rawgeti(L, 2, i);
                    name = (char *)luaL_checkstring(L, -1);
                    tfn = msre_engine_tfn_resolve(msr->modsecurity->msre, name);
                    if (tfn == NULL) {
                        msr_log(msr, 1, "SecRuleScript: Invalid transformation function in getvar() call: %s", name);
                        return 0;
                    }

                    rc = tfn->execute(msr->msc_rule_mptmp, vx->value, vx->value_len, &vx->value, &vx->value_len);

                    if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "T (%d) %s: \"%s\"", rc, tfn->name,
                        log_escape_nq_ex(msr->msc_rule_mptmp, vx->value, vx->value_len));
                }
                }
            } else
            if (lua_isstring(L, 2)) { /* The second parameter may be a simple string? */
                msre_tfn_metadata *tfn = NULL;
                char *name = NULL;
                int rc = 0;

                /* Make a copy so that we don't ruin the original value. */
                vx->value = apr_pstrmemdup(msr->msc_rule_mptmp, vx->value, vx->value_len);

                name = (char *)luaL_checkstring(L, 2);
                tfn = msre_engine_tfn_resolve(msr->modsecurity->msre, name);
                if (tfn == NULL) {
                    msr_log(msr, 1, "SecRuleScript: Invalid transformation function in getvar() call: %s", name);
                    return 0;
                }

                rc = tfn->execute(msr->msc_rule_mptmp, vx->value, vx->value_len, &vx->value, &vx->value_len);

                if (msr->txcfg->debuglog_level >= 9) {
                    msr_log(msr, 9, "T (%d) %s: \"%s\"", rc, tfn->name,
                        log_escape_nq_ex(msr->msc_rule_mptmp, vx->value, vx->value_len));
                }
            }

            lua_pushlstring(L, vx->value, vx->value_len);

            return 1;
        }
    }

    return 0;
}

static const struct luaL_Reg mylib[] = {
    { "log", l_log },
    { "getvar", l_getvar },
    { NULL, NULL }
};

/**
 *
 */
int lua_execute(msre_rule *rule, modsec_rec *msr, char **error_msg) {
    apr_time_t time_before;
    lua_State *L = NULL;
    int rc;

    if (error_msg == NULL) return -1;
    *error_msg = NULL;

    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Lua: Executing script: %s", rule->script->name);
    }

    time_before = apr_time_now();

    /* Create new state. */
    L = lua_open();

    luaL_openlibs(L);

    /* Associate msr with the state. */
    lua_pushlightuserdata(L, (void *)msr);
    lua_setglobal(L, "__msr");

    /* Register functions. */
    luaL_register(L, "m", mylib);

    rc = lua_restore(L, rule->script);
    if (rc) {
        *error_msg = apr_psprintf(msr->mp, "Lua: Failed to restore script with %i.", rc);
        return -1;
    }

    /* Execute script. */
    if (lua_pcall(L, 0, 1, 0)) {
        *error_msg = apr_psprintf(msr->mp, "Lua: Script execution failed: %s", lua_tostring(L, -1));
        return -1;
    }

    // TODO Who will need to free msg?
    *error_msg = (char *)lua_tostring(L, -1);

    /* Destroy state. */
    lua_pop(L, 1);
    lua_close(L);

    /* Returns status code to caller. */
    if (msr->txcfg->debuglog_level >= 9) {
        msr_log(msr, 9, "Lua: Script completed in %" APR_TIME_T_FMT " usec, returning: %s.",
            (apr_time_now() - time_before), *error_msg);
    }

    return ((*error_msg != NULL) ? RULE_MATCH : RULE_NO_MATCH);
}

#endif
