//
//  LLLuaColor.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"
#include "LLLuaPaint.h"

#include "types.h"

static int rgba(lua_State * L);

static int archive(lua_State * L);

static int equals(lua_State * L);
static int tostring(lua_State * L);

static const LLLuaClass luaColorLibrary = {
    .libraryName = "Color",
    .methods = {
        {"rgba", rgba},
        {NULL, NULL}
    }
};

const LLLuaClass luaColorClass = {
    .metatableName = LLColorPaintClassName,
    .superClass = &luaPaintClass,
    .methods = {
        {"archive", archive},
        {"__eq", equals},
        {"__tostring", tostring},
        {NULL, NULL}
    }
};


// Global functions

int bf_lua_loadColor(lua_State * L) {
    bf_lua_loadclass(L, &luaPaintClass);
    lua_pop(L, 1);
    bf_lua_loadmodule(L, &luaColorLibrary, &luaColorClass);
    return 0;
}


// Local functions

static int rgba(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double red   = lua_tonumber(L, 1);
    double green = lua_tonumber(L, 2);
    double blue  = lua_tonumber(L, 3);
    double alpha = lua_tonumber(L, 4);
    LLColorPaintRef colorPaint;
    
    colorPaint = LLColorPaintCreate();
    LLColorPaintSetRGBA(colorPaint, red, green, blue, alpha);
    bf_lua_push(L, colorPaint, LLColorPaintClassName);
    LLRelease(colorPaint);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int archive(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLColorPaintRef colorPaint = *(LLColorPaintRef *)luaL_checkudata(L, 1, LLColorPaintClassName);
    double red = 0;
    double green = 0;
    double blue = 0;
    double alpha = 0;
    
    luaL_argcheck(L, colorPaint, 1, "Color expected");
    
    LLColorPaintGetRGBA(colorPaint, &red, &green, &blue, &alpha);
    
    lua_pushstring(L, "RGBA");
    
    lua_newtable(L);
    lua_pushnumber(L, red);
    lua_setfield(L, -2, "red");
    lua_pushnumber(L, green);
    lua_setfield(L, -2, "green");
    lua_pushnumber(L, blue);
    lua_setfield(L, -2, "blue");
    lua_pushnumber(L, alpha);
    lua_setfield(L, -2, "alpha");
    
    LL_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int equals(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLColorPaintRef colorPaint1 = *(LLColorPaintRef *)luaL_checkudata(L, 1, LLColorPaintClassName);
    LLColorPaintRef colorPaint2 = *(LLColorPaintRef *)luaL_checkudata(L, 2, LLColorPaintClassName);
    
    luaL_argcheck(L, colorPaint1, 1, "Color expected");
    luaL_argcheck(L, colorPaint2, 2, "Color expected");
    
    bool result = LLColorPaintEquals(colorPaint1, colorPaint2);
    
    lua_pushboolean(L, result);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int tostring(lua_State * L) {
    lua_pushstring(L, "");
    return 1;
}
