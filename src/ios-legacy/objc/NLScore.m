
#import <SoTopless/NLScore.h>
#import <SoTopless/NLLocalPlayer.h>
#import <SoTopless/NLDefines.h>
#import "NLUtil.h"
#import "NLScorePrivate.h"
#import "NLDataFormatter.h"
#import "NLTable.h"

@implementation NLScore

@synthesize value=value_;
@synthesize category=category_;
@synthesize date=date_;
@synthesize playerAlias=playerAlias_;
@synthesize playerID=playerID_;
@synthesize rank=rank_;

extern NLLocalPlayer * gLocalPlayer;

- (id)initWithCategory:(NSString *)category {
    if ( self = [super init] ) {
        self.category = category;
        playerAlias_ = [gLocalPlayer.playerAlias retain];
        playerID_ = [gLocalPlayer.playerID retain];
        value_ = -1;
        // do initialization
    }
    return self;
}

- (void) dealloc {
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
            error = [NLUtil errorWithCode:NLErrorInvalidParameter message:@"Score not set."];
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
        
        if ( !gLocalPlayer.isAuthenticated ) {
            error = [NLUtil errorWithCode:NLErrorNotAuthenticated message: ERROR_MESSAGE_AUTHENTICATE];
            goto finally;
        }
        
        // Get current time again when the score is reported.
        [date_ release];
        date_ = [[NSDate date] retain];
        
        
        // Get table names based on the leaderboard name, stored on category_.
        NSString * scoreByScoreKeyTable = [NLTable scoreByScoreKeyTableName:category_];
        assert(scoreByScoreKeyTable);
        
        NSString * scoreKeyByUserTable = [NLTable scoreKeyByUserName:category_];
        assert(scoreKeyByUserTable);
        
        // TODO : This is temporary code. get rid of gTableCreated flag.
        static bool gTableCreated=false;
        if (!gTableCreated) {
            // Create tables
            [[NLTable sharedTable] makeSureToCreateTable:scoreByScoreKeyTable error:&error];
            if (error) goto finally;
            [[NLTable sharedTable] makeSureToCreateTable:scoreKeyByUserTable error:&error];
            if (error) goto finally;
            gTableCreated=true;
        }
        
        // Format data to put into Nanolat Database.
        NSData * jsonScoreData = [NLScore generateJsonFromScore:self error:&error];
        if (error) goto finally;
        assert(jsonScoreData);
        
        NSData * packedPlayerAliasKey = [NLDataFormatter packUserName:playerAlias_];
        if (packedPlayerAliasKey == nil) {
            error = [NLUtil errorWithCode:NLErrorInvalidParameter message:@"Too long player alias."];
            goto finally;
        }
        
        NSData * packedScoreValue = [NLDataFormatter packValue:jsonScoreData packedValueLength:SCORE_DATA_PACKED_LEN];
        assert(packedScoreValue);
        
        NSData * packedScoreKey = [NLDataFormatter packScore:value_ playerAlias:playerAlias_];
        assert(packedScoreKey);
        
        // Put the formatted data to into Nanolat Database.
        // primary index mapping from the score to the serialized NLScore object in Json format.
        [[NLTable sharedTable] put:scoreByScoreKeyTable key:packedScoreKey value:packedScoreValue error:&error];
        if (error) {
            goto finally;
        }
        
        // secondary index mapping from playerAlias to the key of the primary index (packedScoreKey)
        [[NLTable sharedTable] put:scoreKeyByUserTable key:packedPlayerAliasKey value:packedScoreKey error:&error];
        if (error) {
            goto finally;
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

@end
