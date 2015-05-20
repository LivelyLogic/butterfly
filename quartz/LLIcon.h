//
//  LLIcon.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_ICON_H__
#define __LL_ICON_H__

#include "LLBase.h"

#include "LLCanvas.h"

#define LLIconClassName "Ovaltine.Icon"

typedef struct LLIcon * LLIconRef;

LLIconRef LLIconCreate(CGRect boundsRect);
LLIconRef LLIconCreateWithImage(CGImageRef image, double width, double height);

LLCanvasRef LLIconGetCanvas(LLIconRef icon);
CGImageRef LLIconCopyCGImage(LLIconRef icon);

#endif /* __LL_ICON_H__ */
