//
//  LLLuaStyledString.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

#include "types.h"

static int new(lua_State * L);

static int measure(lua_State * L);
static int wrapToWidth(lua_State * L);
static int truncateToWidth(lua_State * L);
static int concat(lua_State * L);
static int tostring(lua_State * L);

static const LLLuaClass luaStyledStringLibrary = {
    .libraryName = "StyledString",
    .methods = {
        {"new", new},
        {NULL, NULL}
    }
};

static const LLLuaClass luaStyledStringClass = {
    .metatableName = LLStyledStringClassName,
    .methods = {
        {"measure", measure},
        {"wrap", wrapToWidth},
        {"truncate", truncateToWidth},
        {"__concat", concat},
        {"__tostring", tostring},
        {NULL, NULL}
    }
};

// Global functions

int initLuaStyledStringLibrary(lua_State * L) {
    initLuaModule(L, &luaStyledStringLibrary, &luaStyledStringClass);
    return 0;
}

// Local functions

static int new(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    const char * string = lua_tostring(L, 1);
    
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "font");
        LLFontRef font = getLuaNilOrUserdata(L, -1, LLFontClassName);
        lua_pop(L, 1);
        
        lua_getfield(L, 2, "superscript");
        int superscript = lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        LLStyledStringRef styledString = LLStyledStringCreate(string, font, superscript);
        pushNewUserdata(L, styledString, LLStyledStringClassName);
        LLRelease(styledString);
    } else {
        lua_pushnil(L);
    }
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int measure(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLStyledStringRef styledString = *(LLStyledStringRef *)luaL_checkudata(L, 1, LLStyledStringClassName);
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    lua_getglobal(L, "Rect");
    lua_getfield(L, -1, "new");
    lua_insert(L, -2);
    
    LLRect rect = LLStyledStringMeasure(styledString);
    lua_newtable(L);
    lua_pushnumber(L, rect.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, rect.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, rect.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, rect.top);
    lua_setfield(L, -2, "top");
    
    l_pcall(L, 2, 1);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int wrapToWidth(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLStyledStringRef styledString = *(LLStyledStringRef *)luaL_checkudata(L, 1, LLStyledStringClassName);
    double width = lua_tonumber(L, 2);
    CFIndex length;
    CFIndex position;
    CFIndex lineIndex;
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    lua_newtable(L);
    length = LLStyledStringGetLength(styledString);
    position = 0;
    lineIndex = 0;
    while (position < length) {
        const int stepSize = 10;
        LLStyledStringRef wrappedStyledStrings[stepSize] = {};
        position = LLStyledStringCreateBreaking(styledString, position, width, stepSize, &wrappedStyledStrings[0]);
        for (int index = 0; index < stepSize; index++) {
            if (wrappedStyledStrings[index]) {
                lua_pushinteger(L, ++lineIndex);
                pushNewUserdata(L, wrappedStyledStrings[index], LLStyledStringClassName);
                lua_settable(L, -3);
                LLRelease(wrappedStyledStrings[index]);
            }
        }
    }
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int truncateToWidth(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLStyledStringRef styledString = *(LLStyledStringRef *)luaL_checkudata(L, 1, LLStyledStringClassName);
    double width = lua_tonumber(L, 2);
    LLStyledStringRef truncatedStyledString;
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    truncatedStyledString = LLStyledStringCreateTruncating(styledString, width);
    pushNewUserdata(L, truncatedStyledString, LLStyledStringClassName);
    LLRelease(truncatedStyledString);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int concat(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLStyledStringRef styledString1 = *(LLStyledStringRef *)luaL_checkudata(L, 1, LLStyledStringClassName);
    LLStyledStringRef styledString2 = *(LLStyledStringRef *)luaL_checkudata(L, 2, LLStyledStringClassName);
    LLStyledStringRef styledString;
        
    styledString = LLStyledStringCreateJoining(styledString1, styledString2);
    pushNewUserdata(L, styledString, LLStyledStringClassName);
    LLRelease(styledString);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int tostring(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    LLStyledStringRef styledString = *(LLStyledStringRef *)luaL_checkudata(L, 1, LLStyledStringClassName);
    char * cString = NULL;
    
    cString = LLStyledStringCopyString(styledString);
    lua_pushstring(L, cString);
    free(cString);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
