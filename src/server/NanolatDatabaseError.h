/*
 * NanolatDatabaseError.h
 *
 *  Created on: 2013. 9. 14.
 *      Author: ilvsusie
 */

#ifndef NANOLAT_DATABASEERROR_H_
#define NANOLAT_DATABASEERROR_H_ (1)

#include <string>

namespace nanolat {
namespace db {

std::string get_nldb_error_message_format(int error_code);

} // db
} // nanolat

#endif /* NANOLATDATABASEERROR_H_ */
