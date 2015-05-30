//
//  DrawingView.m
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#import "DrawingView.h"

#include <lua.h>
#include <lauxlib.h>

#import "libraries.h"
#import "BFLua.h"

#import "BFCanvas.h"
#import "BFCanvasMetrics.h"

@interface DrawingView ()
@property (assign) BOOL scriptIsValid;
@end

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
    bf_lua_load(L);
}

- (void)dealloc
{
    lua_close(L);
}

- (void)setDrawingScriptString:(NSString *)drawingScriptString
{
    // Load a Lua script defining a global `draw` function. (We could just call
    // `luaL_dostring` here, but the blank name will make parsing error messages easier.)
    const char * cstring = [drawingScriptString UTF8String];
    if (luaL_loadbuffer(L, cstring, strlen(cstring), "") || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        self.scriptIsValid = NO;
        [self handleError:[NSString stringWithUTF8String:lua_tostring(L, -1)]];
        lua_pop(L, 1);
    } else {
        self.scriptIsValid = YES;
    }
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
    if (!self.scriptIsValid) {
        return;
    }
    
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
    // Create Rule and the Get Rule. This means that when we call `BFCanvasMetricsCreate`
    // below, we own the returned object and are responsible for releasing it.
    NSRect viewBounds = [self bounds];
    BFRect bounds = { .left = NSMinX(viewBounds), .bottom = NSMinY(viewBounds), .right = NSMaxX(viewBounds), .top = NSMaxY(viewBounds) };
    CGFloat backingScale = [self.window.screen backingScaleFactor];
    BFCanvasMetricsRef canvasMetrics = BFCanvasMetricsCreate(bounds, backingScale, 1);
    
    // Create the canvas object for the Lua scripts to draw into.
    // As above with the canvas metrics, we own the object returned by `BFCanvasCreateForDisplay`.
    BFCanvasRef canvas = BFCanvasCreateForDisplay(context, canvasMetrics);
    
    // Now that we've given `canvasMetrics` to the canvas object, we no longer need it here
    // and it's safe to release.
    BFRelease(canvasMetrics);
    
    // Push `canvas` onto the Lua stack. Note that `bf_lua_push` retains the object
    // on behalf of the Lua state.
    bf_lua_push(L, canvas, BFCanvasClassName);
    
    // Now that the canvas userdata is in the Lua state, `canvas` is safe for us to release.
    // The object will live until Lua's garbage collector collects the userdata.
    BFRelease(canvas);
    
    // Call the drawing function. One argument (`canvas`), zero return values.
    if (lua_pcall(L, 1, 0, 0)) {
        [self handleError:[NSString stringWithUTF8String:lua_tostring(L, -1)]];
        lua_pop(L, 1);
    } else {
        [self didDraw];
    }
}

- (void)didDraw
{
    [self.delegate drawingViewDidDraw:self];
}

- (void)handleError:(NSString *)errorString
{
    // Parse the line number and error message from the error string.
    NSRegularExpression * regexp = [NSRegularExpression regularExpressionWithPattern:@"^[^:]+:(\\d+): (.+)$" options:0 error:NULL];
    NSArray * matches = [regexp matchesInString:errorString options:0 range:NSMakeRange(0, errorString.length)];
    if (matches.count > 0) {
        NSTextCheckingResult * result = matches[0];
        NSInteger lineNumber = [[errorString substringWithRange:[result rangeAtIndex:1]] integerValue];
        NSString * errorMessage = [errorString substringWithRange:[result rangeAtIndex:2]];
        [self.delegate drawingView:self
                 didEncounterError:errorMessage
                            onLine:lineNumber];
    } else {
        [self.delegate drawingView:self
                 didEncounterError:errorString
                            onLine:NSNotFound];
    }
}

@end
