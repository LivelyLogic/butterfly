//
//  BFTransformation.c
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

#include "BFQuartzTypes.h"

struct BFTransformation {
    struct BFBase __base;
    CGAffineTransform affine;
};

static void BFTransformationInit(BFTransformationRef transformation);
static void BFTransformationDealloc(BFTransformationRef transformation);

static const BFBaseFunctions baseFunctions = {
    .name = BFTransformationClassName,
    .dealloc = (BFBaseDeallocFunction)&BFTransformationDealloc,
};

BFTransformationRef BFTransformationCreate(void) {
    BFTransformationRef transformation = BFAlloc(sizeof(struct BFTransformation), &baseFunctions);
    if (transformation) {
        BFTransformationInit(transformation);
    }
    return BFRetain(transformation);
}

static void BFTransformationInit(BFTransformationRef transformation) {
    transformation->affine = CGAffineTransformIdentity;
}

static void BFTransformationDealloc(BFTransformationRef transformation) {
    if (transformation) {
    }
    BFDealloc(transformation);
}

void BFTransformationRotate(BFTransformationRef transformation, double angle) {
    transformation->affine = CGAffineTransformRotate(transformation->affine, angle);
}

void BFTransformationTranslate(BFTransformationRef transformation, double dx, double dy) {
    transformation->affine = CGAffineTransformTranslate(transformation->affine, dx, dy);
}

void BFTransformationScale(BFTransformationRef transformation, double ratio) {
    transformation->affine = CGAffineTransformScale(transformation->affine, ratio, ratio);
}

void BFTransformationInvert(BFTransformationRef transformation) {
    transformation->affine = CGAffineTransformInvert(transformation->affine);
}

void BFTransformationConcat(BFTransformationRef transformation1, BFTransformationRef transformation2) {
    transformation1->affine = CGAffineTransformConcat(transformation1->affine, transformation2->affine);
}

BFPoint BFTransformationTransformPoint(BFTransformationRef transformation, BFPoint point) {
    CGPoint cgPoint = CGPointMake(point.x, point.y);
    cgPoint = CGPointApplyAffineTransform(cgPoint, transformation->affine);
    return (BFPoint){ .x = cgPoint.x, .y = cgPoint.y };
}

BFRect BFTransformationTransformRect(BFTransformationRef transformation, BFRect rect) {
    CGRect cgRect = BFRectToCGRect(rect);
    cgRect = CGRectApplyAffineTransform(cgRect, transformation->affine);
    return BFRectFromCGRect(cgRect);
}

BFTransformationComponents BFTransformationGetComponents(BFTransformationRef transformation) {
    BFTransformationComponents components = {
        .a = transformation->affine.a,
        .b = transformation->affine.b,
        .c = transformation->affine.c,
        .d = transformation->affine.d,
        .tx = transformation->affine.tx,
        .ty = transformation->affine.ty,
    };
    return components;
}

CGAffineTransform BFTransformationGetCGAffineTransform(BFTransformationRef transformation) {
    return transformation->affine;
}
