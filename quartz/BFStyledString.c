//
//  BFStyledString.c
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

struct BFStyledString {
    struct BFBase __base;
    CFAttributedStringRef stringRef;
    CTLineRef lineRef;
    CGPathRef pathRef;
};

static void BFStyledStringInit(BFStyledStringRef styledString, CFAttributedStringRef attributedString, CTLineRef line);
static void BFStyledStringDealloc(BFStyledStringRef styledString);

CF_RETURNS_RETAINED static CFAttributedStringRef BFStyledStringNewAttributedString(const char * cString, BFFontRef font, BFStyledStringAttributes attributes);
CF_RETURNS_RETAINED static CFAttributedStringRef BFStyledStringNewAttributedStringJoining(CFAttributedStringRef string1, CFAttributedStringRef string2);
static void BFStyledStringEnsureLine(BFStyledStringRef styledString);
static void BFStyledStringEnsurePath(BFStyledStringRef styledString);

typedef void (* BFStyledStringRunIterationFunction)(BFStyledStringRef styledString, BFFunctionUserData * userData, CTRunRef run);
typedef void (* BFStyledStringRunGlyphIterationFunction)(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, BFFunctionUserData * userData);
static void BFStyledStringIterateRuns(BFStyledStringRef, BFFunctionUserData * userData);

static void BFStyledStringCTRunToGlyphs(BFStyledStringRef styledString, BFFunctionUserData * userData, CTRunRef run);

static void BFStyledStringAddGlyphsToPath(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path);

typedef struct BFStyledStringDrawGlyphsInContextUserData {
    CGContextRef context;
    CGPoint textPosition;
} BFStyledStringDrawGlyphsInContextUserData;
static void BFStyledStringDrawGlyphsInContext(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, BFStyledStringDrawGlyphsInContextUserData * userData);

static const BFBaseFunctions baseFunctions = {
    .name = BFStyledStringClassName,
    .dealloc = (BFBaseDeallocFunction)&BFStyledStringDealloc,
};

BFStyledStringRef BFStyledStringCreate(const char * string, BFFontRef font, BFStyledStringAttributes attributesStruct) {
    CFAttributedStringRef attributedString = BFStyledStringNewAttributedString(string, font, attributesStruct);
    BFStyledStringRef styledString = BFStyledStringCreateUsingAttributedString(attributedString);
    CFRelease(attributedString);
    return styledString;
}

BFStyledStringRef BFStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString) {
    BFStyledStringRef styledString = BFAlloc(sizeof(struct BFStyledString), &baseFunctions);
    if (styledString) {
        BFStyledStringInit(styledString, attributedString, NULL);
    }
    return BFRetain(styledString);
}

BFStyledStringRef BFStyledStringCreateJoining(BFStyledStringRef styledString1, BFStyledStringRef styledString2) {
    CFAttributedStringRef attributedString = BFStyledStringNewAttributedStringJoining(BFStyledStringGetAttributedString(styledString1), BFStyledStringGetAttributedString(styledString2));
    BFStyledStringRef styledString = BFStyledStringCreateUsingAttributedString(attributedString);
    CFRelease(attributedString);
    return styledString;
}

CFIndex BFStyledStringCreateBreaking(BFStyledStringRef styledString, CFIndex startPosition, double width, CFIndex lineCount, BFStyledStringRef resultStyledStrings[]) {
    CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedString(styledString->stringRef);
    CFIndex stringLength = CFAttributedStringGetLength(styledString->stringRef);
    CFIndex index;
    for (index = 0; index < lineCount; index++) {
        CFIndex lineLength = CTTypesetterSuggestLineBreak(typesetter, startPosition, width);
        CFRange range = CFRangeMake(startPosition, lineLength);
        CFAttributedStringRef attributedString = CFAttributedStringCreateWithSubstring(NULL, styledString->stringRef, range);
        resultStyledStrings[index] = BFStyledStringCreateUsingAttributedString(attributedString);
        CFRelease(attributedString);
        startPosition += lineLength;
        if (startPosition >= stringLength) {
            break;
        }
    }
    CFRelease(typesetter);
    return startPosition;
}

