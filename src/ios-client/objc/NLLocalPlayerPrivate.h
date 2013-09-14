//
//  NLLocalPlayerPrivate.h
//  objc
//
//  Created by Kangmo Kim on 13. 9. 14..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSocketClient.h"
#import "TFramedTransport.h"
#import "TBinaryProtocol.h"
#import "TTransportException.h"
#import "LeaderboardService.h"

@interface NLLocalPlayer() {
    TSocketClient * transport_;
    TFramedTransport * framedTransport_;
    TBinaryProtocol * protocol_;

    LeaderboardServiceClient * service_;
    Session                  * session_;
    NSString * playerPassword_;
}

@property(nonatomic, retain) LeaderboardServiceClient * service;
@property(nonatomic, retain) Session * session;

// Cache the PostScoreReply instance, for the 
@property(nonatomic, retain) PostScoreReply * postScoreReply;

-(void)dealloc;

+ (NLLocalPlayer *) localPlayer;
+ (void) setLocalPlayer:(NLLocalPlayer *)player;

@end
