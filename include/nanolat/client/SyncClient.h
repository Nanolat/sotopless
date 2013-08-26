#ifndef _NANOLAT_CLIENT_SYNC_CLIENT_H_
#define _NANOLAT_CLIENT_SYNC_CLIENT_H_ (1)

#include <nanolat/client/common.h>
#include <string>
namespace nanolat {
	namespace client {

		class connection_t;
		class cursor_t;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Session interfaces
		int nl_connect
			( const std::string & address, const int port, connection_t ** o_conn );
		int nl_disconnect
			(connection_t ** o_conn);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Error interfaces
		std::string nl_get_error
			( connection_t * conn );

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Database interfaces
		int nl_database_create
			( connection_t * conn, const std::string & db_name );
		int nl_database_drop
			( connection_t * conn, const std::string & db_name );
		int nl_database_use
			( connection_t * conn, const std::string & db_name );

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Table interfaces
		int nl_table_create
			( connection_t * conn, const std::string & table_name );
		int nl_table_drop
			( connection_t * conn, const std::string & table_name );
		int nl_table_get_key_count
			( connection_t * conn, const std::string & table_name, key_order_t * o_key_count );
		int nl_table_put
			( connection_t * conn, const std::string & table_name, const std::string& key, const std::string& value );
		int nl_table_get_by_key
			( connection_t * conn, const std::string & table_name, const std::string& key, key_order_t * o_key_order, std::string * o_value );
		int nl_table_get_by_order
			( connection_t * conn, const std::string & table_name, const key_order_t & key_order, std::string * o_key, std::string * o_value );
		int nl_table_del
			( connection_t * conn, const std::string & table_name, const std::string& key );

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Transaction interfaces
		int nl_transaction_begin
			( connection_t * conn );
		int nl_transaction_commit
			( connection_t * conn );
		int nl_transaction_abort
			( connection_t * conn );

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Cursor interfaces
		int nl_cursor_open_by_key
			( connection_t * conn, const std::string & table_name, const std::string& key, const cursor_direction_t & direction, cursor_t ** o_cursor );

		int nl_cursor_open_by_order
			( connection_t * conn, const std::string & table_name, const key_order_t& key_order, const cursor_direction_t & direction, cursor_t ** o_cursor );

		int nl_cursor_fetch
			( connection_t * conn, cursor_t * cursor, const cursor_direction_t & direction, std::string * o_key, key_order_t * o_key_order, std::string * o_value );

		int nl_cursor_close
			( connection_t * conn, cursor_t ** o_cursor );

	} // client
} // nanolat

#endif /* _NANOLAT_CLIENT_SYNC_CLIENT_H_ */
