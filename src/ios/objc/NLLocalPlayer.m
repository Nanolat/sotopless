#import <SoTopless/NLLocalPlayer.h>
#import "NLUtil.h"

#import "TSocketClient.h"
#import "TFramedTransport.h"
#import "TBinaryProtocol.h"
#import "TTransportException.h"

#import "DatabaseService.h"

@interface NLLocalPlayer() {
    TSocketClient *transport_;
    TFramedTransport * framedTransport_;
    TBinaryProtocol *protocol_;
    DatabaseServiceClient * service_;
}

-(id) init;
-(void)dealloc;

@end

@implementation NLLocalPlayer

@synthesize authenticated=authenticated_;

- (id) init {
    if ( self = [super init]) {
        service_ = nil;
        transport_ = nil;
        framedTransport_ = nil;
        protocol_ = nil;
        authenticated_ = FALSE;
    }
    return self;
}

// TODO : the NL_SOTOPLESS_HOST_NAME is changed to "xy.com", then the client hangs.
#define NL_SOTOPLESS_HOST_NAME @"asdasldjasldjas.com"
#define NL_SOTOPLESS_PORT 9090
- (void) setAuthenticateHandler:(void(^)(NSError *error))completionHandler {

    // TODO : This is fake. Implement it.
    NSLog(@"setAuthenticateHandler called.");

    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^ {
        // Background work here
        // Talk to a server via socket, using a binary protocol
        NSError * error = nil;
        
        @try {

            transport_ = [[TSocketClient alloc] initWithHostname:NL_SOTOPLESS_HOST_NAME port:NL_SOTOPLESS_PORT];
            assert(transport_);
            framedTransport_ = [[TFramedTransport alloc] initWithTransport:transport_];
            assert(framedTransport_);
            protocol_ = [[TBinaryProtocol alloc] initWithTransport:transport_ strictRead:YES strictWrite:YES];
            assert(protocol_);
            service_ = [[DatabaseServiceClient alloc] initWithProtocol:protocol_];
            assert(service_);

            ConnectReply * reply = [service_ connect];
            assert(reply);
            
            if (reply) {
                if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
                    // TODO : Create a user account if the account does not exits.
                    // TODO : Authenticate user id and password.
                } else {
                    error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:@"Can't connect to SoTopless erver."];
                    assert(error);
                }
            }
            
        } @catch ( TTransportException * tx_excp ) {
            error = [NLUtil errorWithCode:NLErrorCommunicationsFailure
                                  message:[NLUtil append:@"Can't connect to SoTopless erver. Reason : ", tx_excp.reason, nil] ];
            assert(error);
        }
        
        NSLog(@"Finished work in background");

        dispatch_async(dispatch_get_main_queue(), ^ {
            authenticated_ = TRUE;
            completionHandler(error);
            
            NSLog(@"Back on main thread");
        });
        
    });
}

-(void)dealloc {
    [service_ release];
    [protocol_ release];
    [framedTransport_ release];
    [transport_ release];
    
    [super dealloc];
}

+ (NLLocalPlayer *)localPlayer: (NSString *) playerID playerAlias:(NSString *) playerAlias password:(NSString *) password
{
    // TODO : This is fake. Implement it.
    NLLocalPlayer * lplayer = [[[NLLocalPlayer alloc] init] autorelease];
    
    return lplayer;
}

@end
