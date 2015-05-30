//
//  BFPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFPaint.h"

void BFPaintDealloc(void * object) {
    BFDealloc(object);
}

bool BFPaintSetInContext(BFPaintRef paint, CGContextRef context) {
    bool result = false;
    const BFPaintFunctions * subclass = BFSubclassFunctions(paint);
    if (subclass && subclass->setInContext) {
        subclass->setInContext(paint, context);
        result = true;
    }
    return result;
}

void BFPaintFillRectInContext(BFPaintRef paint, CGContextRef context, CGRect rect) {
    const BFPaintFunctions * subclass = BFSubclassFunctions(paint);
    if (subclass && subclass->fillRectInContext) {
        subclass->fillRectInContext(paint, context, rect);
    }
}
