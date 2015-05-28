//
//  LLBase.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_BASE_H__
#define __LL_BASE_H__

#include "types.h"

typedef void (* LLBaseDeallocFunction)(void *);

typedef struct LLBaseFunctions {
    char * name;
    LLBaseDeallocFunction dealloc;
} LLBaseFunctions;

typedef struct LLBase {
    const LLBaseFunctions * subclass;
    int refcount;
} * LLBaseRef;

void * LLAlloc(size_t size, const LLBaseFunctions * subclass);
void LLDealloc(void * base);

const void * LLSubclassFunctions(void * object);
const char * LLSubclassName(void * object);

#endif /* __LL_BASE_H__ */
