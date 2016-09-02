//
//  libraries.c
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
