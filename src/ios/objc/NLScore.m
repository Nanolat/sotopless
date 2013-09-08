
#import <SoTopless/NLScore.h>

@implementation NLScore

@synthesize value=value_;
@synthesize category=category_;
@synthesize date=date_;
@synthesize playerAlias=playerAlias_;
@synthesize playerID=playerID_;
@synthesize rank=rank_;

- (id)initWithCategory:(NSString *)category {
    if ( self = [super init] ) {
        // do initialization
    }
    return self;
}


- (void)reportScoreWithCompletionHandler:(void(^)(NSError *error))completionHandler {
    // TODO : This is fake. Implement it.
    completionHandler(nil);
}

@end
