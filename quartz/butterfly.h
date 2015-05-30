//
//  butterfly.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __BUTTERFLY_H__
#define __BUTTERFLY_H__

#include <CoreFoundation/CoreFoundation.h>

typedef struct {
    double x;
    double y;
} BFPoint;

typedef struct {
    double left;
    double bottom;
    double right;
    double top;
} BFRect;

void * BFRetain(void * base);
void BFRelease(void * base);

typedef struct BFCanvas * BFCanvasRef;
typedef struct BFCanvasMetrics * BFCanvasMetricsRef;
typedef struct BFColorPaint * BFColorPaintRef;
typedef struct BFFont * BFFontRef;
typedef struct BFGradientPaint * BFGradientPaintRef;
typedef struct BFIcon * BFIconRef;
typedef struct BFPaint * BFPaintRef;
typedef struct BFPaintMode * BFPaintModeRef;
typedef struct BFPath * BFPathRef;
typedef struct BFStyledString * BFStyledStringRef;
typedef struct BFTransformation * BFTransformationRef;

#define BFCanvasClassName "butterfly.Canvas"
#define BFCanvasMetricsClassName "butterfly.CanvasMetrics"
#define BFColorPaintClassName "butterfly.ColorPaint"
#define BFFontClassName "butterfly.Font"
#define BFGradientPaintClassName "butterfly.GradientPaint"
#define BFIconClassName "butterfly.Icon"
#define BFPaintClassName "butterfly.Paint"
#define BFPaintModeClassName "butterfly.PaintMode"
#define BFPathClassName "butterfly.Path"
#define BFStyledStringClassName "butterfly.StyledString"
#define BFTransformationClassName "butterfly.Transformation"

// BFCanvas

// BFCanvasRef BFCanvasCreateForDisplay(CGContextRef context, BFCanvasMetricsRef metrics);
BFCanvasRef BFCanvasCreateForHitTest(BFCanvasMetricsRef metrics);

BFCanvasMetricsRef BFCanvasGetMetrics(BFCanvasRef canvas);
void BFCanvasSetDirtyRect(BFCanvasRef canvas, BFRect rect);
BFRect BFCanvasGetDirtyRect(BFCanvasRef canvas);
void BFCanvasSetOpacity(BFCanvasRef canvas, double opacity);
void BFCanvasSetPaint(BFCanvasRef canvas, BFPaintRef paint);
void BFCanvasSetPaintMode(BFCanvasRef canvas, BFPaintModeRef paintMode);
void BFCanvasSetFont(BFCanvasRef canvas, BFFontRef font);
BFFontRef BFCanvasGetFont(BFCanvasRef canvas);
void BFCanvasSetThickness(BFCanvasRef canvas, double thickness);
void BFCanvasConcatTransformation(BFCanvasRef canvas, BFTransformationRef transformation);
void BFCanvasClipRect(BFCanvasRef canvas, BFRect rect);
void BFCanvasClipPath(BFCanvasRef canvas, const BFPathRef path);
void BFCanvasClipIcon(BFCanvasRef canvas, const BFIconRef icon, BFRect rect);
void BFCanvasPush(BFCanvasRef canvas);
void BFCanvasPop(BFCanvasRef canvas);
void BFCanvasNukeStack(BFCanvasRef canvas);

void BFCanvasFillPath(BFCanvasRef canvas, const BFPathRef path);
void BFCanvasStrokePath(BFCanvasRef canvas, const BFPathRef path);
void BFCanvasDrawStyledString(BFCanvasRef canvas, BFStyledStringRef styledString, BFPoint point);
void BFCanvasStrokeStyledString(BFCanvasRef canvas, BFStyledStringRef styledString, BFPoint point);
void BFCanvasDrawIcon(BFCanvasRef canvas, const BFIconRef icon, BFRect rect);

bool BFCanvasIsHitTest(BFCanvasRef canvas);
bool BFCanvasPerformHitTest(BFCanvasRef canvas);

// BFCanvasMetrics

BFCanvasMetricsRef BFCanvasMetricsCreate(BFRect boundsRect, double backingScale, double pointScale);

BFRect BFCanvasMetricsGetBoundsRect(BFCanvasMetricsRef canvasMetrics);
double BFCanvasMetricsGetBackingScale(BFCanvasMetricsRef canvasMetrics);
double BFCanvasMetricsGetPointScale(BFCanvasMetricsRef canvasMetrics);

// BFColorPaint

BFColorPaintRef BFColorPaintCreate(void);

void BFColorPaintSetRGBA(BFColorPaintRef colorPaint, double r, double g, double b, double a);
void BFColorPaintGetRGBA(BFColorPaintRef colorPaint, double * r, double * g, double * b, double * a);

bool BFColorPaintEquals(BFColorPaintRef colorPaint1, BFColorPaintRef colorPaint2);

