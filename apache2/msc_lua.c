
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
int lua_restore(lua_State *L, msc_script *script) {
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

#endif
