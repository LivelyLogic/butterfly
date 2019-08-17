//
//  BFLuaFont.c
//
//  Copyright (c) 2011-2019 James Rodovich
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

static int get(lua_State * L);
static int getSystem(lua_State * L);
static int getBoldSystem(lua_State * L);

static int getName(lua_State * L);
static int getSize(lua_State * L);
static int getAscent(lua_State * L);
static int getDescent(lua_State * L);
static int getHeight(lua_State * L);
static int getLeading(lua_State * L);
static int getFeatures(lua_State * L);

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
        {"getFeatures", getFeatures},
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
    const char * name;
    const char * feature;
    double size;
    BFFontRef font;
    BFFontFeatures features = {};
    
    lua_getfield(L, 1, "name");
    name = lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 1, "size");
    size = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "lowercase");
    feature = lua_tostring(L, -1);
    if (feature) {
        if (strcmp(feature, "smallCaps") == 0) {
            features.smallCaps = true;
        }
    }
    lua_pop(L, 1);

    lua_getfield(L, 1, "numberCase");
    feature = lua_tostring(L, -1);
    if (feature) {
        if (strcmp(feature, "uppercase") == 0) {
            features.uppercaseNumbers = true;
        } else if (strcmp(feature, "lowercase") == 0) {
            features.lowercaseNumbers = true;
        }
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "numberSpacing");
    feature = lua_tostring(L, -1);
    if (feature) {
        if (strcmp(feature, "proportional") == 0) {
            features.proportionalNumbers = true;
        } else if (strcmp(feature, "monospaced") == 0) {
            features.monospacedNumbers = true;
        }
    }
    lua_pop(L, 1);
        
    font = BFFontCreateWithFeatures(name, size, features);
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

static int getFeatures(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFFontRef font = *(BFFontRef *)luaL_checkudata(L, 1, BFFontClassName);
    
    luaL_argcheck(L, font, 1, "Font expected");
    
    BFFontFeatures features = BFFontGetFeatures(font);
    lua_newtable(L);
    
    if (features.smallCaps) {
        lua_pushstring(L, "smallCaps");
        lua_setfield(L, -2, "lowercase");
    }
    if (features.uppercaseNumbers || features.lowercaseNumbers) {
        if (features.uppercaseNumbers) {
            lua_pushstring(L, "uppercase");
        } else if (features.lowercaseNumbers) {
            lua_pushstring(L, "lowercase");
        }
        lua_setfield(L, -2, "numberCase");
    }
    if (features.proportionalNumbers || features.monospacedNumbers) {
        if (features.proportionalNumbers) {
            lua_pushstring(L, "proportional");
        } else if (features.monospacedNumbers) {
            lua_pushstring(L, "monospaced");
        }
        lua_setfield(L, -2, "numberSpacing");
    }
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
