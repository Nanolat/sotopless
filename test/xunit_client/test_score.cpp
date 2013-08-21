#include "xUnit++/xUnit++.h"
#include <topless.h>
#include <time.h>
#include "test_common.h"
using namespace topl;

FACT("Put Score and Get Score")
{
    user::handle_t u1 = create_or_get_user("User1");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const int score = 1000;
    const std::string situation = "happy";
    const util::timestamp_t now = util::now();

    score::rank_desc_t rank_desc;

    ASSERT_FAILURE( score::get(lb1, u1, &rank_desc) );

    ASSERT_SUCCESS( score::post(lb1, u1, score, situation, now ) );

    ASSERT_SUCCESS( score::get(lb1, u1, &rank_desc) );

    // Check rank descriptor.
    Assert.Equal( rank_desc.rank, 1);
    Assert.Equal( rank_desc.user_handle, u1);
    Assert.Equal( rank_desc.user_identity, "User1");
    Assert.Equal( rank_desc.score, score);
    Assert.Equal( rank_desc.situation, situation);
    Assert.Equal( rank_desc.when, now);

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );
}

FACT("Put Score, Remove Score")
{
    user::handle_t u1 = create_or_get_user("User1");
    user::handle_t u2 = create_or_get_user("User2");
    user::handle_t u3 = create_or_get_user("User3");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const int u1_score = 1000;
    const int u2_score = 2000;
    const int u3_score = 3000;
    const std::string u1_situation = "happy";
    const std::string u2_situation = "happy";
    const std::string u3_situation = "happy";
    const util::timestamp_t u1_now = util::now();
    const util::timestamp_t u2_now = util::now();
    const util::timestamp_t u3_now = util::now();

    score::rank_desc_t rank_desc;

    // Post scores of u1, u2, u3
    ASSERT_SUCCESS( score::post(lb1, u1, u1_score, u1_situation, u1_now ) );

    ASSERT_SUCCESS( score::post(lb1, u2, u2_score, u2_situation, u2_now ) );

    ASSERT_SUCCESS( score::post(lb1, u3, u3_score, u3_situation, u3_now ) );

    // Remove scores of u1, u3
    ASSERT_SUCCESS( score::remove(lb1, u1 ) );
    ASSERT_SUCCESS( score::remove(lb1, u3 ) );

    // Get scores of u1, u3 : should fail
    ASSERT_FAILURE( score::get(lb1, u1, &rank_desc) );
    ASSERT_FAILURE( score::get(lb1, u3, &rank_desc) );

    // Get score of u2 : should succeed.
    ASSERT_SUCCESS( score::get(lb1, u2, &rank_desc) );


    // Check rank descriptor of u2
    Assert.Equal( rank_desc.rank, 1);
    Assert.Equal( rank_desc.user_handle, u2);
    Assert.Equal( rank_desc.user_identity, "User2");
    Assert.Equal( rank_desc.score, u2_score);
    Assert.Equal( rank_desc.situation, u2_situation);
    Assert.Equal( rank_desc.when, u2_now);

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );
}


FACT("Put Higher Score")
{
    user::handle_t u1 = create_or_get_user("User1");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const int score = 1000;
    const int new_score = score + 100;
    const std::string situation = "happy";
    const util::timestamp_t now = util::now();

    score::rank_desc_t rank_desc;

    ASSERT_SUCCESS( score::post(lb1, u1, score, situation, now ) );

    ASSERT_SUCCESS( score::post(lb1, u1, new_score, situation, now ) );

    ASSERT_SUCCESS( score::get(lb1, u1, &rank_desc) );

    // Check rank descriptor.
    Assert.Equal( rank_desc.rank, 1);
    Assert.Equal( rank_desc.user_handle, u1);
    Assert.Equal( rank_desc.user_identity, "User1");

    // The score should match with the new score.
    Assert.Equal( rank_desc.score, new_score);
    Assert.Equal( rank_desc.situation, situation);
    Assert.Equal( rank_desc.when, now);

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );

}

FACT("Put Lower Score")
{
    user::handle_t u1 = create_or_get_user("User1");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const int score = 1000;
    const int new_score = score - 100;
    const std::string situation = "happy";
    const util::timestamp_t now = util::now();

    score::rank_desc_t rank_desc;

    ASSERT_SUCCESS( score::post(lb1, u1, score, situation, now ) );

    ASSERT_SUCCESS( score::post(lb1, u1, new_score, situation, now ) );

    ASSERT_SUCCESS( score::get(lb1, u1, &rank_desc) );

    // Check rank descriptor.
    Assert.Equal( rank_desc.rank, 1);
    Assert.Equal( rank_desc.user_handle, u1);
    Assert.Equal( rank_desc.user_identity, "User1");

    // The score should match with the new score, even though the new score was lower.
    Assert.Equal( rank_desc.score, new_score);
    Assert.Equal( rank_desc.situation, situation);
    Assert.Equal( rank_desc.when, now);

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );
}

