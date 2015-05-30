//
//  BFBase.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#define BF_BASE_DEBUG_REFCOUNTS 0

#if BF_BASE_DEBUG_REFCOUNTS
int refcountTotal = 0;
#endif

void * BFAlloc(size_t size, const BFBaseFunctions * subclass) {
    BFBaseRef base = malloc(size);
    base->subclass = subclass;
    base->refcount = 0;
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
        base->refcount++;
#if BF_BASE_DEBUG_REFCOUNTS
        printf("Retain %s %p (%d refs remaining)\n", base->subclass->name, base, base->refcount);
        printf("(%d references total)\n", ++refcountTotal);
#endif
    }
    return object;
}

void BFRelease(void * object) {
    BFBaseRef base = object;
    if (base) {
        base->refcount--;
#if BF_BASE_DEBUG_REFCOUNTS
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

const void * BFSubclassFunctions(void * object) {
    BFBaseRef base = object;
    return base->subclass;
}

const char * BFSubclassName(void * object) {
    BFBaseRef base = object;
    const BFBaseFunctions * subclass = base->subclass;
    return subclass->name;
}
