/*
 * ThriftSerializer.h
 *
 *  Created on: 2013. 9. 14.
 *      Author: ilvsusie
 */

#ifndef _THRIFT_SERIALIZER_H_
#define _THRIFT_SERIALIZER_H_

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/shared_ptr.hpp>
#include <private/assert.h>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

namespace nanolat {
namespace leaderboard {

class ThriftSerializer {
public:

	ThriftSerializer() {
		memory.reset(new TMemoryBuffer());
		protocol.reset(new TBinaryProtocol(memory));
		memory->open();
	}

	~ThriftSerializer() {
		// TODO Auto-generated destructor stub
		memory->close();
	}

	template <class T>
	void serialize(const T & thrift_object, size_t * data_length, void ** data) {
		NL_ASSERT(data);

		uint8_t* buf;
		uint32_t len;

		memory->resetBuffer();

		thrift_object.write(protocol.get());

		memory->getBuffer(&buf, &len);

		*data_length = len;
		*data = data;
	}

	template <class T>
	void deserialize(size_t data_length, void * data, T * thrift_object) {
		NL_ASSERT(thrift_object);

		memory->resetBuffer((uint8_t*)data, (uint32_t)data_length);

		thrift_object->read(protocol.get());

// TODO : Understand why available_read() is not 0
//		NL_RELEASE_ASSERT(memory->available_read() == 0);
	}

private:
	boost::shared_ptr<TMemoryBuffer>   memory;
	boost::shared_ptr<TBinaryProtocol> protocol;
};

} /* namespace leaderboard */
} /* namespace nanolat */
#endif /* _THRIFT_SERIALIZER_H_ */
