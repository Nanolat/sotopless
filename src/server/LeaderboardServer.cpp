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

/* for test code */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <stdlib.h> // for exit
#include <sstream>
#include <string>
#include <nanolat/client/common.h>
#include <private/stacktrace.h>
#include <private/util.h>
#include <nldb/nldb.h>

#include "LeaderboardImpl.h"
#include "DatabaseManager.h"

using boost::shared_ptr;

using namespace  ::nanolat::leaderboard;
using namespace  ::nanolat::db;



namespace nanolat {
namespace leaderboard {

/*! The singleton that keeps all instances.
 */
global_singleton_t server_instance;



std::string error_message(const std::string & error_message, server_error_t rc) {

	std::stringstream stream;

	stream << "Error Message : " << error_message ;
	if (rc) {
		stream << "Error Code : " << rc ;
	}

	return stream.str();
}

std::string error_message(const std::string & message) {
	return error_message(message, (server_error_t)0);
}

server_error_t get_table(session_context_t * sess_ctx, nldb_tx_t tx, const std::string & table_name, nldb_table_t * o_table) {
	NL_ASSERT(o_table);

	open_database_t * db = sess_ctx->get_using_database();
	NL_ASSERT(db);

	nldb_table_t table = db->get_table(sess_ctx->get_tenant_id(), table_name);
	if (!table) {
	    server_error_t rc = db->open_table(tx, sess_ctx->get_tenant_id(), table_name, &table);
	    if (rc == nanolat::thrift::ErrorCode::NL_TABLE_DOES_NOT_EXIST) {
	    	rc = db->create_table(tx, sess_ctx->get_tenant_id(), table_name);
	    	if (rc) NL_RETURN(rc)

			rc = db->open_table(tx, sess_ctx->get_tenant_id(), table_name, &table);
	    	if (rc) NL_RETURN(rc)

	    }
	    if (rc) NL_RETURN(rc)
	}
	*o_table = table;
	NL_RETURN(0)
}

#define CHECK_ARGUMENT(cond, msg, branch_stmt) {                     \
	if ( !(cond) ) {                                                 \
		_return.status.error_code = ErrorCode::NL_INVALID_ARGUMENT;  \
		_return.status.error_message_format = error_message(msg);    \
		branch_stmt;                                                 \
	}                                                                \
}



class LeaderboardServiceHandler : virtual public LeaderboardServiceIf {
public:
	LeaderboardServiceHandler() {
		server_error_t rc = server_instance.initialize_instance();
		if (rc) {
			printf("Fatal Error while initializing server instance. Error code : %d\n", rc);
			exit(-1);
		}
	}



	std::string ToString(int value) {
		std::stringstream stream;
		stream << value;
		return stream.str();
	}

