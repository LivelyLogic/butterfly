//
//  BFLuaColor.c
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
#include "BFLuaPaint.h"

#include "butterfly.h"

static int rgba(lua_State * L);

static int getComponents(lua_State * L);

static int equals(lua_State * L);

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
        {"getComponents", getComponents},
        {"__eq", equals},
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

static int getComponents(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFColorPaintRef colorPaint = *(BFColorPaintRef *)luaL_checkudata(L, 1, BFColorPaintClassName);
    double red = 0;
    double green = 0;
    double blue = 0;
    double alpha = 0;
    
    luaL_argcheck(L, colorPaint, 1, "Color expected");
    
    BFColorPaintGetRGBA(colorPaint, &red, &green, &blue, &alpha);
    
    lua_newtable(L);
    lua_pushnumber(L, red);
    lua_setfield(L, -2, "red");
    lua_pushnumber(L, green);
    lua_setfield(L, -2, "green");
    lua_pushnumber(L, blue);
    lua_setfield(L, -2, "blue");
    lua_pushnumber(L, alpha);
    lua_setfield(L, -2, "alpha");
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
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
