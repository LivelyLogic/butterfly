//
//  types.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __TYPES_H__
#define __TYPES_H__

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

#endif /* __TYPES_H__ */
