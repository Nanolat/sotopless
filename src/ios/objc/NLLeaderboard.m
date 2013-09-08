#import <SoTopless/NLLeaderboard.h>
#import <SoTopless/NLScore.h>


@interface NLScore(private)
- (void) setScore:(int64_t)value category:(NSString*)category date:(NSDate*)date playerAlias:(NSString*)playerAlias playerID:(NSString*)playerID rank:(NSInteger)rank;

@end

@implementation NLScore(private)

- (void) setScore:(int64_t)value category:(NSString*)category date:(NSDate*)date playerAlias:(NSString*)playerAlias playerID:(NSString*)playerID rank:(NSInteger)rank {
    value_ = value;
    category_ = category;
    date_ = date;
    playerAlias_ = playerAlias;
    playerID_ = playerID;
    rank_ = rank;
}

@end


@implementation NLLeaderboard

@synthesize category=category_;
@synthesize range=range_;
@synthesize loading=loading_;
@synthesize localPlayerScore=localPlayerScore_;
@synthesize scores=scores_;

- (id)init
{
    if ( self = [super init] ) {
        // do initialization here.
    }
    return self;
}

- (void)loadScoresWithCompletionHandler:(void(^)(NSArray *scores, NSError *error))completionHandler
{
    // TODO : This is a fake implementation. Implement it again.
    NSArray * playerAliasArr = [NSArray arrayWithObjects:
        @"Kangmo Kim",
        @"Hyunsook Cho",
        @"Junsang Lee",
        @"Monad",
        @"Minho Kim",
        @"Hyunsaeng Kim",
        @"Kyungjoen Kim",
        @"Test1",
        @"Test2",
        @"Test3",
        nil
    ];
    
    NSArray * playerIDArr = [NSArray arrayWithObjects:
                             @"uuid-0001",
                             @"uuid-0002",
                             @"uuid-0003",
                             @"uuid-0004",
                             @"uuid-0005",
                             @"uuid-0006",
                             @"uuid-0007",
                             @"uuid-0008",
                             @"uuid-0009",
                             @"uuid-0010",
                             nil
                             ];
    
    NLScore * scoreArr[10];
    for (int i=0; i<10; i++) {
        scoreArr[i] = [[[NLScore alloc] init] autorelease];
        [scoreArr[i] setScore:1000-i*2 category:@"all" date:[NSDate date]
                     playerAlias:playerAliasArr[i] playerID:playerIDArr[i] rank:(i+1)];
    }
    
    NSArray * scores = [NSArray arrayWithObjects:scoreArr[0], scoreArr[1], scoreArr[2],
                        scoreArr[3], scoreArr[4], scoreArr[5], scoreArr[6], scoreArr[7],
                        scoreArr[8], scoreArr[9], nil];
    
    completionHandler(scores, nil);
}

@end
