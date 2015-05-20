//
//  LLCanvasMetrics.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_CANVAS_METRICS_H__
#define __LL_CANVAS_METRICS_H__

#include "LLBase.h"

#define LLCanvasMetricsClassName "Ovaltine.CanvasMetrics"

typedef struct LLCanvasMetrics * LLCanvasMetricsRef;

LLCanvasMetricsRef LLCanvasMetricsCreate(LLRect boundsRect, double backingScale, double pointScale);

LLRect LLCanvasMetricsGetBoundsRect(LLCanvasMetricsRef canvasMetrics);
double LLCanvasMetricsGetBackingScale(LLCanvasMetricsRef canvasMetrics);
double LLCanvasMetricsGetPointScale(LLCanvasMetricsRef canvasMetrics);

#endif /* __LL_CANVAS_METRICS_H__ */
