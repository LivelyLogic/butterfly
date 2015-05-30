//
//  quartz.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __QUARTZ_H__
#define __QUARTZ_H__

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

#include "types.h"

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

CTFontRef BFFontGetCTFont(BFFontRef font);

// BFIcon

BFIconRef BFIconCreateWithImage(CGImageRef image, double width, double height);

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

#endif /* __QUARTZ_H__ */
