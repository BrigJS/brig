#import <AppKit/AppKit.h>
#import "mac.h"

void initApplication()
{
	NSApplication *app = [NSApplication sharedApplication];
    [app finishLaunching];
}

void wakeUpApplication()
{
	NSApplication *app = [NSApplication sharedApplication];

	NSEvent *event = [app
		nextEventMatchingMask:NSAnyEventMask
		untilDate:[NSDate distantPast]
		inMode:NSDefaultRunLoopMode
		dequeue:YES];

	[app sendEvent:event];
	[app updateWindows];
}
