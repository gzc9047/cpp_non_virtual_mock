#include "get_function_type.h"
#include "set_function_jump.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <functional>
#include <iostream>
#include <memory>

using namespace std;

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
struct MockTemplateBase {
};

template < typename T >
struct MockTemplate : public MockTemplateBase<T> {
};

template < typename T >
struct MockEntryPoint {
};

template < typename T >
struct MockerStore {
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPoint<I(R(C::*)(P ...) const)> {
    R EntryPoint(P... p) {
        return MockerStore<I(R(C::*)(P ...) const)>::pMocker->MockFunction(p ...);
    }
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPoint<I(R(C::*)(P ...))> {
    R EntryPoint(P... p) {
        return MockerStore<I(R(C::*)(P ...))>::pMocker->MockFunction(p ...);
    }
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

template < typename R, typename ... P>
struct MockTemplateBase<R(P ...)> {
    typedef R FunctionType(P ...);

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
};

template < typename I, typename C, typename R, typename ... P>
struct MockTemplate<I(R(C::*)(P ...) const)> : MockTemplateBase<R(P ...)> {
    typedef I IntegrateType(R(C::*)(P ...) const);
    typedef R (C::*FunctionType)(P ...) const;
    typedef R StubFunctionType(P ...);
    MockTemplate(FunctionType function): originFunction(function) {
        SetFunctionJump(originFunction,
                &MockEntryPoint<IntegrateType>::EntryPoint,
                MockTemplateBase<StubFunctionType>::binaryBackup);
        MockerStore<IntegrateType>::pMocker = this;
    }

    ~MockTemplate() {
        RestoreMock();
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<StubFunctionType>::binaryBackup);
    }

    FunctionType originFunction;
};

template < typename I, typename C, typename R, typename ... P>
struct MockTemplate<I(R(C::*)(P ...))> : MockTemplateBase<R(P ...)> {
    typedef I IntegrateType(R(C::*)(P ...));
    typedef R (C::*FunctionType)(P ...);
    typedef R StubFunctionType(P ...);
    MockTemplate(FunctionType function): originFunction(function) {
        SetFunctionJump(originFunction,
                &MockEntryPoint<IntegrateType>::EntryPoint,
                MockTemplateBase<StubFunctionType>::binaryBackup);
        MockerStore<IntegrateType>::pMocker = this;
    }

    ~MockTemplate() {
        RestoreMock();
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<StubFunctionType>::binaryBackup);
    }

    FunctionType originFunction;
};

template < typename I, typename R, typename ... P>
struct MockTemplate<I(R(P ...))> : MockTemplateBase<R(P ...)> {
    typedef I IntegrateType(R(P ...));
    typedef R FunctionType(P ...);
    MockTemplate(FunctionType function): originFunction(function) {
        SetFunctionJump(originFunction,
                MockEntryPoint<IntegrateType>::EntryPoint,
                MockTemplateBase<FunctionType>::binaryBackup);
        MockerStore<IntegrateType>::pMocker = this;
    }

    ~MockTemplate() {
        RestoreMock();
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<FunctionType>::binaryBackup);
    }

    FunctionType* originFunction;
};

template < typename I >
struct MockerCreator {
    template < typename C, typename R, typename ... P>
    static unique_ptr<MockTemplate<I(R(C::*)(P ...) const)>> createMockerWithIdentity(R (C::*function)(P ...) const) {
        typedef I IntegrateType(R(C::*)(P ...) const);
        return unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename C, typename R, typename ... P>
    static unique_ptr<MockTemplate<I(R(C::*)(P ...))>> createMockerWithIdentity(R (C::*function)(P ...)) {
        typedef I IntegrateType(R(C::*)(P ...));
        return unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename R, typename ... P>
    static unique_ptr<MockTemplate<I(R(P ...))>> createMockerWithIdentity(R function(P ...)) {
        return unique_ptr<MockTemplate<I(R(P ...))>>(new MockTemplate<I(R(P ...))>(function));
    }
};

#define CreateMockerWithIdentity(mocker, function, identity) \
    struct identity {}; \
    auto mocker = MockerCreator<identity>::createMockerWithIdentity(function)

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
    string testGetReturnType(bool, char, TryVariableTemplate*) {
        return "";
    }
    string testConst(bool, char, TryVariableTemplate*) const {
        return "";
    }
};

int testGetReturnType(bool, char, TryVariableTemplate*) {
    return 1;
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

TEST_F(TryVariableTemplate, ConstMemberFunction) {
    CreateMocker(mocker, &TestGetMemberReturnType::testConst);
    EXPECT_CALL(*mocker, MockFunction(false, 'L', nullptr))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("Hello"))
        .WillOnce(::testing::Return("Louix"));
    EXPECT_STREQ("Hello", TestGetMemberReturnType().testConst(false, 'L', nullptr).c_str());
    EXPECT_STREQ("Louix", TestGetMemberReturnType().testConst(false, 'L', nullptr).c_str());
}

TEST_F(TryVariableTemplate, MemberFunction) {
    CreateMocker(mocker, &TestGetMemberReturnType::testGetReturnType);
    EXPECT_CALL(*mocker, MockFunction(false, 'L', nullptr))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return("Hello"))
        .WillOnce(::testing::Return("Louix"));
    EXPECT_STREQ("Hello", TestGetMemberReturnType().testGetReturnType(false, 'L', nullptr).c_str());
    EXPECT_STREQ("Louix", TestGetMemberReturnType().testGetReturnType(false, 'L', nullptr).c_str());
}

TEST_F(TryVariableTemplate, GlobalFunction) {
    CreateMocker(mocker, testGetReturnType);
    EXPECT_CALL(*mocker, MockFunction(false, 'L', nullptr))
        .Times(::testing::AtLeast(0))
        .WillOnce(::testing::Return(2))
        .WillOnce(::testing::Return(0));
    EXPECT_EQ(2, testGetReturnType(false, 'L', nullptr));
    EXPECT_EQ(0, testGetReturnType(false, 'L', nullptr));
}

