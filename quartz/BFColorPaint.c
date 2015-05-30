//
//  BFColorPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"
#include "BFPaint.h"

#include "BFColorPaint.h"

struct BFColorPaint {
    struct BFPaint __base;
    CGColorRef color;
};

static void BFColorPaintInit(BFColorPaintRef colorPaint);
static void BFColorPaintDealloc(BFColorPaintRef colorPaint);
static void BFColorPaintSetInContext(BFColorPaintRef colorPaint, CGContextRef context);
static void BFColorPaintFillRectInContext(BFColorPaintRef colorPaint, CGContextRef context, CGRect rect);

static const BFPaintFunctions baseFunctions = {
    .__base = {
        .name = BFColorPaintClassName,
        .dealloc = (BFBaseDeallocFunction)&BFColorPaintDealloc,
    },
    .setInContext = (BFPaintSetInContextFunction)&BFColorPaintSetInContext,
    .fillRectInContext = (BFPaintFillRectInContextFunction)&BFColorPaintFillRectInContext,
};

BFColorPaintRef BFColorPaintCreate(void) {
    BFColorPaintRef colorPaint = BFAlloc(sizeof(struct BFColorPaint), (const BFBaseFunctions *)&baseFunctions);
    if (colorPaint) {
        BFColorPaintInit(colorPaint);
    }
    return BFRetain(colorPaint);
}

static void BFColorPaintInit(BFColorPaintRef colorPaint) {
    colorPaint->color = NULL;
}

static void BFColorPaintDealloc(BFColorPaintRef colorPaint) {
    if (colorPaint) {
        CGColorRelease(colorPaint->color);
    }
    BFPaintDealloc(colorPaint);
}

void BFColorPaintSetRGBA(BFColorPaintRef colorPaint, double r, double g, double b, double a) {
    const CGFloat components[] = {r, g, b, a};
    CGColorSpaceRef srgbColorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGColorRelease(colorPaint->color);
    colorPaint->color = CGColorCreate(srgbColorSpace, components);
    CGColorSpaceRelease(srgbColorSpace);
}

void BFColorPaintGetRGBA(BFColorPaintRef colorPaint, double * r, double * g, double * b, double * a) {
    const CGFloat * components = CGColorGetComponents(colorPaint->color);
    if (CGColorGetNumberOfComponents(colorPaint->color) == 4) {
        *r = *(components++);
        *g = *(components++);
        *b = *(components++);
        *a = *(components);
    }
}

void BFColorPaintSetInContext(BFColorPaintRef colorPaint, CGContextRef context) {
    if (colorPaint->color) {
        CGContextSetFillColorWithColor(context, colorPaint->color);
        CGContextSetStrokeColorWithColor(context, colorPaint->color);
    }
}

void BFColorPaintFillRectInContext(BFColorPaintRef colorPaint, CGContextRef context, CGRect rect) {
    if (colorPaint->color) {
        CGContextSetFillColorWithColor(context, colorPaint->color);
        CGContextFillRect(context, rect);
    }
}

CGColorRef BFColorPaintGetCGColor(BFColorPaintRef colorPaint) {
    return colorPaint->color;
}

bool BFColorPaintEquals(BFColorPaintRef colorPaint1, BFColorPaintRef colorPaint2) {
    return CGColorEqualToColor(colorPaint1->color, colorPaint2->color);
}
