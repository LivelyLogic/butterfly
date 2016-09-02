//
//  lua.h
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
