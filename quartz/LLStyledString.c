//
//  LLStyledString.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLBase.h"

#include "LLStyledString.h"

#include "LLFont.h"
#include "LLQuartzTypes.h"

struct LLStyledString {
    struct LLBase __base;
    CFAttributedStringRef stringRef;
    CTLineRef lineRef;
    CGPathRef pathRef;
};

static void LLStyledStringInit(LLStyledStringRef styledString, CFAttributedStringRef attributedString, CTLineRef line);
static void LLStyledStringDealloc(LLStyledStringRef styledString);

CF_RETURNS_RETAINED static CFAttributedStringRef LLStyledStringNewAttributedString(const char * cString, LLFontRef font, int superscriptIndex);
CF_RETURNS_RETAINED static CFAttributedStringRef LLStyledStringNewAttributedStringJoining(CFAttributedStringRef string1, CFAttributedStringRef string2);
static void LLStyledStringEnsureLine(LLStyledStringRef styledString);
static void LLStyledStringEnsurePath(LLStyledStringRef styledString);

typedef void (* LLStyledStringRunIterationFunction)(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, void * userData);
static void LLStyledStringIterateRuns(LLStyledStringRef, LLStyledStringRunIterationFunction, void *);

static void LLStyledStringBuildPathRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path);

typedef struct LLStyledStringShowGlyphsRunIterationFunctionUserData {
    CGContextRef context;
    CGPoint textPosition;
} LLStyledStringShowGlyphsRunIterationFunctionUserData;
static void LLStyledStringShowGlyphsRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, LLStyledStringShowGlyphsRunIterationFunctionUserData * userData);

static const LLBaseFunctions baseFunctions = {
    .name = LLStyledStringClassName,
    .dealloc = (LLBaseDeallocFunction)&LLStyledStringDealloc,
};

LLStyledStringRef LLStyledStringCreate(const char * string, LLFontRef font, int superscriptIndex) {
    CFAttributedStringRef attributedString = LLStyledStringNewAttributedString(string, font, superscriptIndex);
    LLStyledStringRef styledString = LLStyledStringCreateUsingAttributedString(attributedString);
    CFRelease(attributedString);
    return styledString;
}

LLStyledStringRef LLStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString) {
    LLStyledStringRef styledString = LLAlloc(sizeof(struct LLStyledString), &baseFunctions);
    if (styledString) {
        LLStyledStringInit(styledString, attributedString, NULL);
    }
    return LLRetain(styledString);
}

LLStyledStringRef LLStyledStringCreateJoining(LLStyledStringRef styledString1, LLStyledStringRef styledString2) {
    CFAttributedStringRef attributedString = LLStyledStringNewAttributedStringJoining(LLStyledStringGetAttributedString(styledString1), LLStyledStringGetAttributedString(styledString2));
    LLStyledStringRef styledString = LLStyledStringCreateUsingAttributedString(attributedString);
    CFRelease(attributedString);
    return styledString;
}

CFIndex LLStyledStringCreateBreaking(LLStyledStringRef styledString, CFIndex startPosition, double width, CFIndex lineCount, LLStyledStringRef resultStyledStrings[]) {
    CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedString(styledString->stringRef);
    CFIndex stringLength = CFAttributedStringGetLength(styledString->stringRef);
    CFIndex index;
    for (index = 0; index < lineCount; index++) {
        CFIndex lineLength = CTTypesetterSuggestLineBreak(typesetter, startPosition, width);
        CFRange range = CFRangeMake(startPosition, lineLength);
        CFAttributedStringRef attributedString = CFAttributedStringCreateWithSubstring(NULL, styledString->stringRef, range);
        resultStyledStrings[index] = LLStyledStringCreateUsingAttributedString(attributedString);
        CFRelease(attributedString);
        startPosition += lineLength;
        if (startPosition >= stringLength) {
            break;
        }
    }
    CFRelease(typesetter);
    return startPosition;
}

LLStyledStringRef LLStyledStringCreateTruncating(LLStyledStringRef styledString, double width) {
    LLRect stringRect = LLStyledStringMeasure(styledString);
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
            styledString = LLStyledStringCreateUsingAttributedString(attributedString);
            CFRelease(attributedString);
        }
        CFRelease(mutableString);
        CFRelease(lineRef);
    }
    return LLRetain(styledString);
}

static void LLStyledStringInit(LLStyledStringRef styledString, CFAttributedStringRef attributedString, CTLineRef line) {
    styledString->stringRef = CFRetain(attributedString);
    styledString->lineRef = line ? CFRetain(line) : NULL;
    styledString->pathRef = NULL;
}

