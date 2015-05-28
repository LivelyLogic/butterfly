//
//  types.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __TYPES_H__
#define __TYPES_H__

#include <CoreFoundation/CoreFoundation.h>

typedef struct {
    double x;
    double y;
} LLPoint;

typedef struct {
    double left;
    double bottom;
    double right;
    double top;
} LLRect;

void * LLRetain(void * base);
void LLRelease(void * base);

typedef struct LLCanvas * LLCanvasRef;
typedef struct LLCanvasMetrics * LLCanvasMetricsRef;
typedef struct LLColorPaint * LLColorPaintRef;
typedef struct LLFont * LLFontRef;
typedef struct LLGradientPaint * LLGradientPaintRef;
typedef struct LLIcon * LLIconRef;
typedef struct LLPaint * LLPaintRef;
typedef struct LLPaintMode * LLPaintModeRef;
typedef struct LLPath * LLPathRef;
typedef struct LLStyledString * LLStyledStringRef;
typedef struct LLTransformation * LLTransformationRef;

#define LLCanvasClassName "Ovaltine.Canvas"
#define LLCanvasMetricsClassName "Ovaltine.CanvasMetrics"
#define LLColorPaintClassName "Ovaltine.ColorPaint"
#define LLFontClassName "Ovaltine.Font"
#define LLGradientPaintClassName "Ovaltine.GradientPaint"
#define LLIconClassName "Ovaltine.Icon"
#define LLPaintClassName "Ovaltine.Paint"
#define LLPaintModeClassName "Ovaltine.PaintMode"
#define LLPathClassName "Ovaltine.Path"
#define LLStyledStringClassName "Ovaltine.StyledString"
#define LLTransformationClassName "Ovaltine.Transformation"

// LLCanvas

LLCanvasRef LLCanvasCreateForHitTest(LLCanvasMetricsRef metrics);

LLCanvasMetricsRef LLCanvasGetMetrics(LLCanvasRef canvas);
void LLCanvasSetDirtyRect(LLCanvasRef canvas, LLRect rect);
LLRect LLCanvasGetDirtyRect(LLCanvasRef canvas);
void LLCanvasSetOpacity(LLCanvasRef canvas, double opacity);
void LLCanvasSetPaint(LLCanvasRef canvas, LLPaintRef paint);
void LLCanvasSetPaintMode(LLCanvasRef canvas, LLPaintModeRef paintMode);
void LLCanvasSetFont(LLCanvasRef canvas, LLFontRef font);
LLFontRef LLCanvasGetFont(LLCanvasRef canvas);
void LLCanvasSetThickness(LLCanvasRef canvas, double thickness);
void LLCanvasConcatTransformation(LLCanvasRef canvas, LLTransformationRef transformation);
void LLCanvasClipRect(LLCanvasRef canvas, LLRect rect);
void LLCanvasClipPath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasClipIcon(LLCanvasRef canvas, const LLIconRef icon, LLRect rect);
void LLCanvasPush(LLCanvasRef canvas);
void LLCanvasPop(LLCanvasRef canvas);
void LLCanvasNukeStack(LLCanvasRef canvas);

void LLCanvasFillPath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasStrokePath(LLCanvasRef canvas, const LLPathRef path);
void LLCanvasDrawStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, LLPoint point);
void LLCanvasStrokeStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, LLPoint point);
void LLCanvasDrawIcon(LLCanvasRef canvas, const LLIconRef icon, LLRect rect);

bool LLCanvasIsHitTest(LLCanvasRef canvas);
bool LLCanvasPerformHitTest(LLCanvasRef canvas);

// LLCanvasMetrics

LLCanvasMetricsRef LLCanvasMetricsCreate(LLRect boundsRect, double backingScale, double pointScale);

LLRect LLCanvasMetricsGetBoundsRect(LLCanvasMetricsRef canvasMetrics);
double LLCanvasMetricsGetBackingScale(LLCanvasMetricsRef canvasMetrics);
double LLCanvasMetricsGetPointScale(LLCanvasMetricsRef canvasMetrics);

// LLColorPaint

LLColorPaintRef LLColorPaintCreate(void);

void LLColorPaintSetRGBA(LLColorPaintRef colorPaint, double r, double g, double b, double a);
void LLColorPaintGetRGBA(LLColorPaintRef colorPaint, double * r, double * g, double * b, double * a);

bool LLColorPaintEquals(LLColorPaintRef colorPaint1, LLColorPaintRef colorPaint2);

// LLFont

