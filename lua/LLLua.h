//
//  LLLua.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_LUA_H__
#define __LL_LUA_H__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define LL_LUA_DEBUG_STACK 1

typedef struct LLLuaClass {
    char * metatableName;
    char * libraryName;
    const struct LLLuaClass * superClass;
    struct luaL_Reg methods [];
} LLLuaClass;

void bf_lua_loadmodule(lua_State * L, const LLLuaClass * luaLibrary, const LLLuaClass * luaClass);
void bf_lua_loadclass(lua_State * L, const LLLuaClass * luaClass);

void * tryLuaUserdata(lua_State * L, int narg, const char * tname);
void * getLuaNilOrUserdata(lua_State * L, int narg, const char * tname);
int l_pcall(lua_State * L, int nargs, int nresults);
void l_pcallNoResults(lua_State * L, int nargs);

#if LL_LUA_DEBUG_STACK
#define LL_LUA_DEBUG_STACK_BEGIN(L) int _top1 = lua_gettop(L); int _top2;
#define LL_LUA_DEBUG_STACK_ENDR(L, ret) _top2 = lua_gettop(L) - ret; \
    if (_top1 != _top2) printf("Stack size changed from %d to %d in function %s\n", _top1, _top2, __FUNCTION__);
#define LL_LUA_DEBUG_STACK_END(L) LL_LUA_DEBUG_STACK_ENDR(L, 0)
#else
#define LL_LUA_DEBUG_STACK_BEGIN(L)
#define LL_LUA_DEBUG_STACK_ENDR(L, ret)
#define LL_LUA_DEBUG_STACK_END(L)
#endif

#endif /* __LL_LUA_H__ */
