//
//  AppDelegate.m
//
//  Copyright (c) 2015 Ripeware, LLC. All rights reserved.
//

#import "AppDelegate.h"

#import "DrawingView.h"

@interface AppDelegate ()
@property (assign) IBOutlet NSTextView * scriptTextView;
@property (assign) IBOutlet DrawingView * drawingView;
@end

@interface AppDelegate (NSTextViewDelegate) <NSTextViewDelegate>
@end

@implementation AppDelegate

- (void)awakeFromNib
{
    self.scriptTextView.delegate = self;
    self.scriptTextView.automaticDashSubstitutionEnabled = NO;
    self.scriptTextView.automaticQuoteSubstitutionEnabled = NO;
    self.scriptTextView.automaticSpellingCorrectionEnabled = NO;
    self.scriptTextView.automaticTextReplacementEnabled = NO;
    self.scriptTextView.string =
    @"draw = function(canvas)\n"
    @"    local purple = Color.rgba(0.5, 0, 1.0, 0.5)\n"
    @"    local oval = Path.oval(canvas:metrics():rect())\n"
    @"    canvas:setPaint(purple):fill(oval)\n"
    @"end\n";
    [self updateDrawingView];
    
    [super awakeFromNib];
}

- (void)updateDrawingView
{
    [self.drawingView setDrawingScriptString:self.scriptTextView.string];
}

@end

@implementation AppDelegate (NSTextViewDelegate)

- (void)textDidChange:(NSNotification *)notification
{
    [self updateDrawingView];
}

@end
