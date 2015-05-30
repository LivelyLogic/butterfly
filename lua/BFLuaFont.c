//
//  BFLuaFont.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "BFLua.h"

#include "types.h"

static int get(lua_State * L);
static int getSystem(lua_State * L);
static int getBoldSystem(lua_State * L);

static int getName(lua_State * L);
static int getSize(lua_State * L);
static int getAscent(lua_State * L);
static int getDescent(lua_State * L);
static int getHeight(lua_State * L);
static int getLeading(lua_State * L);
static int archive(lua_State * L);
static int tostring(lua_State * L);

static const BFLuaClass luaFontLibrary = {
    .libraryName = "Font",
    .methods = {
        {"get", get},
        {"system", getSystem},
        {"boldSystem", getBoldSystem},
        {NULL, NULL}
    }
};

static const BFLuaClass luaFontClass = {
    .metatableName = BFFontClassName,
    .methods = {
        {"getName", getName},
        {"getSize", getSize},
        {"ascent", getAscent},
        {"descent", getDescent},
        {"height", getHeight},
        {"leading", getLeading},
        {"archive", archive},
        {"__tostring", tostring},
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadFont(lua_State * L) {
    bf_lua_loadmodule(L, &luaFontLibrary, &luaFontClass);
    return 0;
}


// Local functions

static int get(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    const char * name = lua_tostring(L, 1);
    double size = lua_tonumber(L, 2);
    BFFontRef font;
    
    font = BFFontCreate(name, size);
    bf_lua_push(L, font, BFFontClassName);
    BFRelease(font);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getSystem(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    double size = lua_tonumber(L, 1);
    BFFontRef font;
    
    font = BFFontCreateSystem(size);
    bf_lua_push(L, font, BFFontClassName);
    BFRelease(font);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getBoldSystem(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    double size = lua_tonumber(L, 1);
    BFFontRef font;
    
    font = BFFontCreateBoldSystem(size);
    bf_lua_push(L, font, BFFontClassName);
    BFRelease(font);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getName(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    char * name = nil;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    name = BFFontCopyName(font);
    lua_pushstring(L, name);
    free(name);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getSize(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    double size = 0;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    size = BFFontGetSize(font);
    lua_pushnumber(L, size);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getAscent(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double ascent = BFFontGetAscent(font);
    lua_pushnumber(L, ascent);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getDescent(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double descent = BFFontGetDescent(font);
    lua_pushnumber(L, descent);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getHeight(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double ascent = BFFontGetAscent(font);
    double descent = BFFontGetDescent(font);
    double height = ascent + descent;
    lua_pushnumber(L, height);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getLeading(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double leading = BFFontGetLeading(font);
    lua_pushnumber(L, leading);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int archive(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    char * name = nil;
    double size = 0;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    name = BFFontCopyName(font);
    size = BFFontGetSize(font);
    
    lua_pushstring(L, "Font");
    
    lua_newtable(L);
    lua_pushstring(L, name);
    lua_setfield(L, -2, "name");
    lua_pushnumber(L, size);
    lua_setfield(L, -2, "size");
    
    free(name);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int tostring(lua_State * L) {
    lua_pushstring(L, "");
    return 1;
}
