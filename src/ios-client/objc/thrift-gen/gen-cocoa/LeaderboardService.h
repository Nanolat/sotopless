/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#import <Foundation/Foundation.h>

#import "TProtocol.h"
#import "TApplicationException.h"
#import "TProtocolException.h"
#import "TProtocolUtil.h"
#import "TProcessor.h"
#import "TObjective-C.h"
#import "TBase.h"


enum ErrorCode {
  ErrorCode_NL_SUCCESS = 0,
  ErrorCode_NL_ERROR_CODE_START = -1000,
  ErrorCode_NL_FAILURE = -1000,
  ErrorCode_NL_INCOMPATIBLE_CLINET_VERSION = -1001,
  ErrorCode_NL_INVALID_ARGUMENT = -1002,
  ErrorCode_NL_INVALID_CREDENTIAL = -1003,
  ErrorCode_NL_ERROR_CODE_END = -1003
};

@interface ReplyStatus : NSObject <TBase, NSCoding> {
  int __error_code;
  NSString * __error_message_format;
  NSMutableArray * __error_message_args;

  BOOL __error_code_isset;
  BOOL __error_message_format_isset;
  BOOL __error_message_args_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, getter=error_code, setter=setError_code:) int error_code;
@property (nonatomic, retain, getter=error_message_format, setter=setError_message_format:) NSString * error_message_format;
@property (nonatomic, retain, getter=error_message_args, setter=setError_message_args:) NSMutableArray * error_message_args;
#endif

- (id) init;
- (id) initWithError_code: (int) error_code error_message_format: (NSString *) error_message_format error_message_args: (NSMutableArray *) error_message_args;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (int) error_code;
- (void) setError_code: (int) error_code;
#endif
- (BOOL) error_codeIsSet;

#if !__has_feature(objc_arc)
- (NSString *) error_message_format;
- (void) setError_message_format: (NSString *) error_message_format;
#endif
- (BOOL) error_message_formatIsSet;

#if !__has_feature(objc_arc)
- (NSMutableArray *) error_message_args;
- (void) setError_message_args: (NSMutableArray *) error_message_args;
#endif
- (BOOL) error_message_argsIsSet;

@end

@interface DefaultReply : NSObject <TBase, NSCoding> {
  ReplyStatus * __status;

  BOOL __status_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, retain, getter=status, setter=setStatus:) ReplyStatus * status;
#endif

- (id) init;
- (id) initWithStatus: (ReplyStatus *) status;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (ReplyStatus *) status;
- (void) setStatus: (ReplyStatus *) status;
#endif
- (BOOL) statusIsSet;

@end

@interface LeaderboardConnectReply : NSObject <TBase, NSCoding> {
  ReplyStatus * __status;
  int32_t __session_handle;
  NSData * __user_data;

  BOOL __status_isset;
  BOOL __session_handle_isset;
  BOOL __user_data_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, retain, getter=status, setter=setStatus:) ReplyStatus * status;
@property (nonatomic, getter=session_handle, setter=setSession_handle:) int32_t session_handle;
@property (nonatomic, retain, getter=user_data, setter=setUser_data:) NSData * user_data;
#endif

- (id) init;
- (id) initWithStatus: (ReplyStatus *) status session_handle: (int32_t) session_handle user_data: (NSData *) user_data;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (ReplyStatus *) status;
- (void) setStatus: (ReplyStatus *) status;
#endif
- (BOOL) statusIsSet;

#if !__has_feature(objc_arc)
- (int32_t) session_handle;
- (void) setSession_handle: (int32_t) session_handle;
#endif
- (BOOL) session_handleIsSet;

#if !__has_feature(objc_arc)
- (NSData *) user_data;
- (void) setUser_data: (NSData *) user_data;
#endif
- (BOOL) user_dataIsSet;

@end

@interface Session : NSObject <TBase, NSCoding> {
  int32_t __session_handle;

  BOOL __session_handle_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, getter=session_handle, setter=setSession_handle:) int32_t session_handle;
#endif

- (id) init;
- (id) initWithSession_handle: (int32_t) session_handle;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (int32_t) session_handle;
- (void) setSession_handle: (int32_t) session_handle;
#endif
- (BOOL) session_handleIsSet;

@end

@interface Score : NSObject <TBase, NSCoding> {
  int64_t __value;
  int64_t __date_epoch;
  NSString * __player_alias;
  NSString * __player_id;
  int32_t __rank;
  NSData * __situation;
  int32_t __vote_up_count;
  int32_t __vote_down_count;

