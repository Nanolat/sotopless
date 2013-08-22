#include <topless.h>

namespace topl { namespace leaderboard {

result create( const identity_t & leaderboard_identity,
               handle_t * handle )
{
    return failure;
}

result get( const identity_t & leaderboard_identity,
            handle_t * handle )
{
    return failure;
}



result list( std::vector<leaderboard_desc_t> * leaderboards )
{
    return failure;
}

result purge( const handle_t & leaderboard_handle,
              const util::timestamp_t & cut_date )
{
    return failure;
}


result drop( const handle_t & handle )
{
    return failure;
}



}} // topl::leaderboard
