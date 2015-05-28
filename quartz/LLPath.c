//
//  LLPath.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include <tgmath.h>

#include "LLBase.h"

#include "LLPath.h"

#include "LLQuartzTypes.h"

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

void LLPathMoveToPoint(LLPathRef path, LLPoint point) {
    CGPathMoveToPoint(path->pathRef, NULL, point.x, point.y);
}

void LLPathAddLineToPoint(LLPathRef path, LLPoint point) {
    CGPathAddLineToPoint(path->pathRef, NULL, point.x, point.y);
}

void LLPathAddCurveToPoint(LLPathRef path, LLPoint point, LLPoint controlPoint1, LLPoint controlPoint2) {
    CGPathAddCurveToPoint(path->pathRef, NULL, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, point.x, point.y);
}

void LLPathAddArc(LLPathRef path, LLPoint centerPoint, double arcAngle) {
    CGPoint currentPoint = CGPathGetCurrentPoint(path->pathRef);
    double radius = hypot(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    double startAngle = atan2(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    double endAngle = startAngle + arcAngle;
    bool clockwise = (arcAngle < 0);
    CGPathAddArc(path->pathRef, NULL, centerPoint.x, centerPoint.y, radius, startAngle, endAngle, clockwise);
}

void LLPathCloseSubpath(LLPathRef path) {
    CGPathCloseSubpath(path->pathRef);
}

void LLPathAddRect(LLPathRef path, LLRect rect) {
    CGPathAddRect(path->pathRef, NULL, LLConvertRectToQuartz(rect));
}

void LLPathAddRoundedRect(LLPathRef path, LLRect rect, double radius) {
    CGPathMoveToPoint(path->pathRef, NULL, rect.left, rect.bottom + radius);
    CGPathAddArc(path->pathRef, NULL, rect.left + radius, rect.top - radius, radius, M_PI, M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, rect.right - radius, rect.top - radius, radius, M_PI_2, 0, 1);
    CGPathAddArc(path->pathRef, NULL, rect.right - radius, rect.bottom + radius, radius, 0, -M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, rect.left + radius, rect.bottom + radius, radius, -M_PI_2, -M_PI, 1);
    CGPathCloseSubpath(path->pathRef);
}

void LLPathAddOvalInRect(LLPathRef path, LLRect rect) {
    CGPathAddEllipseInRect(path->pathRef, NULL, LLConvertRectToQuartz(rect));
}

CGPathRef LLPathGetCGPath(const LLPathRef path) {
    return path->pathRef;
}
