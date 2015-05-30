//
//  BFPaintMode.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
//

#include "BFBase.h"

#include "BFPaintMode.h"

struct BFPaintMode {
    struct BFBase __base;
    BFPaintModeType type;
};

static void BFPaintModeInit(BFPaintModeRef paintMode, BFPaintModeType type);
static void BFPaintModeDealloc(BFPaintModeRef paintMode);

static const BFBaseFunctions baseFunctions = {
    .name = BFPaintModeClassName,
    .dealloc = (BFBaseDeallocFunction)&BFPaintModeDealloc,
};

BFPaintModeRef BFPaintModeCreate(BFPaintModeType type) {
    BFPaintModeRef paintMode = BFAlloc(sizeof(struct BFPaintMode), &baseFunctions);
    if (paintMode) {
        BFPaintModeInit(paintMode, type);
    }
    return BFRetain(paintMode);
}

static void BFPaintModeInit(BFPaintModeRef paintMode, BFPaintModeType type) {
    paintMode->type = type;
}

static void BFPaintModeDealloc(BFPaintModeRef paintMode) {
    if (paintMode) {
    }
    BFDealloc(paintMode);
}

CGBlendMode BFPaintModeCGBlendMode(BFPaintModeRef paintMode) {
    switch (paintMode->type) {
        case kBFPaintModeNormal:
            return kCGBlendModeNormal;
        case kBFPaintModeMultiply:
            return kCGBlendModeMultiply;
        case kBFPaintModeScreen:
            return kCGBlendModeScreen;
        case kBFPaintModeOverlay:
            return kCGBlendModeOverlay;
        case kBFPaintModeDarken:
            return kCGBlendModeDarken;
        case kBFPaintModeLighten:
            return kCGBlendModeLighten;
        case kBFPaintModeColorDodge:
            return kCGBlendModeColorDodge;
        case kBFPaintModeColorBurn:
            return kCGBlendModeColorBurn;
        case kBFPaintModeSoftLight:
            return kCGBlendModeSoftLight;
        case kBFPaintModeHardLight:
            return kCGBlendModeHardLight;
        case kBFPaintModeDifference:
            return kCGBlendModeDifference;
        case kBFPaintModeExclusion:
            return kCGBlendModeExclusion;
        case kBFPaintModeHue:
            return kCGBlendModeHue;
        case kBFPaintModeSaturation:
            return kCGBlendModeSaturation;
        case kBFPaintModeColor:
            return kCGBlendModeColor;
        case kBFPaintModeLuminosity:
            return kCGBlendModeLuminosity;
        case kBFPaintModeClear:
            return kCGBlendModeClear;
        case kBFPaintModeCopy:
            return kCGBlendModeCopy;
        case kBFPaintModeSourceIn:
            return kCGBlendModeSourceIn;
        case kBFPaintModeSourceOut:
            return kCGBlendModeSourceOut;
        case kBFPaintModeSourceAtop:
            return kCGBlendModeSourceAtop;
        case kBFPaintModeDestinationOver:
            return kCGBlendModeDestinationOver;
        case kBFPaintModeDestinationIn:
            return kCGBlendModeDestinationIn;
        case kBFPaintModeDestinationOut:
            return kCGBlendModeDestinationOut;
        case kBFPaintModeDestinationAtop:
            return kCGBlendModeDestinationAtop;
        case kBFPaintModeXOR:
            return kCGBlendModeXOR;
        case kBFPaintModePlusDarker:
            return kCGBlendModePlusDarker;
        case kBFPaintModePlusLighter:
            return kCGBlendModePlusLighter;
        default:
            return kCGBlendModeNormal;
    }
}
