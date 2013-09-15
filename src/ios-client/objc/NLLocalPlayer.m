#import <SoTopless/NLLocalPlayer.h>
#import "NLUtil.h"

#import "NLLocalPlayerPrivate.h"

@interface NLLocalPlayer() {
}

@property(nonatomic, readwrite, retain) TSocketClient *transport;
@property(nonatomic, readwrite, retain) TFramedTransport * framedTransport;
@property(nonatomic, readwrite, retain) TBinaryProtocol *protocol;

@end


@implementation NLLocalPlayer

@synthesize postScoreReply;

@synthesize authenticated=authenticated_;
@synthesize session=session_;
@synthesize transport=transport_;
@synthesize framedTransport=framedTransport_;
@synthesize protocol=protocol_;
@synthesize service=service_;

- (id) initWithPlayerID:(NSString *) playerID playerAlias:(NSString *) playerAlias playerPassword:(NSString*) playerPassword {
    if ( self = [super initWithPlayerID:playerID playerAlias:playerAlias]) {
        postScoreReply = nil;
        session_ = nil;
        service_ = nil;
        transport_ = nil;
        framedTransport_ = nil;
        protocol_ = nil;
        authenticated_ = FALSE;
        
        playerPassword_ = [playerPassword retain];
    }
    return self;
}

// Try three servers for connecting to the server. If any of them is successfully connected, simply use the connection.
NSString * SOTOPLESS_HOST_NAMES[] = {
    @"localhost",
    /*
    @"192.168.103.107"
    @"54.200.19.178",
    @"nanolat.kr",
    @"vigsql.com",
     */
    nil
};

#define NL_SOTOPLESS_PORT 9090
- (void) setAuthenticateHandler:(void(^)(NSString * connectedServerName, NSError *error))completionHandler {

    if (completionHandler == nil) {
        NSLog(@"disconnecting.");
        if (self.session != nil) {
            [service_ disconnect:self.session];
            self.session = nil;
        }
        return;
    }
    
    NSLog(@"setAuthenticateHandler called.");

    // TODO : Should we keep the completionHandler in NLLocalPlayer object and do authentication periodically?
    
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^ {
        // Background work here
        // Talk to a server via socket, using a binary protocol
        NSError * error = nil;
        NSString * serverName = nil;
        TTransportException * transport_exception;

        for (int i=0; SOTOPLESS_HOST_NAMES[i] != nil; i++) {
            transport_exception = nil;
            
            @try {
                NSLog(@"Trying to connect to %@.", SOTOPLESS_HOST_NAMES[i]);
                self.transport = [[TSocketClient alloc] initWithHostname:SOTOPLESS_HOST_NAMES[i] port:NL_SOTOPLESS_PORT];
                assert(transport_);
                self.framedTransport = [[TFramedTransport alloc] initWithTransport:transport_];
                assert(framedTransport_);
                self.protocol = [[TBinaryProtocol alloc] initWithTransport:framedTransport_ strictRead:YES strictWrite:YES];
                assert(protocol_);
                self.service = [[LeaderboardServiceClient alloc] initWithProtocol:protocol_];
                assert(service_);
                
                ConnectReply * reply = [service_ connect:LeaderboardServiceConstants.PROTOCOL_VERSION
                                                 tenant_id:@"xc4edf"
                                                 user_id:playerID_
                                                 user_password:playerPassword_
                                                 user_data:nil];
                assert(reply);
                
                if (reply) {
                    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
                        serverName = reply.server_name;
                        self.session = [[Session alloc] initWithSession_handle:reply.session_handle];
                    } else {
                        NSString * message = [NLUtil append:@"Error while connecting to SoTopless leaderboard server. Detail:", reply.status.error_message_format, nil];
                        error = [NLUtil errorWithCode:reply.status.error_code message:message];
                        assert(error);
                    }
                    break;
                }
                
            } @catch ( TTransportException * tx_excp ) {
                transport_exception = tx_excp;
            }
        }

        if (transport_exception) {
            error = [NLUtil errorWithCode:NLErrorCommunicationsFailure
                                  message:[NLUtil append:@"Can't connect to SoTopless server. Reason : ", transport_exception.reason, nil] ];
            assert(error);
        }
        
        // Retain error object to use it in user thread.
        [error retain];

        NSLog(@"Finished work in background");

        dispatch_async(dispatch_get_main_queue(), ^ {
            if (error) {
                authenticated_ = FALSE;
            } else {
                authenticated_ = TRUE;
            }
            completionHandler(serverName, error);

            // release the error object retained in background thread.
            [error release];

            NSLog(@"Back on main thread");
        });
        
    });
}

-(void)dealloc {
    // TODO : Should we release authenticateHandler?
    self.authenticateHandler = nil;
    
    self.postScoreReply = nil;
    
    self.session = nil;

    self.service = nil;
    self.protocol = nil;
    self.framedTransport = nil;
    self.transport = nil;

    [playerPassword_ release];
    
    [super dealloc];
}

static NLLocalPlayer * localPlayer;
+ (NLLocalPlayer *) localPlayer {
    @synchronized(self) {
        return localPlayer;
    }
}

+ (void) setLocalPlayer:(NLLocalPlayer *)player {
    @synchronized(self) {
        localPlayer = player;
    }
}



+ (NLLocalPlayer *)localPlayer: (NSString *) playerID playerAlias:(NSString *) playerAlias password:(NSString *) password
{
    // TODO : Who should release the global local player object?
    NLLocalPlayer.localPlayer = [[NLLocalPlayer alloc] initWithPlayerID:playerID playerAlias:playerAlias playerPassword:password];
    
    return NLLocalPlayer.localPlayer;
}

@end
