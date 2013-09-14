
#import <SoTopless/NLPlayer.h>

@implementation NLPlayer

@synthesize playerAlias=playerAlias_;
@synthesize playerID=playerID_;

- (id) initWithPlayerID:(NSString *) playerID playerAlias:(NSString *) playerAlias {
    if ( self = [super init]) {
        playerID_ = [playerID retain];
        playerAlias_ = [playerAlias retain];
    }
    return self;
}


- (void) dealloc {
    [playerAlias_ release];
    [playerID_ release];
    
    [super dealloc];
}

@end