BFStyledStringRef BFStyledStringCreateTruncating(BFStyledStringRef styledString, double width) {
    BFRect stringRect = BFStyledStringMeasure(styledString);
    double stringWidth = stringRect.right - stringRect.left;
    if (stringWidth <= width) {
        BFRetain(styledString);
    } else {
        CFStringRef ellipsis = CFStringCreateWithCString(NULL, "…", kCFStringEncodingUTF8);
        CFMutableAttributedStringRef mutableString = CFAttributedStringCreateMutableCopy(NULL, 0, styledString->stringRef);
        CTLineRef lineRef = CTLineCreateWithAttributedString(mutableString);
        CFIndex length = CFAttributedStringGetLength(mutableString);
        CFRange range = { .location = length / 2, .length = 1 };
        // TODO: would a binary search be faster?
        while (length >= 1 && CTLineGetTypographicBounds(lineRef, NULL, NULL, NULL) > width) {
            CFStringRef string = CFAttributedStringGetString(mutableString);
            if (range.location >= 1) {
                UniChar beforeCharacter = CFStringGetCharacterAtIndex(string, range.location - 1);
                if (beforeCharacter >= 0xD800 && beforeCharacter <= 0xDBFF) {
                    range.location--;
                    range.length++;
                    length--;
                }
            }
            if (range.location + range.length < length) {
                UniChar afterCharacter = CFStringGetCharacterAtIndex(string, range.location + range.length);
                if (afterCharacter >= 0xDC00 && afterCharacter <= 0xDFFF) {
                    range.length++;
                    length--;
                }
            }
            CFAttributedStringReplaceString(mutableString, range, ellipsis);
            CFRelease(lineRef);
            lineRef = CTLineCreateWithAttributedString(mutableString);
            length--;
            range.location = length / 2;
            range.length = 2;
        }
        CFRelease(ellipsis);
        if (CTLineGetTypographicBounds(lineRef, NULL, NULL, NULL) < stringWidth) {
            CFAttributedStringRef attributedString = CFAttributedStringCreateCopy(NULL, mutableString);
            styledString = BFStyledStringCreateUsingAttributedString(attributedString);
            CFRelease(attributedString);
        } else {
            BFRetain(styledString);
        }
        CFRelease(mutableString);
        CFRelease(lineRef);
    }
    return styledString;
}

static void BFStyledStringInit(BFStyledStringRef styledString, CFAttributedStringRef attributedString, CTLineRef line) {
    styledString->stringRef = CFRetain(attributedString);
    styledString->lineRef = line ? CFRetain(line) : NULL;
    styledString->pathRef = NULL;
}

static void BFStyledStringDealloc(BFStyledStringRef styledString) {
    if (styledString) {
        if (styledString->stringRef) {
            CFRelease(styledString->stringRef);
        }
        if (styledString->lineRef) {
            CFRelease(styledString->lineRef);
        }
        CGPathRelease(styledString->pathRef);
    }
    BFDealloc(styledString);
}

CF_RETURNS_RETAINED
static CFAttributedStringRef BFStyledStringNewAttributedString(const char * cString, BFFontRef font, BFStyledStringAttributes attributesStruct) {
    if (!cString) {
        cString = "";
    }
    CFStringRef string = CFStringCreateWithCString(NULL, cString, kCFStringEncodingUTF8);
    CFIndex attributeIndex = 0;
    CFStringRef keys[2];
    CFTypeRef values[2];
    if (font) {
        keys[attributeIndex] = kCTFontAttributeName;
        values[attributeIndex++] = CFRetain(BFFontGetCTFont(font));
    }
    if (attributesStruct.superscriptIndex) {
        keys[attributeIndex] = kCTSuperscriptAttributeName;
        values[attributeIndex++] = CFNumberCreate(NULL, kCFNumberIntType, &attributesStruct.superscriptIndex);
    }
    
    CFDictionaryRef attributes = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&keys,
                                                    (const void **)&values, attributeIndex,
                                                    &kCFTypeDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);
    while (--attributeIndex >= 0) {
        CFRelease(values[attributeIndex]);
    }
    
    CFAttributedStringRef attributedString = CFAttributedStringCreate(kCFAllocatorDefault, string, attributes);
    CFRelease(string);
    CFRelease(attributes);
    
    return attributedString;
}

