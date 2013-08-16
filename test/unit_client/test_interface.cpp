using namespace std;

#include <gtest/gtest.h>

class SoToplessTest : public testing::Test  {
public:
    virtual ~SoToplessTest() {
    }

protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(SoToplessTest, foo) {
     printf("hello world\n");
}
