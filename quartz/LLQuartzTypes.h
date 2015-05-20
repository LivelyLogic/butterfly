//
//  LLQuartzTypes.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_QUARTZ_TYPES_H__
#define __LL_QUARTZ_TYPES_H__

#include "LLBase.h"

char * LLConvertQuartzString(CFStringRef string);

#define LLConvertPointToQuartz(point) CGPointMake(point.x, point.y)

#define LLConvertRectToQuartz(rect) CGRectMake(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom)
#define LLConvertQuartzRect(rect) (LLRect){ .left = rect.origin.x, .bottom = rect.origin.y, .right = rect.origin.x + rect.size.width, .top = rect.origin.y + rect.size.height }

#endif /* __LL_QUARTZ_TYPES_H__ */
