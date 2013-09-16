#ifndef _NANOLAT_SERVERIMPL_H_
#define _NANOLAT_SERVERIMPL_H_

#include <stdlib.h>
#include <nldb/nldb.h>
#include <nanolat/client/common.h>
#include "DatabaseService.h"
#include <private/assert.h>
#include <private/stacktrace.h>
#include <functional>
#include <string.h>

// TODO : make sure nldb_value_free is called for all cases getting values from nldb API.


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


#define GET_TABLE(tab, tx, table_name)                              \
		open_database_t * db = sess_ctx->get_using_database();      \
		NL_ASSERT(db);                                              \
		nldb_table_t tab = db->get_table(sess_ctx->get_tenant_id(), table_name);  \
		if (!tab) {                                                 \
	        server_error_t rc = db->open_table(tx, sess_ctx->get_tenant_id(), table_name, &tab);  \
	        if (rc) {                      \
	        	_return.status.error_code = (ErrorCode::type)rc;    \
	        	return;                                             \
	        }                                                       \
        }


#define GET_TRANSACTION(tx, session_ctx) \
        nldb_tx_t tx = session_ctx->get_transaction();


using namespace ::nanolat::client;
using namespace  ::nanolat::thrift;

namespace nanolat {

typedef nldb_rc_t server_error_t;

namespace db {

class cursor_context_t {
public:
	cursor_context_t(int cursor_handle) {
		this->table = NULL;
		this->cursor_handle = cursor_handle;
		cursor = NULL;
	}

	int get_cursor_handle() {
		return cursor_handle;
	}
	nldb_table_t table;
	nldb_cursor_t cursor;
	nldb_cursor_direction_t cursor_direction;
private:
	int cursor_handle;
};

class instant_db_env_t {
public :
	typedef std::function<server_error_t(nldb_tx_t & tx, nldb_table_t & table)> opeartion_t;

private :
	static server_error_t exec(nldb_tx_t & tx, nldb_table_t & table, opeartion_t & operation) {
		server_error_t rc = operation(tx, table);
		NL_RETURN(rc);
	}

	/*! Begin a transaction, run the given operation, commit the transaction.
	 */
	static server_error_t exec(nldb_db_t & db, nldb_table_t & table, opeartion_t & operation) {
		server_error_t rc = NLDB_OK;

		nldb_tx_t tx = NULL;
		bool tx_began = false;

		nldb_rc_t nrc = nldb_tx_init(db, &tx);
		if (nrc) {
			rc = (server_error_t)nrc;
			goto finally;
		}

		nrc = nldb_tx_begin(tx);
		if (nrc) {
			rc = (server_error_t)nrc;
			goto finally;
		}
		tx_began = true;

		rc = exec(tx, table, operation);
		// rc is returned at the end of this function.

	finally:
		if (tx_began) {
			NL_RELEASE_ASSERT( tx );
			if (rc == NLDB_OK)
				nrc = nldb_tx_commit(tx);
			else
				nrc = nldb_tx_abort(tx);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		if (tx) {
			nrc = nldb_tx_destroy(tx);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}

public :
	static server_error_t exec_with_db(nldb_db_t & db, const nldb_table_id_t & table_id, opeartion_t operation) {
		server_error_t rc = NLDB_OK;

		nldb_table_t table = NULL;

		nldb_rc_t nrc = nldb_table_open(db, table_id, &table);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto finally;
		}

		rc = exec(db, table, operation);
		// rc is returned at the end of this function.

	finally:
		if (table) {
			nrc = nldb_table_close(table);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}

	static server_error_t exec_with_db(nldb_db_t & db, nldb_tx_t & tx, const nldb_table_id_t & table_id, opeartion_t operation) {
		server_error_t rc = NLDB_OK;

		nldb_table_t table = NULL;

		nldb_rc_t nrc = nldb_table_open(db, table_id, &table);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto finally;
		}

		rc = exec(tx, table, operation);
		// rc is returned at the end of this function.

	finally:
		if (table) {
			nrc = nldb_table_close(table);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}


	static server_error_t exec_with_db_id(const nldb_db_id_t & dbid, nldb_table_id_t table_id, opeartion_t operation) {
		server_error_t rc = NLDB_OK;

		nldb_db_t db = NULL;

		nldb_rc_t nrc = nldb_db_open(dbid, NULL/*Master Options*/, NULL/*Slave Options*/, &db);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto finally;
		}

		rc = exec_with_db(db, table_id, operation);
		// rc is returned at the end of this function.

	finally:
		if (db) {
			nrc = nldb_db_close(db);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}
};

class meta_t
{
private :
	static server_error_t create_meta_tables(nldb_db_t & db,
			                                 const nldb_table_id_t & name_map_table_id,
			                                 const nldb_table_id_t & next_id_table_id,
			                                 const std::string & next_id_key,
			                                 int next_id_value) {
		server_error_t rc;
		bool name_map_table_created = false;
		bool next_id_table_created = false;

		// The next_id_value will be either next ID of DB or table.
		NL_ASSERT( sizeof(next_id_value) == sizeof(nldb_db_id_t) );
		NL_ASSERT( sizeof(next_id_value) == sizeof(nldb_table_id_t) );

		// Step 1 : Create the META_TABLE_NAME_MAP table.
		nldb_rc_t nrc = nldb_table_create(db, name_map_table_id, NLDB_TABLE_PERSISTENT);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto on_error;
		}
		name_map_table_created = true;

		// Step 2 : Create the META_NEXT_ID table.
		nrc = nldb_table_create(db, next_id_table_id, NLDB_TABLE_PERSISTENT);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto on_error;
		}
		next_id_table_created = true;

		rc = instant_db_env_t::exec_with_db(
				db, next_id_table_id,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)next_id_key.data(), (key_length_t)next_id_key.length() };
					nldb_value_t value = { (void*)&next_id_value, sizeof(next_id_value) };
					nldb_rc_t nrc = nldb_table_put(tx, table, key, value);
					NL_RETURN( (server_error_t)nrc );
				}
		);
		if (rc) NL_RETURN(rc);

