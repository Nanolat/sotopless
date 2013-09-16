/*
 * Logger.h
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#ifndef _NANOLAT_LOGGER_H_
#define _NANOLAT_LOGGER_H_

#include <boost/log/trivial.hpp>

namespace nanolat {

class Logger {
public:
	Logger();
	virtual ~Logger();
};

#if defined(NDEBUG)
#  define NL_LOG_TRACE(log_msg)
#  define NL_LOG_DEBUG(log_msg)
#else
#  define NL_LOG_TRACE(log_msg)   BOOST_LOG_TRIVIAL(trace) << log_msg
#  define NL_LOG_DEBUG(log_msg)   BOOST_LOG_TRIVIAL(debug) << log_msg
#endif

#define NL_LOG_INFO(log_msg)    BOOST_LOG_TRIVIAL(info) << log_msg
#define NL_LOG_WARNING(log_msg) BOOST_LOG_TRIVIAL(warning) << log_msg
#define NL_LOG_ERROR(log_msg)   BOOST_LOG_TRIVIAL(error) << log_msg
#define NL_LOG_FATAL(log_msg)   BOOST_LOG_TRIVIAL(fatal) << log_msg

} /* namespace nanolat */
#endif /* _NANOLAT_LOGGER_H_ */
