//
//  LLStyledString.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_STYLED_STRING_H__
#define __LL_STYLED_STRING_H__

#include "LLBase.h"

#include "LLFont.h"

#define LLStyledStringClassName "Ovaltine.StyledString"

typedef struct LLStyledString * LLStyledStringRef;

LLStyledStringRef LLStyledStringCreate(const char * string, LLFontRef font, int superscriptIndex);
LLStyledStringRef LLStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString);
LLStyledStringRef LLStyledStringCreateJoining(LLStyledStringRef styledString1, LLStyledStringRef styledString2);
LLStyledStringRef LLStyledStringCreateTruncating(LLStyledStringRef styledString, double width);
CFIndex LLStyledStringCreateBreaking(LLStyledStringRef styledString, CFIndex startPosition, double width, CFIndex lineCount, LLStyledStringRef resultStyledStrings[]);

CFIndex LLStyledStringGetLength(LLStyledStringRef styledString);

LLRect LLStyledStringMeasure(LLStyledStringRef styledString);
CFAttributedStringRef LLStyledStringGetAttributedString(LLStyledStringRef styledString);
void LLStyledStringDrawInCGContext(const LLStyledStringRef styledString, CGContextRef context);
CGPathRef LLStyledStringGetCGPath(const LLStyledStringRef styledString);
char * LLStyledStringCopyString(LLStyledStringRef styledString);

#endif /* __LL_STYLED_STRING_H__ */
