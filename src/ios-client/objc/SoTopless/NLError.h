#import <Foundation/Foundation.h>
#import <SoTopless/NLDefines.h>

enum {
    // Error codes set by client SDK
    NLErrorCommunicationsFailure     =  -900,
    NLErrorInvalidUseOfSDK           =  -901,
    NLErrorNotAuthenticated          =  -902,
    // Error codes sent from server
    NLErrorUnknown                   = -1000,
    NLErrorNotSupported              = -1001,
    NLErrorIncompatibleClientVersion = -1002,
    NLErrorInvalidSessionHandle      = -1003,
    NLErrorInvalidArgument           = -1004,
    NLErrorInvalidCredential         = -1005
};

typedef NSInteger NLErrorCode;

