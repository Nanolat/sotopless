namespace cpp nanolat.thrift

/* ID is requested by the client. */
typedef i64 KeyOrder

/* Handle is created by the server. */
typedef i64 CursorHandle

enum ErrorCode {
	NL_SUCCESS = 0,
	NL_ERROR_CODE_START          = -1000,
	NL_FAILURE                   = -1000,
	NL_INVALID_ARGUMENT          = -1001,
	NL_INVALID_SESSION_HANDLE    = -1002,
	NL_INVALID_CURSOR_HANDLE     = -1003,
	NL_TRANSACTION_ALREADY_BEGAN = -1004,
	NL_NO_TRANSACTION_BEGAN      = -1005,
	NL_DATABASE_ALREADY_EXISTS   = -1006,
	NL_DATABASE_DOES_NOT_EXIST   = -1007,
	NL_TABLE_ALREADY_EXISTS      = -1008,
	NL_TABLE_DOES_NOT_EXIST      = -1009,
	NL_ERROR_CODE_END            = -1009
}

struct ReplyStatus
{
	1: ErrorCode    error_code, 
	2: string       error_message_format,
	3: list<string> error_message_args
}

enum CursorDirection {
   CD_FORWARD=1,
   CD_BACKWARD,
}

struct DefaultReply {
    1: ReplyStatus status
}

struct TableGetReply {
    1: ReplyStatus status,
	2: string key,
	3: string value,
	4: KeyOrder key_order
}

struct TableStatReply {
    1: ReplyStatus status,
    2: KeyOrder    key_count
}

struct CursorOpenReply {
    1: ReplyStatus status,
    2: CursorHandle cursor_handle
}

struct CursorFetchReply {
    1: ReplyStatus status,
	2: string key, 
	3: KeyOrder key_order, 
	4: string value
}

struct Session {
    1: i32 session_handle
}

struct ConnectReply {
    1: ReplyStatus status
	2: i32 session_handle
}
service DatabaseService {

    ConnectReply           connect(),
    
    DefaultReply           disconnect           (1: Session session),

    DefaultReply           database_create      (1: Session session, 2: string db_name),
    DefaultReply           database_drop        (1: Session session, 2: string db_name),
    DefaultReply           database_use         (1: Session session, 2: string db_name),
    
    DefaultReply           table_create         (1: Session session, 2: string table_name),
    DefaultReply           table_drop           (1: Session session, 2: string table_name),
    TableStatReply         table_stat           (1: Session session, 2: string table_name),
    
    DefaultReply           table_put            (1: Session session, 2: string table_name, 3: string key, 4: string value),
    TableGetReply          table_get_by_key     (1: Session session, 2: string table_name, 3: string key),
    TableGetReply          table_get_by_order   (1: Session session, 2: string table_name, 3: KeyOrder keyOrder),
    DefaultReply           table_del            (1: Session session, 2: string table_name, 3: string key),

    DefaultReply           transaction_begin    (1: Session session),
    DefaultReply           transaction_abort    (1: Session session),
    DefaultReply           transaction_commit   (1: Session session),
    
    CursorOpenReply        cursor_open_by_key   (1: Session session, 2: string table_name, 3: CursorDirection dir, 4: string key),
    CursorOpenReply        cursor_open_by_order (1: Session session, 2: string table_name, 3: CursorDirection dir, 4: KeyOrder keyOrder),
    CursorFetchReply       cursor_fetch         (1: Session session, 2: CursorHandle cursor_handle, 3: CursorDirection dir),
    DefaultReply           cursor_close         (1: Session session, 2: CursorHandle cursor_handle)
}

