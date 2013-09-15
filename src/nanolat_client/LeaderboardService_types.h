/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef LeaderboardService_TYPES_H
#define LeaderboardService_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace nanolat { namespace leaderboard {

struct ErrorCode {
  enum type {
    NL_SUCCESS = 0,
    NL_ERROR_CODE_START = -1000,
    NL_FAILURE = -1000,
    NL_NOT_SUPPORTED = -1001,
    NL_INCOMPATIBLE_CLINET_VERSION = -1002,
    NL_INVALID_SESSION_HANDLE = -1003,
    NL_INVALID_ARGUMENT = -1004,
    NL_INVALID_CREDENTIAL = -1005,
    NL_ERROR_CODE_END = -1005
  };
};

extern const std::map<int, const char*> _ErrorCode_VALUES_TO_NAMES;

typedef struct _ReplyStatus__isset {
  _ReplyStatus__isset() : error_code(false), error_message_format(false), error_message_args(false) {}
  bool error_code;
  bool error_message_format;
  bool error_message_args;
} _ReplyStatus__isset;

class ReplyStatus {
 public:

  static const char* ascii_fingerprint; // = "D121591FCC85D0E17C727CA264A5F46D";
  static const uint8_t binary_fingerprint[16]; // = {0xD1,0x21,0x59,0x1F,0xCC,0x85,0xD0,0xE1,0x7C,0x72,0x7C,0xA2,0x64,0xA5,0xF4,0x6D};

  ReplyStatus() : error_code((ErrorCode::type)0), error_message_format() {
  }

  virtual ~ReplyStatus() throw() {}

  ErrorCode::type error_code;
  std::string error_message_format;
  std::vector<std::string>  error_message_args;

  _ReplyStatus__isset __isset;

  void __set_error_code(const ErrorCode::type val) {
    error_code = val;
  }

  void __set_error_message_format(const std::string& val) {
    error_message_format = val;
  }

  void __set_error_message_args(const std::vector<std::string> & val) {
    error_message_args = val;
  }

  bool operator == (const ReplyStatus & rhs) const
  {
    if (!(error_code == rhs.error_code))
      return false;
    if (!(error_message_format == rhs.error_message_format))
      return false;
    if (!(error_message_args == rhs.error_message_args))
      return false;
    return true;
  }
  bool operator != (const ReplyStatus &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ReplyStatus & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ReplyStatus &a, ReplyStatus &b);

typedef struct _DefaultReply__isset {
  _DefaultReply__isset() : status(false) {}
  bool status;
} _DefaultReply__isset;

class DefaultReply {
 public:

  static const char* ascii_fingerprint; // = "F6AD8D6EE2A97B9BAFE986CBB5A33301";
  static const uint8_t binary_fingerprint[16]; // = {0xF6,0xAD,0x8D,0x6E,0xE2,0xA9,0x7B,0x9B,0xAF,0xE9,0x86,0xCB,0xB5,0xA3,0x33,0x01};

  DefaultReply() {
  }

  virtual ~DefaultReply() throw() {}

  ReplyStatus status;

  _DefaultReply__isset __isset;

  void __set_status(const ReplyStatus& val) {
    status = val;
  }

