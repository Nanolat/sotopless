#include "DatabaseManager.h"

namespace nanolat {
namespace db {

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
	"Variable Value Size Not Supported",
	"Incompatible Client Version"
};

std::string get_nldb_error_message_format(int error_code)
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

	// Never should come here.
	NL_ASSERT(0);
	return std::string("Unknown Error");
}

} // db
} // nanolat