CF_RETURNS_RETAINED
static CFAttributedStringRef BFStyledStringNewAttributedStringJoining(CFAttributedStringRef string1, CFAttributedStringRef string2) {
    CFMutableAttributedStringRef mutableString = CFAttributedStringCreateMutableCopy(kCFAllocatorDefault, 0, string1);
    CFAttributedStringReplaceAttributedString(mutableString, CFRangeMake(CFAttributedStringGetLength(mutableString), 0), string2);
    CFAttributedStringRef string = CFAttributedStringCreateCopy(kCFAllocatorDefault, mutableString);
    CFRelease(mutableString);
    return string;
}

static void BFStyledStringEnsureLine(BFStyledStringRef styledString) {
    if (styledString->lineRef) {
        return;
    }
    styledString->lineRef = CTLineCreateWithAttributedString(styledString->stringRef);
}

static void BFStyledStringEnsurePath(BFStyledStringRef styledString) {
    if (styledString->pathRef) {
        return;
    }
    
    CGMutablePathRef path = CGPathCreateMutable();
    BFFunctionUserData glyphsUserData = { .function = BFStyledStringAddGlyphsToPath, .userData = path };
    BFFunctionUserData iterationUserData = { .function = BFStyledStringCTRunToGlyphs, .userData = &glyphsUserData };
    BFStyledStringIterateRuns(styledString, &iterationUserData);
    styledString->pathRef = path;
}

static void BFStyledStringCTRunToComponent(BFStyledStringRef styledString, BFFunctionUserData * userData, CTRunRef run) {
    BFStyledStringComponent component = {};

    CFRange range = CTRunGetStringRange(run);
    CFStringRef stringRef = CFAttributedStringGetString(styledString->stringRef);
    component.string = BFConvertQuartzStringRange(stringRef, range);
    
    CFDictionaryRef attributes = CTRunGetAttributes(run);
    CTFontRef font = CFDictionaryGetValue(attributes, kCTFontAttributeName);
    component.font = BFFontCreateWithCTFont(font);

    if (CFDictionaryContainsKey(attributes, kCTSuperscriptAttributeName)) {
        CFNumberRef number = CFDictionaryGetValue(attributes, kCTSuperscriptAttributeName);
        CFNumberGetValue(number, kCFNumberIntType, &component.attributes.superscriptIndex);
    }
    
    ((BFStyledStringComponentIterationFunction)userData->function)(userData->userData, component);
    
    free(component.string);
    BFRelease(component.font);
}

static void BFStyledStringCTRunToGlyphs(BFStyledStringRef styledString, BFFunctionUserData * userData, CTRunRef run) {
    CFDictionaryRef attributes = CTRunGetAttributes(run);
    CTFontRef font = CFDictionaryGetValue(attributes, kCTFontAttributeName);
    double baselineOffset = 0;
    if (CFDictionaryContainsKey(attributes, kCTSuperscriptAttributeName)) {
        CFNumberRef number = CFDictionaryGetValue(attributes, kCTSuperscriptAttributeName);
        int superscriptIndex = 0;
        CFNumberGetValue(number, kCFNumberIntType, &superscriptIndex);
        baselineOffset = superscriptIndex * CTFontGetSize(font) / 2;
    }
    
    CFIndex glyphCount = CTRunGetGlyphCount(run);
    const CGGlyph * glyphs = CTRunGetGlyphsPtr(run);
    CGGlyph * allocatedGlyphs = NULL;
    if (!glyphs) {
        allocatedGlyphs = malloc(glyphCount * sizeof(CGGlyph));
        CTRunGetGlyphs(run, CFRangeMake(0, glyphCount), allocatedGlyphs);
        glyphs = allocatedGlyphs;
    }
    const CGPoint * positions = CTRunGetPositionsPtr(run);
    CGPoint * allocatedPositions = NULL;
    if (!positions) {
        allocatedPositions = malloc(glyphCount * sizeof(CGPoint));
        CTRunGetPositions(run, CFRangeMake(0, glyphCount), allocatedPositions);
        positions = allocatedPositions;
    }
    
    ((BFStyledStringRunGlyphIterationFunction)userData->function)(font, baselineOffset, glyphs, positions, glyphCount, userData->userData);
    
    free(allocatedGlyphs);
    free(allocatedPositions);
}

