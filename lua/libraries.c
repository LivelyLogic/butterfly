//
//  libraries.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"

#include "LLLua.h"

int initLuaLibraries(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    initLuaCanvasLibrary(L);
    initLuaCanvasMetricsLibrary(L);
    initLuaColorLibrary(L);
    initLuaFontLibrary(L);
    initLuaGradientLibrary(L);
    initLuaIconLibrary(L);
    initLuaPaintModeLibrary(L);
    initLuaPathLibrary(L);
    initLuaStyledStringLibrary(L);
    initLuaTransformationLibrary(L);
    LL_LUA_DEBUG_STACK_END(L);
    return 0;
}
