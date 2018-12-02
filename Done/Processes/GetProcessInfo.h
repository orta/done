#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN

@class Moment;

typedef NS_ENUM(NSUInteger, SupportedApps) {
    MVSCode,
    MTerminal,
    MSketch,
    MXcode,
    MSafari,
    MChrome
};

@interface GetProcessInfo : NSObject

/// The debugging version of create:
+ (Moment  * _Nullable )printMomentForApp:(SupportedApps)appType;

// Creates a moment from the front-most app
+ (Moment  * _Nullable)createMomentForFrontAppp;

// This asks for permission
+ (BOOL)canUseAX;

// This does not ask for permission
+ (BOOL)quietlyCanUseAX;

@end

NS_ASSUME_NONNULL_END
