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
		typedef enum error_code_t {
			NL_SUCCESS = 0,
			NL_ERROR_CODE_START          = -1000,
			NL_FAILURE                   = -1000,
			NL_INVALID_ARGUMENT          = -1001,
			NL_INVALID_SESSION_HANDLE    = -1002,
			NL_INVALID_CURSOR_HANDLE     = -1003,
			NL_TRANSACTION_ALREADY_BEGAN = -1004,
			NL_NO_TRANSACTION_BEGAN      = -1005,
			NL_DATABASE_ALREADY_EXISTS   = -1006,
			NL_DATABASE_DOES_NOT_EXIST   = -1007,
			NL_DATABASE_ALREADY_OPEN     = -1008,
			NL_DATABASE_NOT_OPEN         = -1009,
			NL_TABLE_ALREADY_EXISTS      = -1010,
			NL_TABLE_DOES_NOT_EXIST      = -1011,
			NL_TABLE_ALREADY_OPEN        = -1012,
			NL_TABLE_NOT_OPEN            = -1013,
			NL_CURSOR_HAS_NO_MORE_KEYS   = -1014,
			NL_ERROR_CODE_END            = -1014
		} error_code_t ;

		typedef struct client_status_t {
			error_code_t   error_code;
			std::string error_message;
		} client_status_t ;

	} // client
} // nanolat

#endif /* _NANOLAT_ASYNC_CLIENT_H_ */
