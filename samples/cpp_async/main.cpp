// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <nanolat/client/AsyncClient.h>
#include <nanolat/client/common.h>
#include <stdio.h>

using namespace ::nanolat::client;

void print_usage()
{
	printf("cpp_sync <server address> <port>\n");
}

connection_t * g_conn = NULL;

void show_error_and_exit(client_status_t * status)
{
	printf("Error(%d) : %s.\n", status->error_code, status->error_message.c_str() );
	exit(-1);
}

void on_disconnect(client_status_t * status)
{
	if (status && status->error_code != NL_SUCCESS)
	{
		show_error_and_exit(status);
	}
}

void on_create_database(client_status_t * status, const std::string & database_name)
{
	if (status && status->error_code != NL_SUCCESS)
	{
		show_error_and_exit(status);
	}

	printf("Database created : %s.\n", database_name.c_str());

	nl_async_disconnect(g_conn, on_disconnect);
}

void on_connect(client_status_t * status, connection_t * connection)
{
	if (status && status->error_code != NL_SUCCESS)
	{
		show_error_and_exit(status);
	}

	g_conn = connection;

	nl_async_table_create(g_conn, "mydb", on_create_database);
}



int main(int argc, const char **argv)
{

	if (argc != 3)
	{
		print_usage();
		return -1;
	}

	const char * address = argv[1];
	int port = atoi(argv[2]);

	nl_async_connect(address, port, on_connect);

	return 0;
}

