//
//  LLBase.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLBase.h"

#define LL_BASE_DEBUG_REFCOUNTS 0

#if LL_BASE_DEBUG_REFCOUNTS
int refcountTotal = 0;
#endif

void * LLAlloc(size_t size, const LLBaseFunctions * subclass) {
    LLBaseRef base = malloc(size);
    base->subclass = subclass;
    base->refcount = 0;
    return base;
}

void LLDealloc(void * object) {
    if (object) {
        free(object);
    }
}

void * LLRetain(void * object) {
    LLBaseRef base = object;
    if (base) {
        base->refcount++;
#if LL_BASE_DEBUG_REFCOUNTS
        printf("Retain %s %p (%d refs remaining)\n", base->subclass->name, base, base->refcount);
        printf("(%d references total)\n", ++refcountTotal);
#endif
    }
    return object;
}

void LLRelease(void * object) {
    LLBaseRef base = object;
    if (base) {
        base->refcount--;
#if LL_BASE_DEBUG_REFCOUNTS
        printf("Release %s %p (%d refs remaining)\n", base->subclass->name, base, base->refcount);
        printf("(%d references total)\n", --refcountTotal);
#endif
        if (base->refcount <= 0) {
            if (base->subclass && base->subclass->dealloc) {
                base->subclass->dealloc((void *)base);
            }
        }
    }
}

const void * LLSubclassFunctions(void * object) {
    LLBaseRef base = object;
    return base->subclass;
}

const char * LLSubclassName(void * object) {
    LLBaseRef base = object;
    const LLBaseFunctions * subclass = base->subclass;
    return subclass->name;
}
