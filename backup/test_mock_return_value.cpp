#include "set_jump.h"
#include "unprotect_memory_page.h"

#include <iostream>
#include <string>

using namespace std;


string returnNonMockedString() {
    return "Non mocked string";
}

string mockedString = "mocked string";

string returnMockedString() {
    return mockedString;
}

void output1() {
    cout << "Non mocked: " << returnNonMockedString() << endl;
}

void output2() {
    cout << "Mocked: " << returnNonMockedString() << endl;
}

int main() {
    void * const function = reinterpret_cast<char *>(returnNonMockedString);
    unprotect_memory_for_one_page(function);
    output1();
    set_jump(function, reinterpret_cast<void *>(returnMockedString));
    output2();
    return 0;
}
