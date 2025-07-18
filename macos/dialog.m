#import <Cocoa/Cocoa.h>
#include "dialog.h"

static char filePath[1024] = {0};

const char *openFileDialog() {
    @autoreleasepool {
        NSOpenPanel *panel = [NSOpenPanel openPanel];
        [panel setCanChooseFiles:YES];
        [panel setCanChooseDirectories:NO];
        [panel setAllowsMultipleSelection:NO];

        if ([panel runModal] == NSModalResponseOK) {
            NSString *path = [[[panel URLs] firstObject] path];
            strncpy(filePath, [path UTF8String], sizeof(filePath));
            return filePath;
        }
    }
    return NULL;
}

const char *saveFileDialog() {
    @autoreleasepool {
        NSSavePanel *panel = [NSSavePanel savePanel];

        if ([panel runModal] == NSModalResponseOK) {
            NSString *path = [[panel URL] path];
            strncpy(filePath, [path UTF8String], sizeof(filePath));
            return filePath;
        }
    }
    return NULL;
}
