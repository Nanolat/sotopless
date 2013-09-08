//
//  NLDataFormatter.m
//  objc
//
//  Created by Kangmo Kim on 13. 9. 8..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//
#import "SoTopless/NLDefines.h"
#import "NLDataFormatter.h"

#define switch_endian32(v) \
(unsigned int)( ((((unsigned int)v)&0x000000FFU) << 24) | \
((((unsigned int)v)&0x0000FF00U) << 8)  | \
((((unsigned int)v)&0x00FF0000U) >> 8)  | \
((((unsigned int)v)&0xFF000000U) >> 24) )

#define switch_endian64(v) \
(unsigned long long)( ((((unsigned long long)v)&0x00000000000000FFULL) << 56) | \
((((unsigned long long)v)&0x000000000000FF00ULL) << 40) | \
((((unsigned long long)v)&0x0000000000FF0000ULL) << 24) | \
((((unsigned long long)v)&0x00000000FF000000ULL) << 8)  | \
((((unsigned long long)v)&0x000000FF00000000ULL) >> 8)  | \
((((unsigned long long)v)&0x0000FF0000000000ULL) >> 24) | \
((((unsigned long long)v)&0x00FF000000000000ULL) >> 40) | \
((((unsigned long long)v)&0xFF00000000000000ULL) >> 56) )

@implementation NLDataFormatter

+ (bool) is_little_endian {
	int n = 1;
	// little endian if true
	return ( *(char *)&n == 1) ? true : false;
}

// convert to big endian if the host is using little endian
+ (uint32_t) to_big_endian32:(uint32_t) value {
	if ( [NLDataFormatter is_little_endian] )
		return switch_endian32(value);
	return value;
}

// convert to big endian if the host is using little endian
+ (uint64_t) to_big_endian64:(uint64_t) value {
	if ( [NLDataFormatter is_little_endian] )
		return switch_endian64(value);
	return value;
}

// convert to little endian if the host is little endian.
+ (uint32_t) to_host_endian32:(uint32_t) big_endian_value {
	if ( [NLDataFormatter is_little_endian] )
		return switch_endian32(big_endian_value);
	return big_endian_value;
}

// convert to little endian if the host is little endian.
+ (uint64_t) to_host_endian64:(uint64_t) big_endian_value {
	if ( [NLDataFormatter is_little_endian] )
		return switch_endian64(big_endian_value);
	return big_endian_value;
}

+ (uint64_t) hash:(NSString*)str {
    assert(str);
    
	uint64_t hash = [str hash];
    return hash;
}

#define MAX_UINT64_VALUE (0xFFFFFFFFFFFFFFFFLLU)

// pack the given score and hash of user name into a string to use as a key of users_by_score table.
+ (NSData *) packScore:(uint64_t)score playerAlias:(NSString*) playerAlias {
    assert(playerAlias);
    
	// The score needs to be sorted in descending order. 
    uint64_t scoreForDescendingOrder = MAX_UINT64_VALUE-score;
    
	// 1. Step 1 : Convert the score from little endian to big endian.
	uint64_t big_endian_score = [NLDataFormatter to_big_endian64:scoreForDescendingOrder];
    NSMutableData * data = [[[NSMutableData alloc] init] autorelease];
    
    [data appendBytes:&big_endian_score length:sizeof(big_endian_score)];
    
	// 2. Step 2 : Append hash of user name (big endian), to make the score key unique for a leaderboard.
	uint64_t hash_of_user = [NLDataFormatter hash:playerAlias];
	uint64_t big_endian_hash = [NLDataFormatter to_big_endian64:hash_of_user];
    
    [data appendBytes:&big_endian_hash length:sizeof(big_endian_hash)];
    
    return data;
}

// unpack the score from the packed score with pack_score function.
+ (int64_t) unpackScore:(NSData *) packedScore {
    assert(packedScore);
    
    // The packed score should have two uint64_t values. 1) score in big endian, 2) hash of playerAlias in big endian.
    assert( packedScore.length == sizeof(uint64_t) * 2 );

	uint64_t big_endian_score = *((uint64_t*)[packedScore bytes]);
    
	uint64_t scoreForDescendingOrder = [NLDataFormatter to_host_endian64:big_endian_score];
    
    // The score was stored to be sorted in descending order.
    uint64_t score = MAX_UINT64_VALUE-scoreForDescendingOrder;
    
    return (int64_t) score;
}


+ (NSData *) packUserName: (NSString *) userName {
    assert(userName);
    
    NSData* data = [userName dataUsingEncoding:NSUTF8StringEncoding];
    if ( [data length] > NAME_DATA_PACKED_LEN ) {
        return nil;
    }
    
    NSMutableData * paddedData = [NSMutableData dataWithData:data];
    [paddedData increaseLengthBy:4];
    
	return paddedData;
}

+ (NSString *) unpackUserName: (NSData *) packedUserName {
    // TODO : Need to unpad data packed by packUserName
    assert(packedUserName);
    
    NSString* userName = [[[NSString alloc] initWithData:packedUserName encoding:NSASCIIStringEncoding] autorelease];
    assert(userName);
    
	return userName;
}

/* Pack data in a space whose size is exactly packedDataLength 
 * This data is used to format them for fixed size keys and fixed size values to put into Nanolat Database.
 * Return nil if the given data does not fit in packedDataLength.
 */
+ (NSData *) packValue: (NSData*)rawValue packedValueLength:(int)packedValueLength {
    assert(rawValue);
    assert(packedValueLength > 0 );
    
    NSUInteger rawValueLength = rawValue.length;
    if (sizeof(rawValueLength)+rawValue.length > packedValueLength) {
        return nil;
    }
    NSMutableData * mutableData = [[[NSMutableData alloc] init] autorelease];

    [mutableData appendBytes:&rawValueLength length:sizeof(rawValueLength)];

    [mutableData appendBytes:rawValue.bytes length:rawValueLength];
    
    int increment = packedValueLength - mutableData.length ;
    assert( increment >= 0 );
    
    [mutableData increaseLengthBy:increment];
     
    return mutableData;
}

/* Unpack data from a space which is packed with packData 
 * These data are from fixed size keys and fixed size values.
 */
+ (NSData *) unpackValue: (NSData*)packedValue packedValueLength:(int)packedValueLength {
    assert(packedValue);
    assert(packedValueLength > 0);
    assert(packedValue.length == packedValueLength);

    NSUInteger rawValueLength;
    rawValueLength = *((NSUInteger*)(packedValue.bytes));
    
    NSData * unpackedValue = [packedValue subdataWithRange:NSMakeRange(sizeof(rawValueLength), rawValueLength)];
    
    return unpackedValue;
}


@end
