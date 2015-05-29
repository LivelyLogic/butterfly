//
//  LLLuaPaintMode.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

#include "types.h"

static int get(lua_State * L);

static const LLLuaClass luaPaintModeLibrary = {
    .libraryName = "PaintMode",
    .methods = {
        {"get", get},
        {NULL, NULL}
    }
};

static const LLLuaClass luaPaintModeClass = {
    .metatableName = LLPaintModeClassName,
    .methods = {
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadPaintMode(lua_State * L) {
    bf_lua_loadmodule(L, &luaPaintModeLibrary, &luaPaintModeClass);
    return 0;
}


// Local functions

static int get(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    int index = lua_tonumber(L, 1);
    LLPaintModeRef paintMode;
    
    paintMode = LLPaintModeCreate(index);
    bf_lua_push(L, paintMode, LLPaintModeClassName);
    LLRelease(paintMode);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
