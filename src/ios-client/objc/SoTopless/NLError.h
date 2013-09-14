#import <Foundation/Foundation.h>
#import <SoTopless/NLDefines.h>

enum {
    NLErrorUnknown                   = 1,
    NLErrorIncompatibleClientVersion = 2,
    NLErrorInvalidUseOfSDK           = 3,
    NLErrorInvalidParameter          = 4,
    NLErrorNotSupported              = 5,
    NLErrorCommunicationsFailure     = 6,
    NLErrorInvalidCredentials        = 7,
    NLErrorNotAuthenticated          = 8,
    NLErrorAuthenticationInProgress  = 9,
    NLErrorScoreNotSet               = 10
};

typedef NSInteger NLErrorCode;

