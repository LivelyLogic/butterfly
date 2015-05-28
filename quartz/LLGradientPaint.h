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

LLGradientPaintRef LLGradientPaintCreate(void);

void LLGradientPaintSetColors(LLGradientPaintRef gradientPaint, int count, const LLColorPaintRef * colorPaints, const double * locations);
void LLGradientPaintSetLinearLocation(LLGradientPaintRef gradientPaint, LLPoint startPoint, LLPoint endPoint);
void LLGradientPaintSetRadialLocation(LLGradientPaintRef gradientPaint, LLPoint startCenter, double startRadius, LLPoint endCenter, double endRadius);

#endif /* __LL_GRADIENT_PAINT_H__ */
