#import <Foundation/Foundation.h>
#import <Realm/Realm.h>

NS_ASSUME_NONNULL_BEGIN

@interface Moment : RLMObject
@property NSString * _Nonnull name;
@property NSString * _Nonnull bundleID;
@property NSDate * _Nonnull createdAt;
@property NSString * pathOfActiveDocument;
@property NSString * projectName;
@end

NS_ASSUME_NONNULL_END
