//
//  BFQuartzTypes.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_QUARTZ_TYPES_H__
#define __BF_QUARTZ_TYPES_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

#include "BFBase.h"

char * BFConvertQuartzString(CFStringRef string);

#define BFConvertPointToQuartz(point) CGPointMake(point.x, point.y)

#define BFConvertRectToQuartz(rect) CGRectMake(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom)
#define BFConvertQuartzRect(rect) (BFRect){ .left = rect.origin.x, .bottom = rect.origin.y, .right = rect.origin.x + rect.size.width, .top = rect.origin.y + rect.size.height }

#endif /* __BF_QUARTZ_TYPES_H__ */
