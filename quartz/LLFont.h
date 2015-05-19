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

LLFontRef LLFontCreate(const char * name, double size);
LLFontRef LLFontCreateSystem(double size);
LLFontRef LLFontCreateBoldSystem(double size);

char * LLFontCopyName(LLFontRef font);
double LLFontGetSize(LLFontRef font);
double LLFontGetAscent(LLFontRef font);
double LLFontGetDescent(LLFontRef font);
double LLFontGetLeading(LLFontRef font);

CTFontRef LLFontGetCTFont(LLFontRef font);

#endif /* __LL_FONT_H__ */
