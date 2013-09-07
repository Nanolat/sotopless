#import <Foundation/Foundation.h>

@class NLScore;

@interface NLLeaderboard : NSObject {
}

@property(nonatomic, retain)            NSString                    *category;

// The range starts from 1, length maximum is 128.
// The default value for the range is from 1 to 10.
@property(nonatomic, assign)            NSRange                     range;

@property(readonly, getter=isLoading)   BOOL                        loading;

@property(nonatomic, readonly, retain)  GKScore                     *localPlayerScore;

@property(nonatomic, readonly, retain)  NSArray                     *scores;

- (id)init;

- (void)loadScoresWithCompletionHandler:(void(^)(NSArray *scores, NSError *error))completionHandler;

@end
