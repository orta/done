#import "GetProcessInfo.h"
#import "Extensions.h"
#import "Moment.h"
#import "DB.h"
@import AppKit;

//// Notes for sandboxing
////
//// - https://www.jessesquires.com/blog/executing-applescript-in-mac-app-on-macos-mojave/
//// - https://www.felix-schwarz.org/blog/2018/08/new-apple-event-apis-in-macos-mojave
//// - https://www.felix-schwarz.org/blog/2018/06/apple-event-sandboxing-in-macos-mojave

//// - https://gist.github.com/gerad/1645235

@implementation GetProcessInfo

+ (void)sendUserToSysPref
{
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"x-apple.systempreferences:com.apple.preference.security?Privacy_Automation"]];
}

+ (NSString *)bundleIDForApp: (SupportedApps)app {
    switch (app) {
        case MVSCode:
            return @"com.microsoft.VSCode";
        case MTerminal:
            return @"com.apple.Terminal";
        case MSketch:
            return @"com.bohemiancoding.sketch3";
        case MXcode:
            return @"com.apple.dt.Xcode";
        case MSafari:
            return @"com.apple.Safari";
        case MChrome:
            return @"com.google.Chrome";
    }
}

+ (Moment  * _Nullable)printMomentForApp:(SupportedApps)appType
{
    if (![self canUseAXAndAsk:YES]) {
        NSLog(@"Could not use AX");
        return nil;
    }

    NSString *bundleID = [self bundleIDForApp:appType];

    NSArray<NSRunningApplication *> *_Nonnull apps = [[NSWorkspace sharedWorkspace] runningApplications];
    NSRunningApplication *app = [apps first:^BOOL(id obj) {
        return [[obj bundleIdentifier] isEqualToString:bundleID];
    }];

    pid_t pid = [app processIdentifier];

    // Get the accessibility element corresponding to the frontmost application.
    AXUIElementRef appElem = AXUIElementCreateApplication(pid);
    if (!appElem) {
        return nil;
    }

    Moment *moment = [self specializedMomentForApp:app axRoot:appElem];

    NSLog(@"> %@", moment);
    return moment;
}

+ (Moment  * _Nullable)createMomentForFrontApp
{
    NSArray<NSRunningApplication *> *_Nonnull apps = [[NSWorkspace sharedWorkspace] runningApplications];
    NSRunningApplication *app = [apps first:^BOOL(id obj) {
        return [obj isActive];
    }];
    return [self createMomentForApp:app];
}

+ (Moment  * _Nullable)createMomentForApp:(NSRunningApplication *)app
{
    pid_t pid = [app processIdentifier];

    // Get the accessibility element corresponding to the frontmost application.
    AXUIElementRef appElem = AXUIElementCreateApplication(pid);
    if (!appElem) {
        return nil;
    }

    Moment *moment = [self specializedMomentForApp:app axRoot:appElem];
    [DB saveMoment:moment];

    return moment;
}

+ (Moment  * _Nullable)specializedMomentForApp:(NSRunningApplication *)app axRoot:(AXUIElementRef)appElem
{
    NSString *bID = app.bundleIdentifier;
    if ([bID isEqualToString:@"com.microsoft.VSCode"]) {
        return [self momentForVSCode:app axRoot:appElem];
    } else  {
        return [self momentForUnknown:app axRoot:appElem];
    }
}

+ (BOOL)canUseAXAndAsk:(BOOL)quiet
{
    // See if we have accessibility permissions, and if not, prompt the user to
    // visit System Preferences.
    NSDictionary *options = @{(__bridge id)kAXTrustedCheckOptionPrompt: @(quiet)};
    Boolean appHasPermission = AXIsProcessTrustedWithOptions((__bridge CFDictionaryRef)options);

    if (!appHasPermission) {
        NSLog(@"Permission to use AX failed");
#ifdef DEBUG
        NSLog(@"You need to go into the sys prefs, and re-toggle the Done app, then relaunch");
#endif
        return false; // we don't have accessibility permissions
    }

    return true;
}

+ (Moment *)momentForVSCode:(NSRunningApplication *)app axRoot:(AXUIElementRef)appElem
{
    return [self momentForUnknown:app axRoot:appElem];
}

+ (Moment *)momentForUnknown:(NSRunningApplication *)app axRoot:(AXUIElementRef)appElem
{
    Moment *moment = [[Moment alloc] init];
    moment.name = app.localizedName;
    moment.bundleID = app.bundleIdentifier;
    moment.createdAt = [NSDate date];

    AXUIElementRef window = [self getFrontWindowFromAXApp:appElem];
    if (!window) {
        window = [self getFirstWindowFromAXApp:appElem];
    }

    if (window) {
        moment.pathOfActiveDocument = [self getDocumentPathFromAXWindow:window];
    }


//    CFRelease(window);

    return moment;
}


+ (AXUIElementRef)getFrontWindowFromAXApp:(AXUIElementRef)app
{
    // Get the accessibility element corresponding to the frontmost window
    // of the frontmost application.
    AXUIElementRef window = NULL;
    if (AXUIElementCopyAttributeValue(app, kAXFocusedWindowAttribute, (CFTypeRef*)&window) != kAXErrorSuccess) {
        return nil;
    }
    return window;
}

+ (AXUIElementRef)getFirstWindowFromAXApp:(AXUIElementRef)app
{
    // Get the accessibility element corresponding to the frontmost window
    // of the frontmost application.
    AXUIElementRef window = NULL;
    if (AXUIElementCopyAttributeValue(app, kAXFrontmostAttribute, (CFTypeRef*)&window) != kAXErrorSuccess) {
        return nil;
    }
    return window;
}


+ (NSString *)getTitleFromAXWindow:(AXUIElementRef)window
{
    // Finally, get the title of the frontmost window.
    CFStringRef title = NULL;
    AXError result = AXUIElementCopyAttributeValue(window, kAXTitleAttribute, (CFTypeRef*)&title);

    if (result != kAXErrorSuccess) {
        // Failed to get the window title.
        NSLog(@"Couldn't get the title");
        return nil;
    }

    return (NSString *)CFBridgingRelease(title);
}


+ (NSString *)getDocumentPathFromAXWindow:(AXUIElementRef)window
{
    // Finally, get the title of the frontmost window.
    CFStringRef document = NULL;
    AXError result = AXUIElementCopyAttributeValue(window, kAXDocumentAttribute, (CFTypeRef*)&document);

    if (result != kAXErrorSuccess) {
        // Failed to get the window title.
        NSLog(@"Couldn't get the document path");
        return nil;
    }

    return (NSString *)CFBridgingRelease(document);
}


//+ (void)getWindowsForApp:(NSRunningApplication *)app
//{
//    NSMutableArray *arr = [NSMutableArray array];
//
//    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
//    for (NSMutableDictionary* entry in (__bridge NSArray*)windowList)
//    {
//        NSString *ownerName = [entry objectForKey:(id)kCGWindowOwnerName];
//        NSInteger ownerPID = [[entry objectForKey:(id)kCGWindowOwnerPID] integerValue];
//
//        if (ownerPID == app.processIdentifier) {
//            NSLog(@"> %@:%@", ownerName, @(ownerPID));
//        }
//    }
//    CFRelease(windowList);
//
//}


@end
