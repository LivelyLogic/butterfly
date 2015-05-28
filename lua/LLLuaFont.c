//
//  LLLuaFont.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

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

static const LLLuaClass luaFontLibrary = {
    .libraryName = "Font",
    .methods = {
        {"get", get},
        {"system", getSystem},
        {"boldSystem", getBoldSystem},
        {NULL, NULL}
    }
};

static const LLLuaClass luaFontClass = {
    .metatableName = LLFontClassName,
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

int initLuaFontLibrary(lua_State * L) {
    initLuaModule(L, &luaFontLibrary, &luaFontClass);
    return 0;
}


// Local functions

static int get(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    const char * name = lua_tostring(L, 1);
    double size = lua_tonumber(L, 2);
    LLFontRef font;
    
    font = LLFontCreate(name, size);
    pushNewUserdata(L, font, LLFontClassName);
    LLRelease(font);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getSystem(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double size = lua_tonumber(L, 1);
    LLFontRef font;
    
    font = LLFontCreateSystem(size);
    pushNewUserdata(L, font, LLFontClassName);
    LLRelease(font);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getBoldSystem(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double size = lua_tonumber(L, 1);
    LLFontRef font;
    
    font = LLFontCreateBoldSystem(size);
    pushNewUserdata(L, font, LLFontClassName);
    LLRelease(font);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getName(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    char * name = nil;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    name = LLFontCopyName(font);
    lua_pushstring(L, name);
    free(name);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getSize(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    double size = 0;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    size = LLFontGetSize(font);
    lua_pushnumber(L, size);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getAscent(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double ascent = LLFontGetAscent(font);
    lua_pushnumber(L, ascent);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getDescent(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double descent = LLFontGetDescent(font);
    lua_pushnumber(L, descent);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getHeight(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double ascent = LLFontGetAscent(font);
    double descent = LLFontGetDescent(font);
    double height = ascent + descent;
    lua_pushnumber(L, height);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int getLeading(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    double leading = LLFontGetLeading(font);
    lua_pushnumber(L, leading);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int archive(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLFontRef font = *(LLFontRef *)luaL_checkudata(L, 1, LLFontClassName);
    char * name = nil;
    double size = 0;
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    name = LLFontCopyName(font);
    size = LLFontGetSize(font);
    
    lua_pushstring(L, "Font");
    
    lua_newtable(L);
    lua_pushstring(L, name);
    lua_setfield(L, -2, "name");
    lua_pushnumber(L, size);
    lua_setfield(L, -2, "size");
    
    free(name);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 2);
    return 2;
}

static int tostring(lua_State * L) {
    lua_pushstring(L, "");
    return 1;
}
