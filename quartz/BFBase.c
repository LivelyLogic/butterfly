//
//  BFBase.c
//
//  Copyright (c) 2011-2019 James Rodovich
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

#define BF_BASE_DEBUG_REFCOUNTS 0

#if BF_BASE_DEBUG_REFCOUNTS
int refcountTotal = 0;
#endif

void * BFAlloc(size_t size, const BFBaseFunctions * subclass) {
    BFBaseRef base = malloc(size);
    base->subclass = subclass;
    base->_refcount = 0;
    return base;
}

void BFDealloc(void * object) {
    if (object) {
        free(object);
    }
}

void * BFRetain(void * object) {
    BFBaseRef base = object;
    if (base) {
        base->_refcount++;
#if BF_BASE_DEBUG_REFCOUNTS
        printf("Retain %s %p (%d refs remaining)\n", base->subclass->name, base, base->_refcount);
        printf("(%d references total)\n", ++refcountTotal);
#endif
    }
    return object;
}

void BFRelease(void * object) {
    BFBaseRef base = object;
    if (base) {
        base->_refcount--;
#if BF_BASE_DEBUG_REFCOUNTS
        printf("Release %s %p (%d refs remaining)\n", base->subclass->name, base, base->_refcount);
        printf("(%d references total)\n", --refcountTotal);
#endif
        if (base->_refcount <= 0) {
            if (base->subclass && base->subclass->dealloc) {
                base->subclass->dealloc((void *)base);
            }
        }
    }
}

const void * BFSubclassFunctions(void * object) {
    BFBaseRef base = object;
    return base->subclass;
}

const char * BFSubclassName(void * object) {
    BFBaseRef base = object;
    const BFBaseFunctions * subclass = base->subclass;
    return subclass->name;
}
