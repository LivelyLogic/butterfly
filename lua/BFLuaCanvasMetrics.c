//
//  BFLuaCanvasMetrics.c
//
//  Copyright (c) 2011-2019 James Rodovich
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

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

static int rect(lua_State * L);
static int backingScale(lua_State * L);
static int pointScale(lua_State * L);

const BFLuaClass luaCanvasMetricsClass = {
    .metatableName = BFCanvasMetricsClassName,
    .methods = {
        {"rect", rect},
        {"backingScale", backingScale},
        {"pointScale", pointScale},
        {NULL, NULL}
    }
};

int bf_lua_loadCanvasMetrics(lua_State * L) {
    bf_lua_loadmodule(L, NULL, &luaCanvasMetricsClass);
    return 0;
}

static int rect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasMetricsRef canvasMetrics = *(BFCanvasMetricsRef *)luaL_checkudata(L, 1, BFCanvasMetricsClassName);
    BFRect bounds = BFCanvasMetricsGetBoundsRect(canvasMetrics);

    lua_newtable(L);
    lua_pushnumber(L, bounds.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, bounds.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, bounds.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, bounds.top);
    lua_setfield(L, -2, "top");

    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int backingScale(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasMetricsRef canvasMetrics = *(BFCanvasMetricsRef *)luaL_checkudata(L, 1, BFCanvasMetricsClassName);
    double backingScale;

    backingScale = BFCanvasMetricsGetBackingScale(canvasMetrics);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushnumber(L, backingScale);
    return 1;
}

static int pointScale(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasMetricsRef canvasMetrics = *(BFCanvasMetricsRef *)luaL_checkudata(L, 1, BFCanvasMetricsClassName);
    double pointScale;

    pointScale = BFCanvasMetricsGetPointScale(canvasMetrics);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushnumber(L, pointScale);
    return 1;
}
