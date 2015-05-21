//
//  LLPath.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_PATH_H__
#define __LL_PATH_H__

#include "LLBase.h"

#define LLPathClassName "Ovaltine.Path"

LLPathRef LLPathCreate(void);

void LLPathMoveToPoint(LLPathRef path, LLPoint point);
void LLPathAddLineToPoint(LLPathRef path, LLPoint point);
void LLPathAddCurveToPoint(LLPathRef path, LLPoint point, LLPoint controlPoint1, LLPoint controlPoint2);
void LLPathAddArc(LLPathRef path, LLPoint centerPoint, double arcAngle);
void LLPathCloseSubpath(LLPathRef path);
void LLPathAddRect(LLPathRef path, LLRect rect);
void LLPathAddRoundedRect(LLPathRef path, LLRect rect, double radius);
void LLPathAddOvalInRect(LLPathRef path, LLRect rect);

CGPathRef LLPathGetCGPath(const LLPathRef path);

#endif /* __LL_PATH_H__ */
