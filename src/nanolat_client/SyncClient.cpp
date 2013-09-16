#include <nanolat/client/SyncClient.h>
#include <nanolat/client/common.h>

#include "Connection.h"
#include "Cursor.h"
#include "DatabaseService.h"
#include "DatabaseService_constants.h"

// TODO : add test for incompatible client version.
#define NLDB_CLIENT_VERSION "v1.0"

#define CHECK_ARGUMENT(exp) if (!(exp)) return NL_INVALID_ARGUMENT;

namespace nanolat {
namespace client {

/////////////////////////////////////////////////////////////////////////////////////////////////
// Session interfaces

static inline void copy_reply_status(thrift::ReplyStatus & dest, const thrift::ReplyStatus & src) {
	// If
	if (src.error_code == thrift::ErrorCode::NL_SUCCESS) {
		// No need to copy the reply status because we clear the connection_t::reply_status for each client request.
	} else {
		// ReplyStatus has std::vector<std::string> error_message_args,
		// but the std::string in the vector are deep-copied by std::vector.
		// So, dest will have its own data without any reference to src.
		dest = src;

	}
}

int nl_connect(const std::string & address, const int port, connection_t ** o_conn)
{
	CHECK_ARGUMENT(address != "");
	CHECK_ARGUMENT(port > 0);
	CHECK_ARGUMENT(o_conn != NULL);

	connection_t * conn = connection_t::new_connection(address, port);
	NL_ASSERT( conn );

	thrift::ConnectReply reply;

	// The Tenant ID is used for cloud service. Because these APIs are not for cloud service, pass the defaul tenant ID, "D".
	// The tenant ID will become a prefix for all table names.
	conn->get_client().connect(reply, thrift::g_DatabaseService_constants.PROTOCOL_VERSION, "D"/*The default tenant */ );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		conn->set_session(reply.session_handle);
		*o_conn = conn;
	}
	else
	{
		// close the transport.
		conn->disconnect();
		delete conn;
		*o_conn = NULL;
	}

	return (error_code_t)reply.status.error_code;
}

/*! Disconnects the client from the server.
 * When the client gets disconnected successfully, *o_conn is set to NULL, to avoid referencing the deleted connection object.
 */
int nl_disconnect(connection_t ** o_conn)
{
	CHECK_ARGUMENT(o_conn != NULL);

	connection_t * conn = *o_conn;
	NL_ASSERT(conn);

	thrift::DefaultReply reply;

	// Send disconnect request to the server so that the server can clean up all data related to the session.
	conn->get_client().disconnect(reply, conn->get_session() );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		// conn->disconnect closes the transport.
		// close the transport only if the disconnect request was successful.
		conn->disconnect();
		delete conn;
		*o_conn = NULL;
	}

	return (error_code_t)reply.status.error_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Error interfaces

std::string nl_get_error
	( connection_t * conn )
{
	if (conn==NULL)
		return "";

	return conn->get_error_message();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Database interfaces
int nl_database_create
	( connection_t * conn, const std::string & db_name )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(db_name != "");

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().database_create(reply, conn->get_session(), db_name );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_database_drop
	( connection_t * conn, const std::string & db_name )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(db_name != "");

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().database_drop(reply, conn->get_session(), db_name );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_database_use
	( connection_t * conn, const std::string & db_name )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(db_name != "");

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().database_use(reply, conn->get_session(), db_name );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Table interfaces
int nl_table_create
	( connection_t * conn, const std::string & table_name )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().table_create(reply, conn->get_session(), table_name );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_drop
	( connection_t * conn, const std::string & table_name )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().table_drop(reply, conn->get_session(), table_name );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_get_key_count
	( connection_t * conn, const std::string & table_name, key_order_t * o_key_count )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(o_key_count != NULL);


	conn->clear_error();

	thrift::TableStatReply reply;

	conn->get_client().table_stat(reply, conn->get_session(), table_name );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		*o_key_count = reply.key_count;
	}
	else
	{
		*o_key_count = 0;
	}


	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_put
	( connection_t * conn, const std::string & table_name, const std::string& key, const std::string& value )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key.length() > 0);
	// nothing to check for value.

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().table_put(reply, conn->get_session(), table_name, key, value );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_get_by_key
	( connection_t * conn, const std::string & table_name, const std::string& key, key_order_t * o_key_order, std::string * o_value )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key.length() > 0);
	CHECK_ARGUMENT(o_key_order != NULL);
	CHECK_ARGUMENT(o_value != NULL);

	conn->clear_error();

	thrift::TableGetReply reply;

	conn->get_client().table_get_by_key(reply, conn->get_session(), table_name, key );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		NL_ASSERT( reply.key == key );
		*o_key_order = reply.key_order;
		*o_value     = reply.value;
	}
	else
	{
		*o_key_order = 0;
		*o_value = "";
	}

	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_get_by_order
	( connection_t * conn, const std::string & table_name, const key_order_t & key_order, std::string * o_key, std::string * o_value )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key_order > 0);
	CHECK_ARGUMENT(o_key != NULL);
	CHECK_ARGUMENT(o_value != NULL);

	conn->clear_error();

	thrift::TableGetReply reply;

	conn->get_client().table_get_by_order(reply, conn->get_session(), table_name, key_order );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		NL_ASSERT( reply.key_order == key_order );
		*o_key       = reply.key;
		*o_value     = reply.value;
	}
	else
	{
		*o_key = "";
		*o_value = "";
	}

	return (error_code_t)conn->reply_status.error_code;
}

