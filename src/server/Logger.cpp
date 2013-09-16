/*
 * Logger.cpp
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#include "Logger.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/log/attributes/clock.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

namespace nanolat {


Logger::Logger() {
	// TODO Auto-generated constructor stub

	logging::add_file_log
	(
		keywords::file_name = "nanolat-%N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::format = "[%TimeStamp%]: %Message%"
	);

    logging::core::get()->add_global_attribute(
        "TimeStamp",
        attrs::local_clock());

	logging::core::get()->set_filter
	(
#if defined(NDEBUG)
			logging::trivial::severity >= logging::trivial::info
#else
			logging::trivial::severity >= logging::trivial::trace
#endif
	);
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

} /* namespace nanolat */