static void LLStyledStringDealloc(LLStyledStringRef styledString) {
    if (styledString) {
        if (styledString->stringRef) {
            CFRelease(styledString->stringRef);
        }
        if (styledString->lineRef) {
            CFRelease(styledString->lineRef);
        }
        CGPathRelease(styledString->pathRef);
    }
    LLDealloc(styledString);
}

CF_RETURNS_RETAINED
static CFAttributedStringRef LLStyledStringNewAttributedString(const char * cString, LLFontRef font, int superscriptIndex) {
    if (!cString) {
        cString = "";
    }
    CFStringRef string = CFStringCreateWithCString(NULL, cString, kCFStringEncodingUTF8);
    CFIndex attributeCount = 0;
    CFStringRef keys[2];
    CFTypeRef values[2];
    if (font) {
        keys[attributeCount] = kCTFontAttributeName;
        values[attributeCount++] = LLFontGetCTFont(font);
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
static CFAttributedStringRef LLStyledStringNewAttributedStringJoining(CFAttributedStringRef string1, CFAttributedStringRef string2) {
    CFMutableAttributedStringRef mutableString = CFAttributedStringCreateMutableCopy(kCFAllocatorDefault, 0, string1);
    CFAttributedStringReplaceAttributedString(mutableString, CFRangeMake(CFAttributedStringGetLength(mutableString), 0), string2);
    CFAttributedStringRef string = CFAttributedStringCreateCopy(kCFAllocatorDefault, mutableString);
    CFRelease(mutableString);
    return string;
}

static void LLStyledStringEnsureLine(LLStyledStringRef styledString) {
    if (styledString->lineRef) {
        return;
    }
    styledString->lineRef = CTLineCreateWithAttributedString(styledString->stringRef);
}

static void LLStyledStringEnsurePath(LLStyledStringRef styledString) {
    if (styledString->pathRef) {
        return;
    }
    
    CGMutablePathRef path = CGPathCreateMutable();
    LLStyledStringIterateRuns(styledString, (LLStyledStringRunIterationFunction)LLStyledStringBuildPathRunIterationFunction, path);
    styledString->pathRef = path;
}

static void LLStyledStringIterateRuns(LLStyledStringRef styledString, LLStyledStringRunIterationFunction iterationFunction, void * userData) {
    LLStyledStringEnsureLine(styledString);
    
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

static void LLStyledStringBuildPathRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, CGMutablePathRef path) {
    CGAffineTransform glyphTransform = CGAffineTransformIdentity;
    for (CFIndex glyphIndex = 0; glyphIndex < glyphCount; glyphIndex++) {
        glyphTransform.tx = positions[glyphIndex].x;
        glyphTransform.ty = positions[glyphIndex].y + baselineOffset;
        CGPathRef glyphPath = CTFontCreatePathForGlyph(font, glyphs[glyphIndex], &glyphTransform);
        CGPathAddPath(path, NULL, glyphPath);
        CGPathRelease(glyphPath);
    }
}

static void LLStyledStringShowGlyphsRunIterationFunction(CTFontRef font, double baselineOffset, const CGGlyph * glyphs, const CGPoint * positions, CFIndex glyphCount, LLStyledStringShowGlyphsRunIterationFunctionUserData * userData) {
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

CFIndex LLStyledStringGetLength(LLStyledStringRef styledString) {
    return CFAttributedStringGetLength(styledString->stringRef);
}

LLRect LLStyledStringMeasure(LLStyledStringRef styledString) {
    LLStyledStringEnsureLine(styledString);
    double length;
    double ascent, descent, leading;
    length = CTLineGetTypographicBounds(styledString->lineRef, &ascent, &descent, &leading);
    length -= CTLineGetTrailingWhitespaceWidth(styledString->lineRef);
    return (LLRect){
        .left = 0,
        .bottom = 0 - descent - leading,
        .right = length,
        .top = ascent,
    };
}

void LLStyledStringDrawInCGContext(const LLStyledStringRef styledString, CGContextRef context) {
    CGPoint textPosition = CGContextGetTextPosition(context);
    LLStyledStringShowGlyphsRunIterationFunctionUserData userData = { .context = context, .textPosition = textPosition };
    LLStyledStringIterateRuns(styledString, (LLStyledStringRunIterationFunction)LLStyledStringShowGlyphsRunIterationFunction, &userData);
}

CGPathRef LLStyledStringGetCGPath(LLStyledStringRef styledString) {
    LLStyledStringEnsurePath(styledString);
    return styledString->pathRef;
}

CFAttributedStringRef LLStyledStringGetAttributedString(LLStyledStringRef styledString) {
    return styledString->stringRef;
}

char * LLStyledStringCopyString(LLStyledStringRef styledString) {
    CFStringRef stringRef = CFAttributedStringGetString(styledString->stringRef);
    return LLConvertQuartzString(stringRef);
}
