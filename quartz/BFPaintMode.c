//
//  BFPaintMode.c
//
//  Copyright (c) 2011-2016 Ripeware, LLC
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

#include "butterfly.h"
#include "quartz.h"

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
