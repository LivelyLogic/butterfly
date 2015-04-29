//
//  LLFont.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_FONT_H__
#define __LL_FONT_H__

#include "LLBase.h"

#define LLFontClassName "Ovaltine.Font"

typedef struct LLFont * LLFontRef;

LLFontRef LLFontCreate(const char * name, CGFloat size);
LLFontRef LLFontCreateSystem(CGFloat size);
LLFontRef LLFontCreateBoldSystem(CGFloat size);

char * LLFontCopyName(LLFontRef font);
CGFloat LLFontGetSize(LLFontRef font);
CGFloat LLFontGetAscent(LLFontRef font);
CGFloat LLFontGetDescent(LLFontRef font);
CGFloat LLFontGetLeading(LLFontRef font);

CTFontRef LLFontGetCTFont(LLFontRef font);

#endif /* __LL_FONT_H__ */
