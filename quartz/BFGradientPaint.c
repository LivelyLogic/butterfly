//
//  BFGradientPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"
#include "BFPaint.h"

#include "butterfly.h"
#include "quartz.h"

typedef enum BFGradientPaintType {
    kBFGradientPaintLinear,
    kBFGradientPaintRadial,
} BFGradientPaintType;

struct BFGradientPaint {
    struct BFPaint __base;
    CGGradientRef gradient;
    BFGradientPaintType type;
    CGPoint locationPoints[2];
    double locationFloats[2];
};

static void BFGradientPaintInit(BFGradientPaintRef gradientPaint);
static void BFGradientPaintDealloc(BFGradientPaintRef gradientPaint);
static void BFGradientPaintFillRectInContext(BFGradientPaintRef gradientPaint, CGContextRef context, CGRect rect);

static const BFPaintFunctions baseFunctions = {
    .__base = {
        .name = BFGradientPaintClassName,
        .dealloc = (BFBaseDeallocFunction)&BFGradientPaintDealloc,
    },
    .fillRectInContext = (BFPaintFillRectInContextFunction)&BFGradientPaintFillRectInContext,
};

BFGradientPaintRef BFGradientPaintCreate(void) {
    BFGradientPaintRef gradientPaint = BFAlloc(sizeof(struct BFGradientPaint), (const BFBaseFunctions *)&baseFunctions);
    if (gradientPaint) {
        BFGradientPaintInit(gradientPaint);
    }
    return BFRetain(gradientPaint);
}

static void BFGradientPaintInit(BFGradientPaintRef gradientPaint) {
    gradientPaint->gradient = NULL;
}

static void BFGradientPaintDealloc(BFGradientPaintRef gradientPaint) {
    if (gradientPaint) {
        CGGradientRelease(gradientPaint->gradient);
    }
    BFPaintDealloc(gradientPaint);
}

void BFGradientPaintSetColors(BFGradientPaintRef gradientPaint, int count, const BFColorPaintRef * colorPaints, const double * locations) {
    CGColorRef objects[count];
    int index;
    for (index = 0; index < count; index++) {
        objects[index] = BFColorPaintGetCGColor(colorPaints[index]);
    }
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CFArrayRef colors = CFArrayCreate(NULL, (const void **)objects, count, &kCFTypeArrayCallBacks);
    gradientPaint->gradient = CGGradientCreateWithColors(colorSpace, colors, locations);
    CGColorSpaceRelease(colorSpace);
    CFRelease(colors);
}

void BFGradientPaintSetLinearLocation(BFGradientPaintRef gradientPaint, BFPoint startPoint, BFPoint endPoint) {
    gradientPaint->type = kBFGradientPaintLinear;
    gradientPaint->locationPoints[0] = BFPointToCGPoint(startPoint);
    gradientPaint->locationPoints[1] = BFPointToCGPoint(endPoint);
}

void BFGradientPaintSetRadialLocation(BFGradientPaintRef gradientPaint, BFPoint startCenter, double startRadius, BFPoint endCenter, double endRadius) {
    gradientPaint->type = kBFGradientPaintRadial;
    gradientPaint->locationPoints[0] = BFPointToCGPoint(startCenter);
    gradientPaint->locationFloats[0] = startRadius;
    gradientPaint->locationPoints[1] = BFPointToCGPoint(endCenter);
    gradientPaint->locationFloats[1] = endRadius;
}

void BFGradientPaintFillRectInContext(BFGradientPaintRef gradientPaint, CGContextRef context, CGRect rect) {
    if (gradientPaint->gradient) {
        switch (gradientPaint->type) {
            case kBFGradientPaintLinear:
                CGContextDrawLinearGradient(context, gradientPaint->gradient, gradientPaint->locationPoints[0], gradientPaint->locationPoints[1], kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
                break;
            case kBFGradientPaintRadial:
                CGContextDrawRadialGradient(context, gradientPaint->gradient, gradientPaint->locationPoints[0], gradientPaint->locationFloats[0], gradientPaint->locationPoints[1], gradientPaint->locationFloats[1], kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
                break;
        }
    }
}
