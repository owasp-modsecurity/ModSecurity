/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */


#include "src/engine/lua.h"

#include <stdio.h>
#include <string.h>

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

#include "modsecurity/modsecurity.h"
#include "src/utils/string.h"
#include "src/macro_expansion.h"
#include "modsecurity/transaction.h"
#include "modsecurity/collection/variable.h"
#include "src/variables/variable.h"
#include "src/variables/highest_severity.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {
namespace engine {


bool Lua::isCompatible(std::string script, Lua *l, std::string *error) {
#ifdef WITH_LUA
    std::string lua(".lua");
    std::string err;

    if (!(script.size() >= lua.size() &&
        script.compare(script.size() - lua.size(), lua.size(), lua) == 0)) {
        error->assign("Expecting a Lua script: " + script);
        return false;
    }

    if (l->load(script, &err) == false) {
        error->assign("Problems load script: " + err);
        return false;
    }

    return true;
#else
    error->assign("Lua support was not enabled.");
    return false;
#endif
}


bool Lua::load(std::string script, std::string *err) {
#ifdef WITH_LUA
    lua_State *L = NULL;
    L = luaL_newstate();
    luaL_openlibs(L);

    m_scriptName = script;
    if (luaL_loadfile(L, script.c_str())) {
        const char *luaerr = lua_tostring(L, -1);
        err->assign("Failed to compile script '" + script + "");
        if (luaerr) {
            err->append(": " + std::string(luaerr));
        }
        err->append(".");
        lua_close(L);

        return false;
    }

#ifdef WITH_LUA_5_2
    if (lua_dump(L, Lua::blob_keeper, reinterpret_cast<void *>(&m_blob))) {
#else
    if (lua_dump(L, Lua::blob_keeper, reinterpret_cast<void *>(&m_blob), 0)) {
#endif
        const char *luaerr = lua_tostring(L, -1);
        err->assign("Failed to compile script '" + script + "");
        if (luaerr) {
            err->append(": " + std::string(luaerr));
        }
        err->append(".");
        lua_close(L);

        return false;
    }

    lua_close(L);
    return true;
#else
    err->assign("Lua support was not enabled.");
    return false;
#endif
}

#ifdef WITH_LUA
int Lua::blob_keeper(lua_State *L, const void *p, size_t sz, void *ud) {
    LuaScriptBlob *lsb = static_cast<LuaScriptBlob *>(ud);
    lsb->write(p, sz);
    return 0;
}


const char *Lua::blob_reader(lua_State *L, void *ud, size_t *size) {
    LuaScriptBlob *lsb = static_cast<LuaScriptBlob *>(ud);
    const char *data = lsb->read(size);
    return data;
}
#endif

int Lua::run(Transaction *t) {
#ifdef WITH_LUA
    std::string luaRet;
    const char *a = NULL;
    int ret = true;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_newmetatable(L, "luaL_msc");
    lua_newtable(L);

    lua_pushlightuserdata(L, reinterpret_cast<void *>(t));
    lua_setglobal(L, "__transaction");

    luaL_setfuncs(L, mscLuaLib, 0);
    lua_setglobal(L, "m");

    int rc = lua_load(L, Lua::blob_reader, &m_blob, m_scriptName.c_str(),
        NULL);
    if (rc != LUA_OK) {
        std::string e;
        e.assign("Failed to execute lua script: " + m_scriptName + ". ");
        switch (rc) {
            case LUA_ERRSYNTAX:
                e.assign("Syntax error. ");
                break;
            case LUA_ERRMEM:
                e.assign("Memory error. ");
                break;
            case LUA_ERRGCMM:
                e.assign("Garbage Collector error. ");
                break;
        }
        e.append(lua_tostring(L, -1));
#ifndef NO_LOGS
        t->debug(2, e);
#endif
        ret = false;
        goto err;
    }

    if (lua_pcall(L, 0, 0, 0)) {
        std::string e;
        const char *luaerr = lua_tostring(L, -1);
        e.assign("Failed to execute lua script: " + m_scriptName \
            + " (before main)");
        if (luaerr != NULL) {
            e.append(" - ");
            e.append(luaerr);
        }
#ifndef NO_LOGS
        t->debug(2, e);
#endif
        ret = false;
        goto err;
    }

    lua_setglobal(L, "modsec");

    lua_getglobal(L, "main");
    if (lua_pcall(L, 0, 1, 0)) {
        std::string e;
        const char *luaerr = lua_tostring(L, -1);
        e.assign("Failed to execute lua script: " + m_scriptName + " (main)");
        if (luaerr != NULL) {
            e.append(" - ");
            e.append(luaerr);
        }
#ifndef NO_LOGS
        t->debug(2, e);
#endif
        ret = false;
        goto err;
    }

    a = reinterpret_cast<const char *>(lua_tostring(L, -1));
    if (a != NULL) {
        luaRet.assign(a);
    }
#ifndef NO_LOGS
    t->debug(9, "Returning from lua script: " + luaRet);
#endif

    if (luaRet.size() == 0) {
        ret = false;
    }


err:
    lua_pop(L, 1);
    lua_close(L);

    return ret;
#else
#ifndef NO_LOGS
    t->debug(9, "Lua support was not enabled.");
#endif
    return false;
#endif
}


#ifdef WITH_LUA
int Lua::log(lua_State *L) {
    const Transaction *t = NULL;
    const char *text;
    int level;

    /* Retrieve parameters. */
    level = luaL_checknumber(L, 1);
    text = luaL_checkstring(L, 2);

    /* Retrieve msr. */
    lua_getglobal(L, "__transaction");
    t = reinterpret_cast<const Transaction *>(lua_topointer(L, -1));

    /* Log message. */
    if (t != NULL) {
#ifndef NO_LOGS
        t->debug(level, text);
#endif
    }

    return 0;
}


int Lua::getvar(lua_State *L) {
    const char *varname = NULL;
    Transaction *t = NULL;
    void *z = NULL;

    /* Retrieve parameters. */
    varname = reinterpret_cast<const char *>(luaL_checkstring(L, 1));

    lua_getglobal(L, "__transaction");
    z = const_cast<void *>(lua_topointer(L, -1));
    t = reinterpret_cast<Transaction *>(z);

    std::string var = Variables::Variable::stringMatchResolve(t, varname);
    var = applyTransformations(L, t, 2, var);

    if (var.size() == 0) {
        lua_pushnil(L);
        return 0;
    }

    lua_pushlstring(L, var.c_str(), var.size());

    return 1;
}


int Lua::getvars(lua_State *L) {
    const char *varname = NULL;
    Transaction *t = NULL;
    void *z = NULL;
    std::vector<const collection::Variable *> l;
    int idx = 1;

    /* Retrieve parameters. */
    varname = reinterpret_cast<const char *>(luaL_checkstring(L, 1));

    lua_getglobal(L, "__transaction");
    z = const_cast<void *>(lua_topointer(L, -1));
    t = reinterpret_cast<Transaction *>(z);

    Variables::Variable::stringMatchResolveMulti(t, varname, &l);

    lua_newtable(L);
    for (auto i : l) {
        lua_pushnumber(L, idx);
        lua_newtable(L);

        lua_pushstring(L, "name");
        lua_pushlstring(L, i->m_key.c_str(), i->m_key.size());
        lua_settable(L, -3);

        lua_pushstring(L, "value");
        lua_pushlstring(L, i->m_value.c_str(), i->m_value.size());
        lua_settable(L, -3);

        lua_settable(L, -3);
        idx++;
    }

    for (const collection::Variable * i : l) {
        delete i;
    }

    return 1;
}


int Lua::setvar(lua_State *L) {
    Transaction *t = NULL;
    const char *var_value = NULL;
    const char *var_name = NULL;
    std::string vname;
    std::string collection;
    std::string variableName;
    int nargs = lua_gettop(L);
    char *chr = NULL;
    size_t pos;
    void *z = NULL;

    lua_getglobal(L, "__transaction");
    z = const_cast<void *>(lua_topointer(L, -1));
    t = reinterpret_cast<Transaction *>(z);


    if (nargs != 2) {
#ifndef NO_LOGS
        t->debug(8, "m.setvar: Failed m.setvar funtion must has 2 arguments");
#endif
        return -1;
    }
    var_value = luaL_checkstring(L, 2);
    var_name = luaL_checkstring(L, 1);

    lua_pop(L, 2);

    if (var_value == NULL || var_name == NULL) {
        return -1;
    }

    vname.assign(var_name);
    pos = vname.find(".");
    if (pos != std::string::npos) {
        collection = std::string(vname, 0, pos);
        collection = utils::string::toupper(collection);
        variableName = std::string(vname, pos + 1,
            std::string::npos);

    } else {
#ifndef NO_LOGS
        t->debug(8, "m.setvar: Must specify a collection using dot character" \
            " - ie m.setvar(tx.myvar,mydata)");
#endif
        return -1;
    }

    t->m_collections.storeOrUpdateFirst(collection,
        variableName, var_value);

    return 0;
}


std::string Lua::applyTransformations(lua_State *L, Transaction *t,
    int idx, std::string var) {
    std::string newVar = var;

    if (lua_isuserdata(L, idx) || lua_isnoneornil(L, idx)) {
        return var;
    }

    if (lua_istable(L, idx)) {
        const char *name = NULL;
        int i, n = lua_rawlen(L, idx);

        for (i = 1; i <= n; i++) {
            lua_rawgeti(L, idx, i);
            name = reinterpret_cast<const char *>(luaL_checkstring(L, -1));

            /* A "none" means start over */
            if (strcmp("none", name) == 0) {
                newVar = var;
                continue;
            }

            actions::transformations::Transformation *tfn = \
                actions::transformations::Transformation::instantiate(
                    "t:" + std::string(name));
            // FIXME: transformation is not yet returning null.
            if (tfn) {
                newVar = tfn->evaluate(newVar, t);
            } else {
#ifndef NO_LOGS
                t->debug(1, "SecRuleScript: Invalid transformation function: " \
                    + std::string(name));
#endif
            }
            delete tfn;
        }

        return newVar;
    }

    if (lua_isstring(L, idx)) {
        const char *name = NULL;
        name = reinterpret_cast<const char *>(luaL_checkstring(L, idx));

        actions::transformations::Transformation *tfn = \
            actions::transformations::Transformation::instantiate(
                "t:" + std::string(name));

        // FIXME: transformation is not yet returning null.
        if (tfn) {
            newVar = tfn->evaluate(newVar, t);
            delete tfn;
        } else {
#ifndef NO_LOGS
            t->debug(1, "SecRuleScript: Invalid transformation function: " \
                + std::string(name));
#endif
        }
        return newVar;
    }
#ifndef NO_LOGS
    t->debug(8, "SecRuleScript: Transformation parameter must be a " \
        "transformation name or array of transformation names, but found " \
        "" + std::string(lua_typename(L, idx)) + " (type " \
        + std::to_string(lua_type(L, idx)) + ")");
#endif
    return newVar;
}
#endif

}  //  namespace engine
}  //  namespace modsecurity

