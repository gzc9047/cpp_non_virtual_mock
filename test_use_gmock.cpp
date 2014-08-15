#include "mock_non_virtual.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

string getString() {
    return "Non mocked.";
}

DECLARE_FUNCTION_MOCK0(global_getString, string);

class TryGmockTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        INIT_FUNCTION(getString, global_getString);
    }

    virtual void TearDown() {
        FINALIZE_FUNCTION(getString, global_getString);
    }
    MOCK_METHOD5(testMock5, int(bool, char, short, int, string));
};

TEST_F(TryGmockTest, SimpleCase) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(global_getString), global_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("A"));
    EXPECT_STREQ("A", getString().c_str());
    RESTORE_FUNCTION(getString, global_getString);
    EXPECT_STRNE("A", getString().c_str());
}
