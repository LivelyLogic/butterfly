//
//  LLCanvasMetrics.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLBase.h"

#include "LLCanvasMetrics.h"

struct LLCanvasMetrics {
    struct LLBase __base;
    LLRect boundsRect;
    double backingScale;
    double pointScale;
};

static void LLCanvasMetricsInit(LLCanvasMetricsRef canvasMetrics, LLRect boundsRect, double backingScale, double pointScale);
static void LLCanvasMetricsDealloc(LLCanvasMetricsRef canvasMetrics);

static const LLBaseFunctions baseFunctions = {
    .name = LLCanvasMetricsClassName,
    .dealloc = (LLBaseDeallocFunction)&LLCanvasMetricsDealloc,
};

LLCanvasMetricsRef LLCanvasMetricsCreate(LLRect boundsRect, double backingScale, double pointScale) {
    LLCanvasMetricsRef canvasMetrics = LLAlloc(sizeof(struct LLCanvasMetrics), &baseFunctions);
    if (canvasMetrics) {
        LLCanvasMetricsInit(canvasMetrics, boundsRect, backingScale, pointScale);
    }
    return LLRetain(canvasMetrics);
}

static void LLCanvasMetricsInit(LLCanvasMetricsRef canvasMetrics, LLRect boundsRect, double backingScale, double pointScale) {
    canvasMetrics->boundsRect = boundsRect;
    canvasMetrics->backingScale = backingScale;
    canvasMetrics->pointScale = pointScale;
}

static void LLCanvasMetricsDealloc(LLCanvasMetricsRef canvasMetrics) {
    if (canvasMetrics) {
    }
    LLDealloc(canvasMetrics);
}

LLRect LLCanvasMetricsGetBoundsRect(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->boundsRect;
}

double LLCanvasMetricsGetBackingScale(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->backingScale;
}

double LLCanvasMetricsGetPointScale(LLCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->pointScale;
}