  bool operator == (const DefaultReply & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const DefaultReply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DefaultReply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(DefaultReply &a, DefaultReply &b);

typedef struct _Session__isset {
  _Session__isset() : session_handle(false) {}
  bool session_handle;
} _Session__isset;

class Session {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  Session() : session_handle(0) {
  }

  virtual ~Session() throw() {}

  int32_t session_handle;

  _Session__isset __isset;

  void __set_session_handle(const int32_t val) {
    session_handle = val;
  }

  bool operator == (const Session & rhs) const
  {
    if (!(session_handle == rhs.session_handle))
      return false;
    return true;
  }
  bool operator != (const Session &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Session & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Session &a, Session &b);

typedef struct _Score__isset {
  _Score__isset() : value(false), date_epoch(false), user_alias(false), user_id(false), rank(false), situation(false), vote_up_count(false), vote_down_count(false) {}
  bool value;
  bool date_epoch;
  bool user_alias;
  bool user_id;
  bool rank;
  bool situation;
  bool vote_up_count;
  bool vote_down_count;
} _Score__isset;

class Score {
 public:

  static const char* ascii_fingerprint; // = "E447ECEACF2D326FE000A75298B11CD9";
  static const uint8_t binary_fingerprint[16]; // = {0xE4,0x47,0xEC,0xEA,0xCF,0x2D,0x32,0x6F,0xE0,0x00,0xA7,0x52,0x98,0xB1,0x1C,0xD9};

  Score() : value(0), date_epoch(0), user_alias(), user_id(), rank(0), situation(), vote_up_count(0), vote_down_count(0) {
  }

  virtual ~Score() throw() {}

  int64_t value;
  int64_t date_epoch;
  std::string user_alias;
  std::string user_id;
  int32_t rank;
  std::string situation;
  int32_t vote_up_count;
  int32_t vote_down_count;

  _Score__isset __isset;

  void __set_value(const int64_t val) {
    value = val;
  }

  void __set_date_epoch(const int64_t val) {
    date_epoch = val;
  }

  void __set_user_alias(const std::string& val) {
    user_alias = val;
  }

  void __set_user_id(const std::string& val) {
    user_id = val;
  }

  void __set_rank(const int32_t val) {
    rank = val;
  }

  void __set_situation(const std::string& val) {
    situation = val;
  }

  void __set_vote_up_count(const int32_t val) {
    vote_up_count = val;
  }

  void __set_vote_down_count(const int32_t val) {
    vote_down_count = val;
  }

  bool operator == (const Score & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    if (!(date_epoch == rhs.date_epoch))
      return false;
    if (!(user_alias == rhs.user_alias))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    if (!(rank == rhs.rank))
      return false;
    if (!(situation == rhs.situation))
      return false;
    if (!(vote_up_count == rhs.vote_up_count))
      return false;
    if (!(vote_down_count == rhs.vote_down_count))
      return false;
    return true;
  }
  bool operator != (const Score &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Score & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Score &a, Score &b);

typedef struct _UserScoreAndTopScores__isset {
  _UserScoreAndTopScores__isset() : user_score(false), from_rank(false), count(false), top_scores(false) {}
  bool user_score;
  bool from_rank;
  bool count;
  bool top_scores;
} _UserScoreAndTopScores__isset;

class UserScoreAndTopScores {
 public:

  static const char* ascii_fingerprint; // = "BDF6090738155A1FB5C43743D545B40E";
  static const uint8_t binary_fingerprint[16]; // = {0xBD,0xF6,0x09,0x07,0x38,0x15,0x5A,0x1F,0xB5,0xC4,0x37,0x43,0xD5,0x45,0xB4,0x0E};

  UserScoreAndTopScores() : from_rank(0), count(0) {
  }

  virtual ~UserScoreAndTopScores() throw() {}

  Score user_score;
  int32_t from_rank;
  int64_t count;
  std::vector<Score>  top_scores;

  _UserScoreAndTopScores__isset __isset;

  void __set_user_score(const Score& val) {
    user_score = val;
  }

  void __set_from_rank(const int32_t val) {
    from_rank = val;
  }

  void __set_count(const int64_t val) {
    count = val;
  }

  void __set_top_scores(const std::vector<Score> & val) {
    top_scores = val;
  }

  bool operator == (const UserScoreAndTopScores & rhs) const
  {
    if (!(user_score == rhs.user_score))
      return false;
    if (!(from_rank == rhs.from_rank))
      return false;
    if (!(count == rhs.count))
      return false;
    if (!(top_scores == rhs.top_scores))
      return false;
    return true;
  }
  bool operator != (const UserScoreAndTopScores &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserScoreAndTopScores & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(UserScoreAndTopScores &a, UserScoreAndTopScores &b);

typedef struct _PostScoreReply__isset {
  _PostScoreReply__isset() : status(false), scores(false) {}
  bool status;
  bool scores;
} _PostScoreReply__isset;

class PostScoreReply {
 public:

  static const char* ascii_fingerprint; // = "ED20B4254D63522D4314AC4E224FABE5";
  static const uint8_t binary_fingerprint[16]; // = {0xED,0x20,0xB4,0x25,0x4D,0x63,0x52,0x2D,0x43,0x14,0xAC,0x4E,0x22,0x4F,0xAB,0xE5};

  PostScoreReply() {
  }

  virtual ~PostScoreReply() throw() {}

  ReplyStatus status;
  UserScoreAndTopScores scores;

  _PostScoreReply__isset __isset;

  void __set_status(const ReplyStatus& val) {
    status = val;
  }

  void __set_scores(const UserScoreAndTopScores& val) {
    scores = val;
  }

  bool operator == (const PostScoreReply & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(scores == rhs.scores))
      return false;
    return true;
  }
  bool operator != (const PostScoreReply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const PostScoreReply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(PostScoreReply &a, PostScoreReply &b);

typedef struct _GetScoresReply__isset {
  _GetScoresReply__isset() : status(false), scores(false) {}
  bool status;
  bool scores;
} _GetScoresReply__isset;

class GetScoresReply {
 public:

  static const char* ascii_fingerprint; // = "ED20B4254D63522D4314AC4E224FABE5";
  static const uint8_t binary_fingerprint[16]; // = {0xED,0x20,0xB4,0x25,0x4D,0x63,0x52,0x2D,0x43,0x14,0xAC,0x4E,0x22,0x4F,0xAB,0xE5};

  GetScoresReply() {
  }

  virtual ~GetScoresReply() throw() {}

  ReplyStatus status;
  UserScoreAndTopScores scores;

  _GetScoresReply__isset __isset;

  void __set_status(const ReplyStatus& val) {
    status = val;
  }

  void __set_scores(const UserScoreAndTopScores& val) {
    scores = val;
  }

  bool operator == (const GetScoresReply & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(scores == rhs.scores))
      return false;
    return true;
  }
  bool operator != (const GetScoresReply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const GetScoresReply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(GetScoresReply &a, GetScoresReply &b);

typedef struct _ConnectReply__isset {
  _ConnectReply__isset() : status(false), server_name(false), session_handle(false), user_data(false) {}
  bool status;
  bool server_name;
  bool session_handle;
  bool user_data;
} _ConnectReply__isset;

class ConnectReply {
 public:

  static const char* ascii_fingerprint; // = "00B492A3875749A3E33B6E05569ACED5";
  static const uint8_t binary_fingerprint[16]; // = {0x00,0xB4,0x92,0xA3,0x87,0x57,0x49,0xA3,0xE3,0x3B,0x6E,0x05,0x56,0x9A,0xCE,0xD5};

  ConnectReply() : server_name(), session_handle(0), user_data() {
  }

  virtual ~ConnectReply() throw() {}

  ReplyStatus status;
  std::string server_name;
  int32_t session_handle;
  std::string user_data;

  _ConnectReply__isset __isset;

  void __set_status(const ReplyStatus& val) {
    status = val;
  }

  void __set_server_name(const std::string& val) {
    server_name = val;
  }

  void __set_session_handle(const int32_t val) {
    session_handle = val;
  }

  void __set_user_data(const std::string& val) {
    user_data = val;
  }

  bool operator == (const ConnectReply & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(server_name == rhs.server_name))
      return false;
    if (!(session_handle == rhs.session_handle))
      return false;
    if (!(user_data == rhs.user_data))
      return false;
    return true;
  }
  bool operator != (const ConnectReply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ConnectReply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ConnectReply &a, ConnectReply &b);

}} // namespace

#endif
