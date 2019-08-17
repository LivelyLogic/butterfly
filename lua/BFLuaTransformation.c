//
//  BFLuaTransformation.c
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

static int identity(lua_State * L);
static int rotate(lua_State * L);
static int translate(lua_State * L);
static int scale(lua_State * L);
static int invert(lua_State * L);
static int concat(lua_State * L);
static int transformPoint(lua_State * L);
static int transformRect(lua_State * L);
static int getComponents(lua_State * L);

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
        {"getComponents", getComponents},
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
    
    lua_newtable(L);
    lua_pushnumber(L, rect.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, rect.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, rect.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, rect.top);
    lua_setfield(L, -2, "top");
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getComponents(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFTransformationRef transformation = *(BFTransformationRef *)luaL_checkudata(L, 1, BFTransformationClassName);
    
    luaL_argcheck(L, transformation, 1, "Transformation expected");
    
    BFTransformationComponents components = BFTransformationGetComponents(transformation);
    
    lua_newtable(L);
    lua_pushnumber(L, components.a);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, components.b);
    lua_rawseti(L, -2, 2);
    lua_pushnumber(L, components.c);
    lua_rawseti(L, -2, 3);
    lua_pushnumber(L, components.d);
    lua_rawseti(L, -2, 4);
    lua_pushnumber(L, components.tx);
    lua_rawseti(L, -2, 5);
    lua_pushnumber(L, components.ty);
    lua_rawseti(L, -2, 6);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
