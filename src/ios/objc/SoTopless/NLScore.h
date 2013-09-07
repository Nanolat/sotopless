#import <Foundation/Foundation.h>

#import <SoTopless/NLLeaderboard.h>


@interface NLScore : NSObject <NSCoding> {
}

- (id)initWithCategory:(NSString *)category;

@property(nonatomic, assign)            int64_t     value;

@property(nonatomic, retain)            NSString    *category;

@property(nonatomic, readonly, retain)  NSDate      *date;

@property(nonatomic, readonly, retain)  NSString    *playerAlias;

@property(nonatomic, readonly, retain)  NSString    *playerID;

@property(nonatomic, readonly, assign)  NSInteger   rank;

- (void)reportScoreWithCompletionHandler:(void(^)(NSError *error))completionHandler;

@end
