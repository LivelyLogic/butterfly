//
//  LLCanvas.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLCanvas.h"

#include "LLColorPaint.h"
#include "LLQuartzTypes.h"

typedef enum LLCanvasType {
    kLLCanvasDisplay,
    kLLCanvasHitTest,
} LLCanvasType;

typedef struct LLCanvasState {
    LLPaintRef paint;
    LLFontRef font;
    struct LLCanvasState * next;
} LLCanvasState;

struct LLCanvas {
    struct LLBase __base;
    LLCanvasType type;
    CGContextRef context;
    LLCanvasMetricsRef metrics;
    LLRect dirtyRect;
    LLCanvasState state;
    unsigned char hitTestData;
};

static void LLCanvasInit(LLCanvasRef canvas, LLCanvasType type, CGContextRef context, LLCanvasMetricsRef metrics);
static void LLCanvasDealloc(LLCanvasRef canvas);

CF_RETURNS_RETAINED static CGContextRef LLCanvasNewBitmapContext(void * data);
static void LLCanvasStrokeCGPath(LLCanvasRef canvas, CGPathRef path);
static void LLCanvasFillCGPath(LLCanvasRef canvas, CGPathRef path);
static void LLCanvasFillClipBoundingBox(LLCanvasRef canvas);

static const LLBaseFunctions baseFunctions = {
    .name = LLCanvasClassName,
    .dealloc = (LLBaseDeallocFunction)&LLCanvasDealloc,
};

LLCanvasRef LLCanvasCreateForDisplay(CGContextRef context, LLCanvasMetricsRef metrics) {
    LLCanvasRef canvas = LLAlloc(sizeof(struct LLCanvas), &baseFunctions);
    if (canvas) {
        LLCanvasInit(canvas, kLLCanvasDisplay, context, metrics);
    }
    return LLRetain(canvas);
}

LLCanvasRef LLCanvasCreateForHitTest(LLCanvasMetricsRef metrics) {
    LLCanvasRef canvas = LLAlloc(sizeof(struct LLCanvas), &baseFunctions);
    if (canvas) {
        CGContextRef context = LLCanvasNewBitmapContext(&canvas->hitTestData);
        LLCanvasInit(canvas, kLLCanvasHitTest, context, metrics);
        CGContextRelease(context);
    }
    return LLRetain(canvas);
}

static void LLCanvasInit(LLCanvasRef canvas, LLCanvasType type, CGContextRef context, LLCanvasMetricsRef metrics) {
    canvas->type = type;
    canvas->context = CGContextRetain(context);
    canvas->metrics = LLRetain(metrics);
    LLCanvasSetDirtyRect(canvas, LLCanvasMetricsGetBoundsRect(metrics));
    LLColorPaintRef colorPaint = LLColorPaintCreate();
    LLColorPaintSetRGBA(colorPaint, 0, 0, 0, 1);
    canvas->state.paint = (LLPaintRef)colorPaint;
    canvas->state.font = LLFontCreate("Helvetica", 14);
    canvas->state.next = NULL;
    canvas->hitTestData = 0xff;
    CGContextSetLineCap(context, kCGLineCapButt);
    CGContextSetLineJoin(context, kCGLineJoinRound);
    CGContextSetMiterLimit(context, 2);
}

static void LLCanvasDealloc(LLCanvasRef canvas) {
    if (canvas) {
        LLCanvasNukeStack(canvas);
        CGContextRelease(canvas->context);
        LLRelease(canvas->metrics);
        LLRelease(canvas->state.paint);
        LLRelease(canvas->state.font);
    }
    LLDealloc(canvas);
}

