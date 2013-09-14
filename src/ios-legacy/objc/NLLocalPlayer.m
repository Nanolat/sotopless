#import <SoTopless/NLLocalPlayer.h>
#import "NLUtil.h"

#import "TSocketClient.h"
#import "TFramedTransport.h"
#import "TBinaryProtocol.h"
#import "TTransportException.h"

#import "DatabaseService.h"
#import "NLTable.h"

@interface NLLocalPlayer() {
    TSocketClient *transport_;
    TFramedTransport * framedTransport_;
    TBinaryProtocol *protocol_;
    DatabaseServiceClient * service_;
}

-(void)dealloc;

@end

@implementation NLLocalPlayer

@synthesize authenticated=authenticated_;

- (id) initWithPlayerID:(NSString *) playerID playerAlias:(NSString *) playerAlias {
    if ( self = [super initWithPlayerID:playerID playerAlias:playerAlias]) {
        service_ = nil;
        transport_ = nil;
        framedTransport_ = nil;
        protocol_ = nil;
        authenticated_ = FALSE;
    }
    return self;
}

// TODO : the NL_SOTOPLESS_HOST_NAME is changed to "xy.com", then the client hangs.
#define NL_SOTOPLESS_HOST_NAME @"10.11.9.75"
#define NL_SOTOPLESS_PORT 9090
- (void) setAuthenticateHandler:(void(^)(NSError *error))completionHandler {

    // TODO : This is fake. Implement it.
    NSLog(@"setAuthenticateHandler called.");

    // TODO : Should we keep the completionHandler in NLLocalPlayer object and do authentication periodically?
    
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
            protocol_ = [[TBinaryProtocol alloc] initWithTransport:framedTransport_ strictRead:YES strictWrite:YES];
            assert(protocol_);
            service_ = [[DatabaseServiceClient alloc] initWithProtocol:protocol_];
            assert(service_);

            ConnectReply * reply = [service_ connect];
            assert(reply);
            
            if (reply) {
                if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
                    NLTable * table = [NLTable createSharedTable:service_ sessionHandle:reply.session_handle];
                    assert(table);
                    // TODO : Create a user account if the account does not exits.
                    // TODO : Authenticate user id and password.
                } else {
                    error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:@"Can't connect to SoTopless server."];
                    assert(error);
                }
            }
            
        } @catch ( TTransportException * tx_excp ) {
            error = [NLUtil errorWithCode:NLErrorCommunicationsFailure
                                  message:[NLUtil append:@"Can't connect to SoTopless server. Reason : ", tx_excp.reason, nil] ];
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
            completionHandler(error);

            // release the error object retained in background thread.
            [error release];

            NSLog(@"Back on main thread");
        });
        
    });
}

-(void)dealloc {
    // TODO : Should we release authenticateHandler?
    
    [service_ release];
    [protocol_ release];
    [framedTransport_ release];
    [transport_ release];
    
    [super dealloc];
}

NLLocalPlayer * gLocalPlayer = nil;

+ (NLLocalPlayer *)localPlayer: (NSString *) playerID playerAlias:(NSString *) playerAlias password:(NSString *) password
{
    // TODO : Who should release the global local player object?
    gLocalPlayer = [[NLLocalPlayer alloc] initWithPlayerID:playerID playerAlias:playerAlias];
    
    return gLocalPlayer;
}

@end
