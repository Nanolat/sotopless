#include "topless.h"
#include <time.h>

namespace topl { namespace util {

timestamp_t now()
{
    time_t tm;
    time(&tm);

    return tm;
}

}} // topl::util

