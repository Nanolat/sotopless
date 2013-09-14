/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef LeaderboardService_H
#define LeaderboardService_H

#include <thrift/TDispatchProcessor.h>
#include "LeaderboardService_types.h"

namespace nanolat { namespace leaderboard {

class LeaderboardServiceIf {
 public:
  virtual ~LeaderboardServiceIf() {}
  virtual void connect(ConnectReply& _return, const int32_t protocol_version, const std::string& player_id, const std::string& player_password, const std::string& user_data) = 0;
  virtual void disconnect(DefaultReply& _return, const Session& session) = 0;
  virtual void post_score(PostScoreReply& _return, const Session& session, const std::string& category, const Score& score) = 0;
  virtual void get_scores(GetScoresReply& _return, const Session& session, const std::string& categoy, const std::string& player_id, const int32_t from_rank, const int64_t count) = 0;
  virtual void vote_score(DefaultReply& _return, const Session& session, const std::string& voting_player_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment) = 0;
};

class LeaderboardServiceIfFactory {
 public:
  typedef LeaderboardServiceIf Handler;

  virtual ~LeaderboardServiceIfFactory() {}

  virtual LeaderboardServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(LeaderboardServiceIf* /* handler */) = 0;
};

class LeaderboardServiceIfSingletonFactory : virtual public LeaderboardServiceIfFactory {
 public:
  LeaderboardServiceIfSingletonFactory(const boost::shared_ptr<LeaderboardServiceIf>& iface) : iface_(iface) {}
  virtual ~LeaderboardServiceIfSingletonFactory() {}

  virtual LeaderboardServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(LeaderboardServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<LeaderboardServiceIf> iface_;
};

class LeaderboardServiceNull : virtual public LeaderboardServiceIf {
 public:
  virtual ~LeaderboardServiceNull() {}
  void connect(ConnectReply& /* _return */, const int32_t /* protocol_version */, const std::string& /* player_id */, const std::string& /* player_password */, const std::string& /* user_data */) {
    return;
  }
  void disconnect(DefaultReply& /* _return */, const Session& /* session */) {
    return;
  }
  void post_score(PostScoreReply& /* _return */, const Session& /* session */, const std::string& /* category */, const Score& /* score */) {
    return;
  }
  void get_scores(GetScoresReply& /* _return */, const Session& /* session */, const std::string& /* categoy */, const std::string& /* player_id */, const int32_t /* from_rank */, const int64_t /* count */) {
    return;
  }
  void vote_score(DefaultReply& /* _return */, const Session& /* session */, const std::string& /* voting_player_id */, const int64_t /* score_value */, const int64_t /* score_date_epoch */, const int32_t /* vote_up_down */, const std::string& /* comment */) {
    return;
  }
};

typedef struct _LeaderboardService_connect_args__isset {
  _LeaderboardService_connect_args__isset() : protocol_version(false), player_id(false), player_password(false), user_data(false) {}
  bool protocol_version;
  bool player_id;
  bool player_password;
  bool user_data;
} _LeaderboardService_connect_args__isset;

class LeaderboardService_connect_args {
 public:

  LeaderboardService_connect_args() : protocol_version(0), player_id(), player_password(), user_data() {
  }

  virtual ~LeaderboardService_connect_args() throw() {}

  int32_t protocol_version;
  std::string player_id;
  std::string player_password;
  std::string user_data;

  _LeaderboardService_connect_args__isset __isset;

  void __set_protocol_version(const int32_t val) {
    protocol_version = val;
  }

  void __set_player_id(const std::string& val) {
    player_id = val;
  }

  void __set_player_password(const std::string& val) {
    player_password = val;
  }

  void __set_user_data(const std::string& val) {
    user_data = val;
  }

