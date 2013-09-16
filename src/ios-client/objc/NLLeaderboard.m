#import <SoTopless/NLLeaderboard.h>
#import <SoTopless/NLScore.h>
#import <SoTopless/NLLocalPlayer.h>

#import "LeaderboardService.h"

#import "NLUtil.h"
#import "NLScorePrivate.h"
#import "NLLocalPlayerPrivate.h"

extern NLLocalPlayer * gLocalPlayer;

@implementation NLLeaderboard

@synthesize category=category_;
@synthesize range=range_;
@synthesize loading=loading_;
@synthesize localPlayerScore=localPlayerScore_;
@synthesize scores=scores_;

- (id)init
{
    if ( self = [super init] ) {
        range_ = NSMakeRange(1, 10);
        // do initialization here.
    }
    return self;
}

- (void) dealloc {
    [category_ release];
    [localPlayerScore_ release];
    [scores_ release];
    
    [super dealloc];
}

- (void)loadScoresWithCompletionHandler:(void(^)(NSArray *scores, NSError *error))completionHandler
{
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^ {
        // Background work here
        // Talk to a server via socket, using a binary protocol
        NSError * error = nil;
        
        // Reset scores first.
        [scores_ release];
        scores_ = nil;
        [localPlayerScore_ release];
        localPlayerScore_ = nil;
        
        loading_ = FALSE;
        
        if ( [category_ isEqualToString:@""] ) {
            error = [NLUtil errorWithCode:NLErrorInvalidParameter message: @"Invalid Parameter for loadScore. Reason : Leaderboard category is nil or an empty string."];
            goto finally;
        }
        
        if ( range_.location <= 0 ) {
            error = [NLUtil errorWithCode:NLErrorInvalidParameter message: @"Invalid Parameter for loadScore. Reason : location of range should be greater than 1."];
            goto finally;
        }
        
        if ( range_.length > 100 ) {
            error = [NLUtil errorWithCode:NLErrorInvalidParameter message: @"Invalid Parameter for loadScore. Reason : You have hit the maximum length of range, 100."];
            goto finally;
        }
        
        // Get the player's alias
        NSString * playerAlias = NLLocalPlayer.localPlayer.playerAlias;
        if ( playerAlias == nil || [playerAlias isEqualToString:@""] ) {
            error = [NLUtil errorWithCode:NLErrorInvalidUseOfSDK message: ERROR_MESSAGE_CREATE_LOCAL_PLAYER@"Reason : playerAlias of NLScore is nil."];
            goto finally;
        }
        
        if ( ! NLLocalPlayer.localPlayer.isAuthenticated ) {
            error = [NLUtil errorWithCode:NLErrorNotAuthenticated message: ERROR_MESSAGE_AUTHENTICATE];
            goto finally;
        }
        
        loading_ = TRUE;
        
        [localPlayerScore_ release];
        localPlayerScore_ = nil;
        
        [scores_ release];
        scores_ = nil;
        
        // Check cache
        PostScoreReply * postScoreReply = NLLocalPlayer.localPlayer.postScoreReply;
        if ( postScoreReply ) {
            
            localPlayerScore_ = [[NLScore getClientScoreFrom:category_ score:postScoreReply.scores.user_score error:&error ] retain];
            assert(localPlayerScore_);
            if (!error) {
                if (range_.location == postScoreReply.scores.from_rank &&
                    range_.length <= postScoreReply.scores.count ) {
                    
                    scores_ = [NLScore getClientScoresFrom:category_ scores:postScoreReply.scores.top_scores count:range_.length error:&error];
                    if (error) {
                         goto finally;
                    }
                    assert(scores_);
                } else {
                    // TODO : Send a request to server to get the score in a range specified by _range variable.
                }
            }
        }
        
        assert(localPlayerScore_);
        assert(scores_);

        // TODO : Implement get_scores service and use it.
        if (!localPlayerScore_ || !scores_) {
            if (error == nil) {
                error = [NLUtil errorWithCode:NLErrorInvalidUseOfSDK message: @"Internal Error. Currently only top 10 users are supported only for NLLeaderboard.range"];
            }
            goto finally;
        }
            
        // Retain error object to use it in user thread.
    finally:
        [scores_ retain];
        [error retain];
        
        NSLog(@"Finished work in background");
        
        dispatch_async(dispatch_get_main_queue(), ^ {
            completionHandler(scores_, error);
            
            // release the error object retained in background thread.
            [scores_ release];
            [error release];
            
            NSLog(@"Back on main thread");
        });
        
    });

}

@end
