#include "mock_non_virtual_p1.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <string>

// TODO(guzuchao): impl.
using namespace std;
using namespace ::testing;

void __stdcall TestStdCall() {
}
struct TrySpecialCall : public ::testing::Test {
    void TestStdCall() {}
    virtual void SetUp() {
    }

    virtual void TearDown() { }
};

int testReference(bool& boolValue, char& charValue, int& intValue, string& stringValue, const string& stringConstValue) {
    return 0;
}

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

TEST_F(TrySpecialCall, TestReference) { }

