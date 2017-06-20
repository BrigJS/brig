#import <AppKit/AppKit.h>
#import "mac.h"

BOOL powersave = FALSE;
NSApplication *app;
@class BrigRunLoopModeTracker;
@interface BrigRunLoopModeTracker : NSObject {
}
@end

@implementation BrigRunLoopModeTracker

- (id) init
{
    if (self = [super init]) {

        [[NSNotificationCenter defaultCenter]
            addObserver:self
            selector:@selector(receivedNotification:)
            name:nil
            object:[NSApplication sharedApplication]];
    }

    return self;
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];

    [super dealloc];
}

- (void) receivedNotification:(NSNotification *) notification
{
	// Trying to save power
	if ([notification name] == NSApplicationDidChangeOcclusionStateNotification) {
		if ([app occlusionState] & NSApplicationOcclusionStateVisible) {
//			printf("NORMAL POWER\n");
			powersave = FALSE;
		} else {
//			printf("POWERSAVED\n");
			powersave = TRUE;
		}
	} else if ([notification name] == NSApplicationWillBecomeActiveNotification ||
		[notification name] == NSApplicationWillFinishLaunchingNotification) {
//		printf("NORMAL POWER\n");
		powersave = FALSE;
	}
}

@end

bool isPowerSaveMode()
{
	return powersave;
}

void prepareMacWindowSystem()
{
	app = [NSApplication sharedApplication];

	[[BrigRunLoopModeTracker alloc] init];

    [app finishLaunching];
}

void respondMacWindowSystem()
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	while(1) {
		[pool release];
		pool = [[NSAutoreleasePool alloc] init];

		NSEvent *event = [app
			nextEventMatchingMask:NSAnyEventMask
			untilDate:[NSDate distantPast]
			inMode:NSDefaultRunLoopMode
			dequeue:YES];

		if (!event)
			break;

		[app sendEvent:event];
	}

	[pool release];
}

