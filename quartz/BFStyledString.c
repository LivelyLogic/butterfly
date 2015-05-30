//
//  BFStyledString.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFStyledString.h"

#include "BFFont.h"
#include "BFQuartzTypes.h"

struct BFStyledString {
    struct BFBase __base;
    CFAttributedStringRef stringRef;
    CTLineRef lineRef;
    CGPathRef pathRef;
};

static void BFStyledStringInit(BFStyledStringRef styledString, CFAttributedStringRef attributedString, CTLineRef line);
static void BFStyledStringDealloc(BFStyledStringRef styledString);

CF_RETURNS_RETAINED static CFAttributedStringRef BFStyledStringNewAttributedString(const char * cString, BFFontRef font, int superscriptIndex);
CF_RETURNS_RETAINED static CFAttributedStringRef BFStyledStringNewAttributedStringJoining(CFAttributedStringRef string1, CFAttributedStringRef string2);
static void BFStyledStringEnsureLine(BFStyledStringRef styledString);
static void BFStyledStringEnsurePath(BFStyledStringRef styledString);

typedef void (* BFStyledStringRunIterationFunction)(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, void * userData);
static void BFStyledStringIterateRuns(BFStyledStringRef, BFStyledStringRunIterationFunction, void *);

static void BFStyledStringBuildPathRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path);

typedef struct BFStyledStringShowGlyphsRunIterationFunctionUserData {
    CGContextRef context;
    CGPoint textPosition;
} BFStyledStringShowGlyphsRunIterationFunctionUserData;
static void BFStyledStringShowGlyphsRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, BFStyledStringShowGlyphsRunIterationFunctionUserData * userData);

static const BFBaseFunctions baseFunctions = {
    .name = BFStyledStringClassName,
    .dealloc = (BFBaseDeallocFunction)&BFStyledStringDealloc,
};

BFStyledStringRef BFStyledStringCreate(const char * string, BFFontRef font, int superscriptIndex) {
    CFAttributedStringRef attributedString = BFStyledStringNewAttributedString(string, font, superscriptIndex);
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
    if (stringWidth > width) {
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
        }
        CFRelease(mutableString);
        CFRelease(lineRef);
    }
    return BFRetain(styledString);
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
static CFAttributedStringRef BFStyledStringNewAttributedString(const char * cString, BFFontRef font, int superscriptIndex) {
    if (!cString) {
        cString = "";
    }
    CFStringRef string = CFStringCreateWithCString(NULL, cString, kCFStringEncodingUTF8);
    CFIndex attributeCount = 0;
    CFStringRef keys[2];
    CFTypeRef values[2];
    if (font) {
        keys[attributeCount] = kCTFontAttributeName;
        values[attributeCount++] = BFFontGetCTFont(font);
    }
    if (superscriptIndex) {
        keys[attributeCount] = kCTSuperscriptAttributeName;
        values[attributeCount++] = CFNumberCreate(NULL, kCFNumberIntType, &superscriptIndex);
    }
    
    CFDictionaryRef attributes = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&keys,
                                                    (const void **)&values, attributeCount,
                                                    &kCFTypeDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);
    
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
    BFStyledStringIterateRuns(styledString, (BFStyledStringRunIterationFunction)BFStyledStringBuildPathRunIterationFunction, path);
    styledString->pathRef = path;
}

static void BFStyledStringIterateRuns(BFStyledStringRef styledString, BFStyledStringRunIterationFunction iterationFunction, void * userData) {
    BFStyledStringEnsureLine(styledString);
    
    CFArrayRef runs = CTLineGetGlyphRuns(styledString->lineRef);
    CFIndex runCount = CFArrayGetCount(runs);
    for (CFIndex runIndex = 0; runIndex < runCount; runIndex++) {
        CTRunRef run = CFArrayGetValueAtIndex(runs, runIndex);
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
        
        iterationFunction(font, baselineOffset, glyphs, positions, glyphCount, userData);
        
        free(allocatedGlyphs);
        free(allocatedPositions);
    }
}

static void BFStyledStringBuildPathRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path) {
    CGAffineTransform glyphTransform = CGAffineTransformIdentity;
    for (CFIndex glyphIndex = 0; glyphIndex < glyphCount; glyphIndex++) {
        glyphTransform.tx = positions[glyphIndex].x;
        glyphTransform.ty = positions[glyphIndex].y + baselineOffset;
        CGPathRef glyphPath = CTFontCreatePathForGlyph(font, glyphs[glyphIndex], &glyphTransform);
        CGPathAddPath(path, NULL, glyphPath);
        CGPathRelease(glyphPath);
    }
}

static void BFStyledStringShowGlyphsRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, BFStyledStringShowGlyphsRunIterationFunctionUserData * userData) {
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
    BFStyledStringShowGlyphsRunIterationFunctionUserData userData = { .context = context, .textPosition = textPosition };
    BFStyledStringIterateRuns(styledString, (BFStyledStringRunIterationFunction)BFStyledStringShowGlyphsRunIterationFunction, &userData);
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
