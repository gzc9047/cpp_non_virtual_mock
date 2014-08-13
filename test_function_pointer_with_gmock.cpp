#include "mock_non_virtual.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

string getString() {
    return "Non mocked global.";
}

struct SimpleStatic {
    static string getString() {
        return "Non mocked static member.";
    }
};

struct Simple {
    string getString() {
        return "Non mocked static2 member.";
    }
};

DECLARE_FUNCTION_MOCK0(global_getString, string);
DECLARE_MEMBER_FUNCTION_MOCK0(member_getString, string);
DECLARE_STATIC_MEMBER_FUNCTION_MOCK0(static_member_getString, string);
void* globalFunction = reinterpret_cast<void*>(getString);
// Must use a variable to get the real pointer of static member function.
// Or you will get the binary content of this function, not the address. (In clang)
auto staticTransit = SimpleStatic::getString;
void* staticMember = nullptr;
auto memberTransit = &Simple::getString;
void* memberFunction = reinterpret_cast<void*>((size_t&)memberTransit);

class TryGmockStaticMemberTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        staticMember = reinterpret_cast<void*>((size_t&)staticTransit);
        INIT_FUNCTION(globalFunction, global_getString);
        INIT_STATIC_MEMBER_FUNCTION(staticMember, static_member_getString);
        INIT_MEMBER_FUNCTION(memberFunction, member_getString);
    }

    virtual void TearDown() {
        FINALIZE_FUNCTION(globalFunction, global_getString);
        FINALIZE_FUNCTION(staticMember, static_member_getString);
        FINALIZE_FUNCTION(memberFunction, member_getString);
    }
};

TEST_F(TryGmockStaticMemberTest, GlobalFunctionPointerTest) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(global_getString), global_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", getString().c_str());
}

TEST_F(TryGmockStaticMemberTest, MemberFunctionPointerTest) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(member_getString), member_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", Simple().getString().c_str());
}

TEST_F(TryGmockStaticMemberTest, StaticFunctionPointerTest) {
    EXPECT_CALL(*USE_FUNCTION_OBJECT(static_member_getString), static_member_getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("B"));
    EXPECT_STREQ("B", SimpleStatic::getString().c_str());
}
