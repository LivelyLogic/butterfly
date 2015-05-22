//
//  LLLuaIcon.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

#include "LLCanvas.h"
#include "LLIcon.h"

static int new(lua_State * L);

static int getCanvas(lua_State * L);

static const LLLuaClass luaIconLibrary = {
    .libraryName = "Icon",
    .methods = {
        {"new", new},
        {NULL, NULL}
    }
};

static const LLLuaClass luaIconClass = {
    .metatableName = LLIconClassName,
    .methods = {
        {"canvas", getCanvas},
        {NULL, NULL}
    }
};

// Global functions

int initLuaIconLibrary(lua_State * L) {
    initLuaModule(L, &luaIconLibrary, &luaIconClass);
    return 0;
}


// Local functions

static int new(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLRect rect;
    LLIconRef icon;
    
    rect.left = 0;
    rect.bottom = 0;
    lua_getfield(L, 2, "width");
    rect.right = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "height");
    rect.top = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    icon = LLIconCreate(rect);
    
    pushNewUserdata(L, icon, LLIconClassName);
    LLRelease(icon);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getCanvas(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLIconRef icon = *(LLIconRef *)luaL_checkudata(L, 1, LLIconClassName);
    LLCanvasRef canvas = NULL;
    
    canvas = LLIconGetCanvas(icon);
    pushNewUserdata(L, canvas, LLCanvasClassName);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
