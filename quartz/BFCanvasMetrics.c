//
//  BFCanvasMetrics.c
//
//  Copyright (c) 2011-2019 James Rodovich
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
