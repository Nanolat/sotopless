#include <nanolat/topless.h>
#include "user_private.h"

namespace topl { namespace user {

result create( const identity_t & user_identity,
               handle_t * handle )
{
    return failure;
}

result get( const identity_t & user_identity,
            handle_t * handle )
{
    return failure;
}

result get( const std::vector<handle_t> user_handles,
            std::map<handle_t, identity_t > * user_identity_map)
{
    return failure;
}

}} // topl::user
