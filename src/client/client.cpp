#include "DatabaseService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

namespace nanolat {
namespace client {

typedef struct ClientError
{
	int32_t error_code;
} ClientError;

class Connection {
private:
	Connection(const std::string & address, const int port) :
		socket( new TSocket(address, port) ),
		transport(new TFramedTransport(socket)),
		protocol(new TBinaryProtocol(transport)),
		client(protocol)
	{
	    transport->open();
	}
public:
	// Factory method for the Connection class.
	static Connection * new_connection(const std::string & address, const int port) {
		return new Connection(address, port);
	}
	void use_database(const std::string db_name)
	{

	}
	void disconnect()
	{
		transport->close();
	}
private:
    shared_ptr<TSocket> socket;
    shared_ptr<TTransport> transport;
    shared_ptr<TProtocol> protocol;
    nanolat::thrift::DatabaseServiceClient client;
};

typedef void (*on_connect_t)(ClientError * error, Connection * connection);

void connect(const std::string & address, const int port, on_connect_t on_connect) {
	Connection * conn = Connection::new_connection(address, port);
	on_connect(NULL, conn);
}

void disconnect(Connection * conn)
{
	// do something here...
	conn->disconnect();
	delete conn;
}

} // client
} // nanolat
