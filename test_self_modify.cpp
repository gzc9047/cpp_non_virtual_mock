#include "unprotect_memory_page.h"

#include <iostream>

// #include <sys/mman.h>

using namespace std;

int main() {
    char * const function = reinterpret_cast<char*>(main);
    // void * const page = reinterpret_cast<void*>(reinterpret_cast<long long>(main) & (~0xFFFLL));
    // mprotect(page, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC);
    unprotect_memory_for_one_page(function);
    function[0] = 0xC3;
    function[1] = '2';
    cout << "Test: " << function[1] << endl;
    main();
    return 0;
}
