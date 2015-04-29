//
//  LLFont.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLFont.h"

#include "LLQuartzTypes.h"
#include "LLStyledString.h"

struct LLFont {
    struct LLBase __base;
    CTFontRef fontRef;
};

static void LLFontInit(LLFontRef font, CTFontRef fontRef);
static void LLFontDealloc(LLFontRef font);

static const LLBaseFunctions baseFunctions = {
    .name = LLFontClassName,
    .dealloc = (LLBaseDeallocFunction)&LLFontDealloc,
};

LLFontRef LLFontCreate(const char * name, CGFloat size) {
    LLFontRef font = LLAlloc(sizeof(struct LLFont), &baseFunctions);
    if (font) {
        if (!name) {
            name = "";
        }
        CFStringRef fontName = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
        CTFontDescriptorRef fontDescriptor = CTFontDescriptorCreateWithNameAndSize((CFStringRef)fontName, 0);
        CFRelease(fontName);
        CTFontRef fontRef = CTFontCreateWithFontDescriptor(fontDescriptor, size, NULL);
        CFRelease(fontDescriptor);
        LLFontInit(font, fontRef);
    }
    return LLRetain(font);
}

LLFontRef LLFontCreateSystem(CGFloat size) {
    LLFontRef font = LLAlloc(sizeof(struct LLFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontSystemFontType, size, NULL);
        LLFontInit(font, fontRef);
    }
    return LLRetain(font);
}

LLFontRef LLFontCreateBoldSystem(CGFloat size) {
    LLFontRef font = LLAlloc(sizeof(struct LLFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontEmphasizedSystemFontType, size, NULL);
        LLFontInit(font, fontRef);
    }
    return LLRetain(font);
}

static void LLFontInit(LLFontRef font, CTFontRef fontRef) {
    font->fontRef = fontRef;
}

static void LLFontDealloc(LLFontRef font) {
    if (font) {
        if (font->fontRef) {
            CFRelease(font->fontRef);
        }
    }
    LLDealloc(font);
}

char * LLFontCopyName(LLFontRef font) {
    CFStringRef stringRef = CTFontCopyPostScriptName(font->fontRef);
    char * buffer = LLConvertQuartzString(stringRef);
    CFRelease(stringRef);
    return buffer;
}

CGFloat LLFontGetSize(LLFontRef font) {
    return CTFontGetSize(font->fontRef);
}

CGFloat LLFontGetAscent(LLFontRef font) {
    return CTFontGetAscent(font->fontRef);
}

CGFloat LLFontGetDescent(LLFontRef font) {
    return CTFontGetDescent(font->fontRef);
}

CGFloat LLFontGetLeading(LLFontRef font) {
    return CTFontGetLeading(font->fontRef);
}

CTFontRef LLFontGetCTFont(LLFontRef font) {
    return font->fontRef;
}
