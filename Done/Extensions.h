#import <Foundation/Foundation.h>

@interface  NSArray (Exts)
- (NSArray *)map: (id(^)(id obj))block;
- (NSArray *)filter: (BOOL (^)(id obj))block;
- (void)apply: (void(^)(id obj))block;
- (id)first: (BOOL(^)(id obj))block;
@end
