//
//  LLLuaCanvas.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

#include "types.h"

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

static const LLLuaClass luaCanvasClass = {
    .metatableName = LLCanvasClassName,
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
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLPathRef path = getLuaNilOrUserdata(L, 2, LLPathClassName);

    if (path) {
        LLCanvasStrokePath(canvas, path);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int fill(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLPathRef path = getLuaNilOrUserdata(L, 2, LLPathClassName);

    if (path) {
        LLCanvasFillPath(canvas, path);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int drawText(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLStyledStringRef styledString = NULL;
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double alignment = lua_tonumber(L, 5);

    if (lua_isuserdata(L, 2)) {
        styledString = *(LLStyledStringRef *)luaL_checkudata(L, 2, LLStyledStringClassName);
        LLRetain(styledString); /* not necessary, but allows us to avoid an if-check on the release below. */
    } else {
        const char * cstring = lua_tostring(L, 2);
        styledString = LLStyledStringCreate(cstring, LLCanvasGetFont(canvas), 0);
    }

    LLRect stringRect = LLStyledStringMeasure(styledString);
    LLPoint stringPoint = { .x = x - alignment * (stringRect.right - stringRect.left), .y = y };
    LLCanvasDrawStyledString(canvas, styledString, stringPoint);
    LLRelease(styledString);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int strokeText(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLStyledStringRef styledString = NULL;
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    double alignment = lua_tonumber(L, 5);

    if (lua_isuserdata(L, 2)) {
        styledString = *(LLStyledStringRef *)luaL_checkudata(L, 2, LLStyledStringClassName);
        LLRetain(styledString); /* not necessary, but allows us to avoid an if-check on the release below. */
    } else {
        const char * cstring = lua_tostring(L, 2);
        styledString = LLStyledStringCreate(cstring, LLCanvasGetFont(canvas), 0);
    }

    LLRect stringRect = LLStyledStringMeasure(styledString);
    LLPoint stringPoint = { .x = x - alignment * (stringRect.right - stringRect.left), .y = y };
    LLCanvasStrokeStyledString(canvas, styledString, stringPoint);
    LLRelease(styledString);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int drawIcon(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLIconRef icon = getLuaNilOrUserdata(L, 2, LLIconClassName);

    if (icon) {
        LLRect rect;

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

        LLCanvasDrawIcon(canvas, icon, rect);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipIcon(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLIconRef icon = getLuaNilOrUserdata(L, 2, LLIconClassName);

    if (icon) {
        LLRect rect;

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

        LLCanvasClipIcon(canvas, icon, rect);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setOpacity(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    double opacity = lua_tonumber(L, 2);

    if (lua_isnumber(L, 2)) {
        LLCanvasSetOpacity(canvas, opacity);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setPaint(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLPaintRef paint = getLuaNilOrUserdata(L, 2, LLPaintClassName);

    if (paint) {
        LLCanvasSetPaint(canvas, paint);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setPaintMode(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLPaintModeRef paintMode = getLuaNilOrUserdata(L, 2, LLPaintModeClassName);

    if (paintMode) {
        LLCanvasSetPaintMode(canvas, paintMode);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setThickness(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);

    if (lua_isnumber(L, 2)) {
        double thickness = luaL_checknumber(L, 2);
        LLCanvasSetThickness(canvas, thickness);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int setFont(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLFontRef font = getLuaNilOrUserdata(L, 2, LLFontClassName);

    if (font) {
        LLCanvasSetFont(canvas, font);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int getFont(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLFontRef font;

    font = LLCanvasGetFont(canvas);
    bf_lua_push(L, font, LLFontClassName);

    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int concatTransformation(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLTransformationRef transformation = getLuaNilOrUserdata(L, 2, LLTransformationClassName);

    if (transformation) {
        LLCanvasConcatTransformation(canvas, transformation);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipRect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);

    if (lua_istable(L, 2)) {
        LLRect rect;

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

        LLCanvasClipRect(canvas, rect);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int clipPath(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLPathRef path = getLuaNilOrUserdata(L, 2, LLPathClassName);

    if (path) {
        LLCanvasClipPath(canvas, path);
    }

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int preserveState(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);

    LLCanvasPush(canvas);

    lua_pushvalue(L, 2);
    lua_pushvalue(L, 1);
    lua_call(L, 1, 0);

    LLCanvasPop(canvas);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushvalue(L, 1);
    return 1;
}

static int isHitTest(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    bool result = LLCanvasIsHitTest(canvas);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushboolean(L, result);
    return 1;
}

static int test(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    bool hit = LLCanvasPerformHitTest(canvas);

    LL_LUA_DEBUG_STACK_END(L);
    lua_pushboolean(L, hit);
    return 1;
}

static int metrics(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLCanvasMetricsRef metrics = NULL;

    metrics = LLCanvasGetMetrics(canvas);
    bf_lua_push(L, metrics, LLCanvasMetricsClassName);

    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int dirtyRect(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLCanvasRef canvas = *(LLCanvasRef *)luaL_checkudata(L, 1, LLCanvasClassName);
    LLRect dirtyRect;

    dirtyRect = LLCanvasGetDirtyRect(canvas);

    // TODO: this file shouldn't have dependencies on any Lua classes: either return a plain table and convert in Canvas.lua or create an LLRect class.
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

    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
