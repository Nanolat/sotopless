// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

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

#include <nanolat/client/common.h>
using namespace ::nanolat::client;

using namespace  ::nanolat::thrift;

#if !defined(NDEBUG)
#  define TRACE printf
#else
#  define TRACE(...)
#endif


namespace nanolat {
namespace server {

#include <stdlib.h>
#include <nldb/nldb.h>

class cursor_context_t {
public:
	cursor_context_t(int cursor_handle) {
		this->cursor_handle = cursor_handle;
	}

	int get_cursor_handle() {
		return cursor_handle;
	}
private:
	int cursor_handle;
};

class session_context_t {
public :
	session_context_t(int session_handle) {
		this->session_handle = session_handle;
		transaction = NULL;
	}
	int get_session_handle() {
		return session_handle;
	}

	cursor_context_t * new_cursor() {
		/* initialize random seed: */
		srand( time(NULL) );

		int cursor_handle = rand();
		cursor_context_t * cursor = new cursor_context_t(cursor_handle);
		cursor_map[ cursor_handle ] = cursor;
		return cursor;
	}
	void delete_cursor(cursor_context_t * cursor){
		int cursor_handle = cursor->get_cursor_handle();
		NL_ASSERT( cursor_handle );
		cursor_map[ cursor_handle ] = NULL;
		delete cursor;
	}
	cursor_context_t * get_cursor( int cursor_handle ) {
		cursor_context_t * cursor = cursor_map[ cursor_handle ];
		return cursor;
	}

	nldb_tx_t * get_transaction();
private:
	int session_handle;

	// The transaction object
	// If nl_transaction_begin request was received, the transaction object is set to a valid one.
	// If this object is NULL, each request runs in auto-commit mode.
	nldb_tx_t * transaction;

	// The mapping from cursor handle to cursor_context_t object.
	std::map<int, cursor_context_t *> cursor_map;
};

class session_map_t {

public:
	session_map_t() {
	}
	session_context_t * new_session() {
		/* initialize random seed: */
		srand( time(NULL) );

		int session_handle = rand();
		session_context_t * ctx = new session_context_t(session_handle);
		sessions[ session_handle ] = ctx;
		return ctx;
	}
	void delete_session(session_context_t * session){
		int session_handle = session->get_session_handle();
		NL_ASSERT( session_handle );
		sessions[ session_handle ] = NULL;
		delete session;
	}
	session_context_t * get_session( int session_handle ) {
		session_context_t * s = sessions[ session_handle ];
		return s;
	}

private:
	// The mapping from session_handle to session_context_t object.
	// Each time a request is arrived at the server, the server gets the session
	// object by the session key embedded in the request from a session.
	std::map<int, session_context_t *> sessions;

};



/*! Manages meta tables that exists for every database except the meta database(id_id_t : 0).
 * The meta tables convert table names to table id, and provides next table id for 'nl_table_create'.
 * We need a meta table that has mapping from table name to nldb_table_id_t
 * Whenever a table is created, the mapping from the table name to nldb_table_id_t is
 * put into META_TABLE_NAME_MAP meta table and the next table ID in META_NEXT_ID is
 * incremented.
 */
class meta_tables_t
{
public :
	// The table id 0 is reservered for the meta table in nldb library.
	// Therefore, meta tables managed by this object starts from 1.

	/*! A meta table holding mapping from table name to integer table ID (nldb_table_id_t).
     * Nanolat Database uses integer ID to name a table,
     * but the DatabaseServer interface uses string to name a table.
     * This is why we need a mapping from string to integer ID
     * for translating table names to table ID(nldb_table_id_t).
	 */
	const nldb_table_id_t META_TABLE_NAME_MAP = 1;

	/*! A meta table holding integer identifiers such as next table ID.
	 * For each key, it has one kind of next ID.
	 * Currently the only key in this table is "next_table_id",
	 * whose value is the next table ID to use when creating a new table.
	 */
	const nldb_table_id_t META_NEXT_ID = 2;

	meta_tables_t()
	{
	};

	inline nldb_rc_t put_table( const std::string & table_name,
						        nldb_table_id_t * table_id) {
		return NLDB_ERROR;
	}

	inline nldb_rc_t get_table( const std::string & table_name,
						           nldb_table_id_t * table_id ) {
		return NLDB_ERROR;
	}
	inline nldb_rc_t del_table( const std::string & table_name ) {
		return NLDB_ERROR;
	}
private:
};



/*! The meta database that has a meta table for keeping database name to database ID(db_id).
 * Whenever a database is created, the mapping from the database name to db_id is
 * put into META_DATABASE_NAME_MAP meta table and the next database ID in META_NEXT_ID table is
 * incremented.
 * Both META_DATABASE_NAME_MAP and META_NEXT_ID table are in the meta database with database ID, 0.
 */
class meta_database_t
{
public :
	// The table id 0 is reservered for the meta table in nldb library.
	// Therefore, meta tables managed by this object starts from 1.

