//
//  LLPath.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include <tgmath.h>

#include "LLPath.h"

struct LLPath {
    struct LLBase __base;
    CGMutablePathRef pathRef;
};

static void LLPathInit(LLPathRef path);
static void LLPathDealloc(LLPathRef path);

static const LLBaseFunctions baseFunctions = {
    .name = LLPathClassName,
    .dealloc = (LLBaseDeallocFunction)&LLPathDealloc,
};

LLPathRef LLPathCreate(void) {
    LLPathRef path = LLAlloc(sizeof(struct LLPath), &baseFunctions);
    if (path) {
        LLPathInit(path);
    }
    return LLRetain(path);
}

static void LLPathInit(LLPathRef path) {
    path->pathRef = CGPathCreateMutable();
}

static void LLPathDealloc(LLPathRef path) {
    if (path) {
        CGPathRelease(path->pathRef);
    }
    LLDealloc(path);
}

void LLPathMoveToPoint(LLPathRef path, CGPoint aPoint) {
    CGPathMoveToPoint(path->pathRef, NULL, aPoint.x, aPoint.y);
}

void LLPathAddLineToPoint(LLPathRef path, CGPoint aPoint) {
    CGPathAddLineToPoint(path->pathRef, NULL, aPoint.x, aPoint.y);
}

void LLPathAddCurveToPoint(LLPathRef path, CGPoint aPoint, CGPoint controlPoint1, CGPoint controlPoint2) {
    CGPathAddCurveToPoint(path->pathRef, NULL, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, aPoint.x, aPoint.y);
}

void LLPathAddArc(LLPathRef path, CGPoint centerPoint, CGFloat arcAngle) {
    CGPoint currentPoint = CGPathGetCurrentPoint(path->pathRef);
    CGFloat radius = hypot(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    CGFloat startAngle = atan2(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    CGFloat endAngle = startAngle + arcAngle;
    bool clockwise = (arcAngle < 0);
    CGPathAddArc(path->pathRef, NULL, centerPoint.x, centerPoint.y, radius, startAngle, endAngle, clockwise);
}

void LLPathCloseSubpath(LLPathRef path) {
    CGPathCloseSubpath(path->pathRef);
}

void LLPathAddRect(LLPathRef path, CGRect aRect) {
    CGPathAddRect(path->pathRef, NULL, aRect);
}

void LLPathAddRoundedRect(LLPathRef path, CGRect aRect, CGFloat radius) {
    CGPathMoveToPoint(path->pathRef, NULL, aRect.origin.x, aRect.origin.y + radius);
    CGPathAddArc(path->pathRef, NULL, aRect.origin.x + radius, aRect.origin.y + aRect.size.height - radius, radius, M_PI, M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, aRect.origin.x + aRect.size.width - radius, aRect.origin.y + aRect.size.height - radius, radius, M_PI_2, 0, 1);
    CGPathAddArc(path->pathRef, NULL, aRect.origin.x + aRect.size.width - radius, aRect.origin.y + radius, radius, 0, -M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, aRect.origin.x + radius, aRect.origin.y + radius, radius, -M_PI_2, -M_PI, 1);
    CGPathCloseSubpath(path->pathRef);
}

void LLPathAddOvalInRect(LLPathRef path, CGRect aRect) {
    CGPathAddEllipseInRect(path->pathRef, NULL, aRect);
}

CGPathRef LLPathGetCGPath(const LLPathRef path) {
    return path->pathRef;
}
