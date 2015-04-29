//
//  LLGradientPaint.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_GRADIENT_PAINT_H__
#define __LL_GRADIENT_PAINT_H__

#include "LLBase.h"

#include "LLColorPaint.h"

#define LLGradientPaintClassName "Ovaltine.GradientPaint"

typedef struct LLGradientPaint * LLGradientPaintRef;

LLGradientPaintRef LLGradientPaintCreate(void);

void LLGradientPaintSetColors(LLGradientPaintRef gradientPaint, int count, const LLColorPaintRef * colorPaints, const CGFloat * locations);
void LLGradientPaintSetLinearLocation(LLGradientPaintRef gradientPaint, CGPoint startPoint, CGPoint endPoint);
void LLGradientPaintSetRadialLocation(LLGradientPaintRef gradientPaint, CGPoint startCenter, CGFloat startRadius, CGPoint endCenter, CGFloat endRadius);
void LLGradientPaintFillRectInContext(LLGradientPaintRef gradientPaint, CGContextRef context, CGRect rect);

#endif /* __LL_GRADIENT_PAINT_H__ */
