//
//  BFLuaStyledString.c
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

static int new(lua_State * L);

static int measure(lua_State * L);
static int wrapToWidth(lua_State * L);
static int truncateToWidth(lua_State * L);
static int getComponents(lua_State * L);
static int concat(lua_State * L);
static int tostring(lua_State * L);

static const BFLuaClass luaStyledStringLibrary = {
    .libraryName = "StyledString",
    .methods = {
        {"new", new},
        {NULL, NULL}
    }
};

static const BFLuaClass luaStyledStringClass = {
    .metatableName = BFStyledStringClassName,
    .methods = {
        {"measure", measure},
        {"wrap", wrapToWidth},
        {"truncate", truncateToWidth},
        {"getComponents", getComponents},
        {"__concat", concat},
        {"__tostring", tostring},
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadStyledString(lua_State * L) {
    bf_lua_loadmodule(L, &luaStyledStringLibrary, &luaStyledStringClass);
    return 0;
}

// Local functions

static int new(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    const char * string = lua_tostring(L, 1);
    
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "font");
        BFFontRef font = bf_lua_getoptionaluserdata(L, -1, BFFontClassName);
        lua_pop(L, 1);
        
        BFStyledStringAttributes attributes = {};
        lua_getfield(L, 2, "baseline");
        attributes.baselineOffset = lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        BFStyledStringRef styledString = BFStyledStringCreate(string, font, attributes);
        bf_lua_push(L, styledString, BFStyledStringClassName);
        BFRelease(styledString);
    } else {
        lua_pushnil(L);
    }
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int measure(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    BFRect rect = BFStyledStringMeasure(styledString);
    lua_newtable(L);
    lua_pushnumber(L, rect.left);
    lua_setfield(L, -2, "left");
    lua_pushnumber(L, rect.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushnumber(L, rect.right);
    lua_setfield(L, -2, "right");
    lua_pushnumber(L, rect.top);
    lua_setfield(L, -2, "top");
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int wrapToWidth(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    double width = lua_tonumber(L, 2);
    CFIndex length;
    CFIndex position;
    CFIndex lineIndex;
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    lua_newtable(L);
    length = BFStyledStringGetLength(styledString);
    position = 0;
    lineIndex = 0;
    while (position < length) {
        const int stepSize = 10;
        BFStyledStringRef wrappedStyledStrings[stepSize] = {};
        position = BFStyledStringCreateBreaking(styledString, position, width, stepSize, &wrappedStyledStrings[0]);
        for (int index = 0; index < stepSize; index++) {
            if (wrappedStyledStrings[index]) {
                lua_pushinteger(L, ++lineIndex);
                bf_lua_push(L, wrappedStyledStrings[index], BFStyledStringClassName);
                lua_settable(L, -3);
                BFRelease(wrappedStyledStrings[index]);
            }
        }
    }
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int truncateToWidth(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    double width = lua_tonumber(L, 2);
    BFStyledStringRef truncatedStyledString;
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    truncatedStyledString = BFStyledStringCreateTruncating(styledString, width);
    bf_lua_push(L, truncatedStyledString, BFStyledStringClassName);
    BFRelease(truncatedStyledString);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static void getComponents_iteration(lua_State * L, BFStyledStringComponent component) {
    lua_newtable(L);
    
    lua_pushstring(L, component.string);
    lua_setfield(L, -2, "string");
    
    bf_lua_push(L, component.font, BFFontClassName);
    lua_setfield(L, -2, "font");
    
    lua_newtable(L);
    lua_pushnumber(L, component.position.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, component.position.y);
    lua_setfield(L, -2, "y");
    lua_setfield(L, -2, "position");

    if (component.attributes.baselineOffset != 0) {
        lua_pushnumber(L, component.attributes.baselineOffset);
        lua_setfield(L, -2, "baseline");
    }
    
    lua_rawseti(L, -2, lua_objlen(L, -2) + 1);
}

static int getComponents(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    
    luaL_argcheck(L, styledString, 1, "StyledString expected");
    
    lua_newtable(L);
    BFStyledStringIterateComponents(styledString, (BFStyledStringComponentIterationFunction)getComponents_iteration, L);

    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int concat(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString1 = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    BFStyledStringRef styledString2 = *(BFStyledStringRef *)luaL_checkudata(L, 2, BFStyledStringClassName);
    BFStyledStringRef styledString;
        
    styledString = BFStyledStringCreateJoining(styledString1, styledString2);
    bf_lua_push(L, styledString, BFStyledStringClassName);
    BFRelease(styledString);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int tostring(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFStyledStringRef styledString = *(BFStyledStringRef *)luaL_checkudata(L, 1, BFStyledStringClassName);
    char * cString = NULL;
    
    cString = BFStyledStringCopyString(styledString);
    lua_pushstring(L, cString);
    free(cString);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
