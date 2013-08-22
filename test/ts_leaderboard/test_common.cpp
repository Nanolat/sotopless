#include "test_common.h"

#include "xUnit++/xUnit++.h"

using namespace topl;
using namespace xUnitpp;

user::handle_t create_or_get_user(user::identity_t user_identity)
{
    user::handle_t user_handle;
    if ( user::get(user_identity, &user_handle) == failure) {

        Assert.Equal( user::create( user_identity, &user_handle ), success );

    }

    return user_handle;
}


leaderboard::handle_t create_new_leaderboard(leaderboard::identity_t leaderboard_identity)
{
    leaderboard::handle_t leaderboard_handle;

    // Drop the leaderboard if exists.
    if ( leaderboard::get(leaderboard_identity, &leaderboard_handle) == success) {
        Assert.Equal( leaderboard::drop( leaderboard_handle ), success );
    }

    Assert.Equal( leaderboard::create( leaderboard_identity, &leaderboard_handle ), success );

    return leaderboard_handle;
}


bool rank_desc_comparer(const score::rank_desc_t& actual, const score::rank_desc_t& expected)
{
    // do not compare user_handle, because it is hard to sepcify an expected value in
    return actual.rank == expected.rank
            && actual.user_identity == expected.user_identity
            && actual.score == expected.score
            && actual.situation == expected.situation
            && actual.when == expected.when;
}
