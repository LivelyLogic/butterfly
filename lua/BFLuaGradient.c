//
//  BFLuaGradient.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "BFLua.h"
#include "BFLuaPaint.h"

#include "types.h"

static int linear(lua_State * L);
static int radial(lua_State * L);

static const BFLuaClass luaGradientLibrary = {
    .libraryName = "Gradient",
    .methods = {
        {"linear", linear},
        {"radial", radial},
        {NULL, NULL}
    }
};

static const BFLuaClass luaGradientClass = {
    .metatableName = BFGradientPaintClassName,
    .superClass = &luaPaintClass,
    .methods = {
        {NULL, NULL}
    }
};

// Global functions

int bf_lua_loadGradient(lua_State * L) {
    bf_lua_loadclass(L, &luaPaintClass);
    lua_pop(L, 1);
    bf_lua_loadmodule(L, &luaGradientLibrary, &luaGradientClass);
    return 0;
}


// Local functions

static int processArray(lua_State * L, BFColorPaintRef ** retColorPaints, double ** retLocations) {
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
    
    double * locations = NULL;
    BFColorPaintRef * colorPaints = NULL;
    if (count > 0) {
        locations = malloc(count * sizeof(double));
        colorPaints = malloc(count * sizeof(BFColorPaintRef));
        if (locations && colorPaints) {
            BFPaintRef * colorUserdata;
            
            lua_pushnil(L);
            while (lua_next(L, 1) != 0) {
                if (lua_isnumber(L, -2)) {
                    location = lua_tonumber(L, -2);
                    // TODO: make sure this doesn't say "bad arg #-2 to function"...
                    colorUserdata = luaL_checkudata(L, -1, BFColorPaintClassName);
                    locations[index] = ((location - locationMin) / (locationMax - locationMin));
                    colorPaints[index] = (BFColorPaintRef)(*colorUserdata);
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
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPoint p1 = {
        .x = lua_tonumber(L, 2),
        .y = lua_tonumber(L, 3),
    };
    BFPoint p2 = {
        .x = lua_tonumber(L, 4),
        .y = lua_tonumber(L, 5),
    };
    BFGradientPaintRef gradientPaint = BFGradientPaintCreate();
    BFColorPaintRef * colorPaints;
    double * locations;
    int count;
    
    count = processArray(L, &colorPaints, &locations);
    
    BFGradientPaintSetColors(gradientPaint, count, colorPaints, locations);
    free(locations);
    free(colorPaints);
    
    BFGradientPaintSetLinearLocation(gradientPaint, p1, p2);
    
    bf_lua_push(L, gradientPaint, BFGradientPaintClassName);
    BFRelease(gradientPaint);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}

static int radial(lua_State * L) {
    BF_LUA_DEBUG_STACK_BEGIN(L);
    BFPoint p1 = {
        .x = lua_tonumber(L, 2),
        .y = lua_tonumber(L, 3),
    };
    double r1 = lua_tonumber(L, 4);
    BFPoint p2 = {
        .x = lua_tonumber(L, 5),
        .y = lua_tonumber(L, 6),
    };
    double r2 = lua_tonumber(L, 7);
    BFGradientPaintRef gradientPaint = BFGradientPaintCreate();
    BFColorPaintRef * colorPaints;
    double * locations;
    int count;
    
    count = processArray(L, &colorPaints, &locations);
    
    BFGradientPaintSetColors(gradientPaint, count, colorPaints, locations);
    free(locations);
    free(colorPaints);
    
    BFGradientPaintSetRadialLocation(gradientPaint, p1, r1, p2, r2);
    
    bf_lua_push(L, gradientPaint, BFGradientPaintClassName);
    BFRelease(gradientPaint);
    
    BF_LUA_DEBUG_STACK_ENDR(L, 1);
    return 1;
}
