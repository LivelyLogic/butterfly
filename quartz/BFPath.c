//
//  BFPath.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include <tgmath.h>

#include "BFBase.h"

#include "BFPath.h"

#include "BFQuartzTypes.h"

struct BFPath {
    struct BFBase __base;
    CGMutablePathRef pathRef;
};

static void BFPathInit(BFPathRef path);
static void BFPathDealloc(BFPathRef path);

static const BFBaseFunctions baseFunctions = {
    .name = BFPathClassName,
    .dealloc = (BFBaseDeallocFunction)&BFPathDealloc,
};

BFPathRef BFPathCreate(void) {
    BFPathRef path = BFAlloc(sizeof(struct BFPath), &baseFunctions);
    if (path) {
        BFPathInit(path);
    }
    return BFRetain(path);
}

static void BFPathInit(BFPathRef path) {
    path->pathRef = CGPathCreateMutable();
}

static void BFPathDealloc(BFPathRef path) {
    if (path) {
        CGPathRelease(path->pathRef);
    }
    BFDealloc(path);
}

void BFPathMoveToPoint(BFPathRef path, BFPoint point) {
    CGPathMoveToPoint(path->pathRef, NULL, point.x, point.y);
}

void BFPathAddLineToPoint(BFPathRef path, BFPoint point) {
    CGPathAddLineToPoint(path->pathRef, NULL, point.x, point.y);
}

void BFPathAddCurveToPoint(BFPathRef path, BFPoint point, BFPoint controlPoint1, BFPoint controlPoint2) {
    CGPathAddCurveToPoint(path->pathRef, NULL, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, point.x, point.y);
}

void BFPathAddArc(BFPathRef path, BFPoint centerPoint, double arcAngle) {
    CGPoint currentPoint = CGPathGetCurrentPoint(path->pathRef);
    double radius = hypot(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    double startAngle = atan2(currentPoint.y - centerPoint.y, currentPoint.x - centerPoint.x);
    double endAngle = startAngle + arcAngle;
    bool clockwise = (arcAngle < 0);
    CGPathAddArc(path->pathRef, NULL, centerPoint.x, centerPoint.y, radius, startAngle, endAngle, clockwise);
}

void BFPathCloseSubpath(BFPathRef path) {
    CGPathCloseSubpath(path->pathRef);
}

void BFPathAddRect(BFPathRef path, BFRect rect) {
    CGPathAddRect(path->pathRef, NULL, BFConvertRectToQuartz(rect));
}

void BFPathAddRoundedRect(BFPathRef path, BFRect rect, double radius) {
    CGPathMoveToPoint(path->pathRef, NULL, rect.left, rect.bottom + radius);
    CGPathAddArc(path->pathRef, NULL, rect.left + radius, rect.top - radius, radius, M_PI, M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, rect.right - radius, rect.top - radius, radius, M_PI_2, 0, 1);
    CGPathAddArc(path->pathRef, NULL, rect.right - radius, rect.bottom + radius, radius, 0, -M_PI_2, 1);
    CGPathAddArc(path->pathRef, NULL, rect.left + radius, rect.bottom + radius, radius, -M_PI_2, -M_PI, 1);
    CGPathCloseSubpath(path->pathRef);
}

void BFPathAddOvalInRect(BFPathRef path, BFRect rect) {
    CGPathAddEllipseInRect(path->pathRef, NULL, BFConvertRectToQuartz(rect));
}

CGPathRef BFPathGetCGPath(const BFPathRef path) {
    return path->pathRef;
}
