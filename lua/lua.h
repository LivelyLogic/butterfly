//
//  lua.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BUTTERFLY_LUA_H__
#define __BUTTERFLY_LUA_H__

#include <lua.h>

int bf_lua_load(lua_State * L);

int bf_lua_loadCanvas(lua_State * L);
int bf_lua_loadCanvasMetrics(lua_State * L);
int bf_lua_loadColor(lua_State * L);
int bf_lua_loadFont(lua_State * L);
int bf_lua_loadGradient(lua_State * L);
int bf_lua_loadIcon(lua_State * L);
int bf_lua_loadPaintMode(lua_State * L);
int bf_lua_loadPath(lua_State * L);
int bf_lua_loadStyledString(lua_State * L);
int bf_lua_loadTransformation(lua_State * L);

void bf_lua_push(lua_State * L, void * data, const char * tname);

#endif /* __BUTTERFLY_LUA_H__ */