static void BFStyledStringIterateRuns(BFStyledStringRef styledString, BFFunctionUserData * userData) {
    BFStyledStringEnsureLine(styledString);
    
    CFArrayRef runs = CTLineGetGlyphRuns(styledString->lineRef);
    CFIndex runCount = CFArrayGetCount(runs);
    for (CFIndex runIndex = 0; runIndex < runCount; runIndex++) {
        CTRunRef run = CFArrayGetValueAtIndex(runs, runIndex);
        ((BFStyledStringRunIterationFunction)userData->function)(styledString, userData->userData, run);
    }
}

static void BFStyledStringAddGlyphsToPath(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path) {
    CGAffineTransform glyphTransform = CGAffineTransformIdentity;
    for (CFIndex glyphIndex = 0; glyphIndex < glyphCount; glyphIndex++) {
        glyphTransform.tx = positions[glyphIndex].x;
        glyphTransform.ty = positions[glyphIndex].y + baselineOffset;
        CGPathRef glyphPath = CTFontCreatePathForGlyph(font, glyphs[glyphIndex], &glyphTransform);
        CGPathAddPath(path, NULL, glyphPath);
        CGPathRelease(glyphPath);
    }
}

static void BFStyledStringDrawGlyphsInContext(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, BFStyledStringDrawGlyphsInContextUserData * userData) {
    CGContextRef context = userData->context;
    CGPoint textPosition = userData->textPosition;
    
    CGFontRef graphicsFont = CTFontCopyGraphicsFont(font, NULL);
    CGContextSetFont(context, graphicsFont);
    CGContextSetFontSize(context, CTFontGetSize(font));
    CGFontRelease(graphicsFont);
    
    double ty = textPosition.y;
    textPosition.y += baselineOffset;
    CGContextSetTextPosition(context, textPosition.x, textPosition.y);
    textPosition.y = ty;
    
    CGContextShowGlyphsAtPositions(context, glyphs, positions, glyphCount);
}

void BFStyledStringIterateComponents(BFStyledStringRef styledString, BFStyledStringComponentIterationFunction iterationFunction, void * userData) {
    BFFunctionUserData componentUserData = { .function = iterationFunction, .userData = userData };
    BFFunctionUserData iterationUserData = { .function = BFStyledStringCTRunToComponent, .userData = &componentUserData };
    BFStyledStringIterateRuns(styledString, &iterationUserData);
}

CFIndex BFStyledStringGetLength(BFStyledStringRef styledString) {
    return CFAttributedStringGetLength(styledString->stringRef);
}

BFRect BFStyledStringMeasure(BFStyledStringRef styledString) {
    BFStyledStringEnsureLine(styledString);
    double length;
    double ascent, descent, leading;
    length = CTLineGetTypographicBounds(styledString->lineRef, &ascent, &descent, &leading);
    length -= CTLineGetTrailingWhitespaceWidth(styledString->lineRef);
    return (BFRect){
        .left = 0,
        .bottom = 0 - descent - leading,
        .right = length,
        .top = ascent,
    };
}

void BFStyledStringDrawInCGContext(const BFStyledStringRef styledString, CGContextRef context) {
    CGPoint textPosition = CGContextGetTextPosition(context);
    BFStyledStringDrawGlyphsInContextUserData drawGlyphsUserData = { .context = context, .textPosition = textPosition };
    BFFunctionUserData glyphsUserData = { .function = BFStyledStringDrawGlyphsInContext, .userData = &drawGlyphsUserData };
    BFFunctionUserData iterationUserData = { .function = BFStyledStringCTRunToGlyphs, .userData = &glyphsUserData };
    BFStyledStringIterateRuns(styledString, &iterationUserData);
}

CGPathRef BFStyledStringGetCGPath(BFStyledStringRef styledString) {
    BFStyledStringEnsurePath(styledString);
    return styledString->pathRef;
}

CFAttributedStringRef BFStyledStringGetAttributedString(BFStyledStringRef styledString) {
    return styledString->stringRef;
}

char * BFStyledStringCopyString(BFStyledStringRef styledString) {
    CFStringRef stringRef = CFAttributedStringGetString(styledString->stringRef);
    return BFConvertQuartzString(stringRef);
}
