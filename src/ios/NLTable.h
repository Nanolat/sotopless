//
//  NLTable.h
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DatabaseService.h"

@class DatabaseServiceClient;
@class Session;

@interface NLTable : NSObject
{
    Session * session_;
    DatabaseServiceClient * serviceClient_;
}


- (id) initWithServiceClient:(DatabaseServiceClient*)serviceClient sessionHandle:(int32_t)sessionHandle ;

+ (NLTable*) tableWithServiceClient:(DatabaseServiceClient*)serviceClient sessionHandle:(int32_t)sessionHandle ;

+ (NLTable*) createSharedTable:serviceClient sessionHandle:(int32_t)sessionHandle ;

+ (NLTable*) sharedTable ;

// category : The name of the leaderboard
+ (NSString *) scoreByScoreKeyTableName:(NSString*)category ;

// category : The name of the leaderboard
+ (NSString *) scoreKeyByUserName:(NSString*)category ;

// Create a table. Do not return an error even though the table already exists.
- (void) makeSureToCreateTable : (NSString *) tableName error:(NSError**)o_error;

- (void) put: (NSString *) tableName key: (NSData *) key value: (NSData *) value error:(NSError**)o_error;

- (void) get: (NSString *) tableName key: (NSData *) key value: (NSData **) o_value keyOrder:(KeyOrder*)o_keyOrder error:(NSError**)o_error;

- (CursorHandle) cursorOpenByOrder: (NSString *) tableName dir: (int) dir keyOrder: (KeyOrder) keyOrder error:(NSError**)o_error;

- (void) cursorFetch: (CursorHandle) cursorHandle key:(NSData **)o_key keyOrder:(KeyOrder*)o_keyOrder value:(NSData **)o_value endOfCursor:(BOOL*)o_endOfCursor error:(NSError**)o_error;

- (void) cursorClose: (CursorHandle) cursorHandle error:(NSError**)o_error;

@end