FACT("Put Score and List Score")
{
    user::handle_t u1 = create_or_get_user("User1");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const int score = 1000;
    const std::string situation = "happy";
    const util::timestamp_t now = util::now();

    std::vector<score::rank_desc_t> actual_scores;

    // No score found, but score::list succeeds.
    ASSERT_SUCCESS( score::list(lb1, 1, 10, &actual_scores) );
    Assert.Equal( actual_scores.size(), 0 );

    ASSERT_SUCCESS( score::post(lb1, u1, score, situation, now ) );

    // No need to call scores.clear(). It will be cleared by score::list.
    ASSERT_SUCCESS( score::list(lb1, 1, 10, &actual_scores) );

    std::vector<score::rank_desc_t> expected_scores {
        score::rank_desc_t{1, 0, "User1", score, situation, now }
    };

    Assert.Equal(actual_scores.begin(), actual_scores.end(),
                 expected_scores.begin(), expected_scores.end(),
                 rank_desc_comparer );

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );
}

FACT("Put Score, Purge Score, Get Score, List Score")
{
    user::handle_t u1 = create_or_get_user("User1");
    user::handle_t u2 = create_or_get_user("User2");

    leaderboard::handle_t lb1 = create_new_leaderboard("LeaderBoard1");

    const util::timestamp_t PURGE_TIMESTAMP = 1000;

    const int u1_score = 2000;
    const std::string u1_situation = "happy";
    const util::timestamp_t u1_now = PURGE_TIMESTAMP-1; // It will be purged later with timestamp = 1000

    const int u2_score = 1000;
    const std::string u2_situation = "sleepy";
    const util::timestamp_t u2_now = util::now();

    score::rank_desc_t rank_desc;

    // Put Score
    {
        ASSERT_FAILURE( score::get(lb1, u1, &rank_desc) ); // The score data is not existing yet.

        // Put the score data for User1
        ASSERT_SUCCESS( score::post(lb1, u1, u1_score, u1_situation, u1_now ) );

        ASSERT_FAILURE( score::get(lb1, u2, &rank_desc) ); // The score data is not existing yet.

        // Put the score data for User2
        ASSERT_SUCCESS( score::post(lb1, u2, u2_score, u2_situation, u2_now ) );
    }



    // Get Score, Check Score and Rank.
    {
        // Get and check rank descriptor for User1
        ASSERT_SUCCESS( score::get(lb1, u1, &rank_desc) );

        Assert.Equal( rank_desc.rank, 1);
        Assert.Equal( rank_desc.user_handle, u1);
        Assert.Equal( rank_desc.user_identity, "User1");
        Assert.Equal( rank_desc.score, u1_score);
        Assert.Equal( rank_desc.situation, u1_situation);
        Assert.Equal( rank_desc.when, u1_now);

        // Get and check rank descriptor for User2
        ASSERT_SUCCESS( score::get(lb1, u2, &rank_desc) );

        Assert.Equal( rank_desc.rank, 2);
        Assert.Equal( rank_desc.user_handle, u2);
        Assert.Equal( rank_desc.user_identity, "User2");
        Assert.Equal( rank_desc.score, u2_score);
        Assert.Equal( rank_desc.situation, u2_situation);
        Assert.Equal( rank_desc.when, u2_now);

    }

    // List Scores, Check Score and Rank.
    {
        std::vector<score::rank_desc_t> expected_scores {
            score::rank_desc_t{1, 0, "User1", u1_score, u1_situation, u1_now },
            score::rank_desc_t{2, 0, "User2", u2_score, u2_situation, u2_now }
        };

        std::vector<score::rank_desc_t> actual_scores;

        ASSERT_SUCCESS( score::list(lb1, 1, 10, &actual_scores) );

        Assert.Equal(actual_scores.begin(), actual_scores.end(),
                     expected_scores.begin(), expected_scores.end(),
                     rank_desc_comparer );
    }


    // Purge Score for scores posted before (util::timestmp_t)1000
    {
        ASSERT_SUCCESS( leaderboard::purge( lb1, PURGE_TIMESTAMP) );
    }

    // Check Score and Rank again.
    // Because User2's score is purged, User1 should be the 1st rank.
    {
        // No score data for User1, because it is purged.
        ASSERT_FAILURE( score::get(lb1, u1, &rank_desc) );


        // Get and check rank descriptor for User2
        // The rank of User2 is now 1. Because User1's score data is purged.
        ASSERT_SUCCESS( score::get(lb1, u2, &rank_desc) );

        Assert.Equal( rank_desc.rank, 1);
        Assert.Equal( rank_desc.user_handle, u2);
        Assert.Equal( rank_desc.user_identity, "User2");
        Assert.Equal( rank_desc.score, u2_score);
        Assert.Equal( rank_desc.situation, u2_situation);
        Assert.Equal( rank_desc.when, u2_now);
    }

    // List Scores, Check Score and Rank.
    {
        std::vector<score::rank_desc_t> expected_scores {
            score::rank_desc_t{1, 0, "User2", u2_score, u2_situation, u2_now }
        };

        std::vector<score::rank_desc_t> actual_scores;

        ASSERT_SUCCESS( score::list(lb1, 1, 10, &actual_scores) );

        Assert.Equal(actual_scores.begin(), actual_scores.end(),
                     expected_scores.begin(), expected_scores.end(),
                     rank_desc_comparer );
    }

    // Drop the leaderboard.
    ASSERT_SUCCESS( leaderboard::drop(lb1) );
}


