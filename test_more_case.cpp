#include "mock_non_virtual_p1.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <string>

using namespace std;
using namespace ::testing;

struct TestReferenceMatcher {
    MOCK_METHOD2(Match, string(const string&, string&));
    MOCK_METHOD2(MatchNonReference, string(const string, string));
};

struct TryMoreCase : public Test {
    TryMoreCase(): stringConstValue("HELLO") {
    }
    virtual void SetUp() {
        boolValue = false;
        charValue = '0';
        intValue = 9047;
        stringValue = "LOUIX";
    }

    virtual void TearDown() { }
    bool boolValue;
    char charValue;
    int intValue;
    string stringValue;
    const string stringConstValue;
};

int testReference(bool& boolValue, char& charValue, int& intValue, string& stringValue, const string& stringConstValue) {
    return 0;
}

struct TestReference {
    int testReference(bool& boolValue, char& charValue, int& intValue, string& stringValue, const string& stringConstValue) {
        return 0;
    }
    int paddingInt;
    char paddingChar;
    bool paddingBool;
    string paddingString;
};

struct TestSubReference : TestReference {
    int testSubReference(bool& boolValue, char& charValue, int& intValue, string& stringValue, const string& stringConstValue) const {
        return 0;
    }
};

int testReferenceStubP1P2(bool& boolValue, char& charValue) {
    boolValue = !boolValue;
    charValue += 1;
    return 12;
}

int testReferenceStubP3P4P5(int& intValue, string& stringValue, const string& stringConstValue) {
    --intValue;
    stringValue.resize(0);
    cout << "Get stringConstValue: " << stringConstValue << endl;
    return 345;
}

TEST_F(TryMoreCase, TestReferenceMatcher) {
    TestReferenceMatcher testMather;
    EXPECT_CALL(testMather, MatchNonReference(_, _)).Times(AtLeast(0))
        .WillOnce(Return("HELLO"));
    EXPECT_CALL(testMather, Match(_, _)).Times(AtLeast(0))
        .WillOnce(Return("LOUIX"));
    EXPECT_EQ("HELLO", testMather.MatchNonReference(string("_"), string("L")));
    string a = "_";
    string b = "L";
    EXPECT_EQ("LOUIX", testMather.Match(a, b));
}

TEST_F(TryMoreCase, TestReference) {
    CreateMocker(mocker, testReference);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<0, 1>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<2, 3, 4>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
}

TEST_F(TryMoreCase, TestReference2) {
    TestReference testObject;
    cout << "CaseThisPoint: " << &testObject << endl;
    CreateMocker(mocker, &TestReference::testReference);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<0, 1>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<2, 3, 4>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
}

TEST_F(TryMoreCase, TestSubReference) {
    TestSubReference testObject;
    cout << "CaseThisPoint: " << &testObject << endl;
    CreateMocker(mocker, &TestSubReference::testReference);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<0, 1>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<2, 3, 4>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
}

TEST_F(TryMoreCase, TestConstReference) {
    TestSubReference testObject;
    cout << "CaseThisPoint: " << &testObject << endl;
    CreateMocker(mocker, &TestSubReference::testSubReference);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<0, 1>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testObject.testSubReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<2, 3, 4>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testObject.testSubReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
}

TEST_F(TryMoreCase, TestThisCheck) {
    TestSubReference testObject;
    TestSubReference testObject2;
    cout << "CaseThisPoint: " << &testObject << endl;
    CreateMockerWithThisCheck(mocker, &TestSubReference::testReference);
    EXPECT_CALL(*mocker, MockFunction(&testObject, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<1, 2>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(&testObject, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<3, 4, 5>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testObject.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
    EXPECT_CALL(*mocker, MockFunction(_, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(Return(9));
    EXPECT_EQ(9, testObject2.testReference(boolValue, charValue, intValue, stringValue, stringConstValue));
}

TEST_F(TryMoreCase, TestConstThisCheck) {
    TestSubReference testObject;
    TestSubReference testObject2;
    cout << "CaseThisPoint: " << &testObject << endl;
    CreateMockerWithThisCheck(mocker, &TestSubReference::testSubReference);
    EXPECT_CALL(*mocker, MockFunction(&testObject, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<1, 2>(Invoke(testReferenceStubP1P2)));
    EXPECT_EQ(12, testObject.testSubReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_TRUE(boolValue);
    EXPECT_EQ('1', charValue);
    EXPECT_CALL(*mocker, MockFunction(&testObject, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(WithArgs<3, 4, 5>(Invoke(testReferenceStubP3P4P5)));
    EXPECT_EQ(345, testObject.testSubReference(boolValue, charValue, intValue, stringValue, stringConstValue));
    EXPECT_EQ(9046, intValue);
    EXPECT_STREQ("", stringValue.c_str());
    EXPECT_CALL(*mocker, MockFunction(_, boolValue, charValue, intValue, stringValue, _))
        .Times(AtLeast(1))
        .WillOnce(Return(9));
    EXPECT_EQ(9, testObject2.testSubReference(boolValue, charValue, intValue, stringValue, stringConstValue));
}
