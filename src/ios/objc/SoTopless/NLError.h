#import <Foundation/Foundation.h>
#import <SoTopless/NLDefines.h>

enum {
    NLErrorUnknown                   = 1,
    NLErrorInvalidUseOfSDK           = 2,
    NLErrorInvalidParameter          = 2,
    NLErrorNotSupported              = 3,
    NLErrorCommunicationsFailure     = 4,
    NLErrorInvalidCredentials        = 5,
    NLErrorNotAuthenticated          = 6,
    NLErrorAuthenticationInProgress  = 7,
    NLErrorScoreNotSet               = 8,
};

typedef NSInteger NLErrorCode;

