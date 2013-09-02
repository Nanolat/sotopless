/*
 * common.h
 *
 *  Created on: 2013. 9. 1.
 *      Author: ilvsusie
 */

#ifndef _NANOLAT_TEST_CLIENT_COMMON_H_
#define _NANOLAT_TEST_CLIENT_COMMON_H_

#if !defined(NDEBUG)
#  define TRACE printf
#else
#  define TRACE(...)
#endif

#define MAX_USER_NAME_LEN (20)
#define MAX_USER_NAME_LEN_STR "20"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <string>
#include <time.h>
#include <assert.h>
#include <nanolat/client/SyncClient.h>

using namespace nanolat::client;

uint64_t generate_random_number();

void generate_random_string(char * str, const int len);


extern connection_t * g_conn;

void show_error_and_exit(int error_code);

void do_connect(const char * address, int port);

void do_disconnect();

void do_create_db(const char * db_name);

void do_drop_db(const char * db_name);

void do_use_db(const char * db_name);

void do_create_table(const char * table_name);

void do_drop_table(const char * table_name);

typedef struct user_score_t {
	const char * user;
	uint64_t score;
} user_score_t ;

extern user_score_t user_scores[] ;

#define switch_endian32(v) \
        (unsigned int)( ((((unsigned int)v)&0x000000FFU) << 24) | \
                        ((((unsigned int)v)&0x0000FF00U) << 8)  | \
                        ((((unsigned int)v)&0x00FF0000U) >> 8)  | \
                        ((((unsigned int)v)&0xFF000000U) >> 24) )

#define switch_endian64(v) \
        (unsigned long long)( ((((unsigned long long)v)&0x00000000000000FFULL) << 56) | \
                              ((((unsigned long long)v)&0x000000000000FF00ULL) << 40) | \
                              ((((unsigned long long)v)&0x0000000000FF0000ULL) << 24) | \
                              ((((unsigned long long)v)&0x00000000FF000000ULL) << 8)  | \
                              ((((unsigned long long)v)&0x000000FF00000000ULL) >> 8)  | \
                              ((((unsigned long long)v)&0x0000FF0000000000ULL) >> 24) | \
                              ((((unsigned long long)v)&0x00FF000000000000ULL) >> 40) | \
                              ((((unsigned long long)v)&0xFF00000000000000ULL) >> 56) )

bool is_little_endian();

// convert to big endian if the host is using little endian
uint32_t to_big_endian(uint32_t value);

// convert to big endian if the host is using little endian
uint64_t to_big_endian(uint64_t value);

// convert to little endian if the host is little endian.
uint32_t to_host_endian(uint32_t big_endian_value);

// convert to little endian if the host is little endian.
uint64_t to_host_endian(uint64_t big_endian_value);

uint64_t hash(const char *str);

// pack the given score and hash of user name into a string to use as a key of users_by_score table.
std::string pack_score(uint64_t score, const std::string & user);

// unpack the score from the packed score with pack_score function.
int64_t unpack_score(const std::string & packed_score);

// unpack the hash value of the user name from the packed score with pack_score function.
int64_t unpack_hash(const std::string & packed_score);


std::string pack_user_name(const std::string & user_name);

std::string unpack_user_name(const std::string & packed_user_name);

void do_table_put(const char * table_name, const std::string & key, const std::string & value);

void do_post_score(const char * user_name, uint64_t score);

void do_delete_score(const char * user_name, int score);

void do_count_keys(const char * table_name);

void print_packed_score_and_user(const std::string & packed_score_key, const key_order_t & key_order, const std::string & packed_user_value);

void print_user_and_score(const std::string & packed_user_key, const key_order_t & key_order, const std::string & score_value);

void do_list_order_by_score(const cursor_direction_t & direction, bool use_key_to_open_cursor );

void do_list_order_by_user(const cursor_direction_t & direction, bool use_key_to_open_cursor );

void do_get_each_user_key();

void do_get_each_score_key();

// search scores_by_user table by order
void do_get_each_user_key_by_order();

// search users_by_score table by order
void do_get_each_score_key_by_order();

void do_begin_transaction();

void do_abort_transaction();

void do_commit_transaction();


#endif /* COMMON_H_ */
