#ifndef MOCK_NON_VIRTUAL_P1_H
#define MOCK_NON_VIRTUAL_P1_H

#include "get_function_type.h"
#include "set_function_jump.h"

#include "gmock/gmock.h"

#include <iostream>
#include <memory>
#include <vector>

template < typename T >
struct MockTemplateBase {
};

template < typename T >
struct MockTemplate : public MockTemplateBase<T> {
};

template < typename T >
struct MockerStore {
};

template < typename T >
struct MockEntryPoint {
};

template < typename I, typename F >
struct MockerStore<I(F)> {
    static MockTemplate<I(F)>* pMocker;
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

template < typename I, typename F > MockTemplate<I(F)>* MockerStore<I(F)>::pMocker = nullptr;

template < typename R, typename ... P >
struct MockTemplateBase<R(P ...)> {
    typedef R FunctionType(P ...);

    R MockFunction(P... p) {
        gmocker.SetOwnerAndName(this, "MockFunction");
        return gmocker.Invoke(p ...);
    }

    template < typename ... M >
    ::testing::MockSpec<R(P...)>& gmock_MockFunction(M... m) {
        // TODO(guzuchao): need type check M... is Matcher< for every p>
        gmocker.RegisterOwner(this);
        return gmocker.With(m ...);
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
    static std::unique_ptr<MockTemplate<I(R(C::*)(P ...) const)>> createMockerWithIdentity(R (C::*function)(P ...) const) {
        typedef I IntegrateType(R(C::*)(P ...) const);
        return std::unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename C, typename R, typename ... P>
    static std::unique_ptr<MockTemplate<I(R(C::*)(P ...))>> createMockerWithIdentity(R (C::*function)(P ...)) {
        typedef I IntegrateType(R(C::*)(P ...));
        return std::unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename R, typename ... P>
    static std::unique_ptr<MockTemplate<I(R(P ...))>> createMockerWithIdentity(R function(P ...)) {
        return std::unique_ptr<MockTemplate<I(R(P ...))>>(new MockTemplate<I(R(P ...))>(function));
    }
};

#define CreateMockerWithIdentity(mocker, function, identity) \
    struct identity {}; \
    std::cout << "MockIdentity name: " << typeid(identity).name() << std::endl; \
    auto mocker = MockerCreator<identity>::createMockerWithIdentity(function)

#define CreateMockerWithInternal2(mocker, function, identity) \
    CreateMockerWithIdentity(mocker, function, FakeTypeForIdentityFunction##identity)

#define CreateMockerWithInternal(mocker, function, identity) \
    CreateMockerWithInternal2(mocker, function, identity)

#define CreateMocker(mocker, function) \
    CreateMockerWithInternal(mocker, function, __LINE__)

#endif // MOCK_NON_VIRTUAL_P1_H
