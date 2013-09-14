
#import <Foundation/Foundation.h>
#import <SoTopless/NLScore.h>

#import "LeaderboardService.h"

@interface NLScore(private)

- (void) setScore:(int64_t)value category:(NSString*)category date:(NSDate*)date playerAlias:(NSString*)playerAlias playerID:(NSString*)playerID rank:(NSInteger)rank ;

- (NSData *) getScoreHistory:(NSError**)o_error ;


+ (NSNumber*) epochFromDate:(NSDate*)date ;

+ (NSDate*) dateFromEpoch:(NSNumber*)epoch ;

+ (Score*)   getThriftScoreFrom:(NLScore*)score error:(NSError**)o_error;

+ (NLScore*) getClientScoreFrom:(NSString*)category score:(Score*)score error:(NSError**)o_error;

+ (NSMutableArray *) getClientScoresFrom:(NSString*)category scores:(NSMutableArray*)scores count:(int32_t)count error:(NSError**)o_error;

@end
