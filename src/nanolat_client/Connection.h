#ifndef _ASYNC_CLIENT_IMPl_H_
#define _ASYNC_CLIENT_IMPl_H_ (1)

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include "DatabaseService.h"
#include "Util.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

namespace nanolat {
	namespace client {

    class connection_t {
	private:
    	connection_t(const std::string & address, const int port)
		{
			socket.reset( new TSocket(address, port) );
			transport.reset(new TFramedTransport(socket));
			protocol.reset(new TBinaryProtocol(transport));
			client.reset(new nanolat::thrift::DatabaseServiceClient(protocol));

			transport->open();
			reply_status = NULL;
		}
	public:
		// Factory method for the Connection class.
		static connection_t * new_connection(const std::string & address, const int port) {
			return new connection_t(address, port);
		}
		void use_database(const std::string db_name)
		{
		}
		void disconnect()
		{
			transport->close();
		}
		std::string get_error_message() {
			std::string error_message = "";
			if ( reply_status ) {
				error_message = format_with_args(reply_status->error_message_format, reply_status->error_message_args);
			}
			return error_message;
		}
		void clear_error() {
			reply_status = NULL;
		}
	private:
		shared_ptr<TSocket> socket;
		shared_ptr<TTransport> transport;
		shared_ptr<TProtocol> protocol;
		shared_ptr<nanolat::thrift::DatabaseServiceClient> client;

		// Data to keep for each connection
		nanolat::thrift::ReplyStatus * reply_status;
	};

	} // client
} // nanolat

#endif // _ASYNC_CLIENT_IMPl_H_
