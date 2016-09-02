//
//  BFQuartzTypes.c
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

#include "BFQuartzTypes.h"

char * BFConvertQuartzString(CFStringRef string) {
    CFIndex length = CFStringGetLength(string);
    CFRange range = CFRangeMake(0, length);
    CFIndex converted = CFStringGetBytes(string, range, kCFStringEncodingUTF8, 0, false, NULL, 0, &length);
    char * buffer = NULL;
    if (converted > 0) {
        buffer = malloc(length + 1);
        CFStringGetBytes(string, range, kCFStringEncodingUTF8, 0, false, (unsigned char *)buffer, length, NULL);
        buffer[length] = 0;
    }
    return buffer;
}
