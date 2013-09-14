#import <Foundation/Foundation.h>

#import <SoTopless/NLPlayer.h>
#import <SoTopless/NLDefines.h>

@interface NLLocalPlayer : NLPlayer {
    BOOL authenticated_;
}

- (id) initWithPlayerID:(NSString *) playerID playerAlias:(NSString *) playerAlias playerPassword:(NSString*) playerPassword ;
+ (NLLocalPlayer *)localPlayer: (NSString *) playerID playerAlias:(NSString *) playerAlias password:(NSString *) password ;

@property(nonatomic, readonly, getter=isAuthenticated)  BOOL authenticated;

@property(nonatomic, copy) void(^authenticateHandler)(NSString * connectedServerName, NSError *error);

@end