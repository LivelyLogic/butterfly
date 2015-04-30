//
//  AppDelegate.m
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#import "AppDelegate.h"

#import "DrawingView.h"

@interface AppDelegate ()
@property (assign) IBOutlet DrawingView * drawingView;
@end

@implementation AppDelegate

- (void)awakeFromNib
{
    NSString * drawingScriptString =
    @"return function(canvas)"
    @"    local purple = Color.rgba(0.5, 0, 1.0, 0.5)"
    @"    local oval = Path.oval(canvas:metrics():rect())"
    @"    canvas:setPaint(purple):fill(oval)"
    @"end";
    [self.drawingView setDrawingScriptString:drawingScriptString];
    
    [super awakeFromNib];
}

@end
