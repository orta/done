#import <Foundation/Foundation.h>
#import "Moment.h"

NS_ASSUME_NONNULL_BEGIN

@interface DB : NSObject

+ (void)saveMoment:(Moment *_Nonnull)moment;

@end

NS_ASSUME_NONNULL_END
