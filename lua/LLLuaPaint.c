//
//  LLLuaPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "LLLua.h"

#include "types.h"

const LLLuaClass luaPaintClass = {
    .metatableName = LLPaintClassName,
    .methods = {
        {NULL, NULL}
    }
};
