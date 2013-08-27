#ifndef _NANOLAT_SERVERIMPL_H_
#define _NANOLAT_SERVERIMPL_H_

#include <stdlib.h>
#include <nldb/nldb.h>
#include <nanolat/client/common.h>
#include "DatabaseService.h"


using namespace ::nanolat::client;
using namespace  ::nanolat::thrift;

namespace nanolat {
namespace server {



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

	/*! Create meta tables if they do not exist.
	 * If the meta tables exist, do nothing.
	 */
	static ErrorCode::type create_meta_tables(const nldb_db_t & db) {
		// Step 1 : Create the META_TABLE_NAME_MAP table.
		// Step 2 : Create the META_NEXT_ID table.
		return ErrorCode::NL_FAILURE;
	}

	/*! Create a table.
	 */
	static ErrorCode::type create_table( const nldb_db_t & db,
			                          const std::string & table_name ) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Drop a table.
	 */
	static ErrorCode::type drop_table( const nldb_db_t & db,
			                          const std::string & table_name ) {
		return ErrorCode::NL_FAILURE;
	}


	/*! Get the table ID from the meta table searching by the given table_name.
	 */
	static ErrorCode::type get_table_id( const nldb_db_t & db,
			                             const std::string & table_name,
						                 nldb_table_id_t * table_id ) {
		return ErrorCode::NL_FAILURE;
	}

private:
	/*! Get the next table ID from the meta table, increase the next table ID, put the mapping of the table_name and new table ID into meta table.
	 */
	static ErrorCode::type put_table( const nldb_db_t & db,
			                          const std::string & table_name,
						              nldb_table_id_t * table_id) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Delete the mapping of the given table name and its table ID.
	 */
	static ErrorCode::type del_table( const nldb_db_t & db,
			                          const std::string & table_name ) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Increase the next table ID, return the table ID before the increment.
	 */
	static ErrorCode::type next_table_id( nldb_table_id_t * next_table_id ) {
		// TODO : Two threads may increase the next table id at the same time.

		return ErrorCode::NL_FAILURE;
	}

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

	/*! Check if the meta database exists.
	 */
	static ErrorCode::type does_meta_database_exist(bool * exists) {
		*exists = false;
		return ErrorCode::NL_FAILURE;
	}

	/*! Make sure that the meta database was created.
	 * If the meta database already exists, do nothing.
	 */
	static ErrorCode::type create_meta_database() {
		// Step 1 : Create the META_DATABASE_NAME_MAP table.

		// Step 2 : Create the META_NEXT_ID table.

		return ErrorCode::NL_FAILURE;
	}

	/*! Create a new table in Nanolat Database engine. */
	static ErrorCode::type create_database(const std::string & database_name) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Drop a database in Nanolat Database engine. */
	static ErrorCode::type drop_database(const std::string & database_name) {
		// TODO : Make sure there is no race condition between create_database, drop_database, open_database.

		// TODO : Close an open database. The database may be in use by another thread.
		ErrorCode::type rc = del_database(database_name);
		if (rc != ErrorCode::NL_SUCCESS) return rc;

		return ErrorCode::NL_SUCCESS;
	}

	/*! Get the database ID from the meta table searching by the given database_name.
	 */
	static ErrorCode::type get_database( const std::string & database_name,
			                             nldb_db_id_t * db_id ) {
		return ErrorCode::NL_FAILURE;
	}

private:
	/*! Get the next database ID from the meta table, increase the next database ID, put the mapping of the database_name and new database ID into meta table.
	 */
	static ErrorCode::type put_database( const std::string & database_name,
						                 nldb_db_id_t * db_id) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Delete the mapping of the given database name and its database ID.
	 */
	static ErrorCode::type del_database( const std::string & database_name ) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Increase the next database ID, return the database ID before the increment.
	 */
	static ErrorCode::type next_database_id( nldb_db_id_t * next_db_id ) {
		// TODO : Two threads may increase the next database id at the same time.


		return ErrorCode::NL_FAILURE;
	}
};


/*! For each open database, we have this object. The database stays open once it is open by nl_database_use request.
 */
class open_database_t {
public :
	open_database_t(const nldb_db_t & db) {
		this->db = db;
	}

	/*! Create a new table in Nanolat Database engine. */
	ErrorCode::type create_table(const std::string & table_name) {
		return ErrorCode::NL_FAILURE;
	}

	/*! Drop a table in Nanolat Database engine. */
	ErrorCode::type drop_table(const std::string & table_name) {
		return ErrorCode::NL_FAILURE;
	}

