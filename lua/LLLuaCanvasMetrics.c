//
//  LLLuaCanvasMetrics.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLLuaCanvasMetrics.h"

#include "LLLua.h"

#include "LLCanvasMetrics.h"

static int rect(lua_State * L);
static int backingScale(lua_State * L);
static int pointScale(lua_State * L);

const LLLuaClass luaCanvasMetricsClass = {
    .metatableName = LLCanvasMetricsClassName,
    .methods = {
        {"rect", rect},
        {"backingScale", backingScale},
        {"pointScale", pointScale},
        {NULL, NULL}
    }
};

int initLuaCanvasMetricsLibrary(lua_State * L) {
    initLuaModule(L, NULL, &luaCanvasMetricsClass);
    return 0;
}

static int rect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasMetricsRef canvasMetrics = *(LLCanvasMetricsRef *)luaL_checkudata(L, 1, LLCanvasMetricsClassName);
    CGRect bounds = LLCanvasMetricsGetBoundsRect(canvasMetrics);

    lua_newtable(L);
    lua_pushnumber(L, bounds.origin.x);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, bounds.origin.y);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, bounds.origin.x + bounds.size.width);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, bounds.origin.y + bounds.size.height);
    lua_setfield(L, -2, "top");

    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int backingScale(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasMetricsRef canvasMetrics = *(LLCanvasMetricsRef *)luaL_checkudata(L, 1, LLCanvasMetricsClassName);
    double backingScale;

    backingScale = LLCanvasMetricsGetBackingScale(canvasMetrics);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushnumber(L, backingScale);
    return 1;
}

static int pointScale(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasMetricsRef canvasMetrics = *(LLCanvasMetricsRef *)luaL_checkudata(L, 1, LLCanvasMetricsClassName);
    double pointScale;

    pointScale = LLCanvasMetricsGetPointScale(canvasMetrics);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushnumber(L, pointScale);
    return 1;
}
