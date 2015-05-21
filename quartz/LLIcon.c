//
//  LLIcon.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLIcon.h"

#include "LLCanvas.h"
#include "LLCanvasMetrics.h"
#include "LLQuartzTypes.h"

struct LLIcon {
    struct LLBase __base;
    LLCanvasRef canvas;
};

static void LLIconInit(LLIconRef icon, LLCanvasRef canvas);
static void LLIconDealloc(LLIconRef icon);

static LLCanvasRef LLIconNewCanvas(LLRect boundsRect, size_t pixelWidth, size_t pixelHeight);

static const LLBaseFunctions baseFunctions = {
    .name = LLIconClassName,
    .dealloc = (LLBaseDeallocFunction)&LLIconDealloc,
};

LLIconRef LLIconCreate(LLRect boundsRect) {
    LLIconRef icon = LLAlloc(sizeof(struct LLIcon), &baseFunctions);
    if (icon) {
        LLCanvasRef canvas = LLIconNewCanvas(boundsRect, boundsRect.right - boundsRect.left, boundsRect.top - boundsRect.bottom);
        LLIconInit(icon, canvas);
    }
    return LLRetain(icon);
}

LLIconRef LLIconCreateWithImage(CGImageRef image, double width, double height) {
    LLIconRef icon = LLAlloc(sizeof(struct LLIcon), &baseFunctions);
    if (icon) {
        LLRect boundsRect = { .left = 0, .bottom = 0, .right = width, .top = height };
        LLCanvasRef canvas = LLIconNewCanvas(boundsRect, CGImageGetWidth(image), CGImageGetHeight(image));
        CGContextRef context = LLCanvasGetCGContext(canvas);
        CGContextDrawImage(context, LLConvertRectToQuartz(boundsRect), image);
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

static LLCanvasRef LLIconNewCanvas(LLRect boundsRect, size_t pixelWidth, size_t pixelHeight) {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGContextRef context = CGBitmapContextCreate(NULL,
                                                 pixelWidth,
                                                 pixelHeight,
                                                 8,
                                                 4 * pixelWidth,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast);
    CGContextScaleCTM(context, pixelWidth / (boundsRect.right - boundsRect.left), pixelHeight / (boundsRect.top - boundsRect.bottom));
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
