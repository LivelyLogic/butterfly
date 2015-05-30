//
//  BFCanvasMetrics.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFCanvasMetrics.h"

struct BFCanvasMetrics {
    struct BFBase __base;
    BFRect boundsRect;
    double backingScale;
    double pointScale;
};

static void BFCanvasMetricsInit(BFCanvasMetricsRef canvasMetrics, BFRect boundsRect, double backingScale, double pointScale);
static void BFCanvasMetricsDealloc(BFCanvasMetricsRef canvasMetrics);

static const BFBaseFunctions baseFunctions = {
    .name = BFCanvasMetricsClassName,
    .dealloc = (BFBaseDeallocFunction)&BFCanvasMetricsDealloc,
};

BFCanvasMetricsRef BFCanvasMetricsCreate(BFRect boundsRect, double backingScale, double pointScale) {
    BFCanvasMetricsRef canvasMetrics = BFAlloc(sizeof(struct BFCanvasMetrics), &baseFunctions);
    if (canvasMetrics) {
        BFCanvasMetricsInit(canvasMetrics, boundsRect, backingScale, pointScale);
    }
    return BFRetain(canvasMetrics);
}

static void BFCanvasMetricsInit(BFCanvasMetricsRef canvasMetrics, BFRect boundsRect, double backingScale, double pointScale) {
    canvasMetrics->boundsRect = boundsRect;
    canvasMetrics->backingScale = backingScale;
    canvasMetrics->pointScale = pointScale;
}

static void BFCanvasMetricsDealloc(BFCanvasMetricsRef canvasMetrics) {
    if (canvasMetrics) {
    }
    BFDealloc(canvasMetrics);
}

BFRect BFCanvasMetricsGetBoundsRect(BFCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->boundsRect;
}

double BFCanvasMetricsGetBackingScale(BFCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->backingScale;
}

double BFCanvasMetricsGetPointScale(BFCanvasMetricsRef canvasMetrics) {
    return canvasMetrics->pointScale;
}
