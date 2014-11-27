#include "member_function_impl.h"

#include <iostream>
#include <string>

using namespace std;


string NonMocked::returnNonMockedString() {
    return "Non mocked string";
}

string mockedString = "Mocked string";

string Mocked::returnMockedString() {
    return mockedString;
}
