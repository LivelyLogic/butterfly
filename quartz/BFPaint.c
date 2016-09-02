//
//  BFPaint.c
//
//  Copyright (c) 2011-2016 Ripeware, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "butterfly.h"
#include "quartz.h"

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
