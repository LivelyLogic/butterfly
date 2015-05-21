//
//  LLIcon.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_ICON_H__
#define __LL_ICON_H__

#include "LLBase.h"

#define LLIconClassName "Ovaltine.Icon"

LLIconRef LLIconCreate(LLRect boundsRect);
LLIconRef LLIconCreateWithImage(CGImageRef image, double width, double height);

LLCanvasRef LLIconGetCanvas(LLIconRef icon);
CGImageRef LLIconCopyCGImage(LLIconRef icon);

#endif /* __LL_ICON_H__ */