		NL_RETURN( NLDB_OK );

on_error:
		if (next_id_table_created) {
			nrc = nldb_table_drop(db, next_id_table_id);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		if (name_map_table_created) {
			nrc = nldb_table_drop(db, name_map_table_id);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}

public :
	static server_error_t create_meta_tables(const nldb_db_id_t & db_id,
											const nldb_table_id_t & name_map_table_id,
						                    const nldb_table_id_t & next_id_table_id,
						                    const std::string & next_id_key,
						                    int64_t next_id_value) {
		server_error_t rc = NLDB_OK;

		nldb_db_t db = NULL;

		nldb_rc_t nrc = nldb_db_open(db_id, NULL/*Master Options*/, NULL/*Slave Options*/, &db);
		if (nrc) {
			rc = (server_error_t) nrc;
			goto finally;
		}

		rc = create_meta_tables(db, name_map_table_id, next_id_table_id, next_id_key, next_id_value);
		// rc is returned at the end of this function.

	finally:
		if (db) {
			nrc = nldb_db_close(db);
			NL_RELEASE_ASSERT( nrc == NLDB_OK );
		}

		NL_RETURN(rc);
	}

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
	#define META_TABLE_NAME_MAP 1

	/*! A meta table holding integer identifiers such as next table ID.
	 * For each key, it has one kind of next ID.
	 * Currently the only key in this table is "next_table_id",
	 * whose value is the next table ID to use when creating a new table.
	 */
	#define META_NEXT_TABLE_ID 2

	/*! The key in META_NEXT_TABLE_ID that has value for the next table ID.
	 */
	#define NEXT_TABLE_ID_KEY "next_table_id"

	/*! Create meta tables if they do not exist.
	 * If the meta tables exist, do nothing.
	 */
	static server_error_t create_meta_tables(const nldb_db_id_t db_id) {

		server_error_t rc = meta_t::create_meta_tables(db_id, META_TABLE_NAME_MAP, META_NEXT_TABLE_ID, NEXT_TABLE_ID_KEY, META_NEXT_TABLE_ID+1);
		NL_RETURN(rc);
	}

	/*! Create a table.
	 * Get the next table ID from the meta table, increase the next table ID, put the mapping of the table_name and new table ID into meta table.
	 *
	 */
	static server_error_t create_table( nldb_db_t & db,
                             			nldb_tx_t & tx,
			                            const std::string & table_name ) {
		nldb_table_id_t new_table_id;
		server_error_t rc = next_table_id( db, tx, &new_table_id );
		if (rc != NLDB_OK ) NL_RETURN(rc);

		rc = put_table( db, tx, table_name, new_table_id );
		if (rc != NLDB_OK ) NL_RETURN(rc);

		NL_RETURN( NLDB_OK );
	}

	/*! Drop a table.
	 */
	static server_error_t drop_table( nldb_db_t & db,
            						  nldb_tx_t & tx,
			                          const std::string & table_name ) {

		server_error_t rc = del_table(db, tx, table_name);
		if (rc != NLDB_OK ) NL_RETURN(rc);

		NL_RETURN( NLDB_OK );
	}


	/*! Get the table ID from the meta table searching by the given table_name.
	 */
	static server_error_t get_table_id( nldb_db_t & db,
			                            nldb_tx_t & tx,
			                            const std::string & table_name,
						                nldb_table_id_t * o_table_id ) {


		server_error_t rc;

		auto get_table_id_callback = [=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
			nldb_key_t key = { (void*)table_name.data(), (key_length_t)table_name.length() };
			nldb_value_t value;
			nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
			if (nrc == NLDB_ERROR_KEY_NOT_FOUND )
				NL_RETURN( ErrorCode::NL_TABLE_DOES_NOT_EXIST );
			if (nrc) NL_RETURN( (server_error_t)nrc );

			NL_ASSERT(sizeof(nldb_table_id_t) == value.length);
			*o_table_id = *( (nldb_table_id_t*)value.data );

			NL_RETURN( (server_error_t)nrc );
		};

		if (tx) {
			rc = instant_db_env_t::exec_with_db( db, tx, META_TABLE_NAME_MAP, get_table_id_callback);
			NL_RETURN(rc);
		} else {
			rc = instant_db_env_t::exec_with_db( db, META_TABLE_NAME_MAP, get_table_id_callback);
			NL_RETURN(rc);
		}

		NL_RETURN(NLDB_OK);
	}


	static server_error_t get_table_id( nldb_db_t & db,
			                            const std::string & table_name,
						                nldb_table_id_t * o_table_id ) {

		nldb_tx_t null_tx = (nldb_tx_t)NULL; /* No transaction at this point. A new transaction will be created to look up table name from the meta table. */

		server_error_t rc = get_table_id( db, null_tx /*transaction*/, table_name, o_table_id);
		if (rc) NL_RETURN(rc);

		NL_RETURN(NLDB_OK);
	}

private:
	/*! Put the mapping of the table_name and new table ID into meta table.
	 */
	static server_error_t put_table( nldb_db_t & db,
			                         nldb_tx_t & tx,
			                         const std::string & table_name,
						             const nldb_table_id_t & table_id) {

		server_error_t rc;

		rc = instant_db_env_t::exec_with_db(
				db, tx, META_TABLE_NAME_MAP,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)table_name.data(), (key_length_t)table_name.length() };
					nldb_value_t value;
					nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
					if (nrc == NLDB_ERROR_KEY_NOT_FOUND) { // The table does not exist yet. Good, Continue.
						nldb_table_id_t copied_table_id = table_id; // table_id is const. need to copy to local variable to get the address of the variable.
						value.data = &copied_table_id;
						value.length = sizeof(copied_table_id);

						nrc = nldb_table_put(tx, table, key, value);
						if (nrc) NL_RETURN( (server_error_t)nrc );
						NL_RETURN( NLDB_OK );
					}
					if (nrc) NL_RETURN( (server_error_t)nrc );

					// nldb_table_get with table_name succeeded. This means the table already exists.
					NL_ASSERT(nrc == NLDB_OK);
					NL_RETURN( ErrorCode::NL_TABLE_ALREADY_EXISTS );
				}
		);

		NL_RETURN(rc);
	}

	/*! Delete the mapping of the given table name and its table ID.
	 */
	static server_error_t del_table( nldb_db_t & db,
			                         nldb_tx_t & tx,
			                         const std::string & table_name ) {
		server_error_t rc;

		rc = instant_db_env_t::exec_with_db(
				db, tx, META_TABLE_NAME_MAP,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)table_name.data(), (key_length_t)table_name.length() };

					nldb_rc_t nrc = nldb_table_del(tx, table, key);
					if (nrc == NLDB_ERROR_KEY_NOT_FOUND)
						NL_RETURN( ErrorCode::NL_TABLE_DOES_NOT_EXIST );
					NL_RETURN( (server_error_t)nrc );
				}
		);

		NL_RETURN(rc);
	}

	/*! Increase the next table ID, return the table ID before the increment.
	 */
	static server_error_t next_table_id( nldb_db_t & db, nldb_tx_t & tx, nldb_table_id_t * o_next_table_id ) {
		// TODO : Two threads may increase the next table id at the same time.
		server_error_t rc;
		rc = instant_db_env_t::exec_with_db(
				db, tx, META_NEXT_TABLE_ID,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)NEXT_TABLE_ID_KEY, (key_length_t)strlen(NEXT_TABLE_ID_KEY) };
					nldb_value_t value;

					// Get the next table ID to use.
					nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
					if (nrc) NL_RETURN( (server_error_t)nrc );

					NL_ASSERT(sizeof(nldb_table_id_t) == value.length);

					// TODO : Make sure that value.data returned by nldb_table_get is aligned.
					nldb_table_id_t next_table_id = *( (nldb_table_id_t*)value.data );
					nldb_table_id_t increased_next_table_id = next_table_id + 1;

					value.length = sizeof(increased_next_table_id);
					value.data = &increased_next_table_id;

					// Put the increased database ID.
					nrc = nldb_table_put(tx, table, key, value);
					if (nrc) NL_RETURN( (server_error_t)nrc );

					*o_next_table_id = next_table_id;

					NL_RETURN( (server_error_t)nrc );
				}
		);

		NL_RETURN(rc);
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
	/*! The Nanolat Database ID for the meta database which has two tables
	 * 1. Mapping table from database name to database ID.
	 * 2. ID table which has the next database ID to use.
	 */
	#define META_DATABASE_ID (1)

	// The table id 0 is reservered for the meta table in nldb library.
	// Therefore, meta tables managed by this object starts from 1.

	/*! A meta table holding mapping from database name to integer database ID (db_id).
     * Nanolat Database uses integer ID to name a database,
     * but the DatabaseServer interface uses string to name a database.
     * This is why we need a mapping from string to integer ID
     * for translating database names to table ID(db_id).
	 */
	#define META_DATABASE_NAME_MAP (1)

	/*! A meta table holding integer identifiers such as next database ID.
	 * For each key, it has one kind of next ID.
	 * Currently the only key in this table is "next_database_id",
	 * whose value is the next database ID to use when creating a new table.
	 */
	#define META_DATABASE_NEXT_ID (2)

	/*! The key in META_DATABASE_NEXT_ID that has value for the next database ID.
	 */
	#define NEXT_DATABASE_ID_KEY "next_database_id"


	meta_database_t()
	{
	}

	/*! Check if the meta database exists.
	 */
	static server_error_t does_meta_database_exist(bool * exists) {
		nldb_db_t db;
		// Try to open the meta database. If succeeds, it exists.
		nldb_rc_t nrc = nldb_db_open(META_DATABASE_ID, NULL, NULL, &db);
		if (nrc == NLDB_OK) {
			*exists = true;
			nrc = nldb_db_close(db);
			NL_RELEASE_ASSERT(nrc == NLDB_OK);
		} else if (nrc == NLDB_ERROR_DB_NOT_FOUND){
			// TODO : set *exists = false only if the error code indicates that the database does not exist.
			*exists = false;
		} else {
			NL_RETURN( (server_error_t)nrc );
		}

		NL_RETURN( NLDB_OK );
	}

	/*! Make sure that the meta database was created.
	 * If the meta database already exists, do nothing.
	 */
	static server_error_t create_meta_database() {
		bool db_created = false;
		server_error_t rc = NLDB_OK;

		nldb_rc_t nrc = nldb_db_create(META_DATABASE_ID);
		db_created = true;

		rc = meta_t::create_meta_tables(META_DATABASE_ID, META_DATABASE_NAME_MAP, META_DATABASE_NEXT_ID, NEXT_DATABASE_ID_KEY, META_DATABASE_ID+1);
		if ( rc != NLDB_OK) goto on_error;

		NL_RETURN( NLDB_OK );
on_error:
		if (db_created) {
			nrc = nldb_db_drop(META_DATABASE_ID);
			NL_RELEASE_ASSERT(nrc);
		}
		NL_RETURN(rc);
	}

	/*! Create a new table in Nanolat Database engine.
	 * Get the next database ID from the meta table, increase the next database ID, put the mapping of the database_name and new database ID into meta table.
	 */
	static server_error_t create_database(const std::string & database_name) {

		nldb_db_id_t new_database_id;

		server_error_t rc = next_database_id(&new_database_id);
		if (rc != NLDB_OK ) NL_RETURN(rc);

		rc = put_database( database_name, new_database_id);
		if (rc != NLDB_OK ) NL_RETURN(rc);

		NL_RETURN( NLDB_OK );
	}

	/*! Drop a database in Nanolat Database engine. */
	static server_error_t drop_database(const std::string & database_name) {
		// TODO : Make sure there is no race condition between create_database, drop_database, open_database.

		// TODO : Close an open database. The database may be in use by another thread.
		server_error_t rc = del_database(database_name);
		if (rc) NL_RETURN(rc);

		NL_RETURN( NLDB_OK );
	}

	/*! Get the database ID from the meta table searching by the given database_name.
	 */
	static server_error_t get_database_id( const std::string & database_name,
			                                nldb_db_id_t * o_db_id ) {

		server_error_t rc;
		rc = instant_db_env_t::exec_with_db_id(
				META_DATABASE_ID, META_DATABASE_NAME_MAP,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)database_name.data(), (key_length_t)database_name.length() };
					nldb_value_t value;
					nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
					if (nrc == NLDB_ERROR_KEY_NOT_FOUND )
						NL_RETURN( ErrorCode::NL_DATABASE_DOES_NOT_EXIST );

					if (nrc) NL_RETURN( (server_error_t)nrc );

					NL_ASSERT(sizeof(nldb_table_id_t) == value.length);
					*o_db_id = *( (nldb_table_id_t*)value.data );

					NL_RETURN( (server_error_t)nrc );
				}
		);

		NL_RETURN(rc);
	}

