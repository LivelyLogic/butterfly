//
//  BFLuaPaint.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "lua.h"
#include "BFLua.h"

#include "butterfly.h"

const BFLuaClass luaPaintClass = {
    .metatableName = BFPaintClassName,
    .methods = {
        {NULL, NULL}
    }
};
