/*
 * Logger.cpp
 *
 *  Created on: 2013. 9. 16.
 *      Author: ilvsusie
 */

#include "Logger.h"

#include "Poco/Logger.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Message.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

using Poco::Channel;
using Poco::Formatter;

using Poco::SimpleFileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;

namespace nanolat {

Logger gLogger;

Logger::Logger()  {

	AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
	pChannel->setProperty("path", "nanolat.log");
	pChannel->setProperty("rotation", "10 M");

    //"%d-%m-%Y %H:%M:%S: %t"
    AutoPtr<Formatter> formatter(new PatternFormatter("%d-%m-%Y %H:%M:%S %s: %t"));
    AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, pChannel));

    Poco::Logger::root().setChannel(formattingChannel);

    logger_ = & Poco::Logger::get("SoTopless");

#if defined(NDEBUG)
    logger_->setLevel(Poco::Message::PRIO_INFORMATION);
#else
    // Debug mode -> PRIO_TRACE
	logger_->setLevel(Poco::Message::PRIO_TRACE);
#endif

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

} /* namespace nanolat */
