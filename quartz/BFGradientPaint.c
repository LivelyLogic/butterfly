//
//  BFGradientPaint.c
//
//  Copyright (c) 2011-2019 James Rodovich
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

#include "BFPaint.h"

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
