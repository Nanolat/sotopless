#import <Foundation/Foundation.h>

#import <SoTopless/NLLeaderboard.h>

@interface NLScore : NSObject {
    int64_t     value_;
    NSString    *category_;
    NSDate      *date_;
    NSString    *playerAlias_;
    NSString    *playerID_;
    NSInteger   rank_;
    
    // This array has a mapping of timestamp to a NSDictionary instance.
    // The instance contains score increment event such as the delta(how much the score increased?)
    // and a string describing why the score was increased.
    NSMutableDictionary * scoreHistory_;
}

- (id)initWithCategory:(NSString *)category;

@property(nonatomic, readonly, assign)  int64_t     value;

@property(nonatomic, retain)            NSString    *category;

@property(nonatomic, readonly, retain)  NSDate      *date;

@property(nonatomic, readonly, retain)  NSString    *playerAlias;

@property(nonatomic, readonly, retain)  NSString    *playerID;

@property(nonatomic, readonly, assign)  NSInteger   rank;

- (void)reportScoreWithCompletionHandler:(void(^)(NSError *error))completionHandler;

// Increase score(delta > 0 ) or descrease score(delta < 0).
- (void)increaseValue:(int64_t) delta reason:(NSString*) reason;

// Reset score value.
- (void)resetValue;
@end


