#include "type_mapper.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <type_traits>
#include <string>

using namespace std;
using namespace ::testing;

template < typename T >
struct GmockMatcherMapper {
    typedef const ::testing::Matcher<T>& Type;
};

class TryTypeMapper : public Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() { }
};

struct TestType
{
    template < typename ... Args >
    static void func() {
        func2<vector<Args>...>();
    }

    template < typename T1, typename ... Args >
    static void func2() {
        cout << "is_same: " << is_same<T1, vector<int>>::value << endl;
    }
};

TEST_F(TryTypeMapper, Basic) {
    TypeMapper<TypeContainer, TypeContainer<int>>::Result typeA;
    TypePackage<TypeContainer<int>>::Type typeB;
    cout << "typeid(typeA): " << typeid(decltype(typeA)).name() << endl;
    cout << "typeid(typeB): " << typeid(decltype(typeB)).name() << endl;
    cout << "is_same: " << is_same<decltype(typeA), decltype(typeB)>::value << endl;
}

TEST_F(TryTypeMapper, 2Type) {
    typedef TypeContainer<int> SingleType;
    TypeMapper<TypeContainer, SingleType, SingleType>::Result typeA;
    TypePackage<SingleType, SingleType>::Type typeB;
    cout << "typeid(typeA): " << typeid(decltype(typeA)).name() << endl;
    cout << "typeid(typeB): " << typeid(decltype(typeB)).name() << endl;
    cout << "is_same: " << is_same<decltype(typeA), decltype(typeB)>::value << endl;
}

TEST_F(TryTypeMapper, GmockMatcherMapper) {
    TypeMapper<GmockMatcherMapper, int, string>::Result typeA;
    TypePackage<GmockMatcherMapper<int>::Type, GmockMatcherMapper<string>::Type>::Type typeB;
    cout << "typeid(typeA): " << typeid(decltype(typeA)).name() << endl;
    cout << "typeid(typeB): " << typeid(decltype(typeB)).name() << endl;
    cout << "is_same: " << is_same<decltype(typeA), decltype(typeB)>::value << endl;
}

TEST_F(TryTypeMapper, Cpp11VariadicTemplate) {
    TestType::func<int>();
    TestType::func<bool, char>();
}

