//
//  LLTransformation.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLTransformation.h"

#include "LLQuartzTypes.h"

struct LLTransformation {
    struct LLBase __base;
    CGAffineTransform affine;
};

static void LLTransformationInit(LLTransformationRef transformation);
static void LLTransformationDealloc(LLTransformationRef transformation);

static const LLBaseFunctions baseFunctions = {
    .name = LLTransformationClassName,
    .dealloc = (LLBaseDeallocFunction)&LLTransformationDealloc,
};

LLTransformationRef LLTransformationCreate(void) {
    LLTransformationRef transformation = LLAlloc(sizeof(struct LLTransformation), &baseFunctions);
    if (transformation) {
        LLTransformationInit(transformation);
    }
    return LLRetain(transformation);
}

static void LLTransformationInit(LLTransformationRef transformation) {
    transformation->affine = CGAffineTransformIdentity;
}

static void LLTransformationDealloc(LLTransformationRef transformation) {
    if (transformation) {
    }
    LLDealloc(transformation);
}

void LLTransformationRotate(LLTransformationRef transformation, double angle) {
    transformation->affine = CGAffineTransformRotate(transformation->affine, angle);
}

void LLTransformationTranslate(LLTransformationRef transformation, double dx, double dy) {
    transformation->affine = CGAffineTransformTranslate(transformation->affine, dx, dy);
}

void LLTransformationScale(LLTransformationRef transformation, double ratio) {
    transformation->affine = CGAffineTransformScale(transformation->affine, ratio, ratio);
}

void LLTransformationInvert(LLTransformationRef transformation) {
    transformation->affine = CGAffineTransformInvert(transformation->affine);
}

void LLTransformationConcat(LLTransformationRef transformation1, LLTransformationRef transformation2) {
    transformation1->affine = CGAffineTransformConcat(transformation1->affine, transformation2->affine);
}

LLPoint LLTransformationTransformPoint(LLTransformationRef transformation, LLPoint point) {
    CGPoint cgPoint = CGPointMake(point.x, point.y);
    cgPoint = CGPointApplyAffineTransform(cgPoint, transformation->affine);
    return (LLPoint){ .x = cgPoint.x, .y = cgPoint.y };
}

LLRect LLTransformationTransformRect(LLTransformationRef transformation, LLRect rect) {
    CGRect cgRect = LLConvertRectToQuartz(rect);
    cgRect = CGRectApplyAffineTransform(cgRect, transformation->affine);
    return LLConvertQuartzRect(cgRect);
}

CGAffineTransform LLTransformationGetCGAffineTransform(LLTransformationRef transformation) {
    return transformation->affine;
}
