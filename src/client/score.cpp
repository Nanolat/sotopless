#include <topless.h>
#include "user_private.h"

namespace topl { namespace score {

result put( const leaderboard::handle_t & leaderboard_handle,
            const user::handle_t & user_handle,
            const score_t & score,
            const std::string & situation,
            const util::timestamp_t & when)
{
    return failure;
}


/*! Get score and rank of a specific user. user_identity field in user_score_t is set to empty.
 */
static result get_without_user_identity(
                  const leaderboard::handle_t & leaderboard_handle,
                  const user::handle_t & user_handle,
                  rank_desc_t * score)
{
    return failure;
}

/*! List the top N scores from a specific rank. user_identity field in user_score_t is set to empty.
 */
static result list_without_user_identity(
                  const leaderboard::handle_t & leaderboard_handle,
                  const rank_t & from_rank,
                  const rank_t & count,
                  std::vector<rank_desc_t> * scores )
{
    return failure;
}

result get( const leaderboard::handle_t & leaderboard_handle,
            const user::handle_t & user_handle,
            rank_desc_t * score)
{
    return failure;
}

result list( const leaderboard::handle_t & leaderboard_handle,
             const rank_t & from_rank,
             const rank_t & count,
             std::vector<rank_desc_t> * scores )
{
    return failure;
}


}} // topl::score