	/*! A meta table holding mapping from database name to integer database ID (db_id).
     * Nanolat Database uses integer ID to name a database,
     * but the DatabaseServer interface uses string to name a database.
     * This is why we need a mapping from string to integer ID
     * for translating database names to table ID(db_id).
	 */
	const nldb_table_id_t META_TABLE_NAME_MAP = 1;

	/*! A meta table holding integer identifiers such as next database ID.
	 * For each key, it has one kind of next ID.
	 * Currently the only key in this table is "next_database_id",
	 * whose value is the next database ID to use when creating a new table.
	 */
	const nldb_table_id_t META_NEXT_ID = 2;

	meta_database_t()
	{
	}

	inline nldb_rc_t put_table( const std::string & table_name,
						        nldb_table_id_t * table_id) {
		return NLDB_ERROR;
	}

	inline nldb_rc_t get_table( const std::string & table_name,
						           nldb_table_id_t * table_id ) {
		return NLDB_ERROR;
	}

	inline nldb_rc_t del_table( const std::string & table_name ) {
		return NLDB_ERROR;
	}
private:
};


/*
 *
 */
class table_map_t {
public :
	table_map_t() {
	}

	void create_table(std::string table_name) {

	}

	void drop_table(std::string table_name) {

	}

	nldb_table_t * get_table( std::string table_name ) {
		nldb_table_t * table = tables[ table_name ];
		return table;
	}

	void clear() {

	}

private :
	// The mapping from table name to nldb table object.
	std::map<std::string, nldb_table_t*> tables;
};

#define GET_SESSION_CONTEXT(ctx, session) \
	session_context_t * ctx = session_map.get_session(session.session_handle); \
    if (!ctx) {                                                             \
		_return.status.error_code = ErrorCode::NL_INVALID_SESSION_HANDLE;   \
        _return.status.error_message_format = "Invalid Session Handle";     \
        return;                                                             \
	}

#define GET_CURSOR_CONTEXT(session_ctx, cursor_ctx, cursor_handle) \
	cursor_context_t * cursor_ctx = session_ctx->get_cursor(cursor_handle); \
    if (!cursor_ctx) {                                                      \
		_return.status.error_code = ErrorCode::NL_INVALID_CURSOR_HANDLE;    \
        _return.status.error_message_format = "Invalid Cursor Handle";      \
        return;                                                             \
	}

class DatabaseServiceHandler : virtual public DatabaseServiceIf {
private :
	session_map_t session_map;
	table_map_t   table_map;
 public:
	DatabaseServiceHandler() {
	// Your initialization goes here
	}

	void connect(ConnectReply& _return) {
		TRACE("connect\n");

		// Create a new session context.
		session_context_t * sess_ctx = session_map.new_session();
		NL_ASSERT(sess_ctx);

		_return.session_handle = sess_ctx->get_session_handle();

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void disconnect(DefaultReply& _return, const Session& session) {
		TRACE("disconnect\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		// Remove from the session context map, delete it.
		session_map.delete_session( sess_ctx );

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void database_create(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_create\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void database_drop(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_drop\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void database_use(DefaultReply& _return, const Session& session, const std::string& db_name) {
		TRACE("database_use\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_create(DefaultReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_create\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_drop(DefaultReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_drop\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_stat(TableStatReply& _return, const Session& session, const std::string& table_name) {
		TRACE("table_stat\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_put(DefaultReply& _return, const Session& session, const std::string& table_name, const std::string& key, const std::string& value) {
		TRACE("table_put\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_get_by_key(TableGetReply& _return, const Session& session, const std::string& table_name, const std::string& key) {
		TRACE("table_get_by_key\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_get_by_order(TableGetReply& _return, const Session& session, const std::string& table_name, const KeyOrder keyOrder) {
		TRACE("table_get_by_order\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void table_del(DefaultReply& _return, const Session& session, const std::string& table_name, const std::string& key) {
		TRACE("table_del\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void transaction_begin(DefaultReply& _return, const Session& session) {
		TRACE("transaction_begin\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void transaction_abort(DefaultReply& _return, const Session& session) {
		TRACE("transaction_abort\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
	}

	void transaction_commit(DefaultReply& _return, const Session& session) {
		TRACE("transaction_commit\n");
		GET_SESSION_CONTEXT(sess_ctx, session);

		_return.status.error_code = ErrorCode::NL_SUCCESS;
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
