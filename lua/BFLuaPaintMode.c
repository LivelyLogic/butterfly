//
//  BFLuaPaintMode.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

static int get(lua_State * L);

static const BFLuaClass luaPaintModeLibrary = {
    .libraryName = "PaintMode",
    .methods = {
        {"get", get},
        {NULL, NULL}
    }
};

static const BFLuaClass luaPaintModeClass = {
    .metatableName = BFPaintModeClassName,
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
    BF_LUA_DEBUG_STACK_BEGIN(L);
    int index = lua_tonumber(L, 1);
    BFPaintModeRef paintMode;
    
    paintMode = BFPaintModeCreate(index);
    bf_lua_push(L, paintMode, BFPaintModeClassName);
    BFRelease(paintMode);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
