//
//  LLLuaGradient.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLLuaGradient.h"

#include "LLLua.h"
#include "LLLuaPaint.h"

#include "LLPaint.h"
#include "LLGradientPaint.h"

static int linear(lua_State * L);
static int radial(lua_State * L);

static const LLLuaClass luaGradientLibrary = {
    .libraryName = "Gradient",
    .methods = {
        {"linear", linear},
        {"radial", radial},
        {NULL, NULL}
    }
};

static const LLLuaClass luaGradientClass = {
    .metatableName = LLGradientPaintClassName,
    .superClass = &luaPaintClass,
    .methods = {
        {NULL, NULL}
    }
};

// Global functions

int initLuaGradientLibrary(lua_State * L) {
    initLuaClass(L, &luaPaintClass);
    lua_pop(L, 1);
    initLuaModule(L, &luaGradientLibrary, &luaGradientClass);
    return 0;
}


// Local functions

static int processArray(lua_State * L, LLColorPaintRef ** retColorPaints, CGFloat ** retLocations) {
    double location;
    double locationMin = 0;
    double locationMax = 1;
    int index = 0;
    int count = 0;
    
    lua_pushnil(L);
    while (lua_next(L, 1) != 0) {
        if (lua_isnumber(L, -2)) {
            count++;
            location = lua_tonumber(L, -2);
            if (location < locationMin) {
                locationMin = location;
            }
            if (locationMax < location) {
                locationMax = location;
            }
        }
        lua_pop(L, 1);
    }
    
    CGFloat * locations = NULL;
    LLColorPaintRef * colorPaints = NULL;
    if (count > 0) {
        locations = malloc(count * sizeof(CGFloat));
        colorPaints = malloc(count * sizeof(LLColorPaintRef));
        if (locations && colorPaints) {
            LLPaintRef * colorUserdata;
            
            lua_pushnil(L);
            while (lua_next(L, 1) != 0) {
                if (lua_isnumber(L, -2)) {
                    location = lua_tonumber(L, -2);
                    // TODO: make sure this doesn't say "bad arg #-2 to function"...
                    colorUserdata = luaL_checkudata(L, -1, LLColorPaintClassName);
                    locations[index] = (CGFloat)((location - locationMin) / (locationMax - locationMin));
                    colorPaints[index] = (LLColorPaintRef)(*colorUserdata);
                    index++;
                }
                lua_pop(L, 1);
            }
        } else {
            free(locations), locations = NULL;
            free(colorPaints), colorPaints = NULL;
            count = 0;
        }
    }
    
    *retColorPaints = colorPaints;
    *retLocations = locations;
    return count;
}

static int linear(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double x1 = lua_tonumber(L, 2);
    double y1 = lua_tonumber(L, 3);
    double x2 = lua_tonumber(L, 4);
    double y2 = lua_tonumber(L, 5);
    LLGradientPaintRef gradientPaint = LLGradientPaintCreate();
    LLColorPaintRef * colorPaints;
    CGFloat * locations;
    int count;
    
    count = processArray(L, &colorPaints, &locations);
    
    LLGradientPaintSetColors(gradientPaint, count, colorPaints, locations);
    free(locations);
    free(colorPaints);
    
    LLGradientPaintSetLinearLocation(gradientPaint, CGPointMake(x1, y1), CGPointMake(x2, y2));
    
    pushNewUserdata(L, gradientPaint, LLGradientPaintClassName);
    LLRelease(gradientPaint);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int radial(lua_State * L) {
    LL_LUA_DEBUG_STACK_BEGIN(L);
    double x1 = lua_tonumber(L, 2);
    double y1 = lua_tonumber(L, 3);
    double r1 = lua_tonumber(L, 4);
    double x2 = lua_tonumber(L, 5);
    double y2 = lua_tonumber(L, 6);
    double r2 = lua_tonumber(L, 7);
    LLGradientPaintRef gradientPaint = LLGradientPaintCreate();
    LLColorPaintRef * colorPaints;
    CGFloat * locations;
    int count;
    
    count = processArray(L, &colorPaints, &locations);
    
    LLGradientPaintSetColors(gradientPaint, count, colorPaints, locations);
    free(locations);
    free(colorPaints);
    
    LLGradientPaintSetRadialLocation(gradientPaint, CGPointMake(x1, y1), r1, CGPointMake(x2, y2), r2);
    
    pushNewUserdata(L, gradientPaint, LLGradientPaintClassName);
    LLRelease(gradientPaint);
    
    LL_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
