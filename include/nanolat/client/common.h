#ifndef _NANOLAT_CLIENT_COMMON_H_
#define _NANOLAT_CLIENT_COMMON_H_ (1)

#include <cassert>
#include <string>

#define NL_ASSERT assert

namespace nanolat {
	namespace client {
		// type definition.
		typedef int64_t     key_order_t;

		typedef enum cursor_direction_t {
			NL_CURSOR_FORWARD=1,
			NL_CURSOR_BACKWARD
		} cursor_direction_t;

	    // Copied from DatabaseService_types.h
		enum error_code_t {
			NL_SUCCESS = 0,
			NL_INVALID_ARGUMENT,
			NL_INVALID_SESSION_HANDLE,
			NL_INVALID_CURSOR_HANDLE,
			NL_FAILURE
		};

		typedef struct client_status_t {
			error_code_t   error_code;
			std::string error_message;
		} client_status_t ;

	} // client
} // nanolat

#endif /* _NANOLAT_ASYNC_CLIENT_H_ */