private:
	/*! put the mapping of the database_name and new database ID into meta table.
	 */
	static server_error_t put_database( const std::string & database_name,
						                const nldb_db_id_t & db_id) {
		server_error_t rc;

		rc = instant_db_env_t::exec_with_db_id(
				META_DATABASE_ID, META_DATABASE_NAME_MAP,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)database_name.data(), (key_length_t)database_name.length() };
					nldb_value_t value;
					nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
					if (nrc == NLDB_ERROR_KEY_NOT_FOUND) { // The database does not exist yet. Good, Continue.
						nldb_db_id_t copied_db_id = db_id; // db_id is const. need to copy to local variable to get the address of the variable.
						value.data = &copied_db_id;
						value.length = sizeof(copied_db_id);

						nrc = nldb_table_put(tx, table, key, value);
						if (nrc) NL_RETURN( (server_error_t)nrc );
						NL_RETURN( NLDB_OK );
					}
					if (nrc) NL_RETURN( (server_error_t)nrc );

					// nldb_table_get with copied_db_id succeeded. This means the table already exists.
					NL_ASSERT(nrc == NLDB_OK);
					NL_RETURN( ErrorCode::NL_DATABASE_ALREADY_EXISTS );
				}
		);

		NL_RETURN(rc);
	}

	/*! Delete the mapping of the given database name and its database ID.
	 */
	static server_error_t del_database( const std::string & database_name ) {
		server_error_t rc;

		rc = instant_db_env_t::exec_with_db_id(
				META_DATABASE_ID, META_DATABASE_NAME_MAP,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)database_name.data(), (key_length_t)database_name.length() };

					nldb_rc_t nrc = nldb_table_del(tx, table, key);
					if (nrc == NLDB_ERROR_KEY_NOT_FOUND)
						NL_RETURN( ErrorCode::NL_DATABASE_DOES_NOT_EXIST );
					NL_RETURN( (server_error_t)nrc );
				}
		);

		NL_RETURN(rc);
	}

	/*! Increase the next database ID, return the database ID before the increment.
	 */
	static server_error_t next_database_id( nldb_db_id_t * o_next_db_id ) {
		// TODO : Two threads may increase the next database id at the same time.
		server_error_t rc;
		rc = instant_db_env_t::exec_with_db_id(
				META_DATABASE_ID, META_DATABASE_NEXT_ID,
				[=](nldb_tx_t & tx, nldb_table_t & table) -> server_error_t {
					nldb_key_t key = { (void*)NEXT_DATABASE_ID_KEY, (key_length_t)strlen(NEXT_DATABASE_ID_KEY) };
					nldb_value_t value;

					// Get the next database ID to use.
					nldb_rc_t nrc = nldb_table_get(tx, table, key, &value, NULL);
					if (nrc) NL_RETURN( (server_error_t)nrc );

					NL_ASSERT(sizeof(nldb_db_id_t) == value.length);

					// TODO : Make sure that value.data returned by nldb_table_get is aligned.
					nldb_db_id_t next_db_id = *( (nldb_db_id_t*)value.data );
					nldb_db_id_t increased_next_db_id = next_db_id + 1;

					value.length = sizeof(increased_next_db_id);
					value.data = &increased_next_db_id;

					// Put the increased database ID.
					nrc = nldb_table_put(tx, table, key, value);
					if (nrc) NL_RETURN( (server_error_t)nrc );

					*o_next_db_id = next_db_id;

					NL_RETURN( (server_error_t)nrc );
				}
		);

		NL_RETURN(rc);
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
	server_error_t create_table(nldb_tx_t & tx, const std::string & tenant_id, const std::string & table_name) {
		std::string tenant_table_name = tenant_id + "." + table_name; // prepend the table name

		nldb_table_id_t table_id;
		server_error_t rc = NLDB_OK;
		bool table_name_put_into_meta = false;

		rc = meta_tables_t::create_table(db, tx, tenant_table_name);
		if (rc) goto on_error;
		table_name_put_into_meta = true;

		rc = meta_tables_t::get_table_id(db, tx, tenant_table_name, &table_id);
		if (rc) goto on_error;

		// TODO : Need to choose if the user wants to create a persistent table?
		{
			nldb_rc_t nrc = nldb_table_create(db, table_id, NLDB_TABLE_VOLATILE);
			if (nrc) {
				rc = (server_error_t) nrc;
				goto on_error;
			}
		}

		NL_RETURN( NLDB_OK );

on_error:
		if (table_name_put_into_meta) {
			rc = meta_tables_t::drop_table(db, tx, tenant_table_name);
			NL_RELEASE_ASSERT (rc == NLDB_OK);
		}
		NL_RETURN(rc);
	}

	/*! Drop a table in Nanolat Database engine. */
	server_error_t drop_table(nldb_tx_t & tx, const std::string & tenant_id, const std::string & table_name) {
		std::string tenant_table_name = tenant_id + "." + table_name; // prepend the table name

		nldb_table_id_t table_id;

		server_error_t rc = meta_tables_t::get_table_id(db, tx, tenant_table_name, &table_id);
		if (rc) NL_RETURN(rc);

		nldb_rc_t nrc = nldb_table_drop(db, table_id);
		if (nrc) NL_RETURN( (server_error_t)nrc );

		rc = meta_tables_t::drop_table(db, tx, tenant_table_name);
		NL_RELEASE_ASSERT( rc == NLDB_OK );

		NL_RETURN( NLDB_OK );
	}

	/*! get the Nanolat Database table object by table name */
	nldb_table_t get_table(const std::string & tenant_id, const std::string & table_name ) {

		nldb_table_t table = tables[tenant_id][ table_name ];
		return table;
	}

	void set_table(const std::string & tenant_id, const std::string & table_name, const nldb_table_t & table ) {

		tables[tenant_id][ table_name ] = table;
	}

	// TODO : Think about when to close tables.
	/*! get the Nanolat Database table object by table name
	 */
	server_error_t open_table(nldb_tx_t & tx, const std::string & tenant_id, const std::string & table_name, nldb_table_t * o_table ) {
		NL_ASSERT(table_name != "");
		NL_ASSERT(o_table);

		std::string tenant_table_name = tenant_id + "." + table_name; // prepend the table name

		nldb_table_id_t table_id;

		if (get_table(tenant_id, tenant_id)) {
			NL_RETURN( ErrorCode::NL_TABLE_ALREADY_OPEN );
		}

		// Get the table ID by the table name from the meta table.
		server_error_t rc = meta_tables_t::get_table_id(db, tx, tenant_table_name, &table_id);
		if (rc) NL_RETURN(rc);

		nldb_table_t open_table;
		// TODO : Need to check if table is already open?
		nldb_rc_t nrc = nldb_table_open(db, table_id, & open_table );
		if (nrc) NL_RETURN( (server_error_t)nrc );

		NL_RELEASE_ASSERT(open_table);

		// Create a mapping table (the table name, table object) for quick lookup.
		set_table(tenant_id, table_name, open_table);

		*o_table = open_table;

		NL_RETURN( NLDB_OK );
	}

	nldb_db_t get_db() {
		return db;
	}

private :
	// The mapping from tenant_id to (mapping from table name to nldb table object).
	std::map<std::string,  std::map<std::string, nldb_table_t> > tables;

	// The open database.
	nldb_db_t db;
};

