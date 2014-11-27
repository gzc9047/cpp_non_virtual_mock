#include "set_function_jump.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>

#define USE_FUNCTION_MOCK0_OBJECT(method, keyName) \
    fake_##keyName##_##method

#define USE_FUNCTION_MOCK0_WRAP(method, keyName) \
    Wrap_##keyName##_##method

#define DECLARE_FUNCTION_MOCK0(method, keyName, returnValue) \
    struct Fake_##keyName##_##method { MOCK_METHOD0(method, returnValue()); }; \
    Fake_##keyName##_##method* USE_FUNCTION_MOCK0_OBJECT(method, keyName); \
    returnValue USE_FUNCTION_MOCK0_WRAP(method, keyName)() { \
        return USE_FUNCTION_MOCK0_OBJECT(method, keyName)->method(); \
    }

#define INIT_FUNCTION_MOCK0(method, keyName) \
    USE_FUNCTION_MOCK0_OBJECT(method, keyName) = new Fake_##keyName##_##method(); \
    SetFunctionJump(method, USE_FUNCTION_MOCK0_WRAP(method, keyName));

#define FINALIZE_FUNCTION_MOCK0(method, keyName) \
    delete USE_FUNCTION_MOCK0_OBJECT(method, keyName)

using namespace std;

string getString() {
    return "Non mocked.";
}

DECLARE_FUNCTION_MOCK0(getString, global, string);

class TryGmockTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        INIT_FUNCTION_MOCK0(getString, global);
    }

    virtual void TearDown() {
        FINALIZE_FUNCTION_MOCK0(getString, global);
    }
};

TEST(TryGmockTest, SimpleCase) {
    EXPECT_CALL(*USE_FUNCTION_MOCK0_OBJECT(getString, global), getString())
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("A"));
    cout << getString() << endl;
}
