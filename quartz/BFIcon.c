//
//  BFIcon.c
//
//  Copyright (c) 2011-2016 Ripeware, LLC
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

#include "butterfly.h"
#include "quartz.h"

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
        CGContextDrawImage(context, BFRectToCGRect(boundsRect), image);
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