int nl_table_del
	( connection_t * conn, const std::string & table_name, const std::string& key )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key.length() > 0);

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().table_del(reply, conn->get_session(), table_name, key );

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Transaction interfaces
int nl_transaction_begin
	( connection_t * conn )
{
	CHECK_ARGUMENT(conn != NULL);

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().transaction_begin(reply, conn->get_session());

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

int nl_transaction_commit
	( connection_t * conn )
{
	CHECK_ARGUMENT(conn != NULL);

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().transaction_commit(reply, conn->get_session());

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}
int nl_transaction_abort
	( connection_t * conn )
{
	CHECK_ARGUMENT(conn != NULL);

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().transaction_abort(reply, conn->get_session());

	copy_reply_status(conn->reply_status, reply.status);

	return (error_code_t)conn->reply_status.error_code;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Cursor interfaces
static inline thrift::CursorDirection::type get_thrift_cursor_direction(const cursor_direction_t & direction) {
	if (direction == NL_CURSOR_FORWARD)
		return thrift::CursorDirection::CD_FORWARD;
	if (direction == NL_CURSOR_BACKWARD)
		return thrift::CursorDirection::CD_BACKWARD;

	NL_ASSERT(0);
	return (thrift::CursorDirection::type)0;
}

int nl_cursor_open_by_key
	( connection_t * conn, const std::string & table_name, const std::string& key, const cursor_direction_t & direction, cursor_t ** o_cursor )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key.length() > 0);
	CHECK_ARGUMENT(direction == NL_CURSOR_FORWARD || direction == NL_CURSOR_BACKWARD );
	CHECK_ARGUMENT(o_cursor != NULL);

	conn->clear_error();

	thrift::CursorOpenReply reply;

	conn->get_client().cursor_open_by_key(reply, conn->get_session(), table_name,
  			                              get_thrift_cursor_direction(direction), key);

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		cursor_t * cursor = cursor_t::new_cursor( reply.cursor_handle );
		NL_ASSERT( cursor );

		*o_cursor = cursor;
	}
	else
	{
		*o_cursor = NULL;
	}

	return (error_code_t)reply.status.error_code;

}


int nl_cursor_open_by_order
	( connection_t * conn, const std::string & table_name, const key_order_t& key_order, const cursor_direction_t & direction, cursor_t ** o_cursor )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(table_name != "");
	CHECK_ARGUMENT(key_order > 0);
	CHECK_ARGUMENT(direction == NL_CURSOR_FORWARD || direction == NL_CURSOR_BACKWARD );
	CHECK_ARGUMENT(o_cursor != NULL);

	conn->clear_error();

	thrift::CursorOpenReply reply;

	conn->get_client().cursor_open_by_order(reply, conn->get_session(), table_name,
			                                get_thrift_cursor_direction(direction), key_order);

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		cursor_t * cursor = cursor_t::new_cursor( reply.cursor_handle );
		NL_ASSERT( cursor );

		*o_cursor = cursor;
	}
	else
	{
		*o_cursor = NULL;
	}

	return (error_code_t)reply.status.error_code;

}

int nl_cursor_fetch
	( connection_t * conn, cursor_t * cursor, const cursor_direction_t & direction, std::string * o_key, key_order_t * o_key_order, std::string * o_value )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(cursor != NULL);
	CHECK_ARGUMENT(direction == NL_CURSOR_FORWARD || direction == NL_CURSOR_BACKWARD );
	CHECK_ARGUMENT(o_key != NULL);
	CHECK_ARGUMENT(o_key_order != NULL);
	CHECK_ARGUMENT(o_value != NULL);

	conn->clear_error();

	thrift::CursorFetchReply reply;

	conn->get_client().cursor_fetch(reply, conn->get_session(), cursor->get_handle() );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		*o_key       = reply.key;
		*o_key_order = reply.key_order;
		*o_value     = reply.value;
	}
	else
	{
		*o_key       = "";
		*o_key_order = 0;
		*o_value     = "";
	}

	return (error_code_t)conn->reply_status.error_code;
}

/*! Closes an open cursor.
 * When the cursor gets closed successfully, *o_cursor is set to NULL, to avoid referencing the closed cursor object.
 */
int nl_cursor_close
	( connection_t * conn, cursor_t ** o_cursor )
{
	CHECK_ARGUMENT(conn != NULL);
	CHECK_ARGUMENT(o_cursor != NULL);
	CHECK_ARGUMENT(*o_cursor != NULL);

	cursor_t * cursor = *o_cursor;

	conn->clear_error();

	thrift::DefaultReply reply;

	conn->get_client().cursor_close(reply, conn->get_session(), cursor->get_handle() );

	copy_reply_status(conn->reply_status, reply.status);

	if (reply.status.error_code == thrift::ErrorCode::NL_SUCCESS)
	{
		// Delete the cursor only if the server replied with success for the cursor_close request.
		delete cursor;
		*o_cursor = NULL;
	}
	return (error_code_t)conn->reply_status.error_code;
}


} // client
} // nanolat
