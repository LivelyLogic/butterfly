//
//  DrawingView.m
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#import "DrawingView.h"

#include <lua.h>
#include <lauxlib.h>

#import "LLLua.h"
#import "LLLuaCanvas.h"
#import "LLLuaCanvasMetrics.h"
#import "LLLuaColor.h"
#import "LLLuaPath.h"

#import "LLCanvas.h"

@implementation DrawingView
{
    lua_State * L;
}

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    [self initializeLuaState];
    return self;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    [self initializeLuaState];
    return self;
}

- (void)initializeLuaState
{
    L = luaL_newstate();
    
    // Set up the globals and metatables for the classes we're using.
    initLuaCanvasLibrary(L);
    initLuaCanvasMetricsLibrary(L);
    initLuaColorLibrary(L);
    initLuaPathLibrary(L);
}

- (void)dealloc
{
    lua_close(L);
}

- (void)setDrawingScriptString:(NSString *)drawingScriptString
{
    // Load a Lua script defining a global `draw` function.
    if (luaL_dostring(L, [drawingScriptString UTF8String])) {
        NSLog(@"Syntax error: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
    } else {
        [self setNeedsDisplay:YES];
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Push the drawing function onto the stack.
    lua_getglobal(L, "draw");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        return;
    }
    
    // Get the Quartz graphics context for the canvas to use.
    CGContextRef context = [[NSGraphicsContext currentContext] graphicsPort];
    
    // Create the metrics object for the canvas.
    // These functions follow the same naming conventions as Core Foundation, namely the
    // Create Rule and the Get Rule. This means that when we call `LLCanvasMetricsCreate`
    // below, we own the returned object and are responsible for releasing it.
    CGRect bounds = NSRectToCGRect([self bounds]);
    CGFloat backingScale = [self.window.screen backingScaleFactor];
    LLCanvasMetricsRef canvasMetrics = LLCanvasMetricsCreate(bounds, backingScale, 1);
    
    // Create the canvas object for the Lua scripts to draw into.
    // As above with the canvas metrics, we own the object returned by `LLCanvasCreateForDisplay`.
    LLCanvasRef canvas = LLCanvasCreateForDisplay(context, canvasMetrics);
    
    // Now that we've given `canvasMetrics` to the canvas object, we no longer need it here
    // and it's safe to release.
    LLRelease(canvasMetrics);
    
    // Push `canvas` onto the Lua stack. Note that `pushNewUserdata` retains the object
    // on behalf of the Lua state.
    pushNewUserdata(L, canvas, LLCanvasClassName);
    
    // Now that the canvas userdata is in the Lua state, `canvas` is safe for us to release.
    // The object will live until Lua's garbage collector collects the userdata.
    LLRelease(canvas);
    
    // Call the drawing function. One argument (`canvas`), zero return values.
    if (lua_pcall(L, 1, 0, 0)) {
        NSLog(@"Runtime error: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

@end
