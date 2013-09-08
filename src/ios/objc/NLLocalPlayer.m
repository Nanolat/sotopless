#import <SoTopless/NLLocalPlayer.h>

@implementation NLLocalPlayer

@synthesize authenticated=authenticated_;

- (void) set_authenticateHandler:(void(^)(NSError *error))completionHandler {

    // TODO : This is fake. Implement it.
    NSLog(@"set_authenticateHandler called.");
    
    completionHandler(nil);
}

+ (NLLocalPlayer *)localPlayer: (NSString *) playerID playerAlias:(NSString *) playerAlias password:(NSString *) password
{
    // TODO : This is fake. Implement it.
    NLLocalPlayer * lplayer = [[[NLLocalPlayer alloc] init] autorelease];
    
    return lplayer;
}

@end
