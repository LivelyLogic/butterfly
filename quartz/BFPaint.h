//
//  BFPaint.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_PAINT_H__
#define __BF_PAINT_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

#include "BFBase.h"

typedef void (* BFPaintDeallocFunction)(void *);
typedef void (* BFPaintSetInContextFunction)(void *, CGContextRef);
typedef void (* BFPaintFillRectInContextFunction)(void *, CGContextRef, CGRect);
typedef struct BFPaintFunctions {
    BFBaseFunctions __base;
    BFPaintDeallocFunction dealloc;
    BFPaintSetInContextFunction setInContext;
    BFPaintFillRectInContextFunction fillRectInContext;
} BFPaintFunctions;

struct BFPaint {
    struct BFBase __base;
};

void BFPaintDealloc(void * paint);

bool BFPaintSetInContext(BFPaintRef paint, CGContextRef context);
void BFPaintFillRectInContext(BFPaintRef paint, CGContextRef context, CGRect rect);

#endif /* __BF_PAINT_H__ */
