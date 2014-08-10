#include <cstddef>

// TODO(guzuchao): support 64bit long jmp, for now just support 32bit distance.
int set_jump(void * address, const void * destination) {
    char * const function = reinterpret_cast<char *>(address);
    std::size_t function2 = reinterpret_cast<std::size_t>(destination)
        - reinterpret_cast<std::size_t>(address)
        - 5; // For jmp instruction;
    char * const function3 = reinterpret_cast<char *>(&function2);
    function[0] = 0xE9;
    for (int i = 0; i < 4; ++i) {
        function[i + 1] = function3[i];
    }
    return 0;
}
