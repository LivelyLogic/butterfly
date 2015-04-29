//
//  LLGradientPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLGradientPaint.h"

#include "LLPaint.h"

typedef enum LLGradientPaintType {
    kLLGradientPaintLinear,
    kLLGradientPaintRadial,
} LLGradientPaintType;

struct LLGradientPaint {
    struct LLPaint __base;
    CGGradientRef gradient;
    LLGradientPaintType type;
    CGPoint locationPoints[2];
    CGFloat locationFloats[2];
};

static void LLGradientPaintInit(LLGradientPaintRef gradientPaint);
static void LLGradientPaintDealloc(LLGradientPaintRef gradientPaint);

static const LLPaintFunctions baseFunctions = {
    .__base = {
        .name = LLGradientPaintClassName,
        .dealloc = (LLBaseDeallocFunction)&LLGradientPaintDealloc,
    },
    .fillRectInContext = (LLPaintFillRectInContextFunction)&LLGradientPaintFillRectInContext,
};

LLGradientPaintRef LLGradientPaintCreate(void) {
    LLGradientPaintRef gradientPaint = LLAlloc(sizeof(struct LLGradientPaint), (const LLBaseFunctions *)&baseFunctions);
    if (gradientPaint) {
        LLGradientPaintInit(gradientPaint);
    }
    return LLRetain(gradientPaint);
}

static void LLGradientPaintInit(LLGradientPaintRef gradientPaint) {
    gradientPaint->gradient = NULL;
}

static void LLGradientPaintDealloc(LLGradientPaintRef gradientPaint) {
    if (gradientPaint) {
        CGGradientRelease(gradientPaint->gradient);
    }
    LLPaintDealloc(gradientPaint);
}

void LLGradientPaintSetColors(LLGradientPaintRef gradientPaint, int count, const LLColorPaintRef * colorPaints, const CGFloat * locations) {
    CGColorRef objects[count];
    int index;
    for (index = 0; index < count; index++) {
        objects[index] = LLColorPaintGetCGColor(colorPaints[index]);
    }
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CFArrayRef colors = CFArrayCreate(NULL, (const void **)objects, count, &kCFTypeArrayCallBacks);
    gradientPaint->gradient = CGGradientCreateWithColors(colorSpace, colors, locations);
    CGColorSpaceRelease(colorSpace);
    CFRelease(colors);
}

void LLGradientPaintSetLinearLocation(LLGradientPaintRef gradientPaint, CGPoint startPoint, CGPoint endPoint) {
    gradientPaint->type = kLLGradientPaintLinear;
    gradientPaint->locationPoints[0] = startPoint;
    gradientPaint->locationPoints[1] = endPoint;
}

void LLGradientPaintSetRadialLocation(LLGradientPaintRef gradientPaint, CGPoint startCenter, CGFloat startRadius, CGPoint endCenter, CGFloat endRadius) {
    gradientPaint->type = kLLGradientPaintRadial;
    gradientPaint->locationPoints[0] = startCenter;
    gradientPaint->locationFloats[0] = startRadius;
    gradientPaint->locationPoints[1] = endCenter;
    gradientPaint->locationFloats[1] = endRadius;
}

void LLGradientPaintFillRectInContext(LLGradientPaintRef gradientPaint, CGContextRef context, CGRect rect) {
    if (gradientPaint->gradient) {
        switch (gradientPaint->type) {
            case kLLGradientPaintLinear:
                CGContextDrawLinearGradient(context, gradientPaint->gradient, gradientPaint->locationPoints[0], gradientPaint->locationPoints[1], kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
                break;
            case kLLGradientPaintRadial:
                CGContextDrawRadialGradient(context, gradientPaint->gradient, gradientPaint->locationPoints[0], gradientPaint->locationFloats[0], gradientPaint->locationPoints[1], gradientPaint->locationFloats[1], kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
                break;
        }
    }
}
