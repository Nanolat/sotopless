/*
 * Cursor.h
 *
 *  Created on: 2013. 8. 23.
 *      Author: ilvsusie
 */

#ifndef CURSOR_H_
#define CURSOR_H_

#include <nanolat/client/common.h>
#include <string>
#include "DatabaseService.h"

namespace nanolat {
	namespace client {

class cursor_t
{
private :
	cursor_t(thrift::CursorHandle & handle)
    {
		this->handle = handle;
    }

	thrift::CursorHandle handle;

public :
	static cursor_t * new_cursor(thrift::CursorHandle & handle )
	{
		return new cursor_t( handle );
	}

	const thrift::CursorHandle & get_handle() {
		return handle;
	}
};

	} // client
} // nanolat

#endif /* CURSOR_H_ */
