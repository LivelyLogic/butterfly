//
//  DrawingView.h
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol DrawingViewDelegate;

@interface DrawingView : NSView

@property (assign) id <DrawingViewDelegate> delegate;

- (void)setDrawingScriptString:(NSString *)drawingScriptString;

@end

@protocol DrawingViewDelegate <NSObject>
- (void)drawingViewDidDraw:(DrawingView *)drawingView;
- (void)drawingView:(DrawingView *)drawingView didEncounterError:(NSString *)errorMessage onLine:(NSInteger)lineNumber;
@end