	void connect(ConnectReply& _return, const int32_t protocol_version, const std::string& tenant_id, const std::string& user_id, const std::string& user_password, const std::string& user_data) {
		TRACE("connect\n");

		session_context_t * sess_ctx = NULL;
		open_database_t * db = NULL;
		bool user_exists = false;

		try {
			// Check the protocol version
			if ( protocol_version < MINIMUM_PROTOCOL_VERSION ) {
				_return.status.error_code = ErrorCode::NL_INCOMPATIBLE_CLINET_VERSION;
				_return.status.error_message_format = error_message("Incompatible Client Version.");
				goto on_error;
			}

			CHECK_ARGUMENT(tenant_id != "", "Tenant ID should not be an empty string.", goto on_error);
			CHECK_ARGUMENT(user_id != "", "User ID should not be an empty string.", goto on_error);
			CHECK_ARGUMENT(user_password != "", "User Password should not be an empty string.", goto on_error);

			// Create a new session context.
			sess_ctx = server_instance.session_map.new_session( tenant_id );
			NL_ASSERT(sess_ctx);

			// Use the default database.
			db = server_instance.database_mgr.get_database("default");
			if (db == NULL) {
				server_error_t rc = server_instance.database_mgr.open_database("default", &db);
				if (rc) {
					_return.status.error_code = ErrorCode::NL_FAILURE;
					_return.status.error_message_format = error_message("Database Open Error", rc);
					goto on_error;
				}
			}
			sess_ctx->set_using_database(db);

			if ( authenticate_user(sess_ctx, user_id, user_password, &user_exists) != 0 ) {
				if (user_exists) { // User exists, but authentication failed.
					_return.status.error_code = ErrorCode::NL_INVALID_CREDENTIAL;
					goto on_error;
				} else {
					server_error_t rc = register_user(sess_ctx, user_id, user_password, user_data);
					if (rc) {
						_return.status.error_code = ErrorCode::NL_FAILURE;
						_return.status.error_message_format = error_message("User Register Failure", rc);
						goto on_error;
					}
				}
			}
		} catch (apache::thrift::TException * e) {
			_return.status.error_code = ErrorCode::NL_FAILURE;
			_return.status.error_message_format = error_message(e->what());
			goto on_error;
		}

		// TODO : Read user data such as user profile and picture from user db, and set it here.
		_return.user_data = "";
		// TODO : Read the server name from config file and set it here.
		_return.server_name = "Kangdori";
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

	inline std::string by_score_table_name(const std::string & category) {
		return category + "_bs";
	}

	inline std::string by_user_table_name(const std::string & category) {
		return category + "_bu";
	}

	void post_score(PostScoreReply& _return, const Session& session, const std::string& category, const Score& posting_score) {
		TRACE("post_score\n");
		server_error_t rc;
		nldb_table_t by_score_table = NULL;
		nldb_table_t by_user_table = NULL;

		GET_SESSION_CONTEXT(sess_ctx, session);

		sess_ctx->auto_begin_transaction();

		CHECK_ARGUMENT(category != "", "Category should not be an empty string.", goto on_error);
		CHECK_ARGUMENT(posting_score.value >= 0, "Score value should be greater than or equal to 0.", goto on_error);
		CHECK_ARGUMENT(posting_score.user_alias != "", "User Alias in Score object should not be an empty string.", goto on_error);
		CHECK_ARGUMENT(posting_score.user_id != "", "User ID in Score object should not be an empty string.", goto on_error);
		CHECK_ARGUMENT(posting_score.rank == 0, "Rank in Score object should be 0 to post the score.", goto on_error);

		rc = get_table(sess_ctx, sess_ctx->get_transaction(), by_score_table_name(category), &by_score_table);
		if (rc) {
			_return.status.error_code = ErrorCode::NL_FAILURE;
			_return.status.error_message_format = error_message("By-Score Table Open Failure", rc);
			goto on_error;
		}

		rc = get_table(sess_ctx, sess_ctx->get_transaction(), by_user_table_name(category), &by_user_table);
		if (rc) {
			_return.status.error_code = ErrorCode::NL_FAILURE;
			_return.status.error_message_format = error_message("By-User Table Open Failure", rc);
			goto on_error;
		}


		try {
			rc = post_user_score(sess_ctx, by_score_table, by_user_table, posting_score);
			if (rc) {
				_return.status.error_code = ErrorCode::NL_FAILURE;
				_return.status.error_message_format = error_message("User Score Posting Failure", rc);
				goto on_error;
			}

			rc = get_user_score(sess_ctx, by_score_table, by_user_table, posting_score.user_id, & _return.scores.user_score );
			if (rc) {
				_return.status.error_code = ErrorCode::NL_FAILURE;
				_return.status.error_message_format = error_message("User Score Retrieval Failure", rc);
				goto on_error;
			}

			// Get top 10 scores by default.
			_return.scores.from_rank = 1;
			_return.scores.count = 10;

			rc = get_scores_by_ranking(sess_ctx, by_score_table, _return.scores.from_rank, _return.scores.count, & _return.scores.top_scores);
			if (rc) {
				_return.status.error_code = ErrorCode::NL_FAILURE;
				_return.status.error_message_format = error_message("Top 10 Score Retrieval Failure", rc);
				goto on_error;
			}
		} catch (apache::thrift::TException * e) {
			_return.status.error_code = ErrorCode::NL_FAILURE;
			_return.status.error_message_format = error_message(e->what());
			goto on_error;
		}

		sess_ctx->auto_commit_transaction();

		_return.status.error_code = ErrorCode::NL_SUCCESS;

		return;
on_error:
		sess_ctx->auto_abort_transaction();
	}

	void get_scores(GetScoresReply& _return, const Session& session, const std::string& category, const std::string& user_id, const int32_t from_rank, const int64_t count) {
		TRACE("get_scores\n");
		_return.status.error_code = ErrorCode::NL_NOT_SUPPORTED;
	}

	void vote_score(DefaultReply& _return, const Session& session, const std::string& voting_user_id, const int64_t score_value, const int64_t score_date_epoch, const int32_t vote_up_down, const std::string& comment) {
		TRACE("vote_score\n");
		_return.status.error_code = ErrorCode::NL_NOT_SUPPORTED;
	}
};

void print_score(const char * summary, const Score & score) {
	printf("%s : value:%lld, date_epoch:%lld, rank:%d, user_alias:%s, user_id:%s, vote_down_count:%d, vote_up_count%d\n",
			summary, score.value, score.date_epoch, score.rank, score.user_alias.c_str(), score.user_id.c_str(), score.vote_down_count, score.vote_up_count);
}

void Test(LeaderboardServiceHandler * service_handler, int index)
{
	ConnectReply connectReply;

	std::string tenant_id = "test";

	std::string user_id = concat_int("uid", index);
	std::string user_password = concat_int("pwd", index);
	std::string user_data;

	service_handler->connect(connectReply, 1, tenant_id, user_id, user_password, user_data);

	PostScoreReply postScoreReply;
	Score score;
	score.value =  ( (unsigned int)(index * 561) ) % 1000;
	score.date_epoch = time(NULL);
	score.rank = 0;
	score.user_alias = concat_int("alias", index);
	score.user_id = concat_int("uid", index);
	score.vote_down_count = 0;
	score.vote_up_count = 0;

	Session session;
	session.session_handle = connectReply.session_handle;

	std::string category = "TestCategory";

	print_score("posting score", score );

	service_handler->post_score(postScoreReply, session, category, score);

	print_score("user score", postScoreReply.scores.user_score );

	for (std::vector<Score>::iterator it = postScoreReply.scores.top_scores.begin();
		 it != postScoreReply.scores.top_scores.end();
		 it++) {
		print_score("top score", *it);
	}

}

int listen(int port) {
	try {
		LeaderboardServiceHandler * service_handler = new LeaderboardServiceHandler();

		for (int i=0; i<1000; i++) {
			Test(service_handler, i);
		}

		shared_ptr<LeaderboardServiceHandler> handler(service_handler);
	    shared_ptr<TProcessor> processor(new nanolat::leaderboard::LeaderboardServiceProcessor(handler));
	    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	    // using thread pool with maximum 1 thread to handle incoming requests
	    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(1);
	    shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	    threadManager->threadFactory(threadFactory);
	    threadManager->start();
	    TNonblockingServer server(processor, protocolFactory, port, threadManager);
	    server.serve();

	} catch (TException * e) {
		printf("Error : %s\n", e->what() );
		return -1;
	}

    return 0;
}

} // leaderboard

} // nanolat
