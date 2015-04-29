//
//  LLPath.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_PATH_H__
#define __LL_PATH_H__

#include "LLBase.h"

#define LLPathClassName "Ovaltine.Path"

typedef struct LLPath * LLPathRef;

LLPathRef LLPathCreate(void);

void LLPathMoveToPoint(LLPathRef path, CGPoint aPoint);
void LLPathAddLineToPoint(LLPathRef path, CGPoint aPoint);
void LLPathAddCurveToPoint(LLPathRef path, CGPoint aPoint, CGPoint controlPoint1, CGPoint controlPoint2);
void LLPathAddArc(LLPathRef path, CGPoint centerPoint, CGFloat arcAngle);
void LLPathCloseSubpath(LLPathRef path);
void LLPathAddRect(LLPathRef path, CGRect aRect);
void LLPathAddRoundedRect(LLPathRef path, CGRect aRect, CGFloat radius);
void LLPathAddOvalInRect(LLPathRef path, CGRect aRect);

CGPathRef LLPathGetCGPath(const LLPathRef path);

#endif /* __LL_PATH_H__ */
