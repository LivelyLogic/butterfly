//
//  BFLuaPath.c
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

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

static int new(lua_State * L);

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
        {"new", new},
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

static int new(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPathRef path;
    
    path = BFPathCreate();
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
