#import "DB.h"
#import <Realm/Realm.h>

@implementation DB

+ (void) saveMoment: (Moment * _Nonnull )moment
{
    RLMRealm *realm = [RLMRealm defaultRealm];
    [realm transactionWithBlock:^{
        [realm addObject:moment];
    }];
}

@end
