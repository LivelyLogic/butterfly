//
//  LLLuaPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLLuaPaint.h"

#include "LLLua.h"

#include "LLPaint.h"

const LLLuaClass luaPaintClass = {
    .metatableName = LLPaintClassName,
    .methods = {
        {NULL, NULL}
    }
};
