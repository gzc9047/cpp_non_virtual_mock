#ifndef MOCK_NON_VIRTUAL_P1_H
#define MOCK_NON_VIRTUAL_P1_H

#include "get_function_type.h"
#include "set_function_jump.h"
#include "type_mapper.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
struct MockTemplateWithThisPoint : public MockTemplateBase<T> {
};

template < typename T >
struct MockerStore {
};

template < typename T >
struct MockerStoreWithThisPoint {
};

template < typename I, typename F >
struct MockerStore<I(F)> {
    static MockTemplate<I(F)>* pMocker;
};

template < typename I, typename F >
struct MockerStoreWithThisPoint<I(F)> {
    static MockTemplateWithThisPoint<I(F)>* pMocker;
};

template < typename I, typename F > MockTemplate<I(F)>* MockerStore<I(F)>::pMocker = nullptr;
template < typename I, typename F > MockTemplateWithThisPoint<I(F)>* MockerStoreWithThisPoint<I(F)>::pMocker = nullptr;

template < typename T >
struct MockEntryPoint {
};

template < typename T >
struct MockEntryPointWithThisPoint {
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPoint<I(R(C::*)(P ...) const)> {
    R EntryPoint(P... p) {
        std::cout << "Const thisPoint: " << this << std::endl;
        return MockerStore<I(R(C::*)(P ...) const)>::pMocker->MockFunction(p ...);
    }
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPoint<I(R(C::*)(P ...))> {
    R EntryPoint(P... p) {
        std::cout << "ThisPoint: " << this << std::endl;
        return MockerStore<I(R(C::*)(P ...))>::pMocker->MockFunction(p ...);
    }
};

template < typename I, typename R, typename ... P >
struct MockEntryPoint<I(R(P ...))> {
    static R EntryPoint(P... p) {
        return MockerStore<I(R(P ...))>::pMocker->MockFunction(p ...);
    }
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPointWithThisPoint<I(R(C::*)(P ...) const)> {
    R EntryPoint(P... p) {
        std::cout << "M ThisPoint: " << this << std::endl;
        return MockerStoreWithThisPoint<I(R(C::*)(void*, P ...) const)>::pMocker->MockFunction(this, p ...);
    }
};

template < typename I, typename C, typename R, typename ... P >
struct MockEntryPointWithThisPoint<I(R(C::*)(P ...))> {
    R EntryPoint(P... p) {
        std::cout << "M ThisPoint: " << this << std::endl;
        return MockerStoreWithThisPoint<I(R(C::*)(void*, P ...))>::pMocker->MockFunction(this, p ...);
    }
};

template < typename T >
struct GmockMatcherMapper {
    typedef const ::testing::Matcher<T>& Type;
};

template < typename R, typename ... P >
struct MockTemplateBase<R(P ...)> {
    typedef R FunctionType(P ...);

    R MockFunction(P... p) {
        gmocker.SetOwnerAndName(this, "MockFunction");
        return gmocker.Invoke(p ...);
    }

    template < typename ... M >
    ::testing::MockSpec<R(P...)>& gmock_MockFunction(M... m) {
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
        MockerStore<IntegrateType>::pMocker = nullptr;
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
        MockerStore<IntegrateType>::pMocker = nullptr;
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
        MockerStore<IntegrateType>::pMocker = nullptr;
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<FunctionType>::binaryBackup);
    }

    FunctionType* originFunction;
};

template < typename I, typename C, typename R, typename ... P>
struct MockTemplateWithThisPoint<I(R(C::*)(void*, P ...) const)> : MockTemplateBase<R(void*, P ...)> {
    typedef I IntegrateType(R(C::*)(void*, P ...) const);
    typedef I EntryPointType(R(C::*)(P ...) const);
    typedef R (C::*FunctionType)(P ...) const;
    typedef R StubFunctionType(void*, P ...);
    MockTemplateWithThisPoint(FunctionType function): originFunction(function) {
        SetFunctionJump(originFunction,
                &MockEntryPointWithThisPoint<EntryPointType>::EntryPoint,
                MockTemplateBase<StubFunctionType>::binaryBackup);
        MockerStoreWithThisPoint<IntegrateType>::pMocker = this;
    }

    ~MockTemplateWithThisPoint() {
        RestoreMock();
        MockerStoreWithThisPoint<IntegrateType>::pMocker = nullptr;
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<StubFunctionType>::binaryBackup);
    }

    FunctionType originFunction;
};

template < typename I, typename C, typename R, typename ... P>
struct MockTemplateWithThisPoint<I(R(C::*)(void*, P ...))> : MockTemplateBase<R(void*, P ...)> {
    typedef I IntegrateType(R(C::*)(void*, P ...));
    typedef I EntryPointType(R(C::*)(P ...));
    typedef R (C::*FunctionType)(P ...);
    typedef R StubFunctionType(void*, P ...);
    MockTemplateWithThisPoint(FunctionType function): originFunction(function) {
        SetFunctionJump(originFunction,
                &MockEntryPointWithThisPoint<EntryPointType>::EntryPoint,
                MockTemplateBase<StubFunctionType>::binaryBackup);
        MockerStoreWithThisPoint<IntegrateType>::pMocker = this;
    }