  BOOL __value_isset;
  BOOL __date_epoch_isset;
  BOOL __player_alias_isset;
  BOOL __player_id_isset;
  BOOL __rank_isset;
  BOOL __situation_isset;
  BOOL __vote_up_count_isset;
  BOOL __vote_down_count_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, getter=value, setter=setValue:) int64_t value;
@property (nonatomic, getter=date_epoch, setter=setDate_epoch:) int64_t date_epoch;
@property (nonatomic, retain, getter=player_alias, setter=setPlayer_alias:) NSString * player_alias;
@property (nonatomic, retain, getter=player_id, setter=setPlayer_id:) NSString * player_id;
@property (nonatomic, getter=rank, setter=setRank:) int32_t rank;
@property (nonatomic, retain, getter=situation, setter=setSituation:) NSData * situation;
@property (nonatomic, getter=vote_up_count, setter=setVote_up_count:) int32_t vote_up_count;
@property (nonatomic, getter=vote_down_count, setter=setVote_down_count:) int32_t vote_down_count;
#endif

- (id) init;
- (id) initWithValue: (int64_t) value date_epoch: (int64_t) date_epoch player_alias: (NSString *) player_alias player_id: (NSString *) player_id rank: (int32_t) rank situation: (NSData *) situation vote_up_count: (int32_t) vote_up_count vote_down_count: (int32_t) vote_down_count;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (int64_t) value;
- (void) setValue: (int64_t) value;
#endif
- (BOOL) valueIsSet;

#if !__has_feature(objc_arc)
- (int64_t) date_epoch;
- (void) setDate_epoch: (int64_t) date_epoch;
#endif
- (BOOL) date_epochIsSet;

#if !__has_feature(objc_arc)
- (NSString *) player_alias;
- (void) setPlayer_alias: (NSString *) player_alias;
#endif
- (BOOL) player_aliasIsSet;

#if !__has_feature(objc_arc)
- (NSString *) player_id;
- (void) setPlayer_id: (NSString *) player_id;
#endif
- (BOOL) player_idIsSet;

#if !__has_feature(objc_arc)
- (int32_t) rank;
- (void) setRank: (int32_t) rank;
#endif
- (BOOL) rankIsSet;

#if !__has_feature(objc_arc)
- (NSData *) situation;
- (void) setSituation: (NSData *) situation;
#endif
- (BOOL) situationIsSet;

#if !__has_feature(objc_arc)
- (int32_t) vote_up_count;
- (void) setVote_up_count: (int32_t) vote_up_count;
#endif
- (BOOL) vote_up_countIsSet;

#if !__has_feature(objc_arc)
- (int32_t) vote_down_count;
- (void) setVote_down_count: (int32_t) vote_down_count;
#endif
- (BOOL) vote_down_countIsSet;

@end

@interface GetScoresReply : NSObject <TBase, NSCoding> {
  Score * __player_score;
  int32_t __from_rank;
  int64_t __count;
  NSMutableArray * __top_scores;

  BOOL __player_score_isset;
  BOOL __from_rank_isset;
  BOOL __count_isset;
  BOOL __top_scores_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, retain, getter=player_score, setter=setPlayer_score:) Score * player_score;
@property (nonatomic, getter=from_rank, setter=setFrom_rank:) int32_t from_rank;
@property (nonatomic, getter=count, setter=setCount:) int64_t count;
@property (nonatomic, retain, getter=top_scores, setter=setTop_scores:) NSMutableArray * top_scores;
#endif

- (id) init;
- (id) initWithPlayer_score: (Score *) player_score from_rank: (int32_t) from_rank count: (int64_t) count top_scores: (NSMutableArray *) top_scores;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (Score *) player_score;
- (void) setPlayer_score: (Score *) player_score;
#endif
- (BOOL) player_scoreIsSet;

#if !__has_feature(objc_arc)
- (int32_t) from_rank;
- (void) setFrom_rank: (int32_t) from_rank;
#endif
- (BOOL) from_rankIsSet;

#if !__has_feature(objc_arc)
- (int64_t) count;
- (void) setCount: (int64_t) count;
#endif
- (BOOL) countIsSet;

