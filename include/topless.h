#ifndef _TOPL_TOPLESS_H_
#define _TOPL_TOPLESS_H_ (1)

#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <stdint.h>

/*! \brief The SoTopless namespace which has C++ API functions.
 * SoTopless APIs are in the topl namespace. These include creating, purging,
 * dropping a leaderboard, sending score, querying score and rank of a user,
 * and querying top N users from rank K.
 *
 * Why use namespaces instead of classes and objects?
 * This is to help C programmers use the C-like API without
 * any understanding of C++ classes and objects.
 */
namespace topl {
   /*! \brief The enumeration containing the result values of function execution.
    * They are success and fail. 
    */ 
    typedef enum {
       success,
       failure 
    } result;

    /*! \brief The user namespace containing user management.
     */
    namespace user {
        /*! The handle of a user.
         * This handle is used by SoTopless APIs instead of the string identity.
         * Why? Using integer handles boost the performance
         * for looking up a users in a database.
         */
        typedef int64_t handle_t;

        /* The identity of a user. It can be either an email or device ID of a mobile device.
         */
        typedef std::string identity_t;

        /*! Create a user by specifying the email or device ID of the user.
         * \param user_identity [IN] user email or device ID that represents the user.
         * \param handle [OUT] the handle of the created user.
         * \return topl::success on success. topl::failure otherwise.
         */
        result create( const identity_t & user_identity,
                       handle_t * handle );

        /*! Get the handle of a user.
         * The handle is required for using SoTopless APIs related to a specific user.
         * \param user_identity [IN] user email or device ID that represents the user.
         * \param  handle [OUT] the handle of a given user specified by user_identity.
         * \return topl::success on success. topl::failure otherwise.
         */
        result get( const identity_t & user_identity,
                    handle_t * handle );
    }

    /*! \brief The util namespace containing utility features.
     */
    namespace util {
        /*! The timestamp type used to specify the time of posting users' scores.
         */
        typedef int64_t timestamp_t;

        /*! Return the current timestamp.
         * \return The current timestamp.
         */
        timestamp_t now();
    }

    /*! \brief The leaderboard namespace containing leaderboard management.
     */
    namespace leaderboard {
        /*! The handle of a leaderboard.
         * This handle is used by SoTopless APIs
         * instead of the string identity of a leaderboard.
         * Why? Using integer handles boost the performance
         * for looking up a leaderboard in a database.
         */
        typedef int16_t handle_t;

        /*! The name of the leaderboard.
         * */
        typedef std::string identity_t;

        /*! Create a leaderboard.
         * \param leaderboard_identity [IN] the name of the leaderboard to create.
         * \param handle [OUT] the handle of the created leaderboard.
         * \return topl::success on success. topl::failure otherwise.
         */
        result create( const identity_t & leaderboard_identity,
                       handle_t * handle );

        /*! Get the handle of a leaderboard.
         * The handle is required for using SoTopless APIs related to a leaderboard.
         * \param leaderboard_identity [IN] the name of the leaderboard to create.
         * \param handle [OUT] the handle of a given leaderboard specified by leaderboard_identity.
         * \return topl::success on success. topl::failure otherwise.
         */
        result get( const identity_t & leaderboard_identity,
                    handle_t * handle );


        /*! The descriptor of a leaderboard.
         */
        typedef struct leaderboard_desc_t {
            handle_t handle;
            identity_t identity;
        } leaderboard_desc_t;

        /*! Get the list of all leaderboards.
         * \param leaderboards [OUT] the vector containing the list of leaderboard names.
         * \return topl::success on success. topl::failure otherwise.
         */
        result list( std::vector<leaderboard_desc_t> * leaderboards );

        /*! Purge score data on a leaderboard stored before a specific date.
         * \param handle [IN] the handle of the leaderboard to purge.
         * \param cut_date [IN] the score date stored before the cut_date is purged.
         * \return topl::success on success. topl::failure otherwise.
         */
        result purge( const leaderboard::handle_t & leaderboard_handle,
                      const util::timestamp_t & cut_date );

        /*! Drop a leaderboard.
         * \param handle [IN] the handle of the leaderboard to drop.
         * \return topl::success on success. topl::failure otherwise.
         */
        result drop( const handle_t & handle );
    }

    /*! \brief The score namespace for functions posting and getting scores and ranks.
     */
    namespace score {
        /*! score_t; the type of score.
        */
        typedef int64_t score_t;

        /*! rank_t; the type for specifying the rank of a user.
        */
        typedef int64_t rank_t;

        /*! Post a new user score. If a score exists for the user, it is overwritten.
         * \param leaderboard_handle [IN] the handle of the leaderboard to query.
         * \param user_handle [IN] the handle of the user to query.
         * \param score [IN] the score of the given user.
         * \param situation [IN] a string representation of data to store
         *        the situation of the user when he/she got the score.
         * \param when [IN] the date when the user got the score.
         * \return topl::success on success. topl::failure otherwise.
         */
        result post( const leaderboard::handle_t & leaderboard_handle,
                     const user::handle_t & user_handle,
                     const score_t & score,
                     const std::string & situation,
                     const util::timestamp_t & when);

        /*! Remove the score posted by a user.
         * \param leaderboard_handle [IN] the handle of the leaderboard to query.
         * \param user_handle [IN] the handle of the user to query.
         */
        result remove( const leaderboard::handle_t & leaderboard_handle,
        		       const user::handle_t & user_handle);

        /*! The rank_desc_t struct having score and rank information of a user on a leaderboard.
         */
        typedef struct rank_desc_t {
            int64_t           rank;
            int64_t           user_handle;
            user::identity_t  user_identity;
            int64_t           score;
            std::string       situation;
            util::timestamp_t when;
        } rank_desc_t ;

        /*! Get score and rank of a specific user.
         * \param leaderboard_handle [IN] the handle of the leaderboard to query.
         * \param user_handle [IN] the handle of the user to query.
         * \param score [OUT] the information on the user score including the rank and score.
         * \return topl::success on success. topl::failure otherwise.
         */
        result get( const leaderboard::handle_t & leaderboard_handle,
                    const user::handle_t & user_handle,
                    rank_desc_t * score);

        /*! List the top N scores from a specific rank.
         * \param leaderboard_handle [IN] the handle of the leaderboard to query.
         * \param from_rank [IN] the beginning rank for the resulting scores.
         *        from_rank starts from 1. 0 is an invalid argument for the from_rank parameter.
         * \param count [IN] the number of scores to fetch. The value N of 'top N'.
         * \param scores [OUT] the array of scores filled in the rank_desc_t struct.
         * \return topl::success on success. topl::failure otherwise.
         */
        result list( const leaderboard::handle_t & leaderboard_handle,
                     const rank_t & from_rank,
                     const rank_t & count,
                     std::vector<rank_desc_t> * scores );

    }
}

#endif /* _TOPL_TOPLESS_H_ */
