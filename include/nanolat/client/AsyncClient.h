#ifndef _NANOLAT_CLIENT_ASYNC_CLIENT_H_
#define _NANOLAT_CLIENT_ASYNC_CLIENT_H_ (1)

#include <nanolat/client/common.h>
#include <string>

namespace nanolat {
	namespace client {
		class connection_t;
		class cursor_t;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Callback functions

		typedef void (*on_complete_t)(client_status_t * status);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Session interfaces
		typedef void (*on_connect_t)(client_status_t * status, connection_t * connection);

		void nl_async_connect(const std::string & address, const int port, on_connect_t on_connect);

		void nl_async_disconnect(connection_t * conn, on_complete_t on_complete);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Error interfaces
		std::string nl_async_get_error
			( connection_t * conn );

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Database interfaces
		typedef void (*on_complete_database_t)(client_status_t * status, const std::string & db_name);

		void nl_async_database_create
			( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete);
		void nl_async_database_drop
			( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete);
		void nl_async_database_use
			( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Table interfaces
		typedef void (*on_complete_table_t)(client_status_t * status, const std::string & table_name);

		void nl_async_table_create
			( connection_t * conn, const std::string & table_name, on_complete_table_t on_complete);
		void nl_async_table_drop
			( connection_t * conn, const std::string & table_name, on_complete_table_t on_complete);

		typedef void (*on_table_get_key_count_t)(client_status_t * status, const std::string & table_name, const key_order_t & key_count);
		void nl_async_table_get_key_count
			( connection_t * conn, const std::string & table_name, on_table_get_key_count_t on_complete);

		typedef void (*on_table_put_t)(client_status_t * status, const std::string & table_name, const std::string& key, const std::string& value );
		void nl_async_table_put
			( connection_t * conn, const std::string & table_name, const std::string& key, const std::string& value, on_table_put_t on_complete);

		typedef void (*on_table_get_by_key_t)(client_status_t * status, const std::string&table_name, const std::string& key, const key_order_t& key_order, const std::string& value);
		void nl_async_table_get_by_key
			( connection_t * conn, const std::string & table_name, on_table_get_by_key_t on_complete );

		typedef void (*on_table_get_by_order_t)(client_status_t * status, const std::string&table_name, const key_order_t & key_order, const std::string & key, const std::string & value);
		void nl_async_table_get_by_order
			( connection_t * conn, const std::string & table_name, on_table_get_by_order_t on_complete  );

		typedef void (*on_table_del_t)(client_status_t * status, const std::string & table_name, const std::string& key );
		void nl_async_table_del
			( connection_t * conn, const std::string & table_name, const std::string& key, on_table_del_t on_complete);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Transaction interfaces
		void nl_async_transaction_begin
			( connection_t * conn, on_complete_t on_complete);
		void nl_async_transaction_commit
			( connection_t * conn, on_complete_t on_complete);
		void nl_async_transaction_abort
			( connection_t * conn, on_complete_t on_complete);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// Cursor interfaces
		typedef void (*on_cursor_open_by_key_t)(client_status_t * status, const cursor_direction_t & dir, const std::string&table_name, const std::string & key, cursor_t * cursor);
		void nl_async_cursor_open
			( connection_t * conn, const std::string & table_name, const cursor_direction_t & dir, const std::string& key, on_cursor_open_by_key_t on_complete );

		typedef void (*on_cursor_open_by_order_t)(client_status_t * status, const cursor_direction_t & dir, const std::string&table_name, const std::string & key, cursor_t * cursor);
		void nl_async_cursor_open
			( connection_t * conn, const std::string & table_name, const cursor_direction_t & dir, const key_order_t& key_order, on_cursor_open_by_order_t on_complete );

		typedef void (*on_cursor_fetch_next_t)(client_status_t * status, const std::string & table_name, const cursor_direction_t & dir, const std::string & key, const key_order_t & key_order, const std::string & value );
		void nl_async_cursor_fetch_next
			( connection_t * conn, cursor_t * cursor, on_cursor_fetch_next_t on_complete);

		typedef void (*on_cursor_fetch_prev_t)(client_status_t * status, const std::string & table_name, const cursor_direction_t & dir, const std::string & key, const key_order_t & key_order, const std::string & value );
		void nl_async_cursor_fetch_prev
			( connection_t * conn, cursor_t * cursor, on_cursor_fetch_prev_t on_complete );

		typedef void (*on_cursor_close_t)(client_status_t * status, const std::string & table_name);
		void nl_async_cursor_close
			( connection_t * conn, cursor_t * cursor, on_cursor_close_t on_complete);

	} // client
} // nanolat

#endif /* _NANOLAT_CLIENT_ASYNC_CLIENT_H_ */