CF_RETURNS_RETAINED
static CGContextRef LLCanvasNewBitmapContext(void * data) {
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

CGContextRef LLCanvasGetCGContext(LLCanvasRef canvas) {
    return canvas->context;
}

LLCanvasMetricsRef LLCanvasGetMetrics(LLCanvasRef canvas) {
    return canvas->metrics;
}

void LLCanvasSetDirtyRect(LLCanvasRef canvas, LLRect rect) {
    canvas->dirtyRect = rect;
}

LLRect LLCanvasGetDirtyRect(LLCanvasRef canvas) {
    return canvas->dirtyRect;
}

void LLCanvasSetOpacity(LLCanvasRef canvas, double opacity) {
    if (canvas->type != kLLCanvasHitTest) {
        CGContextSetAlpha(canvas->context, opacity);
    }
}

void LLCanvasSetPaint(LLCanvasRef canvas, LLPaintRef paint) {
    if (paint && canvas->type != kLLCanvasHitTest) {
        LLRetain(paint);
        LLRelease(canvas->state.paint);
        canvas->state.paint = paint;
    }
}

void LLCanvasSetPaintMode(LLCanvasRef canvas, LLPaintModeRef paintMode) {
    if (canvas->type != kLLCanvasHitTest) {
        CGContextSetBlendMode(canvas->context, LLPaintModeCGBlendMode(paintMode));
    }
}

void LLCanvasSetFont(LLCanvasRef canvas, LLFontRef font) {
    LLRetain(font);
    LLRelease(canvas->state.font);
    canvas->state.font = font;
}

LLFontRef LLCanvasGetFont(LLCanvasRef canvas) {
    return canvas->state.font;
}

void LLCanvasSetThickness(LLCanvasRef canvas, double thickness) {
    CGContextSetLineWidth(canvas->context, thickness);
}

void LLCanvasConcatTransformation(LLCanvasRef canvas, LLTransformationRef transformation) {
    CGContextConcatCTM(canvas->context, LLTransformationGetCGAffineTransform(transformation));
}

void LLCanvasClipRect(LLCanvasRef canvas, LLRect rect) {
    CGContextClipToRect(canvas->context, LLConvertRectToQuartz(rect));
}

void LLCanvasClipPath(LLCanvasRef canvas, const LLPathRef path) {
    CGContextAddPath(canvas->context, LLPathGetCGPath(path));
    if (!CGContextIsPathEmpty(canvas->context)) {
        CGContextClip(canvas->context);
    }
}

void LLCanvasPush(LLCanvasRef canvas) {
    LLCanvasState * oldState = (LLCanvasState *)malloc(sizeof(LLCanvasState));
    if (oldState) {
        oldState->paint = LLRetain(canvas->state.paint);
        oldState->font = LLRetain(canvas->state.font);
        oldState->next = canvas->state.next;
        canvas->state.next = oldState;
        CGContextSaveGState(canvas->context);
    } else {
        // TODO: error!
    }
}

void LLCanvasPop(LLCanvasRef canvas) {
    LLCanvasState * oldState = canvas->state.next;
    if (canvas->state.next) {
        LLRelease(canvas->state.paint);
        LLRelease(canvas->state.font);
        memcpy(&(canvas->state), oldState, sizeof(LLCanvasState));
        free(oldState);
        CGContextRestoreGState(canvas->context);
    }
}

void LLCanvasNukeStack(LLCanvasRef canvas) {
    while (canvas->state.next) {
        LLCanvasPop(canvas);
    }
}

static void LLCanvasStrokeCGPath(LLCanvasRef canvas, CGPathRef path) {
    CGContextAddPath(canvas->context, path);
    if (LLPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextDrawPath(canvas->context, kCGPathStroke);
    } else {
        if (!CGContextIsPathEmpty(canvas->context)) {
            CGContextReplacePathWithStrokedPath(canvas->context);
            CGContextClip(canvas->context);
            LLCanvasFillClipBoundingBox(canvas);
        }
    }
}

static void LLCanvasFillCGPath(LLCanvasRef canvas, CGPathRef path) {
    CGContextAddPath(canvas->context, path);
    if (LLPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextDrawPath(canvas->context, kCGPathFill);
    } else {
        if (!CGContextIsPathEmpty(canvas->context)) {
            CGContextClip(canvas->context);
            LLCanvasFillClipBoundingBox(canvas);
        }
    }
}

void LLCanvasStrokePath(LLCanvasRef canvas, const LLPathRef path) {
    CGContextSaveGState(canvas->context);
    LLCanvasStrokeCGPath(canvas, LLPathGetCGPath(path));
    CGContextRestoreGState(canvas->context);
}

void LLCanvasFillPath(LLCanvasRef canvas, const LLPathRef path) {
    CGContextSaveGState(canvas->context);
    LLCanvasFillCGPath(canvas, LLPathGetCGPath(path));
    CGContextRestoreGState(canvas->context);
}

static bool LLCanvasIsCGAffineTransformRotated(CGAffineTransform affineTransform) {
    return (affineTransform.b != 0 || affineTransform.c != 0);
}

void LLCanvasDrawStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, LLPoint point) {
    CGContextSaveGState(canvas->context);
    CGAffineTransform ctm = CGContextGetCTM(canvas->context);
    if (!LLCanvasIsCGAffineTransformRotated(ctm) && LLPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextSetTextDrawingMode(canvas->context, kCGTextFill);
        CGContextSetTextMatrix(canvas->context, CGAffineTransformIdentity);
        CGContextSetTextPosition(canvas->context, point.x + round(ctm.tx) - ctm.tx, point.y + round(ctm.ty) - ctm.ty);
        LLStyledStringDrawInCGContext(styledString, canvas->context);
    } else {
        CGAffineTransform transform = CGAffineTransformIdentity;
        transform.tx = point.x;
        transform.ty = point.y;
        CGContextConcatCTM(canvas->context, transform);
        LLCanvasFillCGPath(canvas, LLStyledStringGetCGPath(styledString));
    }
    CGContextRestoreGState(canvas->context);
}

