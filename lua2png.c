//
//  lua2png.c
//
//  Copyright (c) 2015-2019 James Rodovich
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

#include <ImageIO/ImageIO.h>

#include <lua.h>
#include <lauxlib.h>
#include <butterfly/butterfly.h>
#include <butterfly/lua.h>
#include <butterfly/quartz.h>

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

    // Create the Lua state and set up the globals and metatables for the classes we're using.
    lua_State * L = luaL_newstate();
    bf_lua_load(L);

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
    // Create Rule and the Get Rule. This means that when we call `BFCanvasMetricsCreate`
    // below, we own the returned object and are responsible for releasing it.
    BFRect bounds = { 0, 0, width, height };
    BFCanvasMetricsRef canvasMetrics = BFCanvasMetricsCreate(bounds, 1, 1);

    // Create a bitmap graphics context for the canvas to use.
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGContextRef context = CGBitmapContextCreate(buffer, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease(colorSpace);

    // Create the canvas object for the Lua scripts to draw into.
    // As above with the canvas metrics, we own the object returned by `BFCanvasCreateForDisplay`.
    BFCanvasRef canvas = BFCanvasCreateForDisplay(context, canvasMetrics);

    // Push `canvas` onto the Lua stack. Note that `bf_lua_push` retains the object
    // on behalf of the Lua state.
    bf_lua_push(L, canvas, BFCanvasClassName);

    // Normally we should release `canvasMetrics` and `canvas` by calling `BFRelease`,
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
