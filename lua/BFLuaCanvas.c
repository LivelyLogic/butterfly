//
//  BFLuaCanvas.c
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

#include "butterfly.h"

static int stroke(lua_State * L);
static int fill(lua_State * L);
static int drawText(lua_State * L);
static int strokeText(lua_State * L);
static int drawIcon(lua_State * L);
static int clipIcon(lua_State * L);
static int setOpacity(lua_State * L);
static int setPaint(lua_State * L);
static int setPaintMode(lua_State * L);
static int setThickness(lua_State * L);
static int setFont(lua_State * L);
static int getFont(lua_State * L);
static int concatTransformation(lua_State * L);
static int clipRect(lua_State * L);
static int clipPath(lua_State * L);
static int preserveState(lua_State * L);
static int isHitTest(lua_State * L);
static int test(lua_State * L);
static int metrics(lua_State * L);
static int dirtyRect(lua_State * L);

static const BFLuaClass luaCanvasClass = {
    .metatableName = BFCanvasClassName,
    .methods = {
        {"stroke", stroke},
        {"fill", fill},
        {"drawText", drawText},
        {"strokeText", strokeText},
        {"drawIcon", drawIcon},
        {"clipIcon", clipIcon},
        {"setOpacity", setOpacity},
        {"setPaint", setPaint},
        {"setPaintMode", setPaintMode},
        {"setThickness", setThickness},
        {"setFont", setFont},
        {"getFont", getFont},
        {"concatTransformation", concatTransformation},
        {"clipRect", clipRect},
        {"clip", clipPath},
        {"preserve", preserveState},
        {"isHitTest", isHitTest},
        {"test", test},
        {"metrics", metrics},
        {"dirtyRect", dirtyRect},
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadCanvas(lua_State * L) {
    bf_lua_loadmodule(L, NULL, &luaCanvasClass);
    return 0;
}

// Local functions

static int stroke(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFPathRef path = bf_lua_getoptionaluserdata(L, 2, BFPathClassName);

    if (path) {
        BFCanvasStrokePath(canvas, path);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int fill(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFPathRef path = bf_lua_getoptionaluserdata(L, 2, BFPathClassName);

    if (path) {
        BFCanvasFillPath(canvas, path);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int drawText(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFStyledStringRef styledString = NULL;
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double alignment = lua_tonumber(L, 5);

    if (lua_isuserdata(L, 2)) {
        styledString = *(BFStyledStringRef *)luaL_checkudata(L, 2, BFStyledStringClassName);
        BFRetain(styledString); /* not necessary, but allows us to avoid an if-check on the release below. */
    } else {
        const char * cstring = lua_tostring(L, 2);
        styledString = BFStyledStringCreate(cstring, BFCanvasGetFont(canvas), 0);
    }

    BFRect stringRect = BFStyledStringMeasure(styledString);
    BFPoint stringPoint = { .x = x - alignment * (stringRect.right - stringRect.left), .y = y };
    BFCanvasDrawStyledString(canvas, styledString, stringPoint);
    BFRelease(styledString);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int strokeText(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFStyledStringRef styledString = NULL;
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double alignment = lua_tonumber(L, 5);

    if (lua_isuserdata(L, 2)) {
        styledString = *(BFStyledStringRef *)luaL_checkudata(L, 2, BFStyledStringClassName);
        BFRetain(styledString); /* not necessary, but allows us to avoid an if-check on the release below. */
    } else {
        const char * cstring = lua_tostring(L, 2);
        styledString = BFStyledStringCreate(cstring, BFCanvasGetFont(canvas), 0);
    }

    BFRect stringRect = BFStyledStringMeasure(styledString);
    BFPoint stringPoint = { .x = x - alignment * (stringRect.right - stringRect.left), .y = y };
    BFCanvasStrokeStyledString(canvas, styledString, stringPoint);
    BFRelease(styledString);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int drawIcon(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFIconRef icon = bf_lua_getoptionaluserdata(L, 2, BFIconClassName);

    if (icon) {
        BFRect rect;

        lua_getfield(L, 3, "left");
        rect.left = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "bottom");
        rect.bottom = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "right");
        rect.right = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "top");
        rect.top = lua_tonumber(L, -1);
        lua_pop(L, 1);

        BFCanvasDrawIcon(canvas, icon, rect);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipIcon(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFIconRef icon = bf_lua_getoptionaluserdata(L, 2, BFIconClassName);

    if (icon) {
        BFRect rect;

        lua_getfield(L, 3, "left");
        rect.left = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "bottom");
        rect.bottom = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "right");
        rect.right = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 3, "top");
        rect.top = lua_tonumber(L, -1);
        lua_pop(L, 1);

        BFCanvasClipIcon(canvas, icon, rect);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setOpacity(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    double opacity = lua_tonumber(L, 2);

    if (lua_isnumber(L, 2)) {
        BFCanvasSetOpacity(canvas, opacity);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setPaint(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFPaintRef paint = bf_lua_getoptionaluserdata(L, 2, BFPaintClassName);

    if (paint) {
        BFCanvasSetPaint(canvas, paint);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setPaintMode(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFPaintModeRef paintMode = bf_lua_getoptionaluserdata(L, 2, BFPaintModeClassName);

    if (paintMode) {
        BFCanvasSetPaintMode(canvas, paintMode);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setThickness(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);

    if (lua_isnumber(L, 2)) {
        double thickness = luaL_checknumber(L, 2);
        BFCanvasSetThickness(canvas, thickness);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setFont(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFFontRef font = bf_lua_getoptionaluserdata(L, 2, BFFontClassName);

    if (font) {
        BFCanvasSetFont(canvas, font);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int getFont(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFFontRef font;

    font = BFCanvasGetFont(canvas);
    bf_lua_push(L, font, BFFontClassName);

    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int concatTransformation(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFTransformationRef transformation = bf_lua_getoptionaluserdata(L, 2, BFTransformationClassName);

    if (transformation) {
        BFCanvasConcatTransformation(canvas, transformation);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipRect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);

    if (lua_istable(L, 2)) {
        BFRect rect;

        lua_getfield(L, 2, "left");
        rect.left = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 2, "bottom");
        rect.bottom = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 2, "right");
        rect.right = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, 2, "top");
        rect.top = lua_tonumber(L, -1);
        lua_pop(L, 1);

        BFCanvasClipRect(canvas, rect);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipPath(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFPathRef path = bf_lua_getoptionaluserdata(L, 2, BFPathClassName);

    if (path) {
        BFCanvasClipPath(canvas, path);
    }

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int preserveState(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);

    BFCanvasPush(canvas);

    lua_pushvalue(L, 2);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 0);

    BFCanvasPop(canvas);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int isHitTest(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    bool result = BFCanvasIsHitTest(canvas);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushboolean(L, result);
    return 1;
}

static int test(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    bool hit = BFCanvasPerformHitTest(canvas);

    BF_LUA_DEBUG_STACK_END(L);
    lua_pushboolean(L, hit);
    return 1;
}

static int metrics(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFCanvasMetricsRef metrics = NULL;

    metrics = BFCanvasGetMetrics(canvas);
    bf_lua_push(L, metrics, BFCanvasMetricsClassName);

    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int dirtyRect(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFCanvasRef canvas = *(BFCanvasRef *)luaL_checkudata(L, 1, BFCanvasClassName);
    BFRect dirtyRect;

    dirtyRect = BFCanvasGetDirtyRect(canvas);

    // TODO: this file shouldn't have dependencies on any Lua classes: either return a plain table and convert in Canvas.lua or create an BFRect class.
    lua_getglobal(L, "Rect");
    lua_getfield(L, -1, "new");
    lua_insert(L, -2);

    lua_newtable(L);
    lua_pushnumber(L, dirtyRect.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, dirtyRect.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, dirtyRect.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, dirtyRect.top);
    lua_setfield(L, -2, "top");

    lua_pcall(L, 2, 1, 0);

    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
