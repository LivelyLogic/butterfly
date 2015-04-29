//
//  LLLuaPath.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLLuaPath.h"

#include "LLLua.h"

#include "LLPath.h"

static int rect(lua_State * L);
static int oval(lua_State * L);
static int point(lua_State * L);
static int line(lua_State * L);
static int arc(lua_State * L);

static int addRect(lua_State * L);
static int addOval(lua_State * L);
static int addSubpath(lua_State * L);
static int addLine(lua_State * L);
static int addLineXY(lua_State * L);
static int addArc(lua_State * L);
static int closeSubpath(lua_State * L);

static const LLLuaClass luaPathLibrary = {
    .libraryName = "Path",
    .methods = {
        {"rect", rect},
        {"oval", oval},
        {"point", point},
        {"line", line},
        {"arc", arc},
        {NULL, NULL}
    }
};

static const LLLuaClass luaPathClass = {
    .metatableName = LLPathClassName,
    .methods = {
        {"addRect", addRect},
        {"addOval", addOval},
        {"addSubpath", addSubpath},
        {"addLine", addLine},
        {"addLineXY", addLineXY},
        {"addArc", addArc},
        {"closeSubpath", closeSubpath},
        {NULL, NULL}
    }
};

// Global functions

int initLuaPathLibrary(lua_State * L) {
    initLuaModule(L, &luaPathLibrary, &luaPathClass);
    return 0;
}

// Local functions

static int rect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double left, bottom, right, top, radius;
    LLPathRef path;
    
    lua_getfield(L, 1, "left");
    left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "bottom");
    bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "right");
    right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "top");
    top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    radius = lua_tonumber(L, 2);
    
    path = LLPathCreate();
    if (radius > 0) {
        LLPathAddRoundedRect(path, CGRectMake(left, bottom, right-left, top-bottom), radius);
    } else {
        LLPathAddRect(path, CGRectMake(left, bottom, right-left, top-bottom));
    }
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addRect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double left, bottom, right, top, radius;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "left");
    left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "bottom");
    bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "right");
    right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "top");
    top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    radius = lua_tonumber(L, 3);
    
    if (radius > 0) {
        LLPathAddRoundedRect(path, CGRectMake(left, bottom, right-left, top-bottom), radius);
    } else {
        LLPathAddRect(path, CGRectMake(left, bottom, right-left, top-bottom));
    }
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int oval(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double left, bottom, right, top;
    LLPathRef path;
    
    lua_getfield(L, 1, "left");
    left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "bottom");
    bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "right");
    right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "top");
    top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathAddOvalInRect(path, CGRectMake(left, bottom, right-left, top-bottom));
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addOval(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double left, bottom, right, top;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "left");
    left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "bottom");
    bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "right");
    right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "top");
    top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddOvalInRect(path, CGRectMake(left, bottom, right-left, top-bottom));
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int point(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double x, y;
    LLPathRef path;
    
    lua_getfield(L, 1, "x");
    x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, CGPointMake(x, y));
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int line(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double x1, y1, x2, y2;
    LLPathRef path;
    
    lua_getfield(L, 1, "x1");
    x1 = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y1");
    y1 = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "x2");
    x2 = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y2");
    y2 = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, CGPointMake(x1, y1));
    LLPathAddLineToPoint(path, CGPointMake(x2, y2));
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addLine(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double x, y;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddLineToPoint(path, CGPointMake(x, y));
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addLineXY(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double x, y;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    x = lua_tonumber(L, 2);
    y = lua_tonumber(L, 3);
    
    LLPathAddLineToPoint(path, CGPointMake(x, y));
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int arc(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double x, y, cx, cy, angle;
    LLPathRef path;
    
    lua_getfield(L, 1, "x");
    x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "cx");
    cx = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "cy");
    cy = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "angle");
    angle = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, CGPointMake(x, y));
    LLPathAddArc(path, CGPointMake(cx, cy), angle);
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addArc(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double cx, cy, angle;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "cx");
    cx = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "cy");
    cy = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "angle");
    angle = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddArc(path, CGPointMake(cx, cy), angle);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addSubpath(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    double x, y;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathMoveToPoint(path, CGPointMake(x, y));
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int closeSubpath(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    LLPathCloseSubpath(path);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}
