//
//  LLLuaTransformation.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLLuaTransformation.h"

#include "LLLua.h"

#include "LLTransformation.h"

static int identity(lua_State * L);
static int rotate(lua_State * L);
static int translate(lua_State * L);
static int scale(lua_State * L);
static int invert(lua_State * L);
static int concat(lua_State * L);
static int transformPoint(lua_State * L);
static int transformRect(lua_State * L);

static const LLLuaClass luaTransformationLibrary = {
    .libraryName = "Transformation",
    .methods = {
        {"identity", identity},
        {NULL, NULL}
    }
};

static const LLLuaClass luaTransformationClass = {
    .metatableName = LLTransformationClassName,
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

int initLuaTransformationLibrary(lua_State * L) {
    initLuaModule(L, &luaTransformationLibrary, &luaTransformationClass);
    return 0;
}


// Local functions

static int identity(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = LLTransformationCreate();
    pushNewUserdata(L, transformation, LLTransformationClassName);
    LLRelease(transformation);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int rotate(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    double angle = lua_tonumber(L, 2);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    LLTransformationRotate(transformation, angle);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int translate(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    double dx = lua_tonumber(L, 2);
    double dy = lua_tonumber(L, 3);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    LLTransformationTranslate(transformation, dx, dy);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int scale(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    double ratio = lua_tonumber(L, 2);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    LLTransformationScale(transformation, ratio);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int invert(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    LLTransformationInvert(transformation);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int concat(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation1 = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    LLTransformationRef transformation2 = *(LLTransformationRef *)luaL_checkudata(L, 2, LLTransformationClassName);
    
    luaL_argcheck(L, transformation1, 1, "Transformation expected");
    luaL_argcheck(L, transformation1, 2, "Transformation expected");
    LLTransformationConcat(transformation1, transformation2);
    
    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int transformPoint(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    CGPoint point = CGPointMake(x, y);
    point = LLTransformationTransformPoint(transformation, point);
    lua_pushnumber(L, point.x);
    lua_pushnumber(L, point.y);

    LL_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int transformRect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLTransformationRef transformation = *(LLTransformationRef *)luaL_checkudata(L, 1, LLTransformationClassName);
    CGRect rect;
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    
    lua_getfield(L, 2, "left");
    rect.origin.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "bottom");
    rect.origin.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "right");
    rect.size.width = lua_tonumber(L, -1) - rect.origin.x;
    lua_pop(L, 1);
    lua_getfield(L, 2, "top");
    rect.size.height = lua_tonumber(L, -1) - rect.origin.y;
    lua_pop(L, 1);
    
    rect = LLTransformationTransformRect(transformation, rect);
    
    lua_getglobal(L, "Rect");
    lua_getfield(L, -1, "new");
    lua_insert(L, -2);
    
    lua_newtable(L);
    lua_pushnumber(L, rect.origin.x);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, rect.origin.y);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, rect.origin.x + rect.size.width);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, rect.origin.y + rect.size.height);
    lua_setfield(L, -2, "top");
    
    l_pcall(L, 2, 1);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
