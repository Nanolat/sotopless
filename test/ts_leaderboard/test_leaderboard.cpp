#include "xUnit++/xUnit++.h"
#include <nanolat/topless.h>
#include <time.h>
#include "test_common.h"
using namespace topl;

THEORY("Create/Get/Drop Leaderboard Should Succeed", (const std::string & leaderboard_identity),
   std::make_tuple("a"),
   std::make_tuple("a1"),
   std::make_tuple("a01"),
   std::make_tuple("1"),
   std::make_tuple("1a"),
   std::make_tuple("01"),
   std::make_tuple("01a"),
   std::make_tuple("759bfd0a-2c91-4e45-a35c-779740ac5e77") // GUID
)
{
    leaderboard::handle_t searched_handle;

    // The leaderboard does not exist yet. leaderboard::get, should fail.
    ASSERT_FAILURE( leaderboard::get(leaderboard_identity, &searched_handle) );

    leaderboard::handle_t created_handle;
    ASSERT_SUCCESS( leaderboard::create(leaderboard_identity, &created_handle) );

    searched_handle = 0;
    ASSERT_SUCCESS( leaderboard::get(leaderboard_identity, &searched_handle) );

    Assert.Equal( created_handle, searched_handle );

    // Dropping a leaderboard should succeed.
    ASSERT_SUCCESS( leaderboard::drop(searched_handle) );
}

FACT("Purge/Drop Leaderboard With Invalid Handle Should Fail")
{
    leaderboard::handle_t invalidHandle = 0;
    util::timestamp_t cut_date = 0;

    time_t  tm;
    time(&tm);
    cut_date = tm;

    ASSERT_FAILURE( leaderboard::purge(invalidHandle, cut_date) );
    ASSERT_FAILURE( leaderboard::drop(invalidHandle) );
}

FACT("No Leaderboard At The Beginning")
{
    std::vector<leaderboard::leaderboard_desc_t> leaderboards;
    ASSERT_SUCCESS( leaderboard::list( & leaderboards) );

    // At the beginning, the there is no leaderboard at all.
    Assert.Equal(leaderboards.size(), 0);
}

typedef std::vector<std::string> string_vector_t;
typedef enum lb_command_t {
    LB_CREATE, LB_GET, LB_PURGE, LB_DROP
}lb_command_t;

void issue_leaderboad_command(lb_command_t command, const std::string & leaderboard_identity)
{
    using namespace xUnitpp;

    switch (command) {
        case LB_CREATE:
        {
            leaderboard::handle_t created_handle;
            ASSERT_SUCCESS( leaderboard::create(leaderboard_identity, &created_handle) );
        }
        break;
        case LB_GET:
        {
            leaderboard::handle_t searched_handle;
            ASSERT_SUCCESS( leaderboard::get(leaderboard_identity, &searched_handle) );
        }
        break;
        case LB_PURGE:
        {
            util::timestamp_t cut_date = util::now() - 24 * 60 * 60 * 1000;
            leaderboard::handle_t searched_handle;
            ASSERT_SUCCESS( leaderboard::get(leaderboard_identity, &searched_handle) );

            ASSERT_SUCCESS( leaderboard::purge(searched_handle, cut_date) );
        }
        break;
        case LB_DROP:
        {
            leaderboard::handle_t searched_handle;
            ASSERT_SUCCESS( leaderboard::get(leaderboard_identity, &searched_handle) );

            ASSERT_SUCCESS( leaderboard::drop(searched_handle) );
        }
        break;
        default :
        {
            Assert.Fail();
        }
        break;
    }
}

void check_leaderboard_list(lb_command_t command, const std::string & leaderboard_identity, string_vector_t expected_identities)
{
    using namespace xUnitpp;

    std::vector<leaderboard::leaderboard_desc_t> leaderboards;

    issue_leaderboad_command( command, leaderboard_identity);

    ASSERT_SUCCESS( leaderboard::list( & leaderboards) );

    auto vectorItemComparison =
            [](leaderboard::leaderboard_desc_t left, std::string expected_identity)
            {
                return left.identity == expected_identity;
            };

    Assert.Equal(leaderboards.begin(), leaderboards.end(),
                 expected_identities.begin(), expected_identities.end(),
                 vectorItemComparison);
}
/* This did not work, because xUnit++ does not apply tuples serially.
THEORY("List of Leaderboards Should Return Valid Results", (lb_command_t command, const std::string & leaderboard_identity, string_vector_t expected_identities),
        std::make_tuple(LB_CREATE, "test1",  string_vector_t {"test1"}),
        std::make_tuple(LB_CREATE, "test2",  string_vector_t {"test1", "test2"}),
        std::make_tuple(LB_GET,    "test1",  string_vector_t {"test1", "test2"}),
        std::make_tuple(LB_GET,    "test2",  string_vector_t {"test1", "test2"}),
        std::make_tuple(LB_PURGE,  "test1",  string_vector_t {"test1", "test2"}),
        std::make_tuple(LB_PURGE,  "test2",  string_vector_t {"test1", "test2"}),
        std::make_tuple(LB_DROP,   "test1",  string_vector_t {"test2"}),
        std::make_tuple(LB_DROP,   "test2",  string_vector_t {})
)
{
    check_leaderboard_list(command, leaderboard_identity, expected_identities)
}
*/

FACT("List of Leaderboards Should Return Valid Results") {
    check_leaderboard_list(LB_CREATE, "test1",  string_vector_t {"test1"});

    check_leaderboard_list(LB_CREATE, "test2",  string_vector_t {"test1", "test2"});

    check_leaderboard_list(LB_GET,    "test1",  string_vector_t {"test1", "test2"});

    check_leaderboard_list(LB_GET,    "test2",  string_vector_t {"test1", "test2"});

    check_leaderboard_list(LB_PURGE,  "test1",  string_vector_t {"test1", "test2"});

    check_leaderboard_list(LB_PURGE,  "test2",  string_vector_t {"test1", "test2"});

    check_leaderboard_list(LB_DROP,   "test1",  string_vector_t {"test2"});

    check_leaderboard_list(LB_DROP,   "test2",  string_vector_t {});
}


THEORY("Create Leaderboard with Invalid Identiy Should Fail", (const std::string & leaderboard_identity),
   std::make_tuple("")
)
{

    leaderboard::handle_t created_handle;

    ASSERT_FAILURE( leaderboard::create(leaderboard_identity, &created_handle) );
}
