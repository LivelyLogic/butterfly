//
//  BFCanvas.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "butterfly.h"
#include "quartz.h"

typedef enum BFCanvasType {
    kBFCanvasDisplay,
    kBFCanvasHitTest,
} BFCanvasType;

typedef struct BFCanvasState {
    BFPaintRef paint;
    BFFontRef font;
    struct BFCanvasState * next;
} BFCanvasState;

struct BFCanvas {
    struct BFBase __base;
    BFCanvasType type;
    CGContextRef context;
    BFCanvasMetricsRef metrics;
    BFRect dirtyRect;
    BFCanvasState state;
    unsigned char hitTestData;
};

static void BFCanvasInit(BFCanvasRef canvas, BFCanvasType type, CGContextRef context, BFCanvasMetricsRef metrics);
static void BFCanvasDealloc(BFCanvasRef canvas);

CF_RETURNS_RETAINED static CGContextRef BFCanvasNewBitmapContext(void * data);
static void BFCanvasStrokeCGPath(BFCanvasRef canvas, CGPathRef path);
static void BFCanvasFillCGPath(BFCanvasRef canvas, CGPathRef path);
static void BFCanvasFillClipBoundingBox(BFCanvasRef canvas);

static const BFBaseFunctions baseFunctions = {
    .name = BFCanvasClassName,
    .dealloc = (BFBaseDeallocFunction)&BFCanvasDealloc,
};

BFCanvasRef BFCanvasCreateForDisplay(CGContextRef context, BFCanvasMetricsRef metrics) {
    BFCanvasRef canvas = BFAlloc(sizeof(struct BFCanvas), &baseFunctions);
    if (canvas) {
        BFCanvasInit(canvas, kBFCanvasDisplay, context, metrics);
    }
    return BFRetain(canvas);
}

BFCanvasRef BFCanvasCreateForHitTest(BFCanvasMetricsRef metrics) {
    BFCanvasRef canvas = BFAlloc(sizeof(struct BFCanvas), &baseFunctions);
    if (canvas) {
        CGContextRef context = BFCanvasNewBitmapContext(&canvas->hitTestData);
        BFCanvasInit(canvas, kBFCanvasHitTest, context, metrics);
        CGContextRelease(context);
    }
    return BFRetain(canvas);
}

static void BFCanvasInit(BFCanvasRef canvas, BFCanvasType type, CGContextRef context, BFCanvasMetricsRef metrics) {
    canvas->type = type;
    canvas->context = CGContextRetain(context);
    canvas->metrics = BFRetain(metrics);
    BFCanvasSetDirtyRect(canvas, BFCanvasMetricsGetBoundsRect(metrics));
    BFColorPaintRef colorPaint = BFColorPaintCreate();
    BFColorPaintSetRGBA(colorPaint, 0, 0, 0, 1);
    canvas->state.paint = (BFPaintRef)colorPaint;
    canvas->state.font = BFFontCreate("Helvetica", 14);
    canvas->state.next = NULL;
    canvas->hitTestData = 0xff;
    CGContextSetLineCap(context, kCGLineCapButt);
    CGContextSetLineJoin(context, kCGLineJoinRound);
    CGContextSetMiterLimit(context, 2);
}

static void BFCanvasDealloc(BFCanvasRef canvas) {
    if (canvas) {
        BFCanvasNukeStack(canvas);
        CGContextRelease(canvas->context);
        BFRelease(canvas->metrics);
        BFRelease(canvas->state.paint);
        BFRelease(canvas->state.font);
    }
    BFDealloc(canvas);
}

CF_RETURNS_RETAINED
static CGContextRef BFCanvasNewBitmapContext(void * data) {
    CGColorSpaceRef colorSpace;
    CGContextRef context;
    
    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericGray);
    context = CGBitmapContextCreate(data, 1, 1, 8, 1, colorSpace, kCGImageAlphaNone);
    CGColorSpaceRelease(colorSpace);
    
    if (context) {
        CGContextSetShouldAntialias(context, false);
        CGContextSetShouldSmoothFonts(context, false);
    }
    
    return context;
}

CGContextRef BFCanvasGetCGContext(BFCanvasRef canvas) {
    return canvas->context;
}

BFCanvasMetricsRef BFCanvasGetMetrics(BFCanvasRef canvas) {
    return canvas->metrics;
}

