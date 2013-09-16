//
//  NLScorePrivate.m
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import "NLScorePrivate.h"

@implementation NLScore(private)

- (void) setScore:(int64_t)value category:(NSString*)category date:(NSDate*)date playerAlias:(NSString*)playerAlias playerID:(NSString*)playerID rank:(NSInteger)rank {
    value_ = value;
    
    [category_ release];
    category_ = [category retain];
    
    [date_ release];
    date_ = [date retain];

    [playerAlias_ release];
    playerAlias_ = [playerAlias retain];
    
    [playerID_ release];
    playerID_ = [playerID retain];
    
    rank_ = rank;
}

+ (NSNumber*) epochFromDate:(NSDate*)date {
    NSNumber * epoch = [NSNumber numberWithLongLong:[date timeIntervalSince1970]*1000];
    assert(epoch);
    return epoch;
}

+ (NSDate*) dateFromEpoch:(NSNumber*)epoch {
    NSDate * date = [NSDate dateWithTimeIntervalSince1970:[epoch longLongValue]/1000];
    assert(date);
    return date;
}

+ (Score*)   getThriftScoreFrom:(NLScore*)client_score error:(NSError**)o_error{
    assert(client_score);
    assert(o_error);
    
    Score * score = [[[Score alloc] init] autorelease];
    
    score.value        = client_score.value;
    score.date_epoch   = [[NLScore epochFromDate:client_score.date] longLongValue];
    score.user_alias = client_score.playerAlias;
    score.user_id    = client_score.playerID;
    score.rank         = client_score.rank;

    // TODO : v2.0 - add vote up/down count to NLScore
    // score.vote_up_count = client_score.voteUpCount;
    // score.vote_down_count = client_score.voteDownCount;

    NSError * error = nil;
    score.situation = [client_score getScoreHistory:&error];
    if (error) {
        *o_error = error;
        return nil;
    }
    
    return score;
}

+ (NLScore*) getClientScoreFrom:(NSString*)category score:(Score*)score error:(NSError**)o_error{
    
    NLScore * client_score = [[[NLScore alloc] init] autorelease];
    
    NSDate * scoreDate = [NLScore dateFromEpoch:[NSNumber numberWithLongLong:score.date_epoch] ];

    // TODO : v2.0 - add vote up/down count to NLScore
    [client_score setScore:score.value category:category date:scoreDate playerAlias:score.user_alias playerID:score.user_id rank:score.rank];

    // In this function, error can not happen, but have the output error parameter for the consistency of interfaces.
    *o_error = nil;
    
    return client_score;
}

+ (NSMutableArray *) getClientScoresFrom:(NSString*)category scores:(NSMutableArray*)scores count:(int32_t)count error:(NSError**)o_error{

    assert(scores);
    assert(count>0);
    
    NSMutableArray * client_scores = [[[NSMutableArray alloc] init] autorelease];
    
    NSError * error = nil;
    
    for ( Score * score in scores ) {
        NLScore * client_score = [NLScore getClientScoreFrom:category score:score error:&error];
        assert(client_score);
        if (error) {
            *o_error = error;
            return client_scores;
        }
        
        [client_scores addObject:client_score];

        if ( --count <= 0 ) {
            break;
        }
    }
    
    return client_scores;
}

- (NSData *) getScoreHistory:(NSError**)o_error {
    
    NSError * error = nil;
    
    NSData * jsonData = [NSJSONSerialization dataWithJSONObject:scoreHistory_
                                             options:0/*serialize in compact format*/ error:&error];
    if (error) {
        * o_error = error;
        return nil;
    } else {
        * o_error = nil;
    }
    return jsonData;
}


@end
