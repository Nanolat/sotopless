#include "xUnit++/xUnit++.h"
#include <nanolat/topless.h>
#include "test_common.h"
using namespace topl;

FACT("util::now returns greater than 0")
{
    Assert.True( util::now() > 0 );
}
