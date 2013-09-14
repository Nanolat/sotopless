//
//  NLDataFormatter.h
//  objc
//
//  Created by Kangmo Kim on 13. 9. 8..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import <Foundation/Foundation.h>

// Format keys to store in Nanolat Database.
// All keys should be in big endian to get them stored in Nanolat Database.
// Functions for converting the keys back to the host endian are included in this class.

@interface NLDataFormatter : NSObject

+ (NSData *) packScore:(uint64_t)score playerAlias:(NSString*) playerAlias;

+ (int64_t) unpackScore:(NSData *) packedScore ;

+ (NSData *) packUserName: (NSString *) userName ;

+ (NSString *) unpackUserName: (NSData *) packedUserName ;

+ (NSData *) packValue: (NSData*)data packedValueLength:(int)packedValueLength;

+ (NSData *) unpackValue: (NSData*)data packedValueLength:(int)packedValueLength;


@end
