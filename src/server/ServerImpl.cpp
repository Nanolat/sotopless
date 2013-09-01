#include "ServerImpl.h"

namespace nanolat {
namespace server {

/* The error message format to send back to client in case there is any Nanolat Database error. */
static const char * nldb_error_message_format[] = {
	"General Failure in Nanolat Database",
	"Invalid Argument",
	"Database Already Exists",
	"Database Not Found",
	"Table Already Exists",
	"Table Not Found",
	"Transaction Already Began",
	"Transaction Not Began",
	"Transaction Not Ended",
	"Key Not Found",
	"Cursor Not Open",
	"End of Cursor Iteration",
	"Invalid Cursor Iteration",
	"Transaction Already Initialized",
	"Transaction Already Destroyed",
	"No More Plug-in Slots",
	"Not Enough Memory",
	"Not Implemented Yet",
	"Unable to Initialized Replication Message",
	"Unsupported Feature",
	"Variable Key Size Not Supported",
	"Variable Value Size Not Supported"
};
/*
#if (sizeof(nldb_error_message_format)/sizeof(char*)) <> (NLDB_ERROR_CODE_START - NLDB_ERROR_CODE_END + 1)
#   error "The number of error messages in nldb_error_message_format should match the one defined in nldb/nldb_common.h"
#endif
*/

static const char * server_error_message_format[] = {
	"Database Server General Failure",
	"Invalid Argument",
	"Invalid Session Handle",
	"Invalid Cursor Handle",
	"Transaction Already Began",
	"No Transaction Began",
	"Database Already Exists",
	"Database Does Not Exist",
	"Database Already Open",
	"Database Not Open",
	"Table Already Exists",
	"Table Does Not Exist",
	"Table Already Open",
	"Table Not Open",
	"Cursor Has No More Keys"
};
/*
#if (sizeof(server_error_message_format)/sizeof(char*)) <> (NL_ERROR_CODE_START - NL_ERROR_CODE_END + 1)
#   error "The number of error messages in nldb_error_message_format should match the one defined in nldb/nldb_common.h"
#endif
*/

std::string get_error_message_format(int error_code)
{
	NL_ASSERT(error_code < 0);
	NL_ASSERT(NLDB_ERROR_CODE_START < 0);
	NL_ASSERT(NLDB_ERROR_CODE_END < 0);
	NL_ASSERT(NLDB_ERROR_CODE_START > NLDB_ERROR_CODE_END);

	// Is the error code in the range the one defined in Nanolat Database Engine?
	if ( NLDB_ERROR_CODE_START >= error_code && error_code >= NLDB_ERROR_CODE_END )
	{
		// Index in the nldb_error_message_format.
		int index = NLDB_ERROR_CODE_START - error_code;
		std::string message = std::string( nldb_error_message_format[index] );
		return message;
	}

	NL_ASSERT(NL_ERROR_CODE_START < 0);
	NL_ASSERT(NL_ERROR_CODE_END < 0);
	NL_ASSERT(NL_ERROR_CODE_START > NL_ERROR_CODE_END);

	// Is the error code in the range the one defined in the Database Server?
	if ( NL_ERROR_CODE_START >= error_code && error_code >= NL_ERROR_CODE_END )
	{
		// Index in the nldb_error_message_format.
		int index = NL_ERROR_CODE_START - error_code;
		std::string message = std::string( server_error_message_format[index] );
		return message;
	}

	// Never should come here.
	NL_ASSERT(0);
	return std::string("Unknown Error");
}

} // server
} // nanolat
