//
//  BFIcon.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_ICON_H__
#define __BF_ICON_H__

#include <CoreGraphics/CoreGraphics.h>

#include "types.h"

BFIconRef BFIconCreateWithImage(CGImageRef image, double width, double height);

CGImageRef BFIconCopyCGImage(BFIconRef icon);

#endif /* __BF_ICON_H__ */
