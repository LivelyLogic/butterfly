//
//  LLCanvasMetrics.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLCanvasMetrics.h"

struct LLCanvasMetrics {
    struct LLBase __base;
    CGRect boundsRect;
    CGFloat backingScale;
    CGFloat pointScale;
};

static void LLCanvasMetricsInit(LLCanvasMetricsRef canvasMetrics, CGRect boundsRect, CGFloat backingScale, CGFloat pointScale);
static void LLCanvasMetricsDealloc(LLCanvasMetricsRef canvasMetrics);

static const LLBaseFunctions baseFunctions = {
    .name = LLCanvasMetricsClassName,
    .dealloc = (LLBaseDeallocFunction)&LLCanvasMetricsDealloc,
};

LLCanvasMetricsRef LLCanvasMetricsCreate(CGRect boundsRect, CGFloat backingScale, CGFloat pointScale) {
    LLCanvasMetricsRef canvasMetrics = LLAlloc(sizeof(struct LLCanvasMetrics), &baseFunctions);
    if (canvasMetrics) {
        LLCanvasMetricsInit(canvasMetrics, boundsRect, backingScale, pointScale);
    }
    return LLRetain(canvasMetrics);
}

static void LLCanvasMetricsInit(LLCanvasMetricsRef canvasMetrics, CGRect boundsRect, CGFloat backingScale, CGFloat pointScale) {
    canvasMetrics->boundsRect = boundsRect;
    canvasMetrics->backingScale = backingScale;
    canvasMetrics->pointScale = pointScale;
}

static void LLCanvasMetricsDealloc(LLCanvasMetricsRef canvasMetrics) {
    if (canvasMetrics) {
    }
    LLDealloc(canvasMetrics);
}

CGRect LLCanvasMetricsGetBoundsRect(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->boundsRect;
}

CGFloat LLCanvasMetricsGetBackingScale(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->backingScale;
}

CGFloat LLCanvasMetricsGetPointScale(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->pointScale;
}
