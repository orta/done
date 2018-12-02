//
//  AppDelegate.m
//  Done
//
//  Created by Orta Therox on 12/1/18.
//  Copyright © 2018 K.I.S.S. All rights reserved.
//

#import "AppDelegate.h"
#import "GetProcessInfo.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
//    Moment *moment = [GetProcessInfo printMomentForApp:MVSCode];
}

- (void)applicationDidBecomeActive:(NSNotification *)notification
{
    for (NSWindow *window in [NSApp windows]) {
        [window makeKeyAndOrderFront:self];
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
