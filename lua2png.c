//
//  lua2png.c
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#include <ImageIO/ImageIO.h>

#include <lua.h>
#include <lauxlib.h>

#include "LLLua.h"
#include "LLLuaCanvas.h"
#include "LLLuaCanvasMetrics.h"
#include "LLLuaColor.h"
#include "LLLuaFont.h"
#include "LLLuaGradient.h"
#include "LLLuaIcon.h"
#include "LLLuaPath.h"
#include "LLLuaPaintMode.h"
#include "LLLuaStyledString.h"
#include "LLLuaTransformation.h"

#include "LLCanvas.h"
#include "LLCanvasMetrics.h"

int main(int argc, char * argv[]) {
    // Deal with the command-line arguments.
    if (argc < 5) {
        printf("usage: lua2png <width> <height> <input.lua> <output.png>\n");
        return 1;
    }
    size_t width = strtol(argv[1], NULL, 10);
    size_t height = strtol(argv[2], NULL, 10);
    char * inputFileName = argv[3];
    char * outputFileName = argv[4];

    // Create the Lua state.
    lua_State * L = luaL_newstate();

    // Set up the globals and metatables for the classes we're using.
    initLuaCanvasLibrary(L);
    initLuaCanvasMetricsLibrary(L);
    initLuaColorLibrary(L);
    initLuaFontLibrary(L);
    initLuaGradientLibrary(L);
    initLuaIconLibrary(L);
    initLuaPaintModeLibrary(L);
    initLuaPathLibrary(L);
    initLuaStyledStringLibrary(L);
    initLuaTransformationLibrary(L);

    // Load & run the script, expecting it to return a drawing function.
    if (luaL_loadfile(L, inputFileName) || lua_pcall(L, 0, LUA_MULTRET, 0)) {
        printf("OH NO: %s\n", lua_tostring(L, -1));
        return 1;
    } else if (!lua_isfunction(L, -1)) {
        printf("OH NO: the script didn't return a function!\n");
        return 1;
    }

    // Create a buffer for the bitmap data.
    unsigned char * buffer = calloc(width * height, 4);

    // Create the metrics object for the canvas.
    // These functions follow the same naming conventions as Core Foundation, namely the
    // Create Rule and the Get Rule. This means that when we call `LLCanvasMetricsCreate`
    // below, we own the returned object and are responsible for releasing it.
    LLRect bounds = { 0, 0, width, height };
    LLCanvasMetricsRef canvasMetrics = LLCanvasMetricsCreate(bounds, 1, 1);

    // Create a bitmap graphics context for the canvas to use.
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGContextRef context = CGBitmapContextCreate(buffer, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(colorSpace);

    // Create the canvas object for the Lua scripts to draw into.
    // As above with the canvas metrics, we own the object returned by `LLCanvasCreateForDisplay`.
    LLCanvasRef canvas = LLCanvasCreateForDisplay(context, canvasMetrics);

    // Push `canvas` onto the Lua stack. Note that `pushNewUserdata` retains the object
    // on behalf of the Lua state.
    pushNewUserdata(L, canvas, LLCanvasClassName);

    // Normally we should release `canvasMetrics` and `canvas` by calling `LLRelease`,
    // since we no longer need them here. But the process will end soon so all the dynamic
    // memory allocations will be cleaned up then.

    // Call the drawing function. One argument (`canvas`), zero return values.
    if (lua_pcall(L, 1, 0, 0)) {
        printf("OH NO: %s\n", lua_tostring(L, -1));
        return 1;
    }

    // Write the output image file.
    CFStringRef outputFileNameString = CFStringCreateWithCString(NULL, outputFileName, kCFStringEncodingUTF8);
    CFURLRef fileURL = CFURLCreateWithFileSystemPath(NULL, outputFileNameString, kCFURLPOSIXPathStyle, false);
    CGImageDestinationRef imageDestination = CGImageDestinationCreateWithURL(fileURL, CFSTR("public.png"), 1, NULL);
    CGImageRef image = CGBitmapContextCreateImage(context);
    CGImageDestinationAddImage(imageDestination, image, NULL);
    CGImageDestinationFinalize(imageDestination);

    return 0;
}