void BFCanvasSetDirtyRect(BFCanvasRef canvas, BFRect rect) {
    canvas->dirtyRect = rect;
}

BFRect BFCanvasGetDirtyRect(BFCanvasRef canvas) {
    return canvas->dirtyRect;
}

void BFCanvasSetOpacity(BFCanvasRef canvas, double opacity) {
    if (canvas->type != kBFCanvasHitTest) {
        CGContextSetAlpha(canvas->context, opacity);
    }
}

void BFCanvasSetPaint(BFCanvasRef canvas, BFPaintRef paint) {
    if (paint && canvas->type != kBFCanvasHitTest) {
        BFRetain(paint);
        BFRelease(canvas->state.paint);
        canvas->state.paint = paint;
    }
}

void BFCanvasSetPaintMode(BFCanvasRef canvas, BFPaintModeRef paintMode) {
    if (canvas->type != kBFCanvasHitTest) {
        CGContextSetBlendMode(canvas->context, BFPaintModeCGBlendMode(paintMode));
    }
}

void BFCanvasSetFont(BFCanvasRef canvas, BFFontRef font) {
    BFRetain(font);
    BFRelease(canvas->state.font);
    canvas->state.font = font;
}

BFFontRef BFCanvasGetFont(BFCanvasRef canvas) {
    return canvas->state.font;
}

void BFCanvasSetThickness(BFCanvasRef canvas, double thickness) {
    CGContextSetLineWidth(canvas->context, thickness);
}

void BFCanvasConcatTransformation(BFCanvasRef canvas, BFTransformationRef transformation) {
    CGContextConcatCTM(canvas->context, BFTransformationGetCGAffineTransform(transformation));
}

void BFCanvasClipRect(BFCanvasRef canvas, BFRect rect) {
    CGContextClipToRect(canvas->context, BFRectToCGRect(rect));
}

void BFCanvasClipPath(BFCanvasRef canvas, const BFPathRef path) {
    CGContextAddPath(canvas->context, BFPathGetCGPath(path));
    if (!CGContextIsPathEmpty(canvas->context)) {
        CGContextClip(canvas->context);
    }
}

void BFCanvasClipIcon(BFCanvasRef canvas, const BFIconRef icon, BFRect rect) {
    CGImageRef image = BFIconCopyCGImage(icon);
    CGContextClipToMask(canvas->context, BFRectToCGRect(rect), image);
    CGImageRelease(image);
}

void BFCanvasPush(BFCanvasRef canvas) {
    BFCanvasState * oldState = (BFCanvasState *)malloc(sizeof(BFCanvasState));
    if (oldState) {
        oldState->paint = BFRetain(canvas->state.paint);
        oldState->font = BFRetain(canvas->state.font);
        oldState->next = canvas->state.next;
        canvas->state.next = oldState;
        CGContextSaveGState(canvas->context);
    } else {
        // TODO: error!
    }
}

void BFCanvasPop(BFCanvasRef canvas) {
    BFCanvasState * oldState = canvas->state.next;
    if (canvas->state.next) {
        BFRelease(canvas->state.paint);
        BFRelease(canvas->state.font);
        memcpy(&(canvas->state), oldState, sizeof(BFCanvasState));
        free(oldState);
        CGContextRestoreGState(canvas->context);
    }
}

void BFCanvasNukeStack(BFCanvasRef canvas) {
    while (canvas->state.next) {
        BFCanvasPop(canvas);
    }
}

static void BFCanvasStrokeCGPath(BFCanvasRef canvas, CGPathRef path) {
    CGContextAddPath(canvas->context, path);
    if (BFPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextDrawPath(canvas->context, kCGPathStroke);
    } else {
        if (!CGContextIsPathEmpty(canvas->context)) {
            CGContextReplacePathWithStrokedPath(canvas->context);
            CGContextClip(canvas->context);
            BFCanvasFillClipBoundingBox(canvas);
        }
    }
}

static void BFCanvasFillCGPath(BFCanvasRef canvas, CGPathRef path) {
    CGContextAddPath(canvas->context, path);
    if (BFPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextDrawPath(canvas->context, kCGPathFill);
    } else {
        if (!CGContextIsPathEmpty(canvas->context)) {
            CGContextClip(canvas->context);
            BFCanvasFillClipBoundingBox(canvas);
        }
    }
}

