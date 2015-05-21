//
//  LLPaintMode.h
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#ifndef __LL_PAINT_MODE_H__
#define __LL_PAINT_MODE_H__

#include "LLBase.h"

#define LLPaintModeClassName "Ovaltine.PaintMode"

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

CGBlendMode LLPaintModeCGBlendMode(LLPaintModeRef paintMode);

#endif /* __LL_PAINT_MODE_H__ */
