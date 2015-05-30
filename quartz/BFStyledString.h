//
//  BFStyledString.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BF_STYLED_STRING_H__
#define __BF_STYLED_STRING_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

#include "types.h"

BFStyledStringRef BFStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString);

CFAttributedStringRef BFStyledStringGetAttributedString(BFStyledStringRef styledString);
void BFStyledStringDrawInCGContext(const BFStyledStringRef styledString, CGContextRef context);
CGPathRef BFStyledStringGetCGPath(const BFStyledStringRef styledString);

#endif /* __BF_STYLED_STRING_H__ */
