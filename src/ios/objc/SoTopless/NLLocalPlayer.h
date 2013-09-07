#import <Foundation/Foundation.h>

#import <SoTopless/NLPlayer.h>
#import <SoTopless/NLDefines.h>

@interface NLLocalPlayer : NLPlayer {
}

+ (NLLocalPlayer *)localPlayer(NSSTring * playerID, NSSTring * playerAlias, NSSTring * password);

@property(nonatomic, readonly, getter=isAuthenticated)  BOOL authenticated; 

@property(nonatomic, copy) void(^authenticateHandler)(NSError *error);

@end