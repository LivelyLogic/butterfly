//
//  libraries.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"

#include "BFLua.h"

int bf_lua_load(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    bf_lua_loadCanvas(L);
    bf_lua_loadCanvasMetrics(L);
    bf_lua_loadColor(L);
    bf_lua_loadFont(L);
    bf_lua_loadGradient(L);
    bf_lua_loadIcon(L);
    bf_lua_loadPaintMode(L);
    bf_lua_loadPath(L);
    bf_lua_loadStyledString(L);
    bf_lua_loadTransformation(L);
    BF_LUA_DEBUG_STACK_END(L);
    return 0;
}
