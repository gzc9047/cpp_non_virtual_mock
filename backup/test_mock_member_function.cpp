#include "member_function_impl.h"

#include "set_jump.h"
#include "unprotect_memory_page.h"

#include <iostream>
#include <string>

using namespace std;

void output1(NonMocked* object) {
    cout << "Non mocked: " << object->returnNonMockedString() << endl;
}

void output2(NonMocked* object) {
    cout << "Mocked: " << object->returnNonMockedString() << endl;
}

int main() {
    auto origin_function = &NonMocked::returnNonMockedString;
    auto target_function = &Mocked::returnMockedString;
    void * const function = reinterpret_cast<char *>((long long&)origin_function);
    unprotect_memory_for_one_page(function);
    NonMocked object;
    output1(&object);
    set_jump(function, reinterpret_cast<void *>((long long&)target_function));
    output2(&object);
    return 0;
}
