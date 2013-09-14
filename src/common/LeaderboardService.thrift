namespace cpp nanolat.leaderboard

const i32 PROTOCOL_VERSION = 1;

enum ErrorCode {
	NL_SUCCESS = 0,
	NL_ERROR_CODE_START            = -1000,
	NL_FAILURE                     = -1000,
	NL_INCOMPATIBLE_CLINET_VERSION = -1001,
	NL_INVALID_ARGUMENT            = -1002,
	NL_INVALID_CREDENTIAL          = -1003,
	NL_ERROR_CODE_END              = -1003
}

struct ReplyStatus
{
	1: ErrorCode    error_code, 
	2: string       error_message_format,
	3: list<string> error_message_args
}

struct DefaultReply {
    1: ReplyStatus status
}

struct LeaderboardConnectReply {
    1: ReplyStatus status,
	2: i32    session_handle,
	3: binary user_data
}

struct Session {
    1: i32 session_handle
}

struct Score {
    1: i64    value,
    2: i64    date_epoch,
    3: string player_alias,
    4: string player_id,
    5: i32    rank,
    6: binary situation,
    7: i32    vote_up_count,
    8: i32    vote_down_count
}

struct GetScoresReply {
    1: Score       player_score,
    2: i32         from_rank,
    3: i64         count,
    4: list<Score> top_scores
}

struct PostScoreReply {
    1: ReplyStatus    status,
    2: GetScoresReply scores
}

struct ConnectReply {
    1: ReplyStatus status,
    // The server name that can be used to show to users. 
    // Client can be connected to one of available servers, and server_name is user friendly server name for notifying a user to where he/she is connected.
    2: string server_name,
	3: i32    session_handle,
	4: binary user_data
}

service LeaderboardService {
    ConnectReply            connect     (1: i32 protocol_version, 2: string player_id, 3: string player_password, 4: binary user_data),
    DefaultReply            disconnect  (1: Session session),

    PostScoreReply          post_score  (1: Session session, 2: string category, 3: Score score),
    GetScoresReply          get_scores  (1: Session session, 2: string categoy,  3: string player_id, 4: i32 from_rank, 5: i64 count ),
    DefaultReply            vote_score  (1: Session session, 2: string voting_player_id,  3: i64 score_value, 4: i64 score_date_epoch, 5: i32 vote_up_down, 6: string comment ) 
}

