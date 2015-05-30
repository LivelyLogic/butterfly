//
//  BFLuaTransformation.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

static int identity(lua_State * L);
static int rotate(lua_State * L);
static int translate(lua_State * L);
static int scale(lua_State * L);
static int invert(lua_State * L);
static int concat(lua_State * L);
static int transformPoint(lua_State * L);
static int transformRect(lua_State * L);

static const BFLuaClass luaTransformationLibrary = {
    .libraryName = "Transformation",
    .methods = {
        {"identity", identity},
        {NULL, NULL}
    }
};

static const BFLuaClass luaTransformationClass = {
    .metatableName = BFTransformationClassName,
    .methods = {
        {"rotate", rotate},
        {"translate", translate},
        {"scale", scale},
        {"invert", invert},
        {"concat", concat},
        {"transformPoint", transformPoint},
        {"transformRect", transformRect},
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadTransformation(lua_State * L) {
    bf_lua_loadmodule(L, &luaTransformationLibrary, &luaTransformationClass);
    return 0;
}


// Local functions

static int identity(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = BFTransformationCreate();
    bf_lua_push(L, transformation, BFTransformationClassName);
    BFRelease(transformation);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int rotate(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    double angle = lua_tonumber(L, 2);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    BFTransformationRotate(transformation, angle);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int translate(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    double dx = lua_tonumber(L, 2);
    double dy = lua_tonumber(L, 3);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    BFTransformationTranslate(transformation, dx, dy);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int scale(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    double ratio = lua_tonumber(L, 2);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    BFTransformationScale(transformation, ratio);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int invert(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    BFTransformationInvert(transformation);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int concat(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation1 = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    BFTransformationRef transformation2 = *(BFTransformationRef *)luaL_checkudata(L, 2, BFTransformationClassName);
    
    luaL_argcheck(L, transformation1, 1, "Transformation expected");
    luaL_argcheck(L, transformation1, 2, "Transformation expected");
    BFTransformationConcat(transformation1, transformation2);
    
    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int transformPoint(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    BFPoint point = { .x = x, .y = y };
    point = BFTransformationTransformPoint(transformation, point);
    lua_pushnumber(L, point.x);
    lua_pushnumber(L, point.y);

    BF_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int transformRect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    BFRect rect;
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    
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
    
    rect = BFTransformationTransformRect(transformation, rect);
    
    lua_getglobal(L, "Rect");
    lua_getfield(L, -1, "new");
    lua_insert(L, -2);
    
    lua_newtable(L);
    lua_pushnumber(L, rect.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, rect.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, rect.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, rect.top);
    lua_setfield(L, -2, "top");
    
    l_pcall(L, 2, 1);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
