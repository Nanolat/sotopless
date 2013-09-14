#import <Foundation/Foundation.h>

#import <SoTopless/NLDefines.h>
#import <SoTopless/NLError.h>

@interface NLPlayer : NSObject {
    NSString    *playerAlias_;
    NSString    *playerID_;
}

// The displayed alias of the player.
@property(nonatomic, readonly, copy)    NSString    *playerAlias;

// The player ID provided when creating a user.
@property(nonatomic, readonly, retain)  NSString    *playerID;


// TODO : Move to private header. Do not expose it in SDK.
- (id) initWithPlayerID:(NSString *) playerID playerAlias:(NSString *) playerAlias;

@end

