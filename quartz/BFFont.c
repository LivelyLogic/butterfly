//
//  BFFont.c
//
//  Copyright (c) 2011-2016 Ripeware, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "butterfly.h"
#include "quartz.h"

#include "BFQuartzTypes.h"

struct BFFont {
    struct BFBase __base;
    CTFontRef fontRef;
};

static void BFFontInit(BFFontRef font, CTFontRef fontRef);
static void BFFontDealloc(BFFontRef font);

static const BFBaseFunctions baseFunctions = {
    .name = BFFontClassName,
    .dealloc = (BFBaseDeallocFunction)&BFFontDealloc,
};

BFFontRef BFFontCreate(const char * name, double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        if (!name) {
            name = "";
        }
        CFStringRef fontName = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
        CTFontDescriptorRef fontDescriptor = CTFontDescriptorCreateWithNameAndSize((CFStringRef)fontName, 0);
        CFRelease(fontName);
        CTFontRef fontRef = CTFontCreateWithFontDescriptor(fontDescriptor, size, NULL);
        CFRelease(fontDescriptor);
        BFFontInit(font, fontRef);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateSystem(double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontUIFontSystem, size, NULL);
        BFFontInit(font, fontRef);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateBoldSystem(double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontUIFontEmphasizedSystem, size, NULL);
        BFFontInit(font, fontRef);
    }
    return BFRetain(font);
}

static void BFFontInit(BFFontRef font, CTFontRef fontRef) {
    font->fontRef = fontRef;
}

static void BFFontDealloc(BFFontRef font) {
    if (font) {
        if (font->fontRef) {
            CFRelease(font->fontRef);
        }
    }
    BFDealloc(font);
}

char * BFFontCopyName(BFFontRef font) {
    CFStringRef stringRef = CTFontCopyPostScriptName(font->fontRef);
    char * buffer = BFConvertQuartzString(stringRef);
    CFRelease(stringRef);
    return buffer;
}

double BFFontGetSize(BFFontRef font) {
    return CTFontGetSize(font->fontRef);
}

double BFFontGetAscent(BFFontRef font) {
    return CTFontGetAscent(font->fontRef);
}

double BFFontGetDescent(BFFontRef font) {
    return CTFontGetDescent(font->fontRef);
}

double BFFontGetLeading(BFFontRef font) {
    return CTFontGetLeading(font->fontRef);
}

CTFontRef BFFontGetCTFont(BFFontRef font) {
    return font->fontRef;
}
