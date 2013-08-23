#include "DatabaseService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

namespace nanolat {

class DatabaseServiceHandler : virtual public nanolat::thrift::DatabaseServiceIf {
 public:
  DatabaseServiceHandler() {
    // Your initialization goes here
  }

  int32_t ping() {
    // Your implementation goes here
    printf("ping\n");
    return -1;
  }

};

namespace server {
	int listen(int port) {
	  shared_ptr<DatabaseServiceHandler> handler(new DatabaseServiceHandler());
	  shared_ptr<TProcessor> processor(new nanolat::thrift::DatabaseServiceProcessor(handler));
	  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	  server.serve();
	  return 0;
	}
} // server

} // nanolat
