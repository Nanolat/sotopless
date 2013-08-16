#include "xUnit++/xUnit++.h"
#include <topless.h>
#include "test_common.h"
using namespace topl;

// User Identity
#define USER(n) "user"##n
// User Situation
#define SIT(n) "sit"##n
// Leaderboard Identity
#define LB(n) "lboard"##n

typedef enum populating_command_t {
    PUT_SCORE, PURGE_SCORE
} populating_command_t;

// Leaderboard operation.
typedef struct populating_operation_t {
    // The command for the populating operation.
    populating_command_t command;
    // The list of leaderboards to run the command.
    string_vector_t   leaderboards;

    struct {
        std::string       user_identity;
        score::score_t    user_score;
        std::string       user_situation;
        util::timestamp_t when;
    } put_score ;

    struct {
        util::timestamp_t cut_date;
    } purge_score;

} populating_operation_t;


typedef struct leaderboard_verification_t {
    leaderboard::identity_t leaderboard_name;
    score::rank_t from_rank;
    score::rank_t count;
    std::vector<score::rank_desc_t> expected_scores ;
} leaderboard_verification_t ;

typedef struct score_posting_scenario_t {
    std::string                             name; // The name of the scenario
    std::vector<populating_operation_t>     populating_operations;
    std::vector<leaderboard_verification_t> verifications;
} score_posting_scenario_t ;


score_posting_scenario_t NoUser = {
    "No User",
    std::vector<populating_operation_t> {},
    std::vector<leaderboard_verification_t> {}
};

score_posting_scenario_t OnlyOneUser = {
    "Only One User",
    std::vector<populating_operation_t> {
        populating_operation_t {
            PUT_SCORE,
            string_vector_t{LB(0)}, // leaderboards
            {USER(0), 1000, "happy", 100001}, // PUT_SCORE data
            {0} // PURGE_SCORE data
        }
    },
    std::vector<leaderboard_verification_t> {
        leaderboard_verification_t {
            LB(0),
            1,  // from rank
            10, // count
            std::vector<score::rank_desc_t> {
                score::rank_desc_t {1, 0, USER(0), 1000, "happy", 10001}
            }
        }
    }
};

THEORY("Top Score Scenarios", (std::string scenario_name, const score_posting_scenario_t & scenario ),
    std::make_tuple( NoUser.name, NoUser ),
    std::make_tuple( OnlyOneUser.name, OnlyOneUser )
)
{
    // For each leaderboard
    for(const leaderboard_verification_t & v : scenario.verifications) {
        // Step 1 : Truncate all leaderboards
		leaderboard::handle_t lb_handle;

		if ( leaderboard::get(v.leaderboard_name, &lb_handle ) == success ) // If the leaderboard exists
		{
			ASSERT_SUCCESS( leaderboard::drop(lb_handle) ); // Drop it.
		}
		// And then, create it again.
		ASSERT_SUCCESS( leaderboard::create(v.leaderboard_name, &lb_handle ) );
    }

    // For each populating operations
    for(const populating_operation_t & op : scenario.populating_operations) {

        // Step 2 : run operations such as posting score, and purging score.
        for( const leaderboard::identity_t & lb_identity : op.leaderboards ) {

            leaderboard::handle_t lb_handle;
            // Get the leaderboard handle
            ASSERT_SUCCESS( leaderboard::get(lb_identity, &lb_handle ) );


            switch( op.command ) {
                case PUT_SCORE :
                {
                    user::handle_t user_handle;
                    ASSERT_SUCCESS( user::get(op.put_score.user_identity, &user_handle ) );

                    ASSERT_SUCCESS( score::put( lb_handle,
                                                user_handle,
                                                op.put_score.user_score,
                                                op.put_score.user_situation,
                                                op.put_score.when ) );
                }
                break;
                case PURGE_SCORE :
                {
                    ASSERT_SUCCESS( leaderboard::purge( lb_handle, op.purge_score.cut_date) );
                }
                break;
            }
        }
    }

    // For each leaderboard
    for(const leaderboard_verification_t & v : scenario.verifications) {
        // Step 3 : Verify the result
        leaderboard::handle_t lb_handle ;

        ASSERT_SUCCESS( leaderboard::get( v.leaderboard_name, &lb_handle ) );

        std::vector<score::rank_desc_t> actual_scores;

        ASSERT_SUCCESS( score::list(lb_handle, v.from_rank, v.count, &actual_scores) );

        Assert.Equal(actual_scores.begin(), actual_scores.end(),
                     v.expected_scores.begin(), v.expected_scores.end(),
                     rank_desc_comparer );
    }

}

