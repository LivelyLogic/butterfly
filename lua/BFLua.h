//
//  BFLua.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_LUA_H__
#define __BF_LUA_H__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define BF_LUA_DEBUG_STACK 1

typedef struct BFLuaClass {
    char * metatableName;
    char * libraryName;
    const struct BFLuaClass * superClass;
    struct luaL_Reg methods [];
} BFLuaClass;

void bf_lua_loadmodule(lua_State * L, const BFLuaClass * luaLibrary, const BFLuaClass * luaClass);
void bf_lua_loadclass(lua_State * L, const BFLuaClass * luaClass);

void * tryLuaUserdata(lua_State * L, int narg, const char * tname);
void * getLuaNilOrUserdata(lua_State * L, int narg, const char * tname);
int l_pcall(lua_State * L, int nargs, int nresults);
void l_pcallNoResults(lua_State * L, int nargs);

#if BF_LUA_DEBUG_STACK
#define BF_LUA_DEBUG_STACK_BEGIN(L) int _top1 = lua_gettop(L); int _top2;
#define BF_LUA_DEBUG_STACK_ENDR(L, ret) _top2 = lua_gettop(L) - ret; \
    if (_top1 != _top2) printf("Stack size changed from %d to %d in function %s\n", _top1, _top2, __FUNCTION__);
#define BF_LUA_DEBUG_STACK_END(L) BF_LUA_DEBUG_STACK_ENDR(L, 0)
#else
#define BF_LUA_DEBUG_STACK_BEGIN(L)
#define BF_LUA_DEBUG_STACK_ENDR(L, ret)
#define BF_LUA_DEBUG_STACK_END(L)
#endif

#endif /* __BF_LUA_H__ */
