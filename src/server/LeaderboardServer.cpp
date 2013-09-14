// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#define MINIMUM_PROTOCOL_VERSION (1)

#include "LeaderboardService.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::concurrency;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

#include <stdlib.h> // for exit
#include <nanolat/client/common.h>
#include <nldb/nldb.h>
#include "ServerImpl.h"

using boost::shared_ptr;

using namespace  ::nanolat::leaderboard;
using namespace  ::nanolat::db;


namespace nanolat {
namespace leaderboard {

/*! The singleton that keeps all instances.
 */
global_singleton_t server_instance;


class LeaderboardServiceHandler : virtual public LeaderboardServiceIf {
 public:
  LeaderboardServiceHandler() {
		server_error_t rc = server_instance.initialize_instance();
		if (rc) {
			std::string message = get_error_message_format(rc);
			printf("Fatal error while initializing the server instance.\n");
			printf("Error : %s\n", message.c_str() );
			exit(-1);
		}
  }

  void connect(ConnectReply& _return, const int32_t protocol_version, const std::string& player_id, const std::string& player_password, const std::string& user_data) {
		TRACE("connect\n");

		session_context_t * sess_ctx = NULL;
		open_database_t * db = NULL;

		// TODO : Check the protocol version
		if ( protocol_version < MINIMUM_PROTOCOL_VERSION ) {
			_return.status.error_code = ErrorCode::NL_INCOMPATIBLE_CLINET_VERSION;
			goto on_error;
		}

		// Create a new session context.
		sess_ctx = server_instance.session_map.new_session();
		NL_ASSERT(sess_ctx);

		// Use the default database.
		db = server_instance.database_mgr.get_database("default");
		if (db == NULL) {
			server_error_t rc = server_instance.database_mgr.open_database("default", &db);
			if (rc) {
				_return.status.error_code = (ErrorCode::type) rc;
				goto on_error;
			}
		}
		sess_ctx->set_using_database(db);

		_return.session_handle = sess_ctx->get_session_handle();
		_return.status.error_code = ErrorCode::NL_SUCCESS;
		return;
on_error:
		if (sess_ctx)
		{
			server_instance.session_map.delete_session(sess_ctx);
		}
  }

  void disconnect(DefaultReply& _return, const Session& session) {
	TRACE("disconnect\n");
	GET_SESSION_CONTEXT(sess_ctx, session);

	// Remove from the session context map, delete it.
	server_instance.session_map.delete_session( sess_ctx );

	_return.status.error_code = ErrorCode::NL_SUCCESS;
  }

  void post_score(PostScoreReply& _return, const Session& session, const std::string& category, const Score& score) {
	  TRACE("post_score\n");
  }

  void get_scores(GetScoresReply& _return, const Session& session, const std::string& categoy, const std::string& player_id, const int32_t from_rank, const int64_t count) {
	  TRACE("get_scores\n");
	  _return.status.error_code = ErrorCode::NL_NOT_SUPPORTED;
  }

  void vote_score(DefaultReply& _return, const Session& session, const std::string& voting_player_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment) {
	  TRACE("vote_score\n");
	  _return.status.error_code = ErrorCode::NL_NOT_SUPPORTED;
  }
};

int listen(int port) {
	shared_ptr<LeaderboardServiceHandler> handler(new LeaderboardServiceHandler());
    shared_ptr<TProcessor> processor(new nanolat::leaderboard::LeaderboardServiceProcessor(handler));
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    // using thread pool with maximum 1 thread to handle incoming requests
    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(1);
    shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    TNonblockingServer server(processor, protocolFactory, port, threadManager);
    server.serve();

    return 0;
}

} // leaderboard

} // nanolat