    ~MockTemplateWithThisPoint() {
        RestoreMock();
        MockerStoreWithThisPoint<IntegrateType>::pMocker = nullptr;
    }

    void RestoreMock() {
        RestoreJump(originFunction, MockTemplateBase<StubFunctionType>::binaryBackup);
    }

    FunctionType originFunction;
};

template < typename I >
struct MockerCreator {
    template < typename C, typename R, typename ... P >
    static std::unique_ptr<MockTemplate<I(R(C::*)(P ...) const)>>
            createMockerWithIdentity(R (C::*function)(P ...) const) {
        typedef I IntegrateType(R(C::*)(P ...) const);
        return std::unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename C, typename R, typename ... P >
    static std::unique_ptr<MockTemplate<I(R(C::*)(P ...))>>
            createMockerWithIdentity(R (C::*function)(P ...)) {
        typedef I IntegrateType(R(C::*)(P ...));
        std::cout << "Meet member function here" << std::endl;
        return std::unique_ptr<MockTemplate<IntegrateType>>(new MockTemplate<IntegrateType>(function));
    }

    template < typename R, typename ... P >
    static std::unique_ptr<MockTemplate<I(R(P ...))>>
            createMockerWithIdentity(R function(P ...)) {
        std::cout << "Meet normal function here" << std::endl;
        return std::unique_ptr<MockTemplate<I(R(P ...))>>(new MockTemplate<I(R(P ...))>(function));
    }

    template < typename C, typename R, typename ... P >
    static std::unique_ptr<MockTemplateWithThisPoint<I(R(C::*)(void*, P ...) const)>>
            createMockerWithIdentityWithThisCheck(R (C::*function)(P ...) const) {
        typedef I IntegrateType(R(C::*)(void*, P ...) const);
        return std::unique_ptr<MockTemplateWithThisPoint<IntegrateType>>(new MockTemplateWithThisPoint<IntegrateType>(function));
    }

    template < typename C, typename R, typename ... P >
    static std::unique_ptr<MockTemplateWithThisPoint<I(R(C::*)(void*, P ...))>>
            createMockerWithIdentityWithThisCheck(R (C::*function)(P ...)) {
        typedef I IntegrateType(R(C::*)(void*, P ...));
        return std::unique_ptr<MockTemplateWithThisPoint<IntegrateType>>(new MockTemplateWithThisPoint<IntegrateType>(function));
    }
};

#define CreateMockerWithIdentity(mocker, function, identity, creator) \
    struct identity {}; \
    std::cout << "MockIdentity name: " << typeid(identity).name() << std::endl; \
    auto mocker = MockerCreator<identity>::creator(function)

#define CreateMockerWithInternal2(mocker, function, identity, creator) \
    CreateMockerWithIdentity(mocker, function, FakeTypeForIdentityFunction##identity, creator)

#define CreateMockerWithInternal(mocker, function, identity, creator) \
    CreateMockerWithInternal2(mocker, function, identity, creator)

#define CreateMocker(mocker, function) \
    CreateMockerWithInternal(mocker, function, __COUNTER__, createMockerWithIdentity)

#define CreateMockerWithThisCheck(mocker, function) \
    CreateMockerWithInternal(mocker, function, __COUNTER__, createMockerWithIdentityWithThisCheck)

#endif // MOCK_NON_VIRTUAL_P1_H
