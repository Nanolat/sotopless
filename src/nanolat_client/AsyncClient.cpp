#include <future>
#include <nanolat/client/AsyncClient.h>
#include "Connection.h"
#include "Transaction.h"
#include "Cursor.h"

namespace nanolat {
namespace client {

/////////////////////////////////////////////////////////////////////////////////////////////////
// Callback functions
void nl_async_connect(const std::string & address, const int port, on_connect_t on_connect) {

	// Connecting to the server takes time. Do it in another thread.
	std::future<connection_t*> futureConnect = std::async( std::launch::async, connection_t::new_connection, address, port);

	connection_t * conn = futureConnect.get();

	on_connect(NULL, conn );
}

void nl_async_disconnect(connection_t * conn, on_complete_t on_complete)
{
	std::future<void> futureDisconnect = std::async( [=]()  {
			conn->disconnect();
			delete conn;
		}
	);

	futureDisconnect.wait();

	on_complete(NULL);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Error interfaces
std::string nl_async_get_error
	( connection_t * conn )
{
	NL_ASSERT(conn);
	return conn->get_error_message();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Database interfaces
void nl_async_database_create
	( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete)
{

}

void nl_async_database_drop
	( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete)
{

}

void nl_async_database_use
	( connection_t * conn, const std::string & db_name, on_complete_database_t on_complete)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Table interfaces
void nl_async_table_create
	( connection_t * conn, const std::string & table_name, on_complete_table_t on_complete)
{

}

void nl_async_table_drop
	( connection_t * conn, const std::string & table_name, on_complete_table_t on_complete)
{

}

void nl_async_table_get_key_count
	( connection_t * conn, const std::string & table_name, on_table_get_key_count_t on_complete)
{

}

void nl_async_table_put
	( connection_t * conn, const std::string & table_name, const std::string& key, const std::string& value, on_table_put_t on_complete)
{

}

void nl_async_table_get_by_key
	( connection_t * conn, const std::string & table_name, on_table_get_by_key_t on_complete )
{

}

void nl_async_table_get_by_order
	( connection_t * conn, const std::string & table_name, on_table_get_by_order_t on_complete  )
{

}

void nl_async_table_del
	( connection_t * conn, const std::string & table_name, const std::string& key, on_table_del_t on_complete)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Transaction interfaces
void nl_async_transaction_begin
	( connection_t * conn, on_complete_t on_complete)
{

}

void nl_async_transaction_commit
	( connection_t * conn, on_complete_t on_complete)
{

}

void nl_async_transaction_abort
	( connection_t * conn, on_complete_t on_complete)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Cursor interfaces
void nl_async_cursor_open
	( connection_t * conn, const std::string & table_name, const cursor_direction_t & dir, const std::string& key, on_cursor_open_by_key_t on_complete )
{

}

void nl_async_cursor_open
	( connection_t * conn, const std::string & table_name, const cursor_direction_t & dir, const key_order_t& key_order, on_cursor_open_by_order_t on_complete )
{

}

void nl_async_cursor_fetch_next
	( connection_t * conn, cursor_t * cursor, on_cursor_fetch_next_t on_complete)
{

}

void nl_async_cursor_fetch_prev
	( connection_t * conn, cursor_t * cursor, on_cursor_fetch_prev_t on_complete )
{

}

void nl_async_cursor_close
	( connection_t * conn, cursor_t * cursor, on_cursor_close_t on_complete)
{

}

} // client
} // nanolat
