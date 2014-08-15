#include "set_function_jump.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <functional>
#include <iostream>
#include <memory>

using namespace std;

template < typename T >
struct GetFuctionType {
    typedef void ReturnType;
};

template < typename R, typename ... S >
struct GetFuctionType<R(S ...)> {
    typedef R ReturnType;
};

template < typename C, typename R, typename ... S >
struct GetFuctionType<R(C::*)(S ...) const> {
    typedef R ReturnType;
};

template < typename C, typename R, typename ... S >
struct GetFuctionType<R(C::*)(S ...)> {
    typedef R ReturnType;
};

template < typename T >
struct TestMockTemplate {
};

template < typename R, typename ... P>
struct TestMockTemplate<R(P ...)> {
    R MockFunction(P... p) {
        gmocker.SetOwnerAndName(this, "MockFunction");
        return gmocker.Invoke(p ...);
    }

    ::testing::MockSpec<R(P...)>& gmock_MockFunction(P... p) {
        gmocker.RegisterOwner(this);
        return gmocker.With(p ...);
    }
    mutable ::testing::FunctionMocker<R(P...)> gmocker;
};

template < typename T >
struct MockTemplate {
};

template < typename T >
struct MockEntryPoint {
};

template < typename T >
struct MockerStore {
};

template < typename I, typename R, typename ... P >
struct MockEntryPoint<I(R(P ...))> {
    static R EntryPoint(P... p) {
        return MockerStore<I(R(P ...))>::pMocker->MockFunction(p ...);
    }
};

template < typename I, typename F >
struct MockerStore<I(F)> {
    static MockTemplate<I(F)>* pMocker;
};

template < typename I, typename F > MockTemplate<I(F)>* MockerStore<I(F)>::pMocker = nullptr;

template < typename I, typename R, typename ... P>
struct MockTemplate<I(R(P ...))> {
    MockTemplate(R function(P ...)): originFunction(function) {
        SetFunctionJump(function, MockEntryPoint<I(R(P ...))>::EntryPoint, binaryBackup);
        MockerStore<I(R(P ...))>::pMocker = this;
    }

    ~MockTemplate() {
        RestoreJump(originFunction, binaryBackup);
    }

    R MockFunction(P... p) {
        gmocker.SetOwnerAndName(this, "MockFunction");
        return gmocker.Invoke(p ...);
    }

    ::testing::MockSpec<R(P...)>& gmock_MockFunction(P... p) {
        gmocker.RegisterOwner(this);
        return gmocker.With(p ...);
    }

    mutable ::testing::FunctionMocker<R(P...)> gmocker;
    std::vector<char> binaryBackup;
    R (*originFunction)(P ...);
};

template < typename I >
struct MockerCreator {
    template < typename R, typename ... P>
    static unique_ptr<MockTemplate<I(R(P ...))>> createMockerWithIdentity(R function(P ...)) {
        return unique_ptr<MockTemplate<I(R(P ...))>>(new MockTemplate<I(R(P ...))>(function));
    }
};

#define CreateMockerWithIdentity(mocker, function, identity) \
    struct identity {}; \
    auto mocker = MockerCreator<identity>::createMockerWithIdentity(function)
//    template <typename identity(decltype(function))> MockTemplate<identity(decltype(function))>* MockerStore<identity(decltype(function))>::pMocker = nullptr; \

#define CreateMockerWithInternal2(mocker, function, identity) \
    CreateMockerWithIdentity(mocker, function, FakeTypeForIdentityFunction##identity)

#define CreateMockerWithInternal(mocker, function, identity) \
    CreateMockerWithInternal2(mocker, function, identity)

#define CreateMocker(mocker, function) \
    CreateMockerWithInternal(mocker, function, __LINE__)

class TryVariableTemplate : public ::testing::Test {
public:
    TestMockTemplate<int(bool, string)> test2Mocker;
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() { }
};

struct TestGetMemberReturnType {
    int* testGetReturnType(bool, char, TryVariableTemplate*) {
        return nullptr;
    }
};

int testGetReturnType(bool, char, TryVariableTemplate*) {
    return 1;
}

TEST_F(TryVariableTemplate, TemplateMember) {
    GetFuctionType<decltype(testGetReturnType)>::ReturnType a;
    GetFuctionType<decltype(&TestGetMemberReturnType::testGetReturnType)>::ReturnType b = &a;
}

TEST_F(TryVariableTemplate, Test2Mocker) {
    EXPECT_CALL(test2Mocker, MockFunction(false, ""))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return(2));
    EXPECT_CALL(test2Mocker, MockFunction(true, "ANY"))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return(9));
    EXPECT_EQ(2, test2Mocker.MockFunction(false, ""));
    EXPECT_EQ(9, test2Mocker.MockFunction(true, "ANY"));
}

TEST_F(TryVariableTemplate, TestInternalType) {
    struct TestType;
    TestMockTemplate<TestType*()> testInternalType;
}

TEST_F(TryVariableTemplate, GlobalFunction) {
    CreateMocker(mocker, testGetReturnType);
    EXPECT_CALL(*mocker, MockFunction(false, 'L', nullptr))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return(2));
    EXPECT_EQ(2, testGetReturnType(false, 'L', nullptr));
}

