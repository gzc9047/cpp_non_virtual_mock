#include "mock_non_virtual_p1.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <type_traits>

using namespace std;
using namespace ::testing;

template < class T >
struct TestSpecialization {
    enum {
        value = 0
    };
};

template < typename T1, typename T2 >
struct TestSpecialization<is_same<T1, T2>> {
    enum {
        value = 1
    };
};

struct TestStaticMember {
    static string func() {
        return "Non mocked.";
    }
};

string func() {
    return "Non mocked.";
}

#ifdef __APPLE__
TEST(HelloWorld, First) {
    CreateMocker(mocker, func);
    EXPECT_CALL(*mocker, MockFunction()).Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func());
    cout << __cplusplus << endl;
}

TEST(HelloWorld, FunctionPointer) {
    string (*pointer)();
    pointer = &func;
    CreateMocker(mocker, pointer);
    EXPECT_CALL(*mocker, MockFunction()).Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", pointer());
    // cout << TestSpecialization<is_same<int, bool>>::value << endl;
    // cout << TestSpecialization<int, bool>::value << endl;
}

TEST(HelloWorld, TestStaticMember) {
    CreateMocker(mocker, TestStaticMember::func);
}
#endif

