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

LLCanvasMetricsRef LLCanvasMetricsCreate(CGRect boundsRect, CGFloat backingScale, CGFloat pointScale);

CGRect LLCanvasMetricsGetBoundsRect(LLCanvasMetricsRef canvasMetrics);
CGFloat LLCanvasMetricsGetBackingScale(LLCanvasMetricsRef canvasMetrics);
CGFloat LLCanvasMetricsGetPointScale(LLCanvasMetricsRef canvasMetrics);

#endif /* __LL_CANVAS_METRICS_H__ */
