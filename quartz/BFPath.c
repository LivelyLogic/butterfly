//
//  BFPath.c
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

#include <tgmath.h>

#include "butterfly.h"
#include "quartz.h"

struct BFPath {
    struct BFBase __base;
    CGMutablePathRef pathRef;
};

static void BFPathInit(BFPathRef path);
static void BFPathDealloc(BFPathRef path);

typedef struct BFPathCGPathApplierFunctionUserData {
    BFPathComponentIterationFunction iterationFunction;
    void * userData;
} BFPathCGPathApplierFunctionUserData;
static void BFPathCGPathApplierFunction(void * userData, const CGPathElement * element);

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

void BFPathAddQuadCurveToPoint(BFPathRef path, BFPoint point, BFPoint controlPoint) {
    CGPathAddQuadCurveToPoint(path->pathRef, NULL, controlPoint.x, controlPoint.y, point.x, point.y);
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
    CGPathAddRect(path->pathRef, NULL, BFRectToCGRect(rect));
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
    CGPathAddEllipseInRect(path->pathRef, NULL, BFRectToCGRect(rect));
}

void BFPathIterateComponents(BFPathRef path, BFPathComponentIterationFunction iterationFunction, void * userData) {
    BFPathCGPathApplierFunctionUserData cgUserData = { .iterationFunction = iterationFunction, .userData = userData };
    CGPathApply(path->pathRef, &cgUserData, BFPathCGPathApplierFunction);
}

void BFPathCGPathApplierFunction(void * cgUserData, const CGPathElement * element) {
    BFPathCGPathApplierFunctionUserData * userData = (BFPathCGPathApplierFunctionUserData *)cgUserData;
    BFPathComponentType type;
    BFPoint point = {};
    BFPoint controlPoint1 = {};
    BFPoint controlPoint2 = {};
    switch (element->type) {
        case kCGPathElementMoveToPoint:
            type = kBFPathComponentMove;
            point.x = element->points[0].x;
            point.y = element->points[0].y;
            break;
        case kCGPathElementAddLineToPoint:
            type = kBFPathComponentAddLine;
            point.x = element->points[0].x;
            point.y = element->points[0].y;
            break;
        case kCGPathElementAddQuadCurveToPoint:
            type = kBFPathComponentAddQuadCurve;
            controlPoint1.x = element->points[0].x;
            controlPoint1.y = element->points[0].y;
            point.x = element->points[1].x;
            point.y = element->points[1].y;
            break;
        case kCGPathElementAddCurveToPoint:
            type = kBFPathComponentAddCurve;
            controlPoint1.x = element->points[0].x;
            controlPoint1.y = element->points[0].y;
            controlPoint2.x = element->points[1].x;
            controlPoint2.y = element->points[1].y;
            point.x = element->points[2].x;
            point.y = element->points[2].y;
            break;
        case kCGPathElementCloseSubpath:
            type = kBFPathComponentCloseSubpath;
            break;
        default:
            return;
    }
    userData->iterationFunction(userData->userData, type, point, controlPoint1, controlPoint2);
}

CGPathRef BFPathGetCGPath(const BFPathRef path) {
    return path->pathRef;
}
