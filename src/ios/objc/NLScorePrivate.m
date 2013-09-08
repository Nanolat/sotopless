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


+ (NLScore*) scoreFromJson:(NSData*) scoreData category:(NSString*)category rank:(NSInteger)rank error:(NSError**)o_error{
    assert(o_error);

    NSError * error = nil;
    
    NSDictionary* json = [NSJSONSerialization
                          JSONObjectWithData:scoreData //1
                          
                          options:kNilOptions
                          error:&error];
    
    if (error) {
        *o_error = error;
        return nil;
    }
    
    NSNumber * valueNum = [json objectForKey:@"value"];
    assert(valueNum);
    NSNumber * dateEpoch = [json objectForKey:@"dateEpoch"];
    assert(dateEpoch);
    NSDate * date = [NLScore dateFromEpoch:dateEpoch];
    assert(date);
    NSString * playerAlias = [json objectForKey:@"playerAlias"];
    assert(playerAlias);
    NSString * playerID = [json objectForKey:@"playerID"];
    assert(playerID);
    
    NLScore * score = [[[NLScore alloc] init] autorelease];
    [score setScore:[valueNum longLongValue] category:category date:date playerAlias:playerAlias playerID:playerID rank:rank ];
    
    *o_error = nil;
    return score;
}

+ (NSData*)generateJsonFromScore:(NLScore*)score error:(NSError**)o_error{
    assert(o_error);
    
    NSNumber * dateEpoch = [NLScore epochFromDate:score.date];
    assert(dateEpoch);

    NSDictionary* info = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithInteger:score.value],
                          @"value",
                          dateEpoch,
                          @"dateEpoch",
                          score.playerAlias,
                          @"playerAlias",
                          score.playerID,
                          @"playerID",
                          nil];
    NSError * error = nil;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:info
                                                       options:NSJSONWritingPrettyPrinted error:&error];
    if (error) {
        * o_error = error;
        return nil;
    } else {
        * o_error = nil;
    }
    
    return jsonData;
}


@end
