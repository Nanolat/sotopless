#import <Foundation/Foundation.h>

#import <SoTopless/NLDefines.h>
#import <SoTopless/NLError.h>

@interface NLPlayer : NSObject

// The displayed alias of the player.
@property(nonatomic, readonly, copy)    NSString    *playerAlias;

// The player ID provided when creating a user.
@property(nonatomic, readonly, retain)  NSString    *playerID;


@end

