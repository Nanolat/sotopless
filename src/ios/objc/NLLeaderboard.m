#import <SoTopless/NLLeaderboard.h>
#import <SoTopless/NLScore.h>
#import <SoTopless/NLLocalPlayer.h>
#import "NLUtil.h"
#import "NLScorePrivate.h"
#import "NLTable.h"
#import "NLDataFormatter.h"

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
        NSString * playerAlias = [gLocalPlayer playerAlias];
        if ( playerAlias == nil || [playerAlias isEqualToString:@""] ) {
            error = [NLUtil errorWithCode:NLErrorInvalidUseOfSDK message: ERROR_MESSAGE_CREATE_LOCAL_PLAYER@"Reason : playerAlias of NLScore is nil."];
            goto finally;
        }
        
        if ( !gLocalPlayer.isAuthenticated ) {
            error = [NLUtil errorWithCode:NLErrorNotAuthenticated message: ERROR_MESSAGE_AUTHENTICATE];
            goto finally;
        }
        
        loading_ = TRUE;
        
        
        // Get table names based on the leaderboard name, stored on category_.
        NSString * scoreByScoreKeyTable = [NLTable scoreByScoreKeyTableName:category_];
        assert(scoreByScoreKeyTable);
        
        NSString * scoreKeyByUserTable = [NLTable scoreKeyByUserName:category_];
        assert(scoreKeyByUserTable);
    
        // TODO : uncomment following lines for table creation.
        // TODO : And also get rid of gTableCreated flag in NLScore.
/*
        /////////////////////////////////////////////////////////////////////////////////
        // Create tables
        [[NLTable sharedTable] makeSureToCreateTable:scoreByScoreKeyTable error:&error];
        if (error) goto finally;
        [[NLTable sharedTable] makeSureToCreateTable:scoreKeyByUserTable error:&error];
        if (error) goto finally;
  */
        /////////////////////////////////////////////////////////////////////////////////
        // Get the local player's ranking.
        {
            NSData * packedPlayerAliasKey = [NLDataFormatter packUserName:playerAlias];
            if (packedPlayerAliasKey == nil) {
                error = [NLUtil errorWithCode:NLErrorInvalidParameter message:@"Too long player alias."];
                goto finally;
            }
            
            // Get packedScoreKey
            NSData * packedScoreKey;
            KeyOrder dummyKeyOrder;
            [[NLTable sharedTable] get:scoreKeyByUserTable key:packedPlayerAliasKey value:&packedScoreKey keyOrder:&dummyKeyOrder error:&error];
            if (error) goto finally;
            assert(packedScoreKey);
            
            // Get packedScore
            NSData * packedScoreValue;
            KeyOrder userRank;
            [[NLTable sharedTable] get:scoreByScoreKeyTable key:packedScoreKey value:&packedScoreValue keyOrder:&userRank error:&error];
            if (error) goto finally;
            assert(packedScoreValue);
            assert(userRank>0);
            
            // Unpack the score value
            NSData * jsonScoreData = [NLDataFormatter unpackValue:packedScoreValue packedValueLength:SCORE_DATA_PACKED_LEN];
            assert(jsonScoreData);
            
            localPlayerScore_ = [[NLScore scoreFromJson:jsonScoreData category:category_ rank:userRank error:&error] retain];
            if (error) goto finally;
        }
        
        //////////////////////////////////////////////////////////////////////////////////
        // Get ranking data from leaderboard.
        {
            NSMutableArray * mutableScores = [[NSMutableArray alloc] init];
            int scoresToFetch = range_.length;
            
            CursorHandle cursorHandle = [[NLTable sharedTable] cursorOpenByOrder:scoreByScoreKeyTable dir:CursorDirection_CD_FORWARD keyOrder:range_.location error:&error];
            if (error) goto finally;
            assert(cursorHandle > 0);
            
            BOOL endOfCursor = FALSE;
            NSData * packedScoreKey;
            NSData * packedScoreValue;
            KeyOrder userRank;
            while( 1 ) {
                [[NLTable sharedTable] cursorFetch:cursorHandle key:&packedScoreKey keyOrder:&userRank value:&packedScoreValue endOfCursor:&endOfCursor error:&error];
                if (error) goto finally;
                if (endOfCursor) break;
                
                assert(packedScoreKey);
                assert(userRank>0);
                assert(packedScoreValue);
                
                // Unpack the score value
                NSData * jsonScoreData = [NLDataFormatter unpackValue:packedScoreValue packedValueLength:SCORE_DATA_PACKED_LEN];
                assert(jsonScoreData);
                
                NLScore * score = [NLScore scoreFromJson:jsonScoreData category:category_ rank:userRank error:&error];
                if (error) goto finally;
                
                [ mutableScores addObject:score ];
                
                scoresToFetch--;
                if ( scoresToFetch <= 0)
                    break;
            }
            
            [[NLTable sharedTable] cursorClose:cursorHandle error:&error];
            if (error) goto finally;
            
            scores_ = [mutableScores retain];
        }
        
        // Retain error object to use it in user thread.
    finally:
        [error retain];
        
        NSLog(@"Finished work in background");
        
        dispatch_async(dispatch_get_main_queue(), ^ {
            completionHandler(scores_, error);
            
            // release the error object retained in background thread.
            [error release];
            
            NSLog(@"Back on main thread");
        });
        
    });

}

@end
