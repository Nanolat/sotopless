#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <vector>
#include <string>

typedef std::vector<std::string> string_vector_t;

#define ASSERT_SUCCESS(x) Assert.Equal((x), success)
#define ASSERT_FAILURE(x) Assert.Equal((x), failure)


#endif /* TEST_COMMON_H_ */
