//
//  LLTransformation.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_TRANSFORMATION_H__
#define __LL_TRANSFORMATION_H__

#include "LLBase.h"

#define LLTransformationClassName "Ovaltine.Transformation"

typedef struct LLTransformation * LLTransformationRef;

LLTransformationRef LLTransformationCreate(void);

void LLTransformationRotate(LLTransformationRef transformation, double angle);
void LLTransformationTranslate(LLTransformationRef transformation, double dx, double dy);
void LLTransformationScale(LLTransformationRef transformation, double ratio);
void LLTransformationInvert(LLTransformationRef transformation);
void LLTransformationConcat(LLTransformationRef transformation1, LLTransformationRef transformation2);

LLPoint LLTransformationTransformPoint(LLTransformationRef transformation, LLPoint point);
LLRect LLTransformationTransformRect(LLTransformationRef transformation, LLRect rect);

CGAffineTransform LLTransformationGetCGAffineTransform(LLTransformationRef transformation);

#endif /* __LL_TRANSFORMATION_H__ */