void LLCanvasStrokeStyledString(LLCanvasRef canvas, LLStyledStringRef styledString, LLPoint point) {
    CGContextSaveGState(canvas->context);
    CGAffineTransform ctm = CGContextGetCTM(canvas->context);
    if (!LLCanvasIsCGAffineTransformRotated(ctm) && LLPaintSetInContext(canvas->state.paint, canvas->context)) {
        CGContextSetTextDrawingMode(canvas->context, kCGTextStroke);
        CGContextSetTextMatrix(canvas->context, CGAffineTransformIdentity);
        CGContextSetTextPosition(canvas->context, point.x + round(ctm.tx) - ctm.tx, point.y + round(ctm.ty) - ctm.ty);
        LLStyledStringDrawInCGContext(styledString, canvas->context);
    } else {
        CGAffineTransform transform = CGAffineTransformIdentity;
        transform.tx = point.x;
        transform.ty = point.y;
        CGContextConcatCTM(canvas->context, transform);
        CGContextSetFlatness(canvas->context, 1);
        LLCanvasStrokeCGPath(canvas, LLStyledStringGetCGPath(styledString));
    }
    CGContextRestoreGState(canvas->context);
}

static void LLCanvasFillClipBoundingBox(LLCanvasRef canvas) {
    // TODO: this works, but the bounding box will grow when converted to & from device space, if the user space is rotated.
    CGRect deviceRect = CGContextConvertRectToDeviceSpace(canvas->context, CGContextGetClipBoundingBox(canvas->context));
    CGRect integralDeviceRect;
    integralDeviceRect.origin = CGPointMake(floor(CGRectGetMinX(deviceRect)), floor(CGRectGetMinY(deviceRect)));
    integralDeviceRect.size = CGSizeMake(ceil(CGRectGetMaxX(deviceRect)) - integralDeviceRect.origin.x, ceil(CGRectGetMaxY(deviceRect)) - integralDeviceRect.origin.y);
    LLPaintFillRectInContext(canvas->state.paint, canvas->context, CGContextConvertRectToUserSpace(canvas->context, integralDeviceRect));
}

bool LLCanvasIsHitTest(LLCanvasRef canvas) {
    return (canvas->type == kLLCanvasHitTest);
}

bool LLCanvasPerformHitTest(LLCanvasRef canvas) {
    return (canvas->type == kLLCanvasHitTest && canvas->hitTestData != 0xff);
}
