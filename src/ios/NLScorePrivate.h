
#import <Foundation/Foundation.h>
#import <SoTopless/NLScore.h>

@interface NLScore(private)

- (void) setScore:(int64_t)value category:(NSString*)category date:(NSDate*)date playerAlias:(NSString*)playerAlias playerID:(NSString*)playerID rank:(NSInteger)rank;

+ (NLScore*) scoreFromJson:(NSData*) scoreData category:(NSString*)category rank:(NSInteger)rank error:(NSError**)o_error;

+ (NSData*)generateJsonFromScore:(NLScore*)score error:(NSError**)o_error;

@end
