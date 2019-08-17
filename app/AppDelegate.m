//
//  AppDelegate.m
//
//  Copyright (c) 2015-2019 James Rodovich
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
    @"    local oval = Path.new():addOval(canvas:metrics():rect())\n"
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
