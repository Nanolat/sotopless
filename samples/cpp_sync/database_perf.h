#ifndef _NANOLAT_CLIENT_PERF_H_
#define _NANOLAT_CLIENT_PERF_H_ (1)

/* The number of users that posts scores of them to the leaderboard */
#define USER_COUNT (1000000)

/* The number of request to send to the server to get top 100 users */
#define LIST_REQUEST_COUNT (1000000)

/* When we list users by order, How many users do we list from 1st user with the best score? */
#define TOP_N_VALUE (100)

void do_perf_test();

#endif //_NANOLAT_CLIENT_PERF_H_
