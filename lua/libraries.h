//
//  libraries.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LIBRARIES_H__
#define __LIBRARIES_H__

#include <lua.h>

int initLuaLibraries(lua_State * L);

int initLuaCanvasLibrary(lua_State * L);
int initLuaCanvasMetricsLibrary(lua_State * L);
int initLuaColorLibrary(lua_State * L);
int initLuaFontLibrary(lua_State * L);
int initLuaGradientLibrary(lua_State * L);
int initLuaIconLibrary(lua_State * L);
int initLuaPaintModeLibrary(lua_State * L);
int initLuaPathLibrary(lua_State * L);
int initLuaStyledStringLibrary(lua_State * L);
int initLuaTransformationLibrary(lua_State * L);

#endif /* __LIBRARIES_H__ */