// BFFont

BFFontRef BFFontCreate(const char * name, double size);
BFFontRef BFFontCreateSystem(double size);
BFFontRef BFFontCreateBoldSystem(double size);

char * BFFontCopyName(BFFontRef font);
double BFFontGetSize(BFFontRef font);
double BFFontGetAscent(BFFontRef font);
double BFFontGetDescent(BFFontRef font);
double BFFontGetLeading(BFFontRef font);

// BFGradientPaint

BFGradientPaintRef BFGradientPaintCreate(void);

void BFGradientPaintSetColors(BFGradientPaintRef gradientPaint, int count, const BFColorPaintRef * colorPaints, const double * locations);
void BFGradientPaintSetLinearLocation(BFGradientPaintRef gradientPaint, BFPoint startPoint, BFPoint endPoint);
void BFGradientPaintSetRadialLocation(BFGradientPaintRef gradientPaint, BFPoint startCenter, double startRadius, BFPoint endCenter, double endRadius);

// BFIcon

BFIconRef BFIconCreate(BFRect boundsRect);
// BFIconRef BFIconCreateWithImage(CGImageRef image, double width, double height);

BFCanvasRef BFIconGetCanvas(BFIconRef icon);

// BFPaintMode

typedef enum BFPaintModeType {
    kBFPaintModeNormal,
    kBFPaintModeMultiply,
    kBFPaintModeScreen,
    kBFPaintModeOverlay,
    kBFPaintModeDarken,
    kBFPaintModeLighten,
    kBFPaintModeColorDodge,
    kBFPaintModeColorBurn,
    kBFPaintModeSoftLight,
    kBFPaintModeHardLight,
    kBFPaintModeDifference,
    kBFPaintModeExclusion,
    kBFPaintModeHue,
    kBFPaintModeSaturation,
    kBFPaintModeColor,
    kBFPaintModeLuminosity,
    kBFPaintModeClear,
    kBFPaintModeCopy,
    kBFPaintModeSourceIn,
    kBFPaintModeSourceOut,
    kBFPaintModeSourceAtop,
    kBFPaintModeDestinationOver,
    kBFPaintModeDestinationIn,
    kBFPaintModeDestinationOut,
    kBFPaintModeDestinationAtop,
    kBFPaintModeXOR,
    kBFPaintModePlusDarker,
    kBFPaintModePlusLighter,
} BFPaintModeType;

BFPaintModeRef BFPaintModeCreate(BFPaintModeType type);

// BFPath

BFPathRef BFPathCreate(void);

void BFPathMoveToPoint(BFPathRef path, BFPoint point);
void BFPathAddLineToPoint(BFPathRef path, BFPoint point);
void BFPathAddCurveToPoint(BFPathRef path, BFPoint point, BFPoint controlPoint1, BFPoint controlPoint2);
void BFPathAddArc(BFPathRef path, BFPoint centerPoint, double arcAngle);
void BFPathCloseSubpath(BFPathRef path);
void BFPathAddRect(BFPathRef path, BFRect rect);
void BFPathAddRoundedRect(BFPathRef path, BFRect rect, double radius);
void BFPathAddOvalInRect(BFPathRef path, BFRect rect);

// BFStyledString

BFStyledStringRef BFStyledStringCreate(const char * string, BFFontRef font, int superscriptIndex);
// BFStyledStringRef BFStyledStringCreateUsingAttributedString(CFAttributedStringRef attributedString);
BFStyledStringRef BFStyledStringCreateJoining(BFStyledStringRef styledString1, BFStyledStringRef styledString2);
BFStyledStringRef BFStyledStringCreateTruncating(BFStyledStringRef styledString, double width);
CFIndex BFStyledStringCreateBreaking(BFStyledStringRef styledString, CFIndex startPosition, double width, CFIndex lineCount, BFStyledStringRef resultStyledStrings[]);

CFIndex BFStyledStringGetLength(BFStyledStringRef styledString);
BFRect BFStyledStringMeasure(BFStyledStringRef styledString);
char * BFStyledStringCopyString(BFStyledStringRef styledString);

// BFTransformation

BFTransformationRef BFTransformationCreate(void);

void BFTransformationRotate(BFTransformationRef transformation, double angle);
void BFTransformationTranslate(BFTransformationRef transformation, double dx, double dy);
void BFTransformationScale(BFTransformationRef transformation, double ratio);
void BFTransformationInvert(BFTransformationRef transformation);
void BFTransformationConcat(BFTransformationRef transformation1, BFTransformationRef transformation2);

BFPoint BFTransformationTransformPoint(BFTransformationRef transformation, BFPoint point);
BFRect BFTransformationTransformRect(BFTransformationRef transformation, BFRect rect);

#endif /* __BUTTERFLY_H__ */
