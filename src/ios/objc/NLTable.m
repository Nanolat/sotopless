//
//  NLTable.m
//  objc
//
//  Created by Kangmo Kim on 13. 9. 7..
//  Copyright (c) 2013년 ThankyouSoft. All rights reserved.
//

#import "NLTable.h"
#import <SoTopless/NLError.h>
#import "DatabaseService.h"
#import "NLUtil.h"

@implementation NLTable
- (id) initWithServiceClient:(DatabaseServiceClient*)serviceClient sessionHandle:(int32_t)sessionHandle {
    if ( self = [super init]) {
        assert( serviceClient );
        assert( sessionHandle > 0 );
        
        session_ = [[Session alloc] initWithSession_handle:sessionHandle];
        assert(session_);
        
        serviceClient_ = [serviceClient retain];
    }
    return self;
}

- (void) dealloc {
    [session_ release];
    [serviceClient_ release];
    
    [super dealloc];
}


+ (NLTable*) tableWithServiceClient:(DatabaseServiceClient*)serviceClient sessionHandle:(int32_t)sessionHandle {
    id table = [[[NLTable alloc] initWithServiceClient:serviceClient sessionHandle:sessionHandle] autorelease];
    assert(table);
    return table;
}


static NLTable * gTable = nil;

+ (NLTable*) createSharedTable:serviceClient sessionHandle:(int32_t)sessionHandle {
    
    if (gTable == nil) {
        gTable = [[NLTable tableWithServiceClient:serviceClient sessionHandle:sessionHandle] retain];
        assert(gTable);
    }
    
    return gTable;
}

+ (NLTable*) sharedTable {
    return gTable;
}

// category : The name of the leaderboard
+ (NSString *) scoreByScoreKeyTableName:(NSString*)category {
    NSString * score_by_score_key = [category stringByAppendingString:@"_s_by_sk"];
    return score_by_score_key;
}

// category : The name of the leaderboard
+ (NSString *) scoreKeyByUserName:(NSString*)category {
    NSString * score_key_by_user = [category stringByAppendingString:@"_sk_by_u"];
    return score_key_by_user;
}

// TODO : Show message from DB.
+ (NSString *) makeErrorMessage : (NSString *) tableName operation:(NSString*)operation codeFromDB:(enum ErrorCode)dbCode
{
    NSString * message = [NSString stringWithFormat:@"Database Error : %@ failed. Table : %@, DB Error Code : %d", operation, tableName, dbCode];
    return message;
}

+ (NSString *) makeErrorMessageWithCursorHandle : (CursorHandle) cursorHandle operation:(NSString*)operation codeFromDB:(enum ErrorCode)dbCode
{
    NSString * message = [NSString stringWithFormat:@"Database Error : %@ failed. Cursor Handle : %lld, DB Error Code : %d", operation, cursorHandle, dbCode];
    return message;
}

// Create a table. Do not return an error even though the table already exists.
- (void) makeSureToCreateTable : (NSString *) tableName error:(NSError**)o_error
{
    assert(tableName);
    assert(o_error);
    
    DefaultReply * reply = [serviceClient_ table_create:session_ table_name:tableName];
    assert(reply);

    if ( reply.status.error_code == ErrorCode_NL_TABLE_ALREADY_EXISTS ) {
        // ignore the error saying that the table already exists.
        *o_error = nil;
    } else if ( reply.status.error_code != ErrorCode_NL_SUCCESS ) {
        
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessage: tableName operation:@"create table" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);
        
        *o_error = error;
    } else {
        // SUCCESS!
        *o_error = nil;
    }
}

- (void) put: (NSString *) tableName key: (NSData *) key value: (NSData *) value error:(NSError**)o_error
{
    assert(tableName);
    assert(key);
    assert(value);
    assert(o_error);
    
    DefaultReply * reply = [serviceClient_ table_put:session_ table_name:tableName key:key value:value];
    assert(reply);
    
    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
        *o_error = nil;
    } else {
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessage: tableName operation:@"put key" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);
        
        *o_error = error;
    }
}

- (void) get: (NSString *) tableName key: (NSData *) key value: (NSData **) o_value keyOrder:(KeyOrder*)o_keyOrder error:(NSError**)o_error
{
    assert(tableName);
    assert(key);
    assert(o_value);
    assert(o_keyOrder);
    assert(o_error);
    
    TableGetReply * reply = [serviceClient_ table_get_by_key:session_ table_name:tableName key:key];
    assert(reply);
    
    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
        *o_value = reply.value;
        *o_keyOrder = reply.key_order;
        *o_error = nil;
    } else {
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessage: tableName operation:@"get key" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);

        *o_value = nil;
        *o_keyOrder = nil;
        *o_error = error;
    }
    
}

- (CursorHandle) cursorOpenByOrder: (NSString *) tableName dir: (int) dir keyOrder: (KeyOrder) keyOrder error:(NSError**)o_error
{
    assert(tableName);
    assert(keyOrder > 0);
    assert(o_error);
    
    CursorOpenReply * reply = [serviceClient_ cursor_open_by_order:session_ table_name:tableName dir:dir keyOrder:keyOrder];
    assert(reply);
    
    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
        *o_error = nil;
        
        return reply.cursor_handle;
    } else {
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessage: tableName operation:@"get key" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);
        
        *o_error = error;
        return 0;
    }
    return 0;
}

- (void) cursorFetch: (CursorHandle) cursorHandle key:(NSData **)o_key keyOrder:(KeyOrder*)o_keyOrder value:(NSData **)o_value endOfCursor:(BOOL*)o_endOfCursor error:(NSError**)o_error
{
    assert(cursorHandle > 0);
    assert(o_key);
    assert(o_value);
    assert(o_keyOrder);
    assert(o_endOfCursor);
    assert(o_error);

    CursorFetchReply * reply = [serviceClient_ cursor_fetch:session_ cursor_handle:cursorHandle];
    assert(reply);
    
    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
        *o_key = reply.key;
        *o_keyOrder = reply.key_order;
        *o_value = reply.value;
        *o_endOfCursor = FALSE;
        *o_error = nil;
    } else if ( reply.status.error_code == ErrorCode_NL_CURSOR_HAS_NO_MORE_KEYS) {
        *o_key = nil;
        *o_keyOrder = 0;
        *o_value = nil;
        *o_endOfCursor = TRUE;
        // Do not return any error if we reached at the end of the cursor.
        *o_error = nil;
    } else {
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessageWithCursorHandle: cursorHandle operation:@"cursor fetch" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);

        *o_key = nil;
        *o_keyOrder = 0;
        *o_value = nil;
        *o_endOfCursor = TRUE;

        *o_error = error;
    }
}

- (void) cursorClose: (CursorHandle) cursorHandle error:(NSError**)o_error
{
    assert(cursorHandle > 0);
    assert(o_error);
    
    DefaultReply * reply = [serviceClient_ cursor_close:session_ cursor_handle:cursorHandle];
    assert(reply);
    
    if ( reply.status.error_code == ErrorCode_NL_SUCCESS ) {
        *o_error = nil;
    } else {
        // TODO : Think about better error code.
        NSString * errorMessage = [NLTable makeErrorMessageWithCursorHandle: cursorHandle operation:@"close cursor" codeFromDB:reply.status.error_code];
        NSError * error = [NLUtil errorWithCode:NLErrorCommunicationsFailure message:errorMessage];
        assert(error);
        
        *o_error = error;
    }
}

@end