LLFontRef LLFontCreate(const char * name, double size);
LLFontRef LLFontCreateSystem(double size);
LLFontRef LLFontCreateBoldSystem(double size);

char * LLFontCopyName(LLFontRef font);
double LLFontGetSize(LLFontRef font);
double LLFontGetAscent(LLFontRef font);
double LLFontGetDescent(LLFontRef font);
double LLFontGetLeading(LLFontRef font);

// LLGradientPaint

LLGradientPaintRef LLGradientPaintCreate(void);

void LLGradientPaintSetColors(LLGradientPaintRef gradientPaint, int count, const LLColorPaintRef * colorPaints, const double * locations);
void LLGradientPaintSetLinearLocation(LLGradientPaintRef gradientPaint, LLPoint startPoint, LLPoint endPoint);
void LLGradientPaintSetRadialLocation(LLGradientPaintRef gradientPaint, LLPoint startCenter, double startRadius, LLPoint endCenter, double endRadius);

// LLIcon

LLIconRef LLIconCreate(LLRect boundsRect);

LLCanvasRef LLIconGetCanvas(LLIconRef icon);

// LLPaintMode

typedef enum LLPaintModeType {
    kLLPaintModeNormal,
    kLLPaintModeMultiply,
    kLLPaintModeScreen,
    kLLPaintModeOverlay,
    kLLPaintModeDarken,
    kLLPaintModeLighten,
    kLLPaintModeColorDodge,
    kLLPaintModeColorBurn,
    kLLPaintModeSoftLight,
    kLLPaintModeHardLight,
    kLLPaintModeDifference,
    kLLPaintModeExclusion,
    kLLPaintModeHue,
    kLLPaintModeSaturation,
    kLLPaintModeColor,
    kLLPaintModeLuminosity,
    kLLPaintModeClear,
    kLLPaintModeCopy,
    kLLPaintModeSourceIn,
    kLLPaintModeSourceOut,
    kLLPaintModeSourceAtop,
    kLLPaintModeDestinationOver,
    kLLPaintModeDestinationIn,
    kLLPaintModeDestinationOut,
    kLLPaintModeDestinationAtop,
    kLLPaintModeXOR,
    kLLPaintModePlusDarker,
    kLLPaintModePlusLighter,
} LLPaintModeType;

LLPaintModeRef LLPaintModeCreate(LLPaintModeType type);

// LLPath

LLPathRef LLPathCreate(void);

void LLPathMoveToPoint(LLPathRef path, LLPoint point);
void LLPathAddLineToPoint(LLPathRef path, LLPoint point);
void LLPathAddCurveToPoint(LLPathRef path, LLPoint point, LLPoint controlPoint1, LLPoint controlPoint2);
void LLPathAddArc(LLPathRef path, LLPoint centerPoint, double arcAngle);
void LLPathCloseSubpath(LLPathRef path);
void LLPathAddRect(LLPathRef path, LLRect rect);
void LLPathAddRoundedRect(LLPathRef path, LLRect rect, double radius);
void LLPathAddOvalInRect(LLPathRef path, LLRect rect);

// LLStyledString

LLStyledStringRef LLStyledStringCreate(const char * string, LLFontRef font, int superscriptIndex);
LLStyledStringRef LLStyledStringCreateJoining(LLStyledStringRef styledString1, LLStyledStringRef styledString2);
LLStyledStringRef LLStyledStringCreateTruncating(LLStyledStringRef styledString, double width);
CFIndex LLStyledStringCreateBreaking(LLStyledStringRef styledString, CFIndex startPosition, double width, CFIndex lineCount, LLStyledStringRef resultStyledStrings[]);

CFIndex LLStyledStringGetLength(LLStyledStringRef styledString);
LLRect LLStyledStringMeasure(LLStyledStringRef styledString);
char * LLStyledStringCopyString(LLStyledStringRef styledString);

// LLTransformation

LLTransformationRef LLTransformationCreate(void);

void LLTransformationRotate(LLTransformationRef transformation, double angle);
void LLTransformationTranslate(LLTransformationRef transformation, double dx, double dy);
void LLTransformationScale(LLTransformationRef transformation, double ratio);
void LLTransformationInvert(LLTransformationRef transformation);
void LLTransformationConcat(LLTransformationRef transformation1, LLTransformationRef transformation2);

LLPoint LLTransformationTransformPoint(LLTransformationRef transformation, LLPoint point);
LLRect LLTransformationTransformRect(LLTransformationRef transformation, LLRect rect);

#endif /* __TYPES_H__ */
