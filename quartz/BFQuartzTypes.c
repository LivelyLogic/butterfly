//
//  BFQuartzTypes.c
//
//  Copyright (c) 2011-2015 Ripeware, LLC. All rights reserved.
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
