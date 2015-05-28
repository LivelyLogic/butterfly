//
//  LLIcon.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_ICON_H__
#define __LL_ICON_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

LLIconRef LLIconCreateWithImage(CGImageRef image, double width, double height);

CGImageRef LLIconCopyCGImage(LLIconRef icon);

#endif /* __LL_ICON_H__ */