  bool operator == (const LeaderboardService_connect_args & rhs) const
  {
    if (!(protocol_version == rhs.protocol_version))
      return false;
    if (!(player_id == rhs.player_id))
      return false;
    if (!(player_password == rhs.player_password))
      return false;
    if (!(user_data == rhs.user_data))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_connect_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_connect_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LeaderboardService_connect_pargs {
 public:


  virtual ~LeaderboardService_connect_pargs() throw() {}

  const int32_t* protocol_version;
  const std::string* player_id;
  const std::string* player_password;
  const std::string* user_data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_connect_result__isset {
  _LeaderboardService_connect_result__isset() : success(false) {}
  bool success;
} _LeaderboardService_connect_result__isset;

class LeaderboardService_connect_result {
 public:

  LeaderboardService_connect_result() {
  }

  virtual ~LeaderboardService_connect_result() throw() {}

  ConnectReply success;

  _LeaderboardService_connect_result__isset __isset;

  void __set_success(const ConnectReply& val) {
    success = val;
  }

  bool operator == (const LeaderboardService_connect_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_connect_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_connect_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_connect_presult__isset {
  _LeaderboardService_connect_presult__isset() : success(false) {}
  bool success;
} _LeaderboardService_connect_presult__isset;

class LeaderboardService_connect_presult {
 public:


  virtual ~LeaderboardService_connect_presult() throw() {}

  ConnectReply* success;

  _LeaderboardService_connect_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LeaderboardService_disconnect_args__isset {
  _LeaderboardService_disconnect_args__isset() : session(false) {}
  bool session;
} _LeaderboardService_disconnect_args__isset;

class LeaderboardService_disconnect_args {
 public:

  LeaderboardService_disconnect_args() {
  }

  virtual ~LeaderboardService_disconnect_args() throw() {}

  Session session;

  _LeaderboardService_disconnect_args__isset __isset;

  void __set_session(const Session& val) {
    session = val;
  }

  bool operator == (const LeaderboardService_disconnect_args & rhs) const
  {
    if (!(session == rhs.session))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_disconnect_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_disconnect_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LeaderboardService_disconnect_pargs {
 public:


  virtual ~LeaderboardService_disconnect_pargs() throw() {}

  const Session* session;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_disconnect_result__isset {
  _LeaderboardService_disconnect_result__isset() : success(false) {}
  bool success;
} _LeaderboardService_disconnect_result__isset;

class LeaderboardService_disconnect_result {
 public:

  LeaderboardService_disconnect_result() {
  }

  virtual ~LeaderboardService_disconnect_result() throw() {}

  DefaultReply success;

  _LeaderboardService_disconnect_result__isset __isset;

  void __set_success(const DefaultReply& val) {
    success = val;
  }

  bool operator == (const LeaderboardService_disconnect_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_disconnect_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_disconnect_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_disconnect_presult__isset {
  _LeaderboardService_disconnect_presult__isset() : success(false) {}
  bool success;
} _LeaderboardService_disconnect_presult__isset;

class LeaderboardService_disconnect_presult {
 public:


  virtual ~LeaderboardService_disconnect_presult() throw() {}

  DefaultReply* success;

  _LeaderboardService_disconnect_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LeaderboardService_post_score_args__isset {
  _LeaderboardService_post_score_args__isset() : session(false), category(false), score(false) {}
  bool session;
  bool category;
  bool score;
} _LeaderboardService_post_score_args__isset;

class LeaderboardService_post_score_args {
 public:

  LeaderboardService_post_score_args() : category() {
  }

  virtual ~LeaderboardService_post_score_args() throw() {}

  Session session;
  std::string category;
  Score score;

  _LeaderboardService_post_score_args__isset __isset;

  void __set_session(const Session& val) {
    session = val;
  }

  void __set_category(const std::string& val) {
    category = val;
  }

  void __set_score(const Score& val) {
    score = val;
  }

  bool operator == (const LeaderboardService_post_score_args & rhs) const
  {
    if (!(session == rhs.session))
      return false;
    if (!(category == rhs.category))
      return false;
    if (!(score == rhs.score))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_post_score_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_post_score_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LeaderboardService_post_score_pargs {
 public:


  virtual ~LeaderboardService_post_score_pargs() throw() {}

  const Session* session;
  const std::string* category;
  const Score* score;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_post_score_result__isset {
  _LeaderboardService_post_score_result__isset() : success(false) {}
  bool success;
} _LeaderboardService_post_score_result__isset;

class LeaderboardService_post_score_result {
 public:

  LeaderboardService_post_score_result() {
  }

  virtual ~LeaderboardService_post_score_result() throw() {}

  PostScoreReply success;

  _LeaderboardService_post_score_result__isset __isset;

  void __set_success(const PostScoreReply& val) {
    success = val;
  }

  bool operator == (const LeaderboardService_post_score_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_post_score_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_post_score_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_post_score_presult__isset {
  _LeaderboardService_post_score_presult__isset() : success(false) {}
  bool success;
} _LeaderboardService_post_score_presult__isset;

class LeaderboardService_post_score_presult {
 public:


  virtual ~LeaderboardService_post_score_presult() throw() {}

  PostScoreReply* success;

  _LeaderboardService_post_score_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LeaderboardService_get_scores_args__isset {
  _LeaderboardService_get_scores_args__isset() : session(false), categoy(false), player_id(false), from_rank(false), count(false) {}
  bool session;
  bool categoy;
  bool player_id;
  bool from_rank;
  bool count;
} _LeaderboardService_get_scores_args__isset;

class LeaderboardService_get_scores_args {
 public:

  LeaderboardService_get_scores_args() : categoy(), player_id(), from_rank(0), count(0) {
  }

  virtual ~LeaderboardService_get_scores_args() throw() {}

  Session session;
  std::string categoy;
  std::string player_id;
  int32_t from_rank;
  int64_t count;

  _LeaderboardService_get_scores_args__isset __isset;

  void __set_session(const Session& val) {
    session = val;
  }

  void __set_categoy(const std::string& val) {
    categoy = val;
  }

  void __set_player_id(const std::string& val) {
    player_id = val;
  }

  void __set_from_rank(const int32_t val) {
    from_rank = val;
  }

  void __set_count(const int64_t val) {
    count = val;
  }

  bool operator == (const LeaderboardService_get_scores_args & rhs) const
  {
    if (!(session == rhs.session))
      return false;
    if (!(categoy == rhs.categoy))
      return false;
    if (!(player_id == rhs.player_id))
      return false;
    if (!(from_rank == rhs.from_rank))
      return false;
    if (!(count == rhs.count))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_get_scores_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_get_scores_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LeaderboardService_get_scores_pargs {
 public:


  virtual ~LeaderboardService_get_scores_pargs() throw() {}

  const Session* session;
  const std::string* categoy;
  const std::string* player_id;
  const int32_t* from_rank;
  const int64_t* count;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_get_scores_result__isset {
  _LeaderboardService_get_scores_result__isset() : success(false) {}
  bool success;
} _LeaderboardService_get_scores_result__isset;

class LeaderboardService_get_scores_result {
 public:

  LeaderboardService_get_scores_result() {
  }

  virtual ~LeaderboardService_get_scores_result() throw() {}

  GetScoresReply success;

  _LeaderboardService_get_scores_result__isset __isset;

  void __set_success(const GetScoresReply& val) {
    success = val;
  }

  bool operator == (const LeaderboardService_get_scores_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_get_scores_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_get_scores_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_get_scores_presult__isset {
  _LeaderboardService_get_scores_presult__isset() : success(false) {}
  bool success;
} _LeaderboardService_get_scores_presult__isset;

class LeaderboardService_get_scores_presult {
 public:


  virtual ~LeaderboardService_get_scores_presult() throw() {}

  GetScoresReply* success;

  _LeaderboardService_get_scores_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _LeaderboardService_vote_score_args__isset {
  _LeaderboardService_vote_score_args__isset() : session(false), voting_player_id(false), score_value(false), score_date_epoch(false), vote_up_down(false), comment(false) {}
  bool session;
  bool voting_player_id;
  bool score_value;
  bool score_date_epoch;
  bool vote_up_down;
  bool comment;
} _LeaderboardService_vote_score_args__isset;

class LeaderboardService_vote_score_args {
 public:

  LeaderboardService_vote_score_args() : voting_player_id(), score_value(0), score_date_epoch(0), vote_up_down(0), comment() {
  }

  virtual ~LeaderboardService_vote_score_args() throw() {}

  Session session;
  std::string voting_player_id;
  int64_t score_value;
  int64_t score_date_epoch;
  int32_t vote_up_down;
  std::string comment;

  _LeaderboardService_vote_score_args__isset __isset;

  void __set_session(const Session& val) {
    session = val;
  }

  void __set_voting_player_id(const std::string& val) {
    voting_player_id = val;
  }

  void __set_score_value(const int64_t val) {
    score_value = val;
  }

  void __set_score_date_epoch(const int64_t val) {
    score_date_epoch = val;
  }

  void __set_vote_up_down(const int32_t val) {
    vote_up_down = val;
  }

  void __set_comment(const std::string& val) {
    comment = val;
  }

  bool operator == (const LeaderboardService_vote_score_args & rhs) const
  {
    if (!(session == rhs.session))
      return false;
    if (!(voting_player_id == rhs.voting_player_id))
      return false;
    if (!(score_value == rhs.score_value))
      return false;
    if (!(score_date_epoch == rhs.score_date_epoch))
      return false;
    if (!(vote_up_down == rhs.vote_up_down))
      return false;
    if (!(comment == rhs.comment))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_vote_score_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_vote_score_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class LeaderboardService_vote_score_pargs {
 public:


  virtual ~LeaderboardService_vote_score_pargs() throw() {}

  const Session* session;
  const std::string* voting_player_id;
  const int64_t* score_value;
  const int64_t* score_date_epoch;
  const int32_t* vote_up_down;
  const std::string* comment;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_vote_score_result__isset {
  _LeaderboardService_vote_score_result__isset() : success(false) {}
  bool success;
} _LeaderboardService_vote_score_result__isset;

class LeaderboardService_vote_score_result {
 public:

  LeaderboardService_vote_score_result() {
  }

  virtual ~LeaderboardService_vote_score_result() throw() {}

  DefaultReply success;

  _LeaderboardService_vote_score_result__isset __isset;

  void __set_success(const DefaultReply& val) {
    success = val;
  }

  bool operator == (const LeaderboardService_vote_score_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const LeaderboardService_vote_score_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LeaderboardService_vote_score_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _LeaderboardService_vote_score_presult__isset {
  _LeaderboardService_vote_score_presult__isset() : success(false) {}
  bool success;
} _LeaderboardService_vote_score_presult__isset;

class LeaderboardService_vote_score_presult {
 public:


  virtual ~LeaderboardService_vote_score_presult() throw() {}

  DefaultReply* success;

  _LeaderboardService_vote_score_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class LeaderboardServiceClient : virtual public LeaderboardServiceIf {
 public:
  LeaderboardServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  LeaderboardServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void connect(ConnectReply& _return, const int32_t protocol_version, const std::string& player_id, const std::string& player_password, const std::string& user_data);
  void send_connect(const int32_t protocol_version, const std::string& player_id, const std::string& player_password, const std::string& user_data);
  void recv_connect(ConnectReply& _return);
  void disconnect(DefaultReply& _return, const Session& session);
  void send_disconnect(const Session& session);
  void recv_disconnect(DefaultReply& _return);
  void post_score(PostScoreReply& _return, const Session& session, const std::string& category, const Score& score);
  void send_post_score(const Session& session, const std::string& category, const Score& score);
  void recv_post_score(PostScoreReply& _return);
  void get_scores(GetScoresReply& _return, const Session& session, const std::string& categoy, const std::string& player_id, const int32_t from_rank, const int64_t count);
  void send_get_scores(const Session& session, const std::string& categoy, const std::string& player_id, const int32_t from_rank, const int64_t count);
  void recv_get_scores(GetScoresReply& _return);
  void vote_score(DefaultReply& _return, const Session& session, const std::string& voting_player_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment);
  void send_vote_score(const Session& session, const std::string& voting_player_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment);
  void recv_vote_score(DefaultReply& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class LeaderboardServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<LeaderboardServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (LeaderboardServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_connect(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_disconnect(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_post_score(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_scores(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_vote_score(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  LeaderboardServiceProcessor(boost::shared_ptr<LeaderboardServiceIf> iface) :
    iface_(iface) {
    processMap_["connect"] = &LeaderboardServiceProcessor::process_connect;
    processMap_["disconnect"] = &LeaderboardServiceProcessor::process_disconnect;
    processMap_["post_score"] = &LeaderboardServiceProcessor::process_post_score;
    processMap_["get_scores"] = &LeaderboardServiceProcessor::process_get_scores;
    processMap_["vote_score"] = &LeaderboardServiceProcessor::process_vote_score;
  }

  virtual ~LeaderboardServiceProcessor() {}
};

class LeaderboardServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  LeaderboardServiceProcessorFactory(const ::boost::shared_ptr< LeaderboardServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< LeaderboardServiceIfFactory > handlerFactory_;
};

class LeaderboardServiceMultiface : virtual public LeaderboardServiceIf {
 public:
  LeaderboardServiceMultiface(std::vector<boost::shared_ptr<LeaderboardServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~LeaderboardServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<LeaderboardServiceIf> > ifaces_;
  LeaderboardServiceMultiface() {}
  void add(boost::shared_ptr<LeaderboardServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void connect(ConnectReply& _return, const int32_t protocol_version, const std::string& player_id, const std::string& player_password, const std::string& user_data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->connect(_return, protocol_version, player_id, player_password, user_data);
    }
    ifaces_[i]->connect(_return, protocol_version, player_id, player_password, user_data);
    return;
  }

  void disconnect(DefaultReply& _return, const Session& session) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->disconnect(_return, session);
    }
    ifaces_[i]->disconnect(_return, session);
    return;
  }

  void post_score(PostScoreReply& _return, const Session& session, const std::string& category, const Score& score) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->post_score(_return, session, category, score);
    }
    ifaces_[i]->post_score(_return, session, category, score);
    return;
  }

  void get_scores(GetScoresReply& _return, const Session& session, const std::string& categoy, const std::string& player_id, const int32_t from_rank, const int64_t count) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_scores(_return, session, categoy, player_id, from_rank, count);
    }
    ifaces_[i]->get_scores(_return, session, categoy, player_id, from_rank, count);
    return;
  }

  void vote_score(DefaultReply& _return, const Session& session, const std::string& voting_player_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->vote_score(_return, session, voting_player_id, score_value, score_date_epoch, vote_up_down, comment);
    }
    ifaces_[i]->vote_score(_return, session, voting_player_id, score_value, score_date_epoch, vote_up_down, comment);
    return;
  }

};

}} // namespace

#endif
