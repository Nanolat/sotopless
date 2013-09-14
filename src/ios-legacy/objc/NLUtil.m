//
//  NLUtil.m
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import "NLUtil.h"

@implementation NLUtil

+ (NSError*) errorWithCode: (NSInteger) code message: (NSString*)message {
    NSMutableDictionary* details = [NSMutableDictionary dictionary];
    [details setValue:message forKey:NSLocalizedDescriptionKey];

    NSError * error = [NSError errorWithDomain:@"SoTopless" code:code userInfo:details];
    return error;
}

+ (NSString *) append:(id) first, ...
{
    NSString * result = @"";
    id eachArg;
    va_list alist;
    if(first)
    {
    	result = [result stringByAppendingString:first];
    	va_start(alist, first);
    	while ((eachArg = va_arg(alist, id)))
    		result = [result stringByAppendingString:eachArg];
    	va_end(alist);
    }
    return result;
}

@end
