//
//  BFLuaPath.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "BFLua.h"

#include "types.h"

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

static const BFLuaClass luaPathLibrary = {
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

static const BFLuaClass luaPathClass = {
    .metatableName = BFPathClassName,
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

int bf_lua_loadPath(lua_State * L) {
    bf_lua_loadmodule(L, &luaPathLibrary, &luaPathClass);
    return 0;
}

// Local functions

static int rect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFRect rect;
    double radius;
    BFPathRef path;
    
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
    
    path = BFPathCreate();
    if (radius > 0) {
        BFPathAddRoundedRect(path, rect, radius);
    } else {
        BFPathAddRect(path, rect);
    }
    
    bf_lua_push(L, path, BFPathClassName);
    BFRelease(path);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addRect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFRect rect;
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
        BFPathAddRoundedRect(path, rect, radius);
    } else {
        BFPathAddRect(path, rect);
    }
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int oval(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFRect rect;
    BFPathRef path;
    
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
    
    path = BFPathCreate();
    BFPathAddOvalInRect(path, rect);
    
    bf_lua_push(L, path, BFPathClassName);
    BFRelease(path);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addOval(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFRect rect;
    
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
    
    BFPathAddOvalInRect(path, rect);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int point(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPoint point;
    BFPathRef path;
    
    lua_getfield(L, 1, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    path = BFPathCreate();
    BFPathMoveToPoint(path, point);
    
    bf_lua_push(L, path, BFPathClassName);
    BFRelease(path);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int line(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPoint point1, point2;
    BFPathRef path;
    
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
    
    path = BFPathCreate();
    BFPathMoveToPoint(path, point1);
    BFPathAddLineToPoint(path, point2);
    
    bf_lua_push(L, path, BFPathClassName);
    BFRelease(path);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addLine(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    BFPathAddLineToPoint(path, point);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addLineXY(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    point.x = lua_tonumber(L, 2);
    point.y = lua_tonumber(L, 3);
    
    BFPathAddLineToPoint(path, point);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int arc(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPoint point, centerPoint;
    double angle;
    BFPathRef path;
    
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
    
    path = BFPathCreate();
    BFPathMoveToPoint(path, point);
    BFPathAddArc(path, centerPoint, angle);
    
    bf_lua_push(L, path, BFPathClassName);
    BFRelease(path);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int addArc(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFPoint centerPoint;
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
    
    BFPathAddArc(path, centerPoint, angle);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int addSubpath(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    BFPoint point;
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    lua_getfield(L, 2, "x");
    point.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "y");
    point.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    BFPathMoveToPoint(path, point);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int closeSubpath(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path = *(BFPathRef *)luaL_checkudata(L, 1, BFPathClassName);
    
    luaL_argcheck(L, path, 1, "Path expected");
    
    BFPathCloseSubpath(path);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}
