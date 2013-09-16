#if defined(WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include "../thrift/lib/cpp/src/windows/GetTimeOfDay.h"
#else
# include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <string.h>


#include <nanolat/client/SyncClient.h>

#include "common.h"
#include "database_perf.h"

using namespace nanolat::client;

typedef struct rnd_user_score_t {
	char user[MAX_USER_NAME_LEN];
	uint64_t score;
} rnd_user_score_t ;

rnd_user_score_t rnd_users[USER_COUNT];

typedef void (*test_func_t)(int loop);

void measure_performance(int loop_count, const char * test_name, test_func_t test_func )
{
    struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);

	// run test function
	for (int i=0; i<loop_count; i++)
	{
		if ( (i & 0xFFFF) == 0) { // For each 65536 tests, print once.
			printf("Progress : %d\n", i);
		}
		test_func(i);
	}

	gettimeofday(&end_time, NULL);

    double start, end;
    start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << test_name << " performance: ";
    std::cout << (loop_count * 1.0) / (end - start)
              << " transactions/secs" << std::endl;
}

void generate_user_data() {
	for(int i=0; i<USER_COUNT; i++) {
		generate_random_string(rnd_users[i].user, MAX_USER_NAME_LEN);
		rnd_users[i].score = generate_random_number();
	}
}

static inline void perf_post_score(int loop) {
	assert(loop < USER_COUNT);

	// Do not begin a transaction. Server will run in auto-commit mode.
	//do_begin_transaction();

	do_post_score(rnd_users[loop].user, rnd_users[loop].score);

	//do_commit_transaction();
}

void perf_list_score(int loop) {
	// suppress warnings for ununsed arguments.
	loop = loop;

	int rc = NL_SUCCESS;

	TRACE("listing %d users by score. Each user lists top N(=%d) users at the same time. \n",
			USER_COUNT,
			TOP_N_VALUE);

	// Get the first key. The key order starts from 1.
	key_order_t initial_key_order = 1;

	cursor_t * cursor;
	rc = nl_cursor_open_by_order(g_conn, "users_by_score", initial_key_order, NL_CURSOR_FORWARD, &cursor);
	if (rc != NL_SUCCESS)
		show_error_and_exit(rc);

	std::string packed_score_key;
    key_order_t key_order;
	std::string user_value;

	for(int i=0; i<TOP_N_VALUE; i++) {

		rc = nl_cursor_fetch(g_conn, cursor, NL_CURSOR_FORWARD, & packed_score_key, & key_order, & user_value);
		if (rc == NL_CURSOR_HAS_NO_MORE_KEYS)
			break;
		if (rc != NL_SUCCESS)
			show_error_and_exit(rc);

		print_packed_score_and_user(packed_score_key, key_order, user_value);
	}

	rc = nl_cursor_close(g_conn, &cursor);
	if (rc != NL_SUCCESS)
		show_error_and_exit(rc);
}

typedef struct test_case_t {
	const char * test_name;
	test_func_t test_func;
	int        loop_count;
} perf_case_t;

// List of test cases
test_case_t tests[] = {
	{"post_score", perf_post_score, 100000},
	{"list_score", perf_list_score, 100000},
	{NULL, NULL}
};

void do_perf_test() {
	// Generating random data could take time. So generate the random data first.
	generate_user_data();

	for (int i=0; tests[i].test_name != NULL; i++) {
		printf("Running performance tTest : %sd\n", tests[i].test_name);
		measure_performance( tests[i].loop_count, tests[i].test_name, tests[i].test_func);
	}
}
