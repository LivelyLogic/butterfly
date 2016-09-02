//
//  BFLua.h
//
//  Copyright (c) 2011-2016 Ripeware, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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

void * bf_lua_getoptionaluserdata(lua_State * L, int narg, const char * tname);

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
