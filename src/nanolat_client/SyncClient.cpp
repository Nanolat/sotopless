#include <nanolat/client/SyncClient.h>
#include <nanolat/client/common.h>

#include "Connection.h"
#include "DatabaseService.h"

namespace nanolat {
namespace client {

/////////////////////////////////////////////////////////////////////////////////////////////////
// Session interfaces

int nl_connect(const std::string & address, const int port, connection_t ** o_conn)
{
	NL_ASSERT(o_conn);

	connection_t * conn = connection_t::new_connection(address, port);
	NL_ASSERT( conn );

	thrift::ConnectReply reply;

	conn->get_client().connect(reply);

	conn->reply_status = reply.status;

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		conn->set_session(reply.session_key);
	}

	*o_conn = conn;

	return (error_code_t)conn->reply_status.error_code;
}

int nl_disconnect(connection_t * conn)
{
	NL_ASSERT(conn);

	conn->disconnect();
	delete conn;

	return NL_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Error interfaces

std::string nl_get_error
	( connection_t * conn )
{
	NL_ASSERT(conn);
	return conn->get_error_message();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Database interfaces
int nl_database_create
	( connection_t * conn, const std::string & db_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().database_create(reply, conn->get_session(), db_name );

	conn->reply_status = reply.status;

	return (error_code_t)conn->reply_status.error_code;
}

int nl_database_drop
	( connection_t * conn, const std::string & db_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_database_use
	( connection_t * conn, const std::string & db_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Table interfaces
int nl_table_create
	( connection_t * conn, const std::string & table_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_drop
	( connection_t * conn, const std::string & table_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_get_key_count
	( connection_t * conn, const std::string & table_name )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_put
	( connection_t * conn, const std::string & table_name, const std::string& key, const std::string& value )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_get_by_key
	( connection_t * conn, const std::string & table_name, const std::string& key, key_order_t * key_order, std::string * value )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_get_by_order
	( connection_t * conn, const std::string & table_name, const key_order_t & key_order, std::string * key, std::string * value )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_table_del
	( connection_t * conn, const std::string & table_name, const std::string& key )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Transaction interfaces
int nl_transaction_begin
	( connection_t * conn )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_transaction_commit
	( connection_t * conn )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}
int nl_transaction_abort
	( connection_t * conn )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Cursor interfaces
int nl_cursor_open_forward
	( connection_t * conn, const std::string & table_name, const std::string& key, cursor_t ** cursor )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_open_backward
	( connection_t * conn, const std::string & table_name, const std::string& key, cursor_t ** cursor )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_open_forward
	( connection_t * conn, const std::string & table_name, const key_order_t& key_order, cursor_t ** cursor )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_open_backward
	( connection_t * conn, const std::string & table_name, const key_order_t& key_order, cursor_t ** cursor )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_fetch_next
	( connection_t * conn, cursor_t * cursor, std::string * key, key_order_t * key_order, std::string * value )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_fetch_prev
	( connection_t * conn, cursor_t * cursor, std::string * key, key_order_t * key_order, std::string * value )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}

int nl_cursor_close
	( connection_t * conn, cursor_t * cursor )
{
	NL_ASSERT(conn);
	conn->clear_error();

	return NL_FAILURE;
}


} // client
} // nanolat
