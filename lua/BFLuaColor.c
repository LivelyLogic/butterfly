//
//  BFLuaColor.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "lua.h"
#include "BFLua.h"
#include "BFLuaPaint.h"

#include "butterfly.h"

static int rgba(lua_State * L);

static int archive(lua_State * L);

static int equals(lua_State * L);
static int tostring(lua_State * L);

static const BFLuaClass luaColorLibrary = {
    .libraryName = "Color",
    .methods = {
        {"rgba", rgba},
        {NULL, NULL}
    }
};

const BFLuaClass luaColorClass = {
    .metatableName = BFColorPaintClassName,
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
    BF_LUA_DEBUG_STACK_BEGIN(L);
    double red   = lua_tonumber(L, 1);
    double green = lua_tonumber(L, 2);
    double blue  = lua_tonumber(L, 3);
    double alpha = lua_tonumber(L, 4);
    BFColorPaintRef colorPaint;
    
    colorPaint = BFColorPaintCreate();
    BFColorPaintSetRGBA(colorPaint, red, green, blue, alpha);
    bf_lua_push(L, colorPaint, BFColorPaintClassName);
    BFRelease(colorPaint);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int archive(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFColorPaintRef colorPaint = *(BFColorPaintRef *)luaL_checkudata(L, 1, BFColorPaintClassName);
    double red = 0;
    double green = 0;
    double blue = 0;
    double alpha = 0;
    
    luaL_argcheck(L, colorPaint, 1, "Color expected");
    
    BFColorPaintGetRGBA(colorPaint, &red, &green, &blue, &alpha);
    
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
    
    BF_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int equals(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFColorPaintRef colorPaint1 = *(BFColorPaintRef *)luaL_checkudata(L, 1, BFColorPaintClassName);
    BFColorPaintRef colorPaint2 = *(BFColorPaintRef *)luaL_checkudata(L, 2, BFColorPaintClassName);
    
    luaL_argcheck(L, colorPaint1, 1, "Color expected");
    luaL_argcheck(L, colorPaint2, 2, "Color expected");
    
    bool result = BFColorPaintEquals(colorPaint1, colorPaint2);
    
    lua_pushboolean(L, result);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int tostring(lua_State * L) {
    lua_pushstring(L, "");
    return 1;
}
