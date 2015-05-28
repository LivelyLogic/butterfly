//
//  LLCanvas.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_CANVAS_H__
#define __LL_CANVAS_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

LLCanvasRef LLCanvasCreateForDisplay(CGContextRef context, LLCanvasMetricsRef metrics);

CGContextRef LLCanvasGetCGContext(LLCanvasRef canvas);

#endif /* __LL_CANVAS_H__ */
