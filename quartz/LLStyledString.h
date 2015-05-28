//
//  LLStyledString.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_STYLED_STRING_H__
#define __LL_STYLED_STRING_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

#include "types.h"

LLStyledStringRef LLStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString);

CFAttributedStringRef LLStyledStringGetAttributedString(LLStyledStringRef styledString);
void LLStyledStringDrawInCGContext(const LLStyledStringRef styledString, CGContextRef context);
CGPathRef LLStyledStringGetCGPath(const LLStyledStringRef styledString);

#endif /* __LL_STYLED_STRING_H__ */
