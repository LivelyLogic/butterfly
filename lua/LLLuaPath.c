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
    LLRect rect;
    double radius;
    LLPathRef path;
    
    lua_getfield(L, 1, "left");
    rect.left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "bottom");
    rect.bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "right");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "top");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    radius = lua_tonumber(L, 2);
    
    path = LLPathCreate();
    if (radius > 0) {
        LLPathAddRoundedRect(path, rect, radius);
    } else {
        LLPathAddRect(path, rect);
    }
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addRect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLRect rect;
    double radius;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "left");
    rect.left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "bottom");
    rect.bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "right");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "top");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    radius = lua_tonumber(L, 3);
    
    if (radius > 0) {
        LLPathAddRoundedRect(path, rect, radius);
    } else {
        LLPathAddRect(path, rect);
    }
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int oval(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLRect rect;
    LLPathRef path;
    
    lua_getfield(L, 1, "left");
    rect.left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "bottom");
    rect.bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "right");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "top");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathAddOvalInRect(path, rect);
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addOval(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLRect rect;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "left");
    rect.left = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "bottom");
    rect.bottom = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "right");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "top");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddOvalInRect(path, rect);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int point(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPoint point;
    LLPathRef path;
    
    lua_getfield(L, 1, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, point);
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int line(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPoint point1, point2;
    LLPathRef path;
    
    lua_getfield(L, 1, "x1");
    point1.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y1");
    point1.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "x2");
    point2.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y2");
    point2.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, point1);
    LLPathAddLineToPoint(path, point2);
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addLine(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddLineToPoint(path, point);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addLineXY(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    point.x = lua_tonumber(L, 2);
    point.y = lua_tonumber(L, 3);
    
    LLPathAddLineToPoint(path, point);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int arc(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPoint point, centerPoint;
    double angle;
    LLPathRef path;
    
    lua_getfield(L, 1, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "cx");
    centerPoint.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "cy");
    centerPoint.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "angle");
    angle = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = LLPathCreate();
    LLPathMoveToPoint(path, point);
    LLPathAddArc(path, centerPoint, angle);
    
    pushNewUserdata(L, path, LLPathClassName);
    LLRelease(path);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addArc(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLPoint centerPoint;
    double angle;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "cx");
    centerPoint.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "cy");
    centerPoint.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "angle");
    angle = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathAddArc(path, centerPoint, angle);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addSubpath(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLPathRef path = *(LLPathRef *)luaL_checkudata(L, 1, LLPathClassName);
    LLPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    LLPathMoveToPoint(path, point);
    
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
