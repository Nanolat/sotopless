#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <vector>
#include <topless.h>

using namespace topl;

typedef std::vector<std::string> string_vector_t;

#define ASSERT_SUCCESS(x) Assert.Equal((x), success)
#define ASSERT_FAILURE(x) Assert.Equal((x), failure)

/*! Create the user if it does not exist. Return the user handle.
 */
user::handle_t create_or_get_user(user::identity_t user_identity);


leaderboard::handle_t create_new_leaderboard(leaderboard::identity_t leaderboard_identity);


bool rank_desc_comparer(const score::rank_desc_t& actual, const score::rank_desc_t& expected);


#endif /* TEST_COMMON_H_ */
