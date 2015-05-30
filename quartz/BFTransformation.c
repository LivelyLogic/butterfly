//
//  BFTransformation.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFTransformation.h"

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
    CGRect cgRect = BFConvertRectToQuartz(rect);
    cgRect = CGRectApplyAffineTransform(cgRect, transformation->affine);
    return BFConvertQuartzRect(cgRect);
}

CGAffineTransform BFTransformationGetCGAffineTransform(BFTransformationRef transformation) {
    return transformation->affine;
}
