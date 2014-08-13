#include "mock_non_virtual.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

template < typename T >
string getString() {
    return "Non mocked.";
}

template < typename T >
struct Simple {
    T t;
    string getString() {
        return "Non mocked.";
    }
};

DECLARE_FUNCTION_MOCK0(template_getString, string);
DECLARE_MEMBER_FUNCTION_MOCK0(template_member_getString, string);

class TryGmockTemplateTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        INIT_FUNCTION(getString<int>, template_getString);
        INIT_MEMBER_FUNCTION(&Simple<int>::getString, template_member_getString);
    }

    virtual void TearDown() {
        FINALIZE_FUNCTION(getString<int>, template_getString);
        FINALIZE_FUNCTION(&Simple<int>::getString, template_member_getString);
    }
};

TEST_F(TryGmockTemplateTest, TemplateMember) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(template_member_getString), template_member_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", Simple<int>().getString().c_str());
    RESTORE_FUNCTION(&Simple<int>::getString, template_member_getString);
    EXPECT_STRNE("B", Simple<int>().getString().c_str());
}

TEST_F(TryGmockTemplateTest, TemplateGlobal) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(template_getString), template_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", getString<int>().c_str());
    RESTORE_FUNCTION(getString<int>, template_getString);
    EXPECT_STRNE("B", getString<int>().c_str());
}
