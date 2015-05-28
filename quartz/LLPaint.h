//
//  LLPaint.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_PAINT_H__
#define __LL_PAINT_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

#include "LLBase.h"

typedef void (* LLPaintDeallocFunction)(void *);
typedef void (* LLPaintSetInContextFunction)(void *, CGContextRef);
typedef void (* LLPaintFillRectInContextFunction)(void *, CGContextRef, CGRect);
typedef struct LLPaintFunctions {
    LLBaseFunctions __base;
    LLPaintDeallocFunction dealloc;
    LLPaintSetInContextFunction setInContext;
    LLPaintFillRectInContextFunction fillRectInContext;
} LLPaintFunctions;

struct LLPaint {
    struct LLBase __base;
};

void LLPaintDealloc(void * paint);

bool LLPaintSetInContext(LLPaintRef paint, CGContextRef context);
void LLPaintFillRectInContext(LLPaintRef paint, CGContextRef context, CGRect rect);

#endif /* __LL_PAINT_H__ */