#if !__has_feature(objc_arc)
- (NSMutableArray *) top_scores;
- (void) setTop_scores: (NSMutableArray *) top_scores;
#endif
- (BOOL) top_scoresIsSet;

@end

@interface PostScoreReply : NSObject <TBase, NSCoding> {
  ReplyStatus * __status;
  GetScoresReply * __scores;

  BOOL __status_isset;
  BOOL __scores_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, retain, getter=status, setter=setStatus:) ReplyStatus * status;
@property (nonatomic, retain, getter=scores, setter=setScores:) GetScoresReply * scores;
#endif

- (id) init;
- (id) initWithStatus: (ReplyStatus *) status scores: (GetScoresReply *) scores;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (ReplyStatus *) status;
- (void) setStatus: (ReplyStatus *) status;
#endif
- (BOOL) statusIsSet;

#if !__has_feature(objc_arc)
- (GetScoresReply *) scores;
- (void) setScores: (GetScoresReply *) scores;
#endif
- (BOOL) scoresIsSet;

@end

@interface ConnectReply : NSObject <TBase, NSCoding> {
  ReplyStatus * __status;
  NSString * __server_name;
  int32_t __session_handle;
  NSData * __user_data;

  BOOL __status_isset;
  BOOL __server_name_isset;
  BOOL __session_handle_isset;
  BOOL __user_data_isset;
}

#if TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
@property (nonatomic, retain, getter=status, setter=setStatus:) ReplyStatus * status;
@property (nonatomic, retain, getter=server_name, setter=setServer_name:) NSString * server_name;
@property (nonatomic, getter=session_handle, setter=setSession_handle:) int32_t session_handle;
@property (nonatomic, retain, getter=user_data, setter=setUser_data:) NSData * user_data;
#endif

- (id) init;
- (id) initWithStatus: (ReplyStatus *) status server_name: (NSString *) server_name session_handle: (int32_t) session_handle user_data: (NSData *) user_data;

- (void) read: (id <TProtocol>) inProtocol;
- (void) write: (id <TProtocol>) outProtocol;

- (void) validate;

#if !__has_feature(objc_arc)
- (ReplyStatus *) status;
- (void) setStatus: (ReplyStatus *) status;
#endif
- (BOOL) statusIsSet;

#if !__has_feature(objc_arc)
- (NSString *) server_name;
- (void) setServer_name: (NSString *) server_name;
#endif
- (BOOL) server_nameIsSet;

#if !__has_feature(objc_arc)
- (int32_t) session_handle;
- (void) setSession_handle: (int32_t) session_handle;
#endif
- (BOOL) session_handleIsSet;

#if !__has_feature(objc_arc)
- (NSData *) user_data;
- (void) setUser_data: (NSData *) user_data;
#endif
- (BOOL) user_dataIsSet;

@end

@protocol LeaderboardService <NSObject>
- (ConnectReply *) connect: (int32_t) protocol_version player_id: (NSString *) player_id player_password: (NSString *) player_password user_data: (NSData *) user_data;  // throws TException
- (DefaultReply *) disconnect: (Session *) session;  // throws TException
- (PostScoreReply *) post_score: (Session *) session category: (NSString *) category score: (Score *) score;  // throws TException
- (GetScoresReply *) get_scores: (Session *) session categoy: (NSString *) categoy player_id: (NSString *) player_id from_rank: (int32_t) from_rank count: (int64_t) count;  // throws TException
- (DefaultReply *) vote_score: (Session *) session voting_player_id: (NSString *) voting_player_id score_value: (int64_t) score_value score_date_epoch: (int64_t) score_date_epoch vote_up_down: (int32_t) vote_up_down comment: (NSString *) comment;  // throws TException
@end

@interface LeaderboardServiceClient : NSObject <LeaderboardService> {
  id <TProtocol> inProtocol;
  id <TProtocol> outProtocol;
}
- (id) initWithProtocol: (id <TProtocol>) protocol;
- (id) initWithInProtocol: (id <TProtocol>) inProtocol outProtocol: (id <TProtocol>) outProtocol;
@end

@interface LeaderboardServiceProcessor : NSObject <TProcessor> {
  id <LeaderboardService> mService;
  NSDictionary * mMethodMap;
}
- (id) initWithLeaderboardService: (id <LeaderboardService>) service;
- (id<LeaderboardService>) service;
@end

@interface LeaderboardServiceConstants : NSObject {
}
+ (int32_t) PROTOCOL_VERSION;
@end
