/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "LeaderboardService_types.h"

#include <algorithm>

namespace nanolat { namespace leaderboard {

int _kErrorCodeValues[] = {
  ErrorCode::NL_SUCCESS,
  ErrorCode::NL_ERROR_CODE_START,
  ErrorCode::NL_FAILURE,
  ErrorCode::NL_INCOMPATIBLE_CLINET_VERSION,
  ErrorCode::NL_INVALID_ARGUMENT,
  ErrorCode::NL_INVALID_CREDENTIAL,
  ErrorCode::NL_ERROR_CODE_END
};
const char* _kErrorCodeNames[] = {
  "NL_SUCCESS",
  "NL_ERROR_CODE_START",
  "NL_FAILURE",
  "NL_INCOMPATIBLE_CLINET_VERSION",
  "NL_INVALID_ARGUMENT",
  "NL_INVALID_CREDENTIAL",
  "NL_ERROR_CODE_END"
};
const std::map<int, const char*> _ErrorCode_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(7, _kErrorCodeValues, _kErrorCodeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* ReplyStatus::ascii_fingerprint = "D121591FCC85D0E17C727CA264A5F46D";
const uint8_t ReplyStatus::binary_fingerprint[16] = {0xD1,0x21,0x59,0x1F,0xCC,0x85,0xD0,0xE1,0x7C,0x72,0x7C,0xA2,0x64,0xA5,0xF4,0x6D};

uint32_t ReplyStatus::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->error_code = (ErrorCode::type)ecast0;
          this->__isset.error_code = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->error_message_format);
          this->__isset.error_message_format = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->error_message_args.clear();
            uint32_t _size1;
            ::apache::thrift::protocol::TType _etype4;
            xfer += iprot->readListBegin(_etype4, _size1);
            this->error_message_args.resize(_size1);
            uint32_t _i5;
            for (_i5 = 0; _i5 < _size1; ++_i5)
            {
              xfer += iprot->readString(this->error_message_args[_i5]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.error_message_args = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ReplyStatus::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ReplyStatus");

  xfer += oprot->writeFieldBegin("error_code", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->error_code);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("error_message_format", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->error_message_format);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("error_message_args", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->error_message_args.size()));
    std::vector<std::string> ::const_iterator _iter6;
    for (_iter6 = this->error_message_args.begin(); _iter6 != this->error_message_args.end(); ++_iter6)
    {
      xfer += oprot->writeString((*_iter6));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ReplyStatus &a, ReplyStatus &b) {
  using ::std::swap;
  swap(a.error_code, b.error_code);
  swap(a.error_message_format, b.error_message_format);
  swap(a.error_message_args, b.error_message_args);
  swap(a.__isset, b.__isset);
}

const char* DefaultReply::ascii_fingerprint = "F6AD8D6EE2A97B9BAFE986CBB5A33301";
const uint8_t DefaultReply::binary_fingerprint[16] = {0xF6,0xAD,0x8D,0x6E,0xE2,0xA9,0x7B,0x9B,0xAF,0xE9,0x86,0xCB,0xB5,0xA3,0x33,0x01};

uint32_t DefaultReply::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->status.read(iprot);
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t DefaultReply::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("DefaultReply");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->status.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DefaultReply &a, DefaultReply &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.__isset, b.__isset);
}

const char* LeaderboardConnectReply::ascii_fingerprint = "0FD6FDFD2C321318E1F5A2261708F3A5";
const uint8_t LeaderboardConnectReply::binary_fingerprint[16] = {0x0F,0xD6,0xFD,0xFD,0x2C,0x32,0x13,0x18,0xE1,0xF5,0xA2,0x26,0x17,0x08,0xF3,0xA5};

uint32_t LeaderboardConnectReply::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->status.read(iprot);
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->session_handle);
          this->__isset.session_handle = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->user_data);
          this->__isset.user_data = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t LeaderboardConnectReply::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("LeaderboardConnectReply");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->status.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("session_handle", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->session_handle);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("user_data", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeBinary(this->user_data);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(LeaderboardConnectReply &a, LeaderboardConnectReply &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.session_handle, b.session_handle);
  swap(a.user_data, b.user_data);
  swap(a.__isset, b.__isset);
}

const char* Session::ascii_fingerprint = "E86CACEB22240450EDCBEFC3A83970E4";
const uint8_t Session::binary_fingerprint[16] = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

uint32_t Session::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->session_handle);
          this->__isset.session_handle = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Session::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Session");

  xfer += oprot->writeFieldBegin("session_handle", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->session_handle);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Session &a, Session &b) {
  using ::std::swap;
  swap(a.session_handle, b.session_handle);
  swap(a.__isset, b.__isset);
}

const char* Score::ascii_fingerprint = "E447ECEACF2D326FE000A75298B11CD9";
const uint8_t Score::binary_fingerprint[16] = {0xE4,0x47,0xEC,0xEA,0xCF,0x2D,0x32,0x6F,0xE0,0x00,0xA7,0x52,0x98,0xB1,0x1C,0xD9};

