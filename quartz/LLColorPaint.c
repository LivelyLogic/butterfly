//
//  LLColorPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLColorPaint.h"

#include "LLPaint.h"

struct LLColorPaint {
    struct LLPaint __base;
    CGColorRef color;
};

static void LLColorPaintInit(LLColorPaintRef colorPaint);
static void LLColorPaintDealloc(LLColorPaintRef colorPaint);

static const LLPaintFunctions baseFunctions = {
    .__base = {
        .name = LLColorPaintClassName,
        .dealloc = (LLBaseDeallocFunction)&LLColorPaintDealloc,
    },
    .setInContext = (LLPaintSetInContextFunction)&LLColorPaintSetInContext,
    .fillRectInContext = (LLPaintFillRectInContextFunction)&LLColorPaintFillRectInContext,
};

LLColorPaintRef LLColorPaintCreate(void) {
    LLColorPaintRef colorPaint = LLAlloc(sizeof(struct LLColorPaint), (const LLBaseFunctions *)&baseFunctions);
    if (colorPaint) {
        LLColorPaintInit(colorPaint);
    }
    return LLRetain(colorPaint);
}

static void LLColorPaintInit(LLColorPaintRef colorPaint) {
    colorPaint->color = NULL;
}

static void LLColorPaintDealloc(LLColorPaintRef colorPaint) {
    if (colorPaint) {
        CGColorRelease(colorPaint->color);
    }
    LLPaintDealloc(colorPaint);
}

void LLColorPaintSetRGBA(LLColorPaintRef colorPaint, double r, double g, double b, double a) {
    const CGFloat components[] = {r, g, b, a};
    CGColorSpaceRef srgbColorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGColorRelease(colorPaint->color);
    colorPaint->color = CGColorCreate(srgbColorSpace, components);
    CGColorSpaceRelease(srgbColorSpace);
}

void LLColorPaintGetRGBA(LLColorPaintRef colorPaint, double * r, double * g, double * b, double * a) {
    const CGFloat * components = CGColorGetComponents(colorPaint->color);
    if (CGColorGetNumberOfComponents(colorPaint->color) == 4) {
        *r = *(components++);
        *g = *(components++);
        *b = *(components++);
        *a = *(components);
    }
}

void LLColorPaintSetInContext(LLColorPaintRef colorPaint, CGContextRef context) {
    if (colorPaint->color) {
        CGContextSetFillColorWithColor(context, colorPaint->color);
        CGContextSetStrokeColorWithColor(context, colorPaint->color);
    }
}

void LLColorPaintFillRectInContext(LLColorPaintRef colorPaint, CGContextRef context, CGRect rect) {
    if (colorPaint->color) {
        CGContextSetFillColorWithColor(context, colorPaint->color);
        CGContextFillRect(context, rect);
    }
}

CGColorRef LLColorPaintGetCGColor(LLColorPaintRef colorPaint) {
    return colorPaint->color;
}

bool LLColorPaintEquals(LLColorPaintRef colorPaint1, LLColorPaintRef colorPaint2) {
    return CGColorEqualToColor(colorPaint1->color, colorPaint2->color);
}
