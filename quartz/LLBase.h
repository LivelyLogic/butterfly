//
//  LLBase.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_BASE_H__
#define __LL_BASE_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

#include "butterfly.h"

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
void * LLRetain(void * base);
void LLRelease(void * base);

const void * LLSubclassFunctions(void * object);
const char * LLSubclassName(void * object);

#endif /* __LL_BASE_H__ */
