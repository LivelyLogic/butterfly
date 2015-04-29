//
//  LLCanvas.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_CANVAS_H__
#define __LL_CANVAS_H__

#include "LLBase.h"

#include "LLCanvasMetrics.h"
#include "LLFont.h"
#include "LLPaint.h"
#include "LLPaintMode.h"
#include "LLPath.h"
#include "LLStyledString.h"
#include "LLTransformation.h"

#define LLCanvasClassName "Ovaltine.Canvas"

typedef struct LLCanvas * LLCanvasRef;

LLCanvasRef LLCanvasCreateForDisplay(CGContextRef context, LLCanvasMetricsRef metrics);
LLCanvasRef LLCanvasCreateForHitTest(LLCanvasMetricsRef metrics);

CGContextRef LLCanvasGetCGContext(LLCanvasRef canvas);
LLCanvasMetricsRef LLCanvasGetMetrics(LLCanvasRef canvas);
void LLCanvasSetDirtyRect(LLCanvasRef canvas, CGRect rect);
CGRect LLCanvasGetDirtyRect(LLCanvasRef canvas);
void LLCanvasSetOpacity(LLCanvasRef canvas, double opacity);
void LLCanvasSetPaint(LLCanvasRef canvas, LLPaintRef paint);
void LLCanvasSetPaintMode(LLCanvasRef canvas, LLPaintModeRef paintMode);
void LLCanvasSetFont(LLCanvasRef canvas, LLFontRef font);
LLFontRef LLCanvasGetFont(LLCanvasRef canvas);
void LLCanvasSetThickness(LLCanvasRef canvas, CGFloat thickness);
void LLCanvasConcatTransformation(LLCanvasRef canvas, LLTransformationRef transformation);
void LLCanvasClipRect(LLCanvasRef canvas, CGRect rect);
void LLCanvasClipPath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasPush(LLCanvasRef canvas);
void LLCanvasPop(LLCanvasRef canvas);
void LLCanvasNukeStack(LLCanvasRef canvas);

void LLCanvasFillPath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasStrokePath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasDrawStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, CGPoint point);
void LLCanvasStrokeStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, CGPoint point);

bool LLCanvasIsHitTest(LLCanvasRef canvas);
bool LLCanvasPerformHitTest(LLCanvasRef canvas);

#endif /* __LL_CANVAS_H__ */
