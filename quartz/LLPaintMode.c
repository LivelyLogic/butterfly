//
//  LLPaintMode.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "LLPaintMode.h"

struct LLPaintMode {
    struct LLBase __base;
    LLPaintModeType type;
};

static void LLPaintModeInit(LLPaintModeRef paintMode, LLPaintModeType type);
static void LLPaintModeDealloc(LLPaintModeRef paintMode);

static const LLBaseFunctions baseFunctions = {
    .name = LLPaintModeClassName,
    .dealloc = (LLBaseDeallocFunction)&LLPaintModeDealloc,
};

LLPaintModeRef LLPaintModeCreate(LLPaintModeType type) {
    LLPaintModeRef paintMode = LLAlloc(sizeof(struct LLPaintMode), &baseFunctions);
    if (paintMode) {
        LLPaintModeInit(paintMode, type);
    }
    return LLRetain(paintMode);
}

static void LLPaintModeInit(LLPaintModeRef paintMode, LLPaintModeType type) {
    paintMode->type = type;
}

static void LLPaintModeDealloc(LLPaintModeRef paintMode) {
    if (paintMode) {
    }
    LLDealloc(paintMode);
}

CGBlendMode LLPaintModeCGBlendMode(LLPaintModeRef paintMode) {
    switch (paintMode->type) {
        case kLLPaintModeNormal:
            return kCGBlendModeNormal;
        case kLLPaintModeMultiply:
            return kCGBlendModeMultiply;
        case kLLPaintModeScreen:
            return kCGBlendModeScreen;
        case kLLPaintModeOverlay:
            return kCGBlendModeOverlay;
        case kLLPaintModeDarken:
            return kCGBlendModeDarken;
        case kLLPaintModeLighten:
            return kCGBlendModeLighten;
        case kLLPaintModeColorDodge:
            return kCGBlendModeColorDodge;
        case kLLPaintModeColorBurn:
            return kCGBlendModeColorBurn;
        case kLLPaintModeSoftLight:
            return kCGBlendModeSoftLight;
        case kLLPaintModeHardLight:
            return kCGBlendModeHardLight;
        case kLLPaintModeDifference:
            return kCGBlendModeDifference;
        case kLLPaintModeExclusion:
            return kCGBlendModeExclusion;
        case kLLPaintModeHue:
            return kCGBlendModeHue;
        case kLLPaintModeSaturation:
            return kCGBlendModeSaturation;
        case kLLPaintModeColor:
            return kCGBlendModeColor;
        case kLLPaintModeLuminosity:
            return kCGBlendModeLuminosity;
        case kLLPaintModeClear:
            return kCGBlendModeClear;
        case kLLPaintModeCopy:
            return kCGBlendModeCopy;
        case kLLPaintModeSourceIn:
            return kCGBlendModeSourceIn;
        case kLLPaintModeSourceOut:
            return kCGBlendModeSourceOut;
        case kLLPaintModeSourceAtop:
            return kCGBlendModeSourceAtop;
        case kLLPaintModeDestinationOver:
            return kCGBlendModeDestinationOver;
        case kLLPaintModeDestinationIn:
            return kCGBlendModeDestinationIn;
        case kLLPaintModeDestinationOut:
            return kCGBlendModeDestinationOut;
        case kLLPaintModeDestinationAtop:
            return kCGBlendModeDestinationAtop;
        case kLLPaintModeXOR:
            return kCGBlendModeXOR;
        case kLLPaintModePlusDarker:
            return kCGBlendModePlusDarker;
        case kLLPaintModePlusLighter:
            return kCGBlendModePlusLighter;
        default:
            return kCGBlendModeNormal;
    }
}
