//
//  BFFont.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "types.h"
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
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontSystemFontType, size, NULL);
        BFFontInit(font, fontRef);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateBoldSystem(double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontEmphasizedSystemFontType, size, NULL);
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
