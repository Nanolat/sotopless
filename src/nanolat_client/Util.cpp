#include <boost/format.hpp>
#include "Util.h"

std::string format_with_args(const std::string& format_string, const std::vector<std::string>& args)
{
    boost::format f(format_string);
    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        f % *it;
    }
    return f.str();
}
