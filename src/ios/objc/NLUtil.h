//
//  NLUtil.h
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NLUtil : NSObject
+ (NSError*) errorWithCode: (NSInteger) code message: (NSString*)message;
+ (NSString *) append:(id) first, ...;

@end