void BFCanvasStrokePath(BFCanvasRef canvas, const BFPathRef path) {
    CGContextSaveGState(canvas->context);
    BFCanvasStrokeCGPath(canvas, BFPathGetCGPath(path));
    CGContextRestoreGState(canvas->context);
}

void BFCanvasFillPath(BFCanvasRef canvas, const BFPathRef path) {
    CGContextSaveGState(canvas->context);
    BFCanvasFillCGPath(canvas, BFPathGetCGPath(path));
    CGContextRestoreGState(canvas->context);
}

static bool BFCanvasIsCGAffineTransformRotated(CGAffineTransform affineTransform) {
    return (affineTransform.b != 0 || affineTransform.c != 0);
}

void BFCanvasDrawStyledString(BFCanvasRef canvas, BFStyledStringRef styledString, BFPoint point) {
    CGContextSaveGState(canvas->context);
    CGAffineTransform ctm = CGContextGetCTM(canvas->context);
    if (!BFCanvasIsCGAffineTransformRotated(ctm) && BFPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextSetTextDrawingMode(canvas->context, kCGTextFill);
        CGContextSetTextMatrix(canvas->context, CGAffineTransformIdentity);
        CGContextSetTextPosition(canvas->context, point.x + round(ctm.tx) - ctm.tx, point.y + round(ctm.ty) - ctm.ty);
        BFStyledStringDrawInCGContext(styledString, canvas->context);
    } else {
        CGAffineTransform transform = CGAffineTransformIdentity;
        transform.tx = point.x;
        transform.ty = point.y;
        CGContextConcatCTM(canvas->context, transform);
        BFCanvasFillCGPath(canvas, BFStyledStringGetCGPath(styledString));
    }
    CGContextRestoreGState(canvas->context);
}

void BFCanvasStrokeStyledString(BFCanvasRef canvas, BFStyledStringRef styledString, BFPoint point) {
    CGContextSaveGState(canvas->context);
    CGAffineTransform ctm = CGContextGetCTM(canvas->context);
    if (!BFCanvasIsCGAffineTransformRotated(ctm) && BFPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextSetTextDrawingMode(canvas->context, kCGTextStroke);
        CGContextSetTextMatrix(canvas->context, CGAffineTransformIdentity);
        CGContextSetTextPosition(canvas->context, point.x + round(ctm.tx) - ctm.tx, point.y + round(ctm.ty) - ctm.ty);
        BFStyledStringDrawInCGContext(styledString, canvas->context);
    } else {
        CGAffineTransform transform = CGAffineTransformIdentity;
        transform.tx = point.x;
        transform.ty = point.y;
        CGContextConcatCTM(canvas->context, transform);
        CGContextSetFlatness(canvas->context, 1);
        BFCanvasStrokeCGPath(canvas, BFStyledStringGetCGPath(styledString));
    }
    CGContextRestoreGState(canvas->context);
}

void BFCanvasDrawIcon(BFCanvasRef canvas, const BFIconRef icon, BFRect rect) {
    CGImageRef image = BFIconCopyCGImage(icon);
    CGContextDrawImage(canvas->context, BFRectToCGRect(rect), image);
    CGImageRelease(image);
}

static void BFCanvasFillClipBoundingBox(BFCanvasRef canvas) {
    // TODO: this works, but the bounding box will grow when converted to & from device space, if the user space is rotated.
    CGRect deviceRect = CGContextConvertRectToDeviceSpace(canvas->context, CGContextGetClipBoundingBox(canvas->context));
    CGRect integralDeviceRect;
    integralDeviceRect.origin = CGPointMake(floor(CGRectGetMinX(deviceRect)), floor(CGRectGetMinY(deviceRect)));
    integralDeviceRect.size = CGSizeMake(ceil(CGRectGetMaxX(deviceRect)) - integralDeviceRect.origin.x, ceil(CGRectGetMaxY(deviceRect)) - integralDeviceRect.origin.y);
    BFPaintFillRectInContext(canvas->state.paint, canvas->context, CGContextConvertRectToUserSpace(canvas->context, integralDeviceRect));
}

bool BFCanvasIsHitTest(BFCanvasRef canvas) {
    return (canvas->type == kBFCanvasHitTest);
}

bool BFCanvasPerformHitTest(BFCanvasRef canvas) {
    return (canvas->type == kBFCanvasHitTest && canvas->hitTestData != 0xff);
}
