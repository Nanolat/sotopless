#import <Foundation/Foundation.h>

#import <SoTopless/NLScore.h>
#import <SoTopless/NLLocalPlayer.h>
#import <SoTopless/NLDefines.h>
#import "NLUtil.h"
#import "NLScorePrivate.h"
#import "NLLocalPlayerPrivate.h"

@implementation NLScore

@synthesize value=value_;
@synthesize category=category_;
@synthesize date=date_;
@synthesize playerAlias=playerAlias_;
@synthesize playerID=playerID_;
@synthesize rank=rank_;

- (id)initWithCategory:(NSString *)category {
    if ( self = [super init] ) {
        self.category = category;
        playerAlias_ = [NLLocalPlayer.localPlayer.playerAlias retain];
        playerID_ = [NLLocalPlayer.localPlayer.playerID retain];
        value_ = 0;
        scoreHistory_ = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void) dealloc {
    [scoreHistory_ release];
    
    [category_ release];
    [date_ release];
    [playerAlias_ release];
    [playerID_ release];

    [super dealloc];
}


- (void)reportScoreWithCompletionHandler:(void(^)(NSError *error))completionHandler {
    
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^ {
        // Background work here
        // Talk to a server via socket, using a binary protocol
        NSError * error = nil;
        
        if ( value_ < 0 ) {
            error = [NLUtil errorWithCode:NLErrorInvalidArgument message:@"Score not set."];
            goto finally;
        }
        
        if ( playerAlias_ == nil || [playerAlias_ isEqualToString:@""] ) {
            error = [NLUtil errorWithCode:NLErrorInvalidUseOfSDK message: ERROR_MESSAGE_CREATE_LOCAL_PLAYER@"Reason : playerAlias of NLScore is nil."];
            goto finally;
        }
        
        if ( playerID_ == nil || [playerID_ isEqualToString:@""] ) {
            error = [NLUtil errorWithCode:NLErrorInvalidUseOfSDK message: ERROR_MESSAGE_CREATE_LOCAL_PLAYER@"Reason : playerID of NLScore is nil."];
            goto finally;
        }
        
        if ( ! NLLocalPlayer.localPlayer.isAuthenticated ) {
            error = [NLUtil errorWithCode:NLErrorNotAuthenticated message: ERROR_MESSAGE_AUTHENTICATE];
            goto finally;
        }
        
        // Get current time again when the score is reported.
        [date_ release];
        date_ = [[NSDate date] retain];

        NLLocalPlayer.localPlayer.postScoreReply = nil;
        
        Score * score = [NLScore getThriftScoreFrom:self error:&error];
        if (error) {
             goto finally;
        }
        assert(score);
        
        
        @try {
            PostScoreReply * reply = [ NLLocalPlayer.localPlayer.service post_score:NLLocalPlayer.localPlayer.session
                                                                           category:category_ score:score];
            assert(reply);
            
            if (reply) {
                if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
                    // In the reply of post score request, the server sends the score of the user and top 10 scores.
                    // Cache the result so that we don't have to send a new request in NLLeaderboard.loadScoresWithCompletionHandler.
                    NLLocalPlayer.localPlayer.postScoreReply = reply;
                } else {
                    // TODO : print arguments with format.
                    NSString * message = [NLUtil append:@"Error while posting user score to SoTopless leaderboard server. Detail : ", reply.status.error_message_format, nil];
                    
                    error = [NLUtil errorWithCode:reply.status.error_code message:message];
                    assert(error);
                }
            }
      
        } @catch ( TTransportException * tx_excp ) {
            error = [NLUtil errorWithCode:NLErrorCommunicationsFailure
                            message:[NLUtil append:@"Failed to post score. Reason : ", tx_excp.reason, nil] ];
            assert(error);
        }

        // Retain error object to use it in user thread.
    finally:
        [error retain];
        
        NSLog(@"Finished work in background");
        
        dispatch_async(dispatch_get_main_queue(), ^ {
            completionHandler(error);
            
            // release the error object retained in background thread.
            [error release];
            
            NSLog(@"Back on main thread");
        });
    });
}

- (void)increaseValue:(int64_t) delta reason:(NSString*) reason {
    NSNumber * nowEpoch = [NLScore epochFromDate:[NSDate date]];
    assert(nowEpoch);
    
    NSNumber * deltaNumber = [NSNumber numberWithLongLong: delta];
    assert(deltaNumber);
    
    NSDictionary * scoreChangeEnvet = [NSDictionary dictionaryWithObjectsAndKeys:deltaNumber, @"delta", nowEpoch, @"epoch", reason, @"reason", nil];
    assert(scoreChangeEnvet);
    
    [scoreHistory_ setObject:scoreChangeEnvet forKey:@"event"];
    
    value_ += delta;
}

// Reset score value.
- (void)resetValue {
     value_ = 0;
}

@end