	/*! get the Nanolat Database table object by table name */
	nldb_table_t get_table(const std::string & table_name ) {
		nldb_table_t table = tables[ table_name ];
		return table;
	}

	/*! get the Nanolat Database table object by table name */
	ErrorCode::type open_table(const std::string & table_name, nldb_table_t * o_table ) {
		nldb_table_id_t table_id;

		// Get the table ID by the table name from the meta table.
		ErrorCode::type rc = meta_tables_t::get_table_id(db, table_name, &table_id);
		if (rc != NLDB_OK) return rc;

		if (table_id) { // If the table name exists in the meta table, open it.
			nldb_table_t open_table;
			// TODO : Need to check if table is already open?
			nldb_rc_t nrc = nldb_table_open(db, table_id, & open_table );
			if (nrc) return (ErrorCode::type) nrc;
			*o_table = open_table;
			return ErrorCode::NL_SUCCESS;
		}
		return ErrorCode::NL_TABLE_DOES_NOT_EXIST;
	}


	nldb_db_t get_db() {
		return db;
	}

private :
	// The mapping from table name to nldb table object.
	std::map<std::string, nldb_table_t> tables;

	// The open database.
	nldb_db_t db;
};

class session_context_t {
public :
	session_context_t(int session_handle) {
		this->session_handle = session_handle;
		transaction = NULL;
		using_database = NULL;
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

	open_database_t * get_using_database() {
		return using_database;
	}

	// TODO : What happens if there is an on-going transaction in the previous open database?
	void set_using_database( open_database_t * database ) {
		this->using_database = database;
	}

	nldb_tx_t get_transaction() {
		return transaction;
	}
	void set_transaction(const nldb_tx_t & transction) {
		this->transaction = transction;
	}

private:
	// The session handle generated by the server. Each client receives the session_handle when it connects to the server.
	// For the subsequent requests, the client sends the received session_handle along with the request.
	// The server checks if the session handle is the one provided by the server to see if the client is authorized to access the server.
	// The server generates the session handle with a random number generator.
	int session_handle;

	// The database we are using. A client can request 'nl_database_use' to use another database.
	// By default, a client uses the "default" database.
	open_database_t * using_database;

	// The transaction object
	// If nl_transaction_begin request was received, the transaction object is set to a valid one.
	// If this object is NULL, each request runs in auto-commit mode.
	nldb_tx_t transaction;

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



/*! This is the mapping from database name to database_t object for all open databases.
 */
class open_database_map_t {
public :
	open_database_map_t() {
	}

	ErrorCode::type open_database( const std::string & database_name, open_database_t ** o_database ) {
		return ErrorCode::NL_FAILURE;
	}

	/*! get the Nanolat Database object by database name */
	open_database_t * get_database( const std::string & database_name ) {
		open_database_t * database = open_databases[ database_name ];
		return database;
	}

private :
	// The mapping from database name to table_map_t table object.
	std::map<std::string, open_database_t*> open_databases;
};


class global_singleton_t {

public :
	session_map_t       session_map;
	open_database_map_t open_databases;

	/*! Initialize the Nanolat Server instance */
	ErrorCode::type initialize_instance() {
		ErrorCode::type rc;

		// Step 1 : Create meta database if it does not exist.
		bool meta_database_exists;
		rc = meta_database_t::does_meta_database_exist( & meta_database_exists );
		if (rc != ErrorCode::NL_SUCCESS) return rc;

		if (meta_database_exists) { // The meta database does not exist. create a one.
			rc = meta_database_t::create_meta_database();
			if (rc != ErrorCode::NL_SUCCESS) return rc;
		}

		// Step 2 : Create the "default" database if it does not exist.
		nldb_db_id_t default_db_id;
		rc = meta_database_t::get_database( std::string("default"), &default_db_id);
		if (rc != ErrorCode::NL_SUCCESS) return rc;

		if ( default_db_id == 0 ) { // The "default" db does not exist. Create a new one.
			rc = meta_database_t::create_database( std::string("default") );
			if (rc != ErrorCode::NL_SUCCESS) return rc;
		}

		return ErrorCode::NL_SUCCESS;
	}
};

/*! Get the error message format from the error code.
 * The error message format includes arguments such as "Invalid table, %1%".
 * The formatting routine uses boost::format, so, the message format should use %1% to denote the first argument, %2% for the 2nd argument, etc.
 *
 * The error code includes one from Nanolat Database engin(nldb) as well as its Server.
 * Error codes of them do not conflict using separate range of integer.
 */
std::string get_error_message_format(int error_code);

} // namespace server
} // namespace nanolat


#endif /* _NANOLAT_SERVERIMPL_H_ */
