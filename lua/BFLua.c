//
//  BFLua.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFLua.h"

#include "butterfly.h"

static int retainLuaObject(lua_State * L);
static int releaseLuaObject(lua_State * L);

void bf_lua_loadmodule(lua_State * L, const BFLuaClass * luaLibrary, const BFLuaClass * luaClass) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    lua_pushvalue(L, 1);
    if (luaLibrary) {
        bf_lua_loadclass(L, luaLibrary);
    } else {
        lua_pushnil(L);
    }
    if (luaClass) {
        bf_lua_loadclass(L, luaClass);
    } else {
        lua_pushnil(L);
    }
    if (lua_isfunction(L, -3)) {
        lua_call(L, 2, 0);
    } else {
        lua_pop(L, 3);
    }
    BF_LUA_DEBUG_STACK_END(L);
}

void bf_lua_loadclass(lua_State * L, const BFLuaClass * luaClass) {
    if (luaClass->metatableName) {
        luaL_newmetatable(L, luaClass->metatableName);
    } else {
        assert(luaClass->libraryName);
        // TODO: can import some of luaL_register...
    }
    luaL_register(L, luaClass->libraryName, luaClass->methods);
    if (luaClass->superClass) {
        luaL_getmetatable(L, luaClass->superClass->metatableName);
        lua_setmetatable(L, -2);
    }
    lua_pushcfunction(L, &retainLuaObject);
    lua_setfield(L, -2, "_ref");
    lua_pushcfunction(L, &releaseLuaObject);
    lua_setfield(L, -2, "__gc");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
}

static int retainLuaObject(lua_State * L) {
    // TODO: check the userdata type here!
    BFBaseRef base = *(BFBaseRef *)lua_touserdata(L, 1);
    BFRetain(base);
    return 0;
}

static int releaseLuaObject(lua_State * L) {
    // TODO: check the userdata type here!
    BFBaseRef base = *(BFBaseRef *)lua_touserdata(L, 1);
    BFRelease(base);
    return 0;
}

void * tryLuaUserdata(lua_State * L, int narg, const char * tname) {
    void ** userdata = lua_touserdata(L, narg);
    if (userdata) {
        lua_pushvalue(L, narg);
        lua_getfield(L, LUA_REGISTRYINDEX, tname);
        lua_pushvalue(L, -2);
        int mtcount = 0;
        while (lua_getmetatable(L, -1)) {
            mtcount++;
            if (lua_rawequal(L, -1, -2 - mtcount)) {
                lua_pop(L, 3 + mtcount);
                return *userdata;
            }
        }
        lua_pop(L, 3 + mtcount);
    }
    return NULL;
}

void * getLuaNilOrUserdata(lua_State * L, int narg, const char * tname) {
    if (!lua_toboolean(L, narg)) {
        return NULL;
    }
    void * result = tryLuaUserdata(L, narg, tname);
    if (!result) {
        luaL_typerror(L, narg, tname);
    }
    return result;
}

void bf_lua_push(lua_State * L, void * data, const char * tname) {
    void ** userdata = (void **)lua_newuserdata(L, sizeof(void *));
    *userdata = data;
    if (tname) {
        luaL_getmetatable(L, tname);
        lua_pushvalue(L, -1);
        lua_setmetatable(L, -3);
        lua_getfield(L, -1, "_ref");
        if (lua_isfunction(L, -1)) {
            lua_pushvalue(L, -3);
            l_pcallNoResults(L, 1);
        } else {
            lua_pop(L, 1); /* _ref */
        }
        lua_pop(L, 1); /* metatable */
    }
}

int l_pcall(lua_State * L, int nargs, int nresults) {
    int top = lua_gettop(L) - nargs - 1;
    int error = 0;
    lua_getglobal(L, "warn");
    lua_insert(L, -2 - nargs); /* warn: skipping function, nargs */
    error = lua_pcall(L, nargs, nresults, -2 - nargs);
    if (error) {
        lua_pop(L, 1);
        nresults = 0;
    } else if (nresults == LUA_MULTRET) {
        nresults = lua_gettop(L) - top;
    }
    lua_remove(L, -1 - nresults); /* warn: skipping nresults */
    return error;
}

void l_pcallNoResults(lua_State * L, int nargs) {
    l_pcall(L, nargs, 0);
}
