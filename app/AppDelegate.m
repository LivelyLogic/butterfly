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
@property (assign) IBOutlet NSTextField * errorLabel;
@end

@interface AppDelegate (NSTextViewDelegate) <NSTextViewDelegate>
@end

@interface AppDelegate (DrawingViewDelegate) <DrawingViewDelegate>
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
    
    self.drawingView.delegate = self;
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


@implementation AppDelegate (DrawingViewDelegate)

- (void)drawingViewDidDraw:(DrawingView *)drawingView
{
    self.errorLabel.hidden = YES;;
    self.scriptTextView.textColor = [NSColor textColor];
}

- (void)drawingView:(DrawingView *)drawingView
  didEncounterError:(NSString *)errorMessage
             onLine:(NSInteger)lineNumber
{
    self.errorLabel.stringValue = errorMessage;
    self.errorLabel.hidden = NO;
    
    self.scriptTextView.textColor = [NSColor textColor];
    NSString * inputString = self.scriptTextView.string;
    NSInteger lineIndex = 0;
    NSUInteger position = 0;
    while (position < inputString.length) {
        NSRange lineRange = [inputString lineRangeForRange:NSMakeRange(position, 0)];
        if (++lineIndex == lineNumber) {
            [self.scriptTextView.textStorage addAttribute:NSForegroundColorAttributeName
                                                    value:[NSColor redColor]
                                                    range:lineRange];
            return;
        }
        position = NSMaxRange(lineRange);
    }
}

@end