class session_context_t {
public :
	session_context_t(const std::string & tenant_id, int session_handle) {
		this->session_handle = session_handle;
		this->tenant_id = tenant_id;
		transaction = NULL;
		using_database = NULL;
		is_auto_commit = false;
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

	server_error_t begin_transaction() {
		nldb_tx_t new_transaction;

		if (this->transaction != NULL) {
			NL_RETURN( ErrorCode::NL_TRANSACTION_ALREADY_BEGAN );
		}

		nldb_db_t db = using_database->get_db() ;

		// TODO : Optimization : keep the nldb_tx_t object into session_context_t initialized, so that it does not have to be initialized for each request.
		nldb_rc_t rc = nldb_tx_init( db, &new_transaction);
		NL_RELEASE_ASSERT(rc == NLDB_OK); // what happens if there is not enough memory?

		rc = nldb_tx_begin( new_transaction );
		NL_RELEASE_ASSERT(rc == NLDB_OK);

		this->transaction = new_transaction;

		NL_RETURN( NLDB_OK );
	}

	typedef enum end_transaction_type_t {
		END_TX_COMMIT=1,
		END_TX_ABORT
	}end_transaction_type_t;

	server_error_t end_transaction(const end_transaction_type_t & end_type) {
		if ( this->transaction == NULL)  {
			NL_RETURN( ErrorCode::NL_NO_TRANSACTION_BEGAN );
		}

		switch (end_type) {
			case END_TX_COMMIT:
			{
				nldb_rc_t rc = nldb_tx_commit( this->transaction );
				NL_RELEASE_ASSERT(rc == NLDB_OK); // what happens if there is not enough memory?
				break;
			}
			case END_TX_ABORT:
			{
				nldb_rc_t rc = nldb_tx_abort( this->transaction );
				NL_RELEASE_ASSERT(rc == NLDB_OK); // what happens if there is not enough memory?
				break;
			}
			default:
				NL_RELEASE_ASSERT(0);
				break;
		}

		nldb_rc_t rc = nldb_tx_destroy( transaction );
		NL_RELEASE_ASSERT(rc == NLDB_OK);

		this->transaction = NULL;

		NL_RETURN( NLDB_OK );
	}

	server_error_t abort_transaction() {
		server_error_t rc = end_transaction( END_TX_ABORT );
		NL_RETURN(rc);
	}

	server_error_t commit_transaction() {
		server_error_t rc = end_transaction( END_TX_COMMIT );
		NL_RETURN(rc);
	}

	/*! If the client did not request nl_transaction_begin, the server processes each put/get/del/cursor request with an automatically created transaction.
	 *
	 * begin the auto transaction only if there is no transaction.
	 */
	void auto_begin_transaction() {
		if (this->transaction == NULL) {
			is_auto_commit = true;
			server_error_t rc = begin_transaction();
			NL_RELEASE_ASSERT( rc == NLDB_OK);
		}
	}

	void auto_commit_transaction() {
		if (is_auto_commit) {
			server_error_t rc = commit_transaction();
			NL_RELEASE_ASSERT( rc == NLDB_OK);
			is_auto_commit = false;
		}
	}

	void auto_abort_transaction() {
		if (is_auto_commit) {
			server_error_t rc = abort_transaction();
			NL_RELEASE_ASSERT( rc == NLDB_OK);
			is_auto_commit = false;
		}
	}

	inline std::string get_tenant_id() {
		return tenant_id;
	}

private:
	// The tenant ID. Multiple tenants can use the same database.
	// To provide isolated namespace for tables for each tenant, tenant id is used for the prefix of the database table.
	std::string tenant_id;

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

    // Indicates if the auto commit mode is on.
	// If the client does not request nl_transaction_begin, the server runs in auto commit mode.
	bool is_auto_commit;

	// The mapping from cursor handle to cursor_context_t object.
	std::map<int, cursor_context_t *> cursor_map;
};

class session_map_t {

public:
	session_map_t() {
	}
	/*! Multiple tenants can use the database service. Each tenant has unique tenant_id.
	 * All table names start with the given tenant_id. It is similar to database schema name.
	 */
	session_context_t * new_session(const std::string & tenant_id) {
		/* initialize random seed: */
		srand( time(NULL) );

		int session_handle = rand();
		session_context_t * ctx = new session_context_t(tenant_id, session_handle);
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
class database_mgr_t {
public :
	database_mgr_t() {
	}

	static server_error_t create_database(const std::string & database_name) {
		nldb_db_id_t db_id;
		server_error_t rc = NLDB_OK;

		bool db_name_put_into_meta = false;
		bool nldb_db_created = false;

		rc = meta_database_t::create_database(database_name);
		if (rc) goto on_error;
		db_name_put_into_meta = true;

		rc = meta_database_t::get_database_id(database_name, &db_id);
		if (rc) goto on_error;

		{
			nldb_rc_t nrc = nldb_db_create(db_id);
			if (nrc) {
				rc = (server_error_t) nrc;
				goto on_error;
			}
			nldb_db_created = true;
		}

		// Create meta tables in the database.
		rc = meta_tables_t::create_meta_tables(db_id);
		if (rc) goto on_error;

		NL_RETURN( NLDB_OK );

on_error:
		if( nldb_db_created ) {
			nldb_rc_t nrc = nldb_db_drop(db_id);
			NL_RELEASE_ASSERT (nrc == NLDB_OK );
		}

		if (db_name_put_into_meta) {
			rc = meta_database_t::drop_database(database_name);
			NL_RELEASE_ASSERT (rc == NLDB_OK);
		}
		NL_RETURN(rc);
	}

	/*! Drop a database in Nanolat Database engine. */
	static server_error_t drop_database(const std::string & database_name) {
		nldb_db_id_t db_id;

		server_error_t rc = meta_database_t::get_database_id(database_name, &db_id);
		if (rc) NL_RETURN(rc);

		nldb_rc_t nrc = nldb_db_drop(db_id);
		if (nrc) NL_RETURN( (server_error_t)nrc );

		rc = meta_database_t::drop_database(database_name);
		NL_RELEASE_ASSERT( rc == NLDB_OK );

		NL_RETURN( NLDB_OK );
	}
	/*! Open a database,
	 *
	 */
	server_error_t open_database( const std::string & database_name, open_database_t ** o_database ) {
		NL_ASSERT(database_name != "");
		NL_ASSERT(o_database);

		nldb_db_id_t db_id;

		if (get_database(database_name)) {
			NL_RETURN( ErrorCode::NL_DATABASE_ALREADY_OPEN );
		}

		server_error_t rc = meta_database_t::get_database_id( database_name, &db_id);
		if (rc) NL_RETURN(rc);

		nldb_db_t db;
		nldb_rc_t nrc = nldb_db_open(db_id,
									 NULL, /* Master Options */
									 NULL, /* Slave Options */
									 &db);
		if (nrc) NL_RETURN( (server_error_t)nrc );


		open_database_t * opendb = new open_database_t(db);
		NL_RELEASE_ASSERT(opendb);

		// Create a mapping from database name to the open database object to for the quick look up.
		set_database( database_name, opendb);

		*o_database = opendb;

		NL_RETURN( NLDB_OK );
	}

	/*! get the Nanolat Database object by database name */
	open_database_t * get_database( const std::string & database_name ) {
		NL_ASSERT(database_name != "");
		open_database_t * database = open_databases[ database_name ];
		return database;
	}

	void set_database( const std::string & database_name, open_database_t * db) {
		open_databases[ database_name ] = db;
	}

private :
	// The mapping from database name to table_map_t table object.
	std::map<std::string, open_database_t*> open_databases;
};


class global_singleton_t {

public :
	session_map_t       session_map;
	database_mgr_t      database_mgr;

	/*! Initialize the Nanolat Server instance */
	server_error_t initialize_instance() {
		server_error_t rc;

		// TODO : When user presses CTRL+C while the server process is running,
		// Shutdown the server process. During the shutdown, call nldb_destroy().
		nldb_rc_t nrc = nldb_init();
		if (nrc) NL_RETURN( (server_error_t)nrc );

		// Step 1 : Create meta database if it does not exist.
		bool meta_database_exists;
		rc = meta_database_t::does_meta_database_exist( & meta_database_exists );
		if (rc) NL_RETURN(rc);

		if (!meta_database_exists) { // The meta database does not exist. create a one.
			rc = meta_database_t::create_meta_database();
			if (rc) NL_RETURN(rc);
		}

		// Step 2 : Create the "default" database if it does not exist.
		nldb_db_id_t default_db_id;
		rc = meta_database_t::get_database_id( std::string("default"), &default_db_id);
		if ( rc == ErrorCode::NL_DATABASE_DOES_NOT_EXIST ) { // The "default" db does not exist. Create a new one.
			rc = database_mgr.create_database( std::string("default") );
			if (rc) NL_RETURN(rc);
		} else {
			if (rc) NL_RETURN(rc);
		}

		NL_RETURN( NLDB_OK );
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
