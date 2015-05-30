//
//  BFLuaIcon.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

static int new(lua_State * L);

static int getCanvas(lua_State * L);

static const BFLuaClass luaIconLibrary = {
    .libraryName = "Icon",
    .methods = {
        {"new", new},
        {NULL, NULL}
    }
};

static const BFLuaClass luaIconClass = {
    .metatableName = BFIconClassName,
    .methods = {
        {"canvas", getCanvas},
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadIcon(lua_State * L) {
    bf_lua_loadmodule(L, &luaIconLibrary, &luaIconClass);
    return 0;
}


// Local functions

static int new(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFRect rect;
    BFIconRef icon;
    
    rect.left = 0;
    rect.bottom = 0;
    lua_getfield(L, 2, "width");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "height");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    icon = BFIconCreate(rect);
    
    bf_lua_push(L, icon, BFIconClassName);
    BFRelease(icon);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getCanvas(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFIconRef icon = *(BFIconRef *)luaL_checkudata(L, 1, BFIconClassName);
    BFCanvasRef canvas = NULL;
    
    canvas = BFIconGetCanvas(icon);
    bf_lua_push(L, canvas, BFCanvasClassName);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
