/*
 * LeaderboardImpl.h
 *
 *  Created on: 2013. 9. 14.
 *      Author: ilvsusie
 */

#ifndef LEADERBOARDIMPL_H_
#define LEADERBOARDIMPL_H_

#include <string>
#include "DatabaseManager.h"
#include "LeaderboardService.h"

namespace nanolat {
namespace leaderboard {

server_error_t authenticate_user (db::session_context_t * sess_ctx, const std::string & player_id, const std::string & player_password, bool * user_exists);
server_error_t register_user (db::session_context_t * sess_ctx, const std::string & player_id, const std::string & player_password, const std::string& user_data);
server_error_t post_user_score(db::session_context_t * sess_ctx, nldb_table_t table_by_score, nldb_table_t table_by_user, const Score & score);
server_error_t get_user_score(db::session_context_t * sess_ctx, nldb_table_t table_by_score, nldb_table_t table_by_user, const std::string & player_id, Score * score);
server_error_t get_scores_by_ranking(db::session_context_t * sess_ctx, nldb_table_t table_by_score, const int from_rank, const int count, std::vector<Score> * scores);

} /* namespace leaderboard */
} /* namespace nanolat */
#endif /* LEADERBOARDIMPL_H_ */
