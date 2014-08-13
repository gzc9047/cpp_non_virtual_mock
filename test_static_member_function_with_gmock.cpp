#include "mock_non_virtual.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

struct Simple {
    static string getString() {
        return "Non mocked.";
    }
};

DECLARE_STATIC_MEMBER_FUNCTION_MOCK0(global_getString, string);

class TryGmockStaticMemberTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        INIT_STATIC_MEMBER_FUNCTION(Simple::getString, global_getString);
    }

    virtual void TearDown() {
        FINALIZE_FUNCTION(Simple::getString, global_getString);
    }
};

TEST_F(TryGmockStaticMemberTest, SimpleCase) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(global_getString), global_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", Simple::getString().c_str());
    RESTORE_FUNCTION(Simple::getString, global_getString);
    EXPECT_STRNE("B", Simple().getString().c_str());
}
