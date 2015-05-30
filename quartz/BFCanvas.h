//
//  BFCanvas.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_CANVAS_H__
#define __BF_CANVAS_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

BFCanvasRef BFCanvasCreateForDisplay(CGContextRef context, BFCanvasMetricsRef metrics);

CGContextRef BFCanvasGetCGContext(BFCanvasRef canvas);

#endif /* __BF_CANVAS_H__ */
