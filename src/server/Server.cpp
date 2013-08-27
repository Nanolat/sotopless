#include "DatabaseService.h"
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

using boost::shared_ptr;

#include <stdlib.h> // for exit
#include <nanolat/client/common.h>
#include <nldb/nldb.h>
#include "ServerImpl.h"

using namespace ::nanolat::client;
using namespace  ::nanolat::thrift;

#if !defined(NDEBUG)
#  define TRACE printf
#else
#  define TRACE(...)
#endif


namespace nanolat {
namespace server {

#define GET_SESSION_CONTEXT(ctx, session) \
	session_context_t * ctx = server_instance.session_map.get_session(session.session_handle); \
    if (!ctx) {                                                                                \
		_return.status.error_code = ErrorCode::NL_INVALID_SESSION_HANDLE;                      \
        _return.status.error_message_format = "Invalid Session Handle";                        \
        return;                                                                                \
	}

#define GET_CURSOR_CONTEXT(session_ctx, cursor_ctx, cursor_handle) \
	cursor_context_t * cursor_ctx = session_ctx->get_cursor(cursor_handle); \
    if (!cursor_ctx) {                                                      \
		_return.status.error_code = ErrorCode::NL_INVALID_CURSOR_HANDLE;    \
        _return.status.error_message_format = "Invalid Cursor Handle";      \
        return;                                                             \
	}


#define GET_TABLE(tab, table_name)                                  \
		open_database_t * db = sess_ctx->get_using_database();      \
		NL_ASSERT(db);                                              \
		nldb_table_t tab = db->get_table(table_name);               \
		if (!tab) {                                                 \
	        ErrorCode::type rc = db->open_table(table_name, &tab);  \
	        if (rc != ErrorCode::NL_SUCCESS) {                      \
	        	_return.status.error_code = rc;                     \
	        	return;                                             \
	        }                                                       \
        }

// A macro that begins a transaction for auto commit for table put/get/del operations where nl_transaction_begin is not called at all.
// TODO : Optimization : keep the nldb_tx_t object into session_context_t so that we don't have to initialize it, destory it for each request.
#define AUTO_BEGIN_TRANSACTION(session_ctx) \
		nldb_tx_t auto_commit_tx; \
		if (session_ctx->get_transaction() == NULL) { \
			nldb_db_t db = session_ctx->get_using_database()->get_db() ; \
	        nldb_rc_t rc = nldb_tx_init( db, &auto_commit_tx);      \
	        if (rc != NLDB_OK) {                                    \
             	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
	        rc = nldb_tx_begin( auto_commit_tx );                   \
	        if (rc != NLDB_OK) {                                    \
	         	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
        }

#define AUTO_COMMIT_TRANSACTION(session_ctx) \
		if (session_ctx->get_transaction() == NULL) { \
	        nldb_rc_t rc = nldb_tx_commit( auto_commit_tx );        \
	        if (rc != NLDB_OK) {                                    \
             	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
	        rc = nldb_tx_destroy( auto_commit_tx );                 \
	        if (rc != NLDB_OK) {                                    \
             	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
        }

// TODO : What happens if the function returns after beginning a transaction, but before committing or aborting transaction?
// This function is called when an error happens. Abort the transaction in the session context as well.
#define ABORT_TRANSACTION(session_ctx) \
		if (session_ctx->get_transaction() == NULL) { \
	        nldb_rc_t rc = nldb_tx_abort( auto_commit_tx );        \
	        if (rc != NLDB_OK) {                                    \
             	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
	        rc = nldb_tx_destroy( auto_commit_tx );                 \
	        if (rc != NLDB_OK) {                                    \
             	_return.status.error_code = (ErrorCode::type) rc;   \
	            return;                                             \
            }                                                       \
        } else {                                                    \
        	nldb_tx_t session_tx = session_ctx->get_transaction();  \
			nldb_rc_t rc = nldb_tx_abort( session_tx );             \
			if (rc != NLDB_OK) {                                    \
				_return.status.error_code = (ErrorCode::type) rc;   \
				return;                                             \
			}                                                       \
        }


#define GET_TRANSACTION(tx, session_ctx) \
        nldb_tx_t tx = session_ctx->get_transaction() ? session_ctx->get_transaction() : auto_commit_tx ;

/*! The singleton that keeps all instances.
 */
global_singleton_t server_instance;


class DatabaseServiceHandler : virtual public DatabaseServiceIf {
private :

 public:
	DatabaseServiceHandler() {
		ErrorCode::type rc = server_instance.initialize_instance();
		if (rc != ErrorCode::NL_SUCCESS) {
			std::string message = get_error_message_format(rc);
			printf("Fatal error while initializing the server instance.\n");
			printf("Error : %s\n", message.c_str() );
			exit(-1);
		}
	// Your initialization goes here
	}

	void connect(ConnectReply& _return) {
		TRACE("connect\n");

		// Create a new session context.
		session_context_t * sess_ctx = server_instance.session_map.new_session();
		NL_ASSERT(sess_ctx);

		// Use the default database.
		open_database_t * db = server_instance.open_databases.get_database("default");
		sess_ctx->set_using_database(db);

		_return.session_handle = sess_ctx->get_session_handle();
		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void disconnect(DefaultReply& _return, const Session& session) {
		TRACE("disconnect\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		// Remove from the session context map, delete it.
		server_instance.session_map.delete_session( sess_ctx );

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void database_create(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_create\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		ErrorCode::type rc = meta_database_t::create_database(db_name);

		_return.status.error_code = rc;
	}

	void database_drop(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_drop\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		ErrorCode::type rc = meta_database_t::create_database(db_name);

		_return.status.error_code = rc;
	}

	void database_use(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_use\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		open_database_t * db = server_instance.open_databases.get_database(db_name);
		if (db) {
			_return.status.error_code = ErrorCode::NL_SUCCESS;
		} else { // The DB was not open yet.
			ErrorCode::type rc = server_instance.open_databases.open_database(db_name, &db);
			_return.status.error_code = rc;
		}

		NL_ASSERT(db);

		sess_ctx->set_using_database(db);
	}

	void table_create(DefaultReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_create\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		ErrorCode::type rc = meta_tables_t::create_table( sess_ctx->get_using_database()->get_db(), table_name);

		_return.status.error_code = rc;
	}

	void table_drop(DefaultReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_drop\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		ErrorCode::type rc = meta_tables_t::drop_table( sess_ctx->get_using_database()->get_db(), table_name);

		_return.status.error_code = rc;
	}

	void table_stat(TableStatReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_stat\n");
		bool transaction_began = false;
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_TABLE(table, table_name);

		AUTO_BEGIN_TRANSACTION(sess_ctx);
		transaction_began = true;

		GET_TRANSACTION(tx, sess_ctx);

		nldb_table_stat_t stat;
		nldb_rc_t rc = nldb_table_stat(tx, table, &stat);
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}
		AUTO_COMMIT_TRANSACTION(sess_ctx);
		_return.status.error_code = ErrorCode::NL_SUCCESS;
		_return.key_count = stat.key_count;

		return;
on_error:
        if (transaction_began) {
        	ABORT_TRANSACTION(sess_ctx);
        }
	}

	inline nldb_key_t to_nldb_key(const std::string & key) {
		return nldb_key_t { (void*)key.data(), (key_length_t )key.length() };
	}
	inline std::string to_string_key(const nldb_key_t & key) {
		return std::string((const char*)key.data, key.length);
	}

	inline nldb_value_t to_nldb_value(const std::string & value) {
		return nldb_value_t { (void*)value.data(), (value_length_t ) value.length() };
	}
	inline std::string to_string_value(const nldb_value_t & value) {
		return std::string((const char*)value.data, value.length);
	}


	void table_put(DefaultReply& _return, const Session& session, const std::string& table_name, const std::string& key, const std::string& value) {
		TRACE("table_put\n");
		bool transaction_began = false;
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_TABLE(table, table_name);

		AUTO_BEGIN_TRANSACTION(sess_ctx);
		transaction_began = true;

		GET_TRANSACTION(tx, sess_ctx);


		nldb_rc_t rc = nldb_table_put(tx, table, to_nldb_key(key), to_nldb_value(value) );
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		AUTO_COMMIT_TRANSACTION(sess_ctx);
		_return.status.error_code = ErrorCode::NL_SUCCESS;
		return;
on_error:
        if (transaction_began) {
        	ABORT_TRANSACTION(sess_ctx);
        }
	}

	void table_get_by_key(TableGetReply& _return, const Session& session, const std::string& table_name, const std::string& key) {
		TRACE("table_get_by_key\n");
		bool transaction_began = false;
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_TABLE(table, table_name);

		AUTO_BEGIN_TRANSACTION(sess_ctx);
		transaction_began = true;

		GET_TRANSACTION(tx, sess_ctx);

		nldb_order_t key_order;
		nldb_value_t nldb_value;
		nldb_rc_t rc = nldb_table_get(tx, table, to_nldb_key(key), &nldb_value, &key_order );
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		AUTO_COMMIT_TRANSACTION(sess_ctx);

		// TODO : Fatal : Need to free memory in key and value?
		_return.key = key;
		_return.key_order = key_order;
		_return.value = to_string_value(nldb_value);
		_return.status.error_code = ErrorCode::NL_SUCCESS;

		rc = nldb_value_free(table, nldb_value);
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		return;
on_error:
        if (transaction_began) {
        	ABORT_TRANSACTION(sess_ctx);
        }
	}

	void table_get_by_order(TableGetReply& _return, const Session& session, const std::string& table_name, const KeyOrder key_order) {
		TRACE("table_get_by_order\n");
		bool transaction_began = false;
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_TABLE(table, table_name);

		AUTO_BEGIN_TRANSACTION(sess_ctx);
		transaction_began = true;

		GET_TRANSACTION(tx, sess_ctx);

		nldb_key_t nldb_key;
		nldb_value_t nldb_value;
		nldb_rc_t rc = nldb_table_get(tx, table, key_order, &nldb_key, &nldb_value );
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		AUTO_COMMIT_TRANSACTION(sess_ctx);

		_return.key = to_string_key(nldb_key);
		_return.key_order = key_order;
		_return.value = to_string_value(nldb_value);
		_return.status.error_code = ErrorCode::NL_SUCCESS;

		rc = nldb_key_free(table, nldb_key);
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		rc = nldb_value_free(table, nldb_value);
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		return;
on_error:
        if (transaction_began) {
        	ABORT_TRANSACTION(sess_ctx);
        }
	}

	void table_del(DefaultReply& _return, const Session& session, const std::string& table_name, const std::string& key) {
		TRACE("table_del\n");
		bool transaction_began = false;
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_TABLE(table, table_name);

		AUTO_BEGIN_TRANSACTION(sess_ctx);
		transaction_began = true;

		GET_TRANSACTION(tx, sess_ctx);

		nldb_rc_t rc = nldb_table_del(tx, table, to_nldb_key(key));
		if (rc != NLDB_OK) {
			_return.status.error_code = (ErrorCode::type) rc;
			goto on_error;
		}

		AUTO_COMMIT_TRANSACTION(sess_ctx);
		_return.status.error_code = ErrorCode::NL_SUCCESS;
		return;
on_error:
        if (transaction_began) {
        	ABORT_TRANSACTION(sess_ctx);
        }
	}



	#define AUTO_COMMIT_TRANSACTION(session_ctx) \
			if (session_ctx->get_transaction() == NULL) { \
		        nldb_rc_t rc = nldb_tx_commit( auto_commit_tx );        \
		        if (rc != NLDB_OK) {                                    \
	             	_return.status.error_code = (ErrorCode::type) rc;   \
		            return;                                             \
	            }                                                       \
		        rc = nldb_tx_destroy( auto_commit_tx );                 \
		        if (rc != NLDB_OK) {                                    \
	             	_return.status.error_code = (ErrorCode::type) rc;   \
		            return;                                             \
	            }                                                       \
	        }

	// TODO : What happens if the function returns after aborting a transaction, but destroying it?
	// This function is called when an error happens. Abort the transaction in the session context as well.
	#define ABORT_TRANSACTION(session_ctx) \
			if (session_ctx->get_transaction() == NULL) { \
		        nldb_rc_t rc = nldb_tx_abort( auto_commit_tx );        \
		        if (rc != NLDB_OK) {                                    \
	             	_return.status.error_code = (ErrorCode::type) rc;   \
		            return;                                             \
	            }                                                       \
		        rc = nldb_tx_destroy( auto_commit_tx );                 \
		        if (rc != NLDB_OK) {                                    \
	             	_return.status.error_code = (ErrorCode::type) rc;   \
		            return;                                             \
	            }                                                       \
	        } else {                                                    \
	        	nldb_tx_t session_tx = session_ctx->get_transaction();  \
				nldb_rc_t rc = nldb_tx_abort( session_tx );             \
				if (rc != NLDB_OK) {                                    \
					_return.status.error_code = (ErrorCode::type) rc;   \
					return;                                             \
				}                                                       \
	        }


	void transaction_begin(DefaultReply& _return, const Session& session) {
		TRACE("transaction_begin\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		nldb_tx_t new_transaction;
		bool done_tx_init = false;

		if (sess_ctx->get_transaction() == NULL) {
			nldb_db_t db = sess_ctx->get_using_database()->get_db() ;

	        nldb_rc_t rc = nldb_tx_init( db, &new_transaction);
	        if (rc != NLDB_OK) {
             	_return.status.error_code = (ErrorCode::type) rc;
	            goto on_error;
            }
	        done_tx_init = true;
	        rc = nldb_tx_begin( new_transaction );
	        if (rc != NLDB_OK) {
	         	_return.status.error_code = (ErrorCode::type) rc;
	            goto on_error;
            }

	        sess_ctx->set_transaction( new_transaction );
			_return.status.error_code = ErrorCode::NL_SUCCESS;
        } else {
        	_return.status.error_code = ErrorCode::NL_TRANSACTION_ALREADY_BEGAN;
        }
		return;
on_error:
		if (done_tx_init) {
			nldb_rc_t rc = nldb_tx_destroy( new_transaction);
			NL_ASSERT( rc == NLDB_OK );
		}
		return;
	}

	void transaction_abort(DefaultReply& _return, const Session& session) {
		TRACE("transaction_abort\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		nldb_tx_t transaction = sess_ctx->get_transaction();

		if ( transaction == NULL)  {
			_return.status.error_code = ErrorCode::NL_NO_TRANSACTION_BEGAN;
		} else {
	        nldb_rc_t rc = nldb_tx_abort( transaction );
	        if (rc != NLDB_OK) {
             	_return.status.error_code = (ErrorCode::type) rc;
             	// TODO : Can we return without destroying transaction?
	            return;
            }
	        rc = nldb_tx_destroy( transaction );
	        if (rc != NLDB_OK) {
             	_return.status.error_code = (ErrorCode::type) rc;
	            return;
            }
	        sess_ctx->set_transaction(NULL);
			_return.status.error_code = ErrorCode::NL_SUCCESS;
		}
		return;
	}

	void transaction_commit(DefaultReply& _return, const Session& session) {
		TRACE("transaction_commit\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		nldb_tx_t transaction = sess_ctx->get_transaction();

		if ( transaction == NULL)  {
			_return.status.error_code = ErrorCode::NL_NO_TRANSACTION_BEGAN;
		} else {
	        nldb_rc_t rc = nldb_tx_commit( transaction );
	        if (rc != NLDB_OK) {
             	_return.status.error_code = (ErrorCode::type) rc;
             	// TODO : Can we return without destroying transaction?
	            return;
            }
	        rc = nldb_tx_destroy( transaction );
	        if (rc != NLDB_OK) {
             	_return.status.error_code = (ErrorCode::type) rc;
	            return;
            }
	        sess_ctx->set_transaction(NULL);
			_return.status.error_code = ErrorCode::NL_SUCCESS;
		}
		return;
	}

	void cursor_open_by_order(CursorOpenReply& _return, const Session& session, const std::string& table_name, const CursorDirection::type dir, const KeyOrder keyOrder) {
		TRACE("cursor_open_by_order\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void cursor_open_by_key(CursorOpenReply& _return, const Session& session, const std::string& table_name, const CursorDirection::type dir, const std::string& key) {
		TRACE("cursor_open_by_key\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void cursor_fetch(CursorFetchReply& _return, const Session& session, const CursorHandle cursor_handle, const CursorDirection::type dir) {
		TRACE("cursor_fetch\n");
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_CURSOR_CONTEXT(sess_ctx, cur_ctx, cursor_handle);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void cursor_close(DefaultReply& _return, const Session& session, const CursorHandle cursor_handle) {
		TRACE("cursor_close\n");
		GET_SESSION_CONTEXT(sess_ctx, session);
		GET_CURSOR_CONTEXT(sess_ctx, cur_ctx, cursor_handle);

		sess_ctx->delete_cursor(cur_ctx);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}
};


	int listen(int port) {
		meta_database_t::create_meta_database();

		shared_ptr<DatabaseServiceHandler> handler(new DatabaseServiceHandler());
	    shared_ptr<TProcessor> processor(new nanolat::thrift::DatabaseServiceProcessor(handler));
	    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	    // using thread pool with maximum 15 threads to handle incoming requests
	    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
	    shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	    threadManager->threadFactory(threadFactory);
	    threadManager->start();
	    TNonblockingServer server(processor, protocolFactory, port, threadManager);
	    server.serve();

	    return 0;
	}

} // server

} // nanolat
