//
//  BFLuaPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "libraries.h"
#include "BFLua.h"

#include "types.h"

const BFLuaClass luaPaintClass = {
    .metatableName = BFPaintClassName,
    .methods = {
        {NULL, NULL}
    }
};
