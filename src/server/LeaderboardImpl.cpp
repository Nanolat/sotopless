/*
 * LeaderboardImpl.cpp
 *
 *  Created on: 2013. 9. 14.
 *      Author: ilvsusie
 */
#include <sstream>
#include <string>
#include <private/util.h>
#include <private/stacktrace.h>
#include <nldb/nldb.h>

#include "LeaderboardImpl.h"
#include "ThriftSerializer.h"


namespace nanolat {
namespace leaderboard {



// pack the given score and hash of user name into a string to use as a key of users_by_score table.
std::string pack_score_key(uint64_t score, uint64_t epoch) {
	// The packed score will be used as a key in "users_by_score" table.
	// The memcmp will be used for comparing the key, so we need to convert the score to big endian.
	//
	// 1. Step 1 : Convert the score from little endian to big endian.
	uint64_t big_endian_score = to_big_endian(score);
	std::string score_key((const char*)&big_endian_score, sizeof(big_endian_score));

	// 2. Step 2 : Append timestamp(epoch), to make the score key unique for a leaderboard.
	uint64_t big_endian_hash = to_big_endian(epoch);
	return score_key.append((const char*)&big_endian_hash, sizeof(big_endian_hash));
}

// unpack the score from the packed score with pack_score function.
int64_t unpack_score_from_key(const std::string & packed_score) {
	const char * p = packed_score.c_str();
	uint64_t big_endian_score = *((uint64_t*)p);
	return to_host_endian(big_endian_score);
}

// unpack the hash value of the user name from the packed score with pack_score function.
int64_t unpack_epoch_from_key(const std::string & packed_score) {
	const char * p = packed_score.c_str();
	uint64_t big_endian_epoch = *((uint64_t*)(p+sizeof(uint64_t)));
	return to_host_endian(big_endian_epoch);
}


std::string pack_user_id_key(const std::string & user_id) {
	assert(user_id.length() <= 20);
	char buffer[20+1];
	sprintf(buffer, "%-20s", user_id.c_str() );

	std::string packed_string = buffer;
	return packed_string;
}

std::string unpack_user_name_key(const std::string & packed_user_id) {
	std::stringstream trimmer;
	trimmer << packed_user_id;

	return trimmer.str();
}

// TODO : Multi-thread : Need to have this object for each session, in case we support multi threading.
ThriftSerializer serializer;
ThriftDeserializer deserializer;

inline void pack_score_value(const Score & score, size_t * packed_score_length, void ** packed_score_data ) {
	serializer.serialize<Score>(score, packed_score_length, packed_score_data );
}

inline void unpack_score_value(size_t packed_score_length, void * packed_score_data, Score * score) {
	deserializer.deserialize<Score>(packed_score_length, packed_score_data, score);
}

server_error_t authenticate_user (db::session_context_t * sess_ctx, const std::string & player_id, const std::string & player_password, bool * user_exists) {
	// TODO : Implement.
	*user_exists = true;
	return 0;
}

server_error_t register_user (db::session_context_t * sess_ctx, const std::string & player_id, const std::string & player_password, const std::string& user_data) {
	return -1;
}

server_error_t post_user_score(db::session_context_t * sess_ctx, nldb_table_t table_by_score, nldb_table_t table_by_user, const Score & score) {
	nldb_tx_t tx = sess_ctx->get_transaction();
	NL_ASSERT(tx);

	std::string packed_score_key_string = pack_score_key(score.value, score.date_epoch);
	std::string packed_user_id_key_string  = pack_user_id_key(score.user_id);

	void *packed_score_data;
	size_t packed_score_length;
	pack_score_value(score, &packed_score_length, &packed_score_data);

	nldb_key_t packed_score_key = { (void*)packed_score_key_string.data(), (key_length_t)packed_score_key_string.length() };
	nldb_key_t packed_user_id_key = { (void*)packed_user_id_key_string.data(), (key_length_t)packed_user_id_key_string.length() };

	nldb_value_t packed_score_value = {packed_score_data, (value_length_t)packed_score_length};
	nldb_value_t packed_score_key_for_value = { (void*)packed_score_key_string.data(), (value_length_t)packed_score_key_string.length() };

	nldb_rc_t rc = nldb_table_put(tx, table_by_score, packed_score_key, packed_score_value);
	if (rc) NL_RETURN( (server_error_t) rc);

	rc = nldb_table_put(tx, table_by_user, packed_user_id_key, packed_score_key_for_value);
	if (rc) NL_RETURN( (server_error_t) rc);

	NL_RETURN( (server_error_t) NLDB_OK );
}

int get_descending_key_order(nldb_order_t key_count, nldb_order_t ascending_key_order) {
	return key_count - ascending_key_order + 1;
}

server_error_t get_user_score(db::session_context_t * sess_ctx, nldb_table_t table_by_score, nldb_table_t table_by_user, const std::string & player_id, Score * score) {
	nldb_tx_t tx = sess_ctx->get_transaction();
	NL_ASSERT(tx);

    nldb_order_t ascending_score_order;
	nldb_table_stat_t stat;

	std::string packed_user_id_key_string  = pack_user_id_key(player_id);
	nldb_key_t packed_user_id_key = { (void*)packed_user_id_key_string.data(), (key_length_t)packed_user_id_key_string.length() };
	nldb_value_t packed_score_key_for_value;
	nldb_value_t packed_score_value;

	// Get the packed score key to search table_by_score by a specific score record.
	nldb_rc_t nrc = nldb_table_get(tx, table_by_user, packed_user_id_key, & packed_score_key_for_value);
	if (nrc) NL_RETURN( (server_error_t) nrc );

	nldb_key_t packed_score_key = { (void*)packed_score_key_for_value.data, (key_length_t)packed_score_key_for_value.length };
	nrc = nldb_table_get(tx, table_by_score, packed_score_key, & packed_score_value, &ascending_score_order);
	if (nrc) NL_RETURN( (server_error_t) nrc );

	unpack_score_value(packed_score_value.length, packed_score_value.data, score);

	nrc = nldb_table_stat(tx, table_by_score, &stat);
	if (nrc) NL_RETURN( (server_error_t) nrc );

	score->rank = get_descending_key_order(stat.key_count, ascending_score_order);

	NL_RETURN( (server_error_t) NLDB_OK );
}


server_error_t get_scores_by_ranking(db::session_context_t * sess_ctx, nldb_table_t table_by_score, const int from_rank, const int count, std::vector<Score> * scores) {

	nldb_key_t packed_score_key;
    nldb_value_t packed_score_value;
    nldb_order_t ascending_score_order;

	nldb_table_stat_t stat;

	key_order_t initial_key_order = -1;

	nldb_cursor_t cursor = NULL;
	nldb_rc_t nrc = NLDB_OK;

	int score_count_left = count;

	nldb_tx_t tx = sess_ctx->get_transaction();
	NL_ASSERT(tx);

	nrc = nldb_table_stat(tx, table_by_score, &stat);
	if (nrc) goto on_error;

	if (stat.key_count >= from_rank) {
		initial_key_order = stat.key_count - from_rank + 1;

		nrc = nldb_cursor_open(tx, table_by_score, &cursor);
		if (nrc) goto on_error;

		nrc = nldb_cursor_seek(cursor, NLDB_CURSOR_BACKWARD, initial_key_order);
		if (nrc) goto on_error;

		while(1) {
			nrc = nldb_cursor_fetch(cursor, & packed_score_key, & packed_score_value, &ascending_score_order);
			if (nrc == NLDB_ERROR_END_OF_ITERATION)
				break;
			if (nrc) {
				goto on_error;
			}

			Score score;
			unpack_score_value(packed_score_value.length, packed_score_value.data, &score);

			score.rank = get_descending_key_order(stat.key_count, ascending_score_order);

			scores->push_back(score);

	        nrc = nldb_key_free( table_by_score, packed_score_key );
	        NL_RELEASE_ASSERT(nrc==NLDB_OK);

	        nrc = nldb_value_free( table_by_score, packed_score_value );
	        NL_RELEASE_ASSERT(nrc==NLDB_OK);

	        if (--score_count_left <= 0) {
	        	break;
	        }
		}
	} else {
		// the rank is out of range. do not return any score to the client.
	}

on_error:
	if (cursor) {
		nrc = nldb_cursor_close(cursor);
	}

	NL_RETURN( (server_error_t) nrc );
}




} /* namespace leaderboard */
} /* namespace nanolat */
