//
//  quartz.h
//
//  Copyright (c) 2011-2019 James Rodovich
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

#ifndef __BUTTERFLY_QUARTZ_H__
#define __BUTTERFLY_QUARTZ_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

#include "butterfly.h"

// BFPoint

#define BFPointToCGPoint(point) CGPointMake(point.x, point.y)
#define BFPointFromCGPoint(point) (BFPoint){ .x = point.x, .y = point.y }

// BFRect

#define BFRectToCGRect(rect) CGRectMake(rect.left, rect.bottom, rect.right - rect.left, rect.top - rect.bottom)
#define BFRectFromCGRect(rect) (BFRect){ .left = rect.origin.x, .bottom = rect.origin.y, .right = rect.origin.x + rect.size.width, .top = rect.origin.y + rect.size.height }

// BFCanvas

BFCanvasRef BFCanvasCreateForDisplay(CGContextRef context, BFCanvasMetricsRef metrics);

CGContextRef BFCanvasGetCGContext(BFCanvasRef canvas);

// BFColorPaint

CGColorRef BFColorPaintGetCGColor(BFColorPaintRef colorPaint);

// BFFont

BFFontRef BFFontCreateWithCTFont(CTFontRef font);

CTFontRef BFFontGetCTFont(BFFontRef font);

// BFIcon

BFIconRef BFIconCreateWithCGImage(CGImageRef image, double width, double height);

CGImageRef BFIconCopyCGImage(BFIconRef icon);

// BFPaint

bool BFPaintSetInContext(BFPaintRef paint, CGContextRef context);
void BFPaintFillRectInContext(BFPaintRef paint, CGContextRef context, CGRect rect);

// BFPaintMode

CGBlendMode BFPaintModeCGBlendMode(BFPaintModeRef paintMode);

// BFPath

CGPathRef BFPathGetCGPath(const BFPathRef path);

// BFStyledString

BFStyledStringRef BFStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString);

CFAttributedStringRef BFStyledStringGetAttributedString(BFStyledStringRef styledString);
void BFStyledStringDrawInCGContext(const BFStyledStringRef styledString, CGContextRef context);
CGPathRef BFStyledStringGetCGPath(const BFStyledStringRef styledString);

// BFTransformation

CGAffineTransform BFTransformationGetCGAffineTransform(BFTransformationRef transformation);

#endif /* __BUTTERFLY_QUARTZ_H__ */
