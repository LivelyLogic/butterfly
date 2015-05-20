//
//  LLIcon.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLIcon.h"

struct LLIcon {
    struct LLBase __base;
    LLCanvasRef canvas;
};

static void LLIconInit(LLIconRef icon, LLCanvasRef canvas);
static void LLIconDealloc(LLIconRef icon);

static LLCanvasRef LLIconNewCanvas(CGRect boundsRect, size_t width, size_t height);

static const LLBaseFunctions baseFunctions = {
    .name = LLIconClassName,
    .dealloc = (LLBaseDeallocFunction)&LLIconDealloc,
};

LLIconRef LLIconCreate(CGRect boundsRect) {
    LLIconRef icon = LLAlloc(sizeof(struct LLIcon), &baseFunctions);
    if (icon) {
        LLCanvasRef canvas = LLIconNewCanvas(boundsRect, boundsRect.size.width, boundsRect.size.height);
        LLIconInit(icon, canvas);
    }
    return LLRetain(icon);
}

LLIconRef LLIconCreateWithImage(CGImageRef image, double width, double height) {
    LLIconRef icon = LLAlloc(sizeof(struct LLIcon), &baseFunctions);
    if (icon) {
        CGRect boundsRect = CGRectMake(0, 0, width, height);
        LLCanvasRef canvas = LLIconNewCanvas(boundsRect, CGImageGetWidth(image), CGImageGetHeight(image));
        CGContextRef context = LLCanvasGetCGContext(canvas);
        CGContextDrawImage(context, boundsRect, image);
        LLIconInit(icon, canvas);
    }
    return LLRetain(icon);
}

static void LLIconInit(LLIconRef icon, LLCanvasRef canvas) {
    icon->canvas = canvas;
}

static void LLIconDealloc(LLIconRef icon) {
    if (icon) {
        LLRelease(icon->canvas);
    }
    LLDealloc(icon);
}

static LLCanvasRef LLIconNewCanvas(CGRect boundsRect, size_t width, size_t height) {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGContextRef context = CGBitmapContextCreate(NULL,
                                                 width,
                                                 height,
                                                 8,
                                                 4 * width,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast);
    CGContextScaleCTM(context, width / boundsRect.size.width, height / boundsRect.size.height);
    CGColorSpaceRelease(colorSpace);
    LLCanvasMetricsRef metrics = LLCanvasMetricsCreate(boundsRect, 1, 1);
    LLCanvasRef canvas = LLCanvasCreateForDisplay(context, metrics);
    CGContextRelease(context);
    LLRelease(metrics);
    return canvas;
}

LLCanvasRef LLIconGetCanvas(LLIconRef icon) {
    return icon->canvas;
}

CGImageRef LLIconCopyCGImage(LLIconRef icon) {
    CGContextRef context = LLCanvasGetCGContext(icon->canvas);
    return CGBitmapContextCreateImage(context);
}
