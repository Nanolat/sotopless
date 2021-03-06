// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#include <stdio.h>
#include <stdint.h>
#include <sstream>
#include <time.h>
#include <assert.h>
#include <nanolat/client/SyncClient.h>

#include "common.h"
#include "database_perf.h"

using namespace nanolat::client;

int database_client(const char * address, int port) {
	do_connect(address, port);

	do_create_db("test_db");

	do_use_db("test_db");

	do_create_table("users_by_score");
	do_create_table("scores_by_user");

	// post scores, commit transaction
	do_begin_transaction();
	{
		for (int i=0; user_scores[i].user != NULL; i++) {
			do_post_score(user_scores[i].user, user_scores[i].score);
		}
	}
	do_commit_transaction();

/*
	// delete scores, abort transaction
	do_begin_transaction();
	{
		for (int i=0; user_scores[i].user != NULL; i++) {
			do_delete_score(user_scores[i].user, user_scores[i].score);
		}
	}
	do_abort_transaction();
*/

	// search, list scores and users.
	do_begin_transaction();
	{
		do_count_keys("users_by_score");

		do_count_keys("scores_by_user");

		do_list_order_by_score(NL_CURSOR_FORWARD, true);
		do_list_order_by_score(NL_CURSOR_FORWARD, false);

		do_list_order_by_score(NL_CURSOR_BACKWARD, true);
		do_list_order_by_score(NL_CURSOR_BACKWARD, false);

		do_list_order_by_user(NL_CURSOR_FORWARD, true);
		do_list_order_by_user(NL_CURSOR_FORWARD, false);
		do_list_order_by_user(NL_CURSOR_BACKWARD, true);
		do_list_order_by_user(NL_CURSOR_BACKWARD, false);

		do_get_each_user_key();

		do_get_each_score_key();

		do_get_each_user_key_by_order();

		do_get_each_score_key_by_order();

	}
	do_commit_transaction();

//	do_perf_test();

	// delete scores, commit transaction.
	do_begin_transaction();
	{
		for (int i=0; user_scores[i].user != NULL; i++) {
			do_delete_score(user_scores[i].user, user_scores[i].score);
		}
	}
	do_commit_transaction();

	// count scores and users.
	do_begin_transaction();
	{
		do_count_keys("users_by_score");

		do_count_keys("scores_by_user");
	}
	do_commit_transaction();

	do_drop_table("users_by_score");

	do_drop_table("scores_by_user");

	// TODO : Can the user drop a db without closing it?

	do_drop_db("test_db");

	do_disconnect();

	return 0;
}