uint32_t Score::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->value);
          this->__isset.value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->date_epoch);
          this->__isset.date_epoch = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->player_alias);
          this->__isset.player_alias = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->player_id);
          this->__isset.player_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->rank);
          this->__isset.rank = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->situation);
          this->__isset.situation = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->vote_up_count);
          this->__isset.vote_up_count = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->vote_down_count);
          this->__isset.vote_down_count = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Score::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Score");

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("date_epoch", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->date_epoch);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("player_alias", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->player_alias);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("player_id", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->player_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("rank", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->rank);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("situation", ::apache::thrift::protocol::T_STRING, 6);
  xfer += oprot->writeBinary(this->situation);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("vote_up_count", ::apache::thrift::protocol::T_I32, 7);
  xfer += oprot->writeI32(this->vote_up_count);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("vote_down_count", ::apache::thrift::protocol::T_I32, 8);
  xfer += oprot->writeI32(this->vote_down_count);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Score &a, Score &b) {
  using ::std::swap;
  swap(a.value, b.value);
  swap(a.date_epoch, b.date_epoch);
  swap(a.player_alias, b.player_alias);
  swap(a.player_id, b.player_id);
  swap(a.rank, b.rank);
  swap(a.situation, b.situation);
  swap(a.vote_up_count, b.vote_up_count);
  swap(a.vote_down_count, b.vote_down_count);
  swap(a.__isset, b.__isset);
}

const char* GetScoresReply::ascii_fingerprint = "BDF6090738155A1FB5C43743D545B40E";
const uint8_t GetScoresReply::binary_fingerprint[16] = {0xBD,0xF6,0x09,0x07,0x38,0x15,0x5A,0x1F,0xB5,0xC4,0x37,0x43,0xD5,0x45,0xB4,0x0E};

uint32_t GetScoresReply::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->player_score.read(iprot);
          this->__isset.player_score = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->from_rank);
          this->__isset.from_rank = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->count);
          this->__isset.count = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->top_scores.clear();
            uint32_t _size7;
            ::apache::thrift::protocol::TType _etype10;
            xfer += iprot->readListBegin(_etype10, _size7);
            this->top_scores.resize(_size7);
            uint32_t _i11;
            for (_i11 = 0; _i11 < _size7; ++_i11)
            {
              xfer += this->top_scores[_i11].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.top_scores = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t GetScoresReply::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("GetScoresReply");

  xfer += oprot->writeFieldBegin("player_score", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->player_score.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("from_rank", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->from_rank);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("count", ::apache::thrift::protocol::T_I64, 3);
  xfer += oprot->writeI64(this->count);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("top_scores", ::apache::thrift::protocol::T_LIST, 4);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->top_scores.size()));
    std::vector<Score> ::const_iterator _iter12;
    for (_iter12 = this->top_scores.begin(); _iter12 != this->top_scores.end(); ++_iter12)
    {
      xfer += (*_iter12).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(GetScoresReply &a, GetScoresReply &b) {
  using ::std::swap;
  swap(a.player_score, b.player_score);
  swap(a.from_rank, b.from_rank);
  swap(a.count, b.count);
  swap(a.top_scores, b.top_scores);
  swap(a.__isset, b.__isset);
}

const char* PostScoreReply::ascii_fingerprint = "ED20B4254D63522D4314AC4E224FABE5";
const uint8_t PostScoreReply::binary_fingerprint[16] = {0xED,0x20,0xB4,0x25,0x4D,0x63,0x52,0x2D,0x43,0x14,0xAC,0x4E,0x22,0x4F,0xAB,0xE5};

uint32_t PostScoreReply::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->status.read(iprot);
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->scores.read(iprot);
          this->__isset.scores = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t PostScoreReply::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("PostScoreReply");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->status.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("scores", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->scores.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(PostScoreReply &a, PostScoreReply &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.scores, b.scores);
  swap(a.__isset, b.__isset);
}

const char* ConnectReply::ascii_fingerprint = "00B492A3875749A3E33B6E05569ACED5";
const uint8_t ConnectReply::binary_fingerprint[16] = {0x00,0xB4,0x92,0xA3,0x87,0x57,0x49,0xA3,0xE3,0x3B,0x6E,0x05,0x56,0x9A,0xCE,0xD5};

uint32_t ConnectReply::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->status.read(iprot);
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->server_name);
          this->__isset.server_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->session_handle);
          this->__isset.session_handle = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->user_data);
          this->__isset.user_data = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ConnectReply::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ConnectReply");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->status.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("server_name", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->server_name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("session_handle", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->session_handle);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("user_data", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeBinary(this->user_data);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ConnectReply &a, ConnectReply &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.server_name, b.server_name);
  swap(a.session_handle, b.session_handle);
  swap(a.user_data, b.user_data);
  swap(a.__isset, b.__isset);
}

}} // namespace