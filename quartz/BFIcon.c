//
//  BFIcon.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFIcon.h"

#include "BFCanvas.h"
#include "BFCanvasMetrics.h"
#include "BFQuartzTypes.h"

struct BFIcon {
    struct BFBase __base;
    BFCanvasRef canvas;
};

static void BFIconInit(BFIconRef icon, BFCanvasRef canvas);
static void BFIconDealloc(BFIconRef icon);

static BFCanvasRef BFIconNewCanvas(BFRect boundsRect, size_t pixelWidth, size_t pixelHeight);

static const BFBaseFunctions baseFunctions = {
    .name = BFIconClassName,
    .dealloc = (BFBaseDeallocFunction)&BFIconDealloc,
};

BFIconRef BFIconCreate(BFRect boundsRect) {
    BFIconRef icon = BFAlloc(sizeof(struct BFIcon), &baseFunctions);
    if (icon) {
        BFCanvasRef canvas = BFIconNewCanvas(boundsRect, boundsRect.right - boundsRect.left, boundsRect.top - boundsRect.bottom);
        BFIconInit(icon, canvas);
    }
    return BFRetain(icon);
}

BFIconRef BFIconCreateWithImage(CGImageRef image, double width, double height) {
    BFIconRef icon = BFAlloc(sizeof(struct BFIcon), &baseFunctions);
    if (icon) {
        BFRect boundsRect = { .left = 0, .bottom = 0, .right = width, .top = height };
        BFCanvasRef canvas = BFIconNewCanvas(boundsRect, CGImageGetWidth(image), CGImageGetHeight(image));
        CGContextRef context = BFCanvasGetCGContext(canvas);
        CGContextDrawImage(context, BFConvertRectToQuartz(boundsRect), image);
        BFIconInit(icon, canvas);
    }
    return BFRetain(icon);
}

static void BFIconInit(BFIconRef icon, BFCanvasRef canvas) {
    icon->canvas = canvas;
}

static void BFIconDealloc(BFIconRef icon) {
    if (icon) {
        BFRelease(icon->canvas);
    }
    BFDealloc(icon);
}

static BFCanvasRef BFIconNewCanvas(BFRect boundsRect, size_t pixelWidth, size_t pixelHeight) {
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
    BFCanvasMetricsRef metrics = BFCanvasMetricsCreate(boundsRect, 1, 1);
    BFCanvasRef canvas = BFCanvasCreateForDisplay(context, metrics);
    CGContextRelease(context);
    BFRelease(metrics);
    return canvas;
}

BFCanvasRef BFIconGetCanvas(BFIconRef icon) {
    return icon->canvas;
}

CGImageRef BFIconCopyCGImage(BFIconRef icon) {
    CGContextRef context = BFCanvasGetCGContext(icon->canvas);
    return CGBitmapContextCreateImage(context);
}
