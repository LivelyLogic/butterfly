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
    BFFontFeatures features;
};

static void BFFontInit(BFFontRef font, CTFontRef fontRef, BFFontFeatures features);
static void BFFontDealloc(BFFontRef font);

static CFDictionaryRef BFFontCreateFontFeatureCFDictionary(int featureType, int featureSelector);
static CFArrayRef BFFontCreateFontFeaturesCFArray(BFFontFeatures features);

static const BFBaseFunctions baseFunctions = {
    .name = BFFontClassName,
    .dealloc = (BFBaseDeallocFunction)&BFFontDealloc,
};

BFFontRef BFFontCreate(const char * name, double size) {
    BFFontFeatures noFeatures = {};
    return BFFontCreateWithFeatures(name, size, noFeatures);
}

BFFontRef BFFontCreateWithFeatures(const char * name, double size, BFFontFeatures featuresStruct) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        if (!name) {
            name = "";
        }
        CFStringRef fontName = CFStringCreateWithCString(NULL, name, kCFStringEncodingUTF8);
        CFArrayRef features = BFFontCreateFontFeaturesCFArray(featuresStruct);
        const void * keys[2] = { kCTFontNameAttribute, kCTFontFeatureSettingsAttribute };
        const void * values[2] = { fontName, features };
        CFDictionaryRef attributes = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values, 2,
                                                        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFRelease(fontName);
        CFRelease(features);
        CTFontDescriptorRef fontDescriptor = CTFontDescriptorCreateWithAttributes(attributes);
        CFRelease(attributes);
        CTFontRef fontRef = CTFontCreateWithFontDescriptor(fontDescriptor, size, NULL);
        CFRelease(fontDescriptor);
        BFFontInit(font, fontRef, featuresStruct);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateSystem(double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontUIFontSystem, size, NULL);
        BFFontFeatures features = {};
        BFFontInit(font, fontRef, features);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateBoldSystem(double size) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        CTFontRef fontRef = CTFontCreateUIFontForLanguage(kCTFontUIFontEmphasizedSystem, size, NULL);
        BFFontFeatures features = {};
        BFFontInit(font, fontRef, features);
    }
    return BFRetain(font);
}

BFFontRef BFFontCreateWithCTFont(CTFontRef fontRef) {
    BFFontRef font = BFAlloc(sizeof(struct BFFont), &baseFunctions);
    if (font) {
        BFFontFeatures features = {};
        BFFontInit(font, CFRetain(fontRef), features);
    }
    return BFRetain(font);
}

static void BFFontInit(BFFontRef font, CTFontRef fontRef, BFFontFeatures features) {
    font->fontRef = fontRef;
    font->features = features;
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

BFFontFeatures BFFontGetFeatures(BFFontRef font) {
    return font->features;
}

CTFontRef BFFontGetCTFont(BFFontRef font) {
    return font->fontRef;
}

CFDictionaryRef BFFontCreateFontFeatureCFDictionary(int featureType, int featureSelector) {
    CFNumberRef featureTypeNumber = CFNumberCreate(NULL, kCFNumberIntType, &featureType);
    CFNumberRef featureSelectorNumber = CFNumberCreate(NULL, kCFNumberIntType, &featureSelector);
    const void * keys[2] = { kCTFontFeatureTypeIdentifierKey, kCTFontFeatureSelectorIdentifierKey };
    const void * values[2] = { featureTypeNumber, featureSelectorNumber };
    CFDictionaryRef feature = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values, 2,
                                                 &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFRelease(featureTypeNumber);
    CFRelease(featureSelectorNumber);
    return feature;
}

CFArrayRef BFFontCreateFontFeaturesCFArray(BFFontFeatures featuresStruct) {
    CFIndex index = 0;
    CFDictionaryRef array[3];
    if (featuresStruct.smallCaps) {
        array[index++] = BFFontCreateFontFeatureCFDictionary(kLowerCaseType, kLowerCaseSmallCapsSelector);
    }
    if (featuresStruct.lowercaseNumbers) {
        array[index++] = BFFontCreateFontFeatureCFDictionary(kNumberCaseType, kLowerCaseNumbersSelector);
    } else if (featuresStruct.uppercaseNumbers) {
        array[index++] = BFFontCreateFontFeatureCFDictionary(kNumberCaseType, kUpperCaseNumbersSelector);
    }
    if (featuresStruct.monospacedNumbers) {
        array[index++] = BFFontCreateFontFeatureCFDictionary(kNumberSpacingType, kMonospacedNumbersSelector);
    } else if (featuresStruct.proportionalNumbers) {
        array[index++] = BFFontCreateFontFeatureCFDictionary(kNumberSpacingType, kProportionalNumbersSelector);
    }
    CFArrayRef cfArray = CFArrayCreate(NULL, (const void **)&array, index, &kCFTypeArrayCallBacks);
    while (--index >= 0) {
        CFRelease(array[index]);
    }
    return cfArray;
}
