/*
 * Logger.h
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Poco/Logger.h"
#include <sstream>

namespace nanolat {

class Logger {
public:
	Logger();
	virtual ~Logger();
	inline Poco::Logger * get_poco_logger() {
		return logger_;
	}
private :
	Poco::Logger * logger_;
};

extern Logger gLogger;

#define NL_LOG(poco_logging_func, stream_of_log_messages)      \
{                                                              \
	std::stringstream stream;                                  \
	stream << stream_of_log_messages;                          \
	gLogger.get_poco_logger()->poco_logging_func(stream.str()); \
}


#if defined(NDEBUG)
#  define NL_LOG_TRACE(log_msg_stream)
#  define NL_LOG_DEBUG(log_msg_stream)
#else
#  define NL_LOG_TRACE(log_msg_stream)   NL_LOG(trace, log_msg_stream)
#  define NL_LOG_DEBUG(log_msg_stream)   NL_LOG(debug, log_msg_stream)
#endif

#define NL_LOG_INFO(log_msg_stream)      NL_LOG(information, log_msg_stream)
#define NL_LOG_WARNING(log_msg_stream)   NL_LOG(warning, log_msg_stream)
#define NL_LOG_ERROR(log_msg_stream)     NL_LOG(error, log_msg_stream)
#define NL_LOG_FATAL(log_msg_stream)     NL_LOG(fatal, log_msg_stream)


} /* namespace nanolat */
#endif /* LOGGER_H_ */
