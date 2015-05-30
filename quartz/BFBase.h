//
//  BFBase.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_BASE_H__
#define __BF_BASE_H__

#include "types.h"

typedef void (* BFBaseDeallocFunction)(void *);

typedef struct BFBaseFunctions {
    char * name;
    BFBaseDeallocFunction dealloc;
} BFBaseFunctions;

typedef struct BFBase {
    const BFBaseFunctions * subclass;
    int refcount;
} * BFBaseRef;

void * BFAlloc(size_t size, const BFBaseFunctions * subclass);
void BFDealloc(void * base);

const void * BFSubclassFunctions(void * object);
const char * BFSubclassName(void * object);

#endif /* __BF_BASE_H__ */
