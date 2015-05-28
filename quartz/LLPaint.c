//
//  LLPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLBase.h"

#include "LLPaint.h"

void LLPaintDealloc(void * object) {
    LLDealloc(object);
}

bool LLPaintSetInContext(LLPaintRef paint, CGContextRef context) {
    bool result = false;
    const LLPaintFunctions * subclass = LLSubclassFunctions(paint);
    if (subclass && subclass->setInContext) {
        subclass->setInContext(paint, context);
        result = true;
    }
    return result;
}

void LLPaintFillRectInContext(LLPaintRef paint, CGContextRef context, CGRect rect) {
    const LLPaintFunctions * subclass = LLSubclassFunctions(paint);
    if (subclass && subclass->fillRectInContext) {
        subclass->fillRectInContext(paint, context, rect);
    }
}
