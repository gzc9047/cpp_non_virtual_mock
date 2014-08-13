#ifndef MOCK_NON_VIRTUAL_H
#define MOCK_NON_VIRTUAL_H

#include "set_function_jump.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <vector>

#define USE_FUNCTION_OBJECT(keyName) \
    fake_##keyName

#define USE_FUNCTION_WRAP(keyName) \
    Wrap_##keyName

#define DEFINE_MOCK0_CLASS(keyName, returnValue) \
    struct Fake_##keyName { MOCK_METHOD0(keyName, returnValue()); std::vector<char> binary_backup; }

#define DECLARE_FUNCTION_MOCK0(keyName, returnValue) \
    DEFINE_MOCK0_CLASS(keyName, returnValue); \
    Fake_##keyName* USE_FUNCTION_OBJECT(keyName); \
    returnValue USE_FUNCTION_WRAP(keyName)() { \
        return USE_FUNCTION_OBJECT(keyName)->keyName(); \
    }

#define DECLARE_MEMBER_FUNCTION_MOCK0(keyName, returnValue) \
    DEFINE_MOCK0_CLASS(keyName, returnValue); \
    Fake_##keyName* USE_FUNCTION_OBJECT(keyName); \
    struct Fake_memder_##keyName { \
        returnValue USE_FUNCTION_WRAP(keyName)() { \
            return USE_FUNCTION_OBJECT(keyName)->keyName(); \
        } \
    }

#define DECLARE_STATIC_MEMBER_FUNCTION_MOCK0(keyName, returnValue) DECLARE_FUNCTION_MOCK0(keyName, returnValue)

#define INIT_FUNCTION(method, keyName) \
    USE_FUNCTION_OBJECT(keyName) = new Fake_##keyName(); \
    SetFunctionJump(method, &USE_FUNCTION_WRAP(keyName), USE_FUNCTION_OBJECT(keyName)->binary_backup)

#define INIT_MEMBER_FUNCTION(method, keyName) \
    USE_FUNCTION_OBJECT(keyName) = new Fake_##keyName(); \
    SetFunctionJump(method, &Fake_memder_##keyName::USE_FUNCTION_WRAP(keyName), USE_FUNCTION_OBJECT(keyName)->binary_backup)

#define INIT_STATIC_MEMBER_FUNCTION(method, keyName) INIT_FUNCTION(method, keyName)

#define RESTORE_FUNCTION(method, keyName) \
    RestoreJump(method, USE_FUNCTION_OBJECT(keyName)->binary_backup)

#define FINALIZE_FUNCTION(method, keyName) \
    RESTORE_FUNCTION(method, keyName); \
    delete USE_FUNCTION_OBJECT(keyName)

#endif // MOCK_NON_VIRTUAL_H
