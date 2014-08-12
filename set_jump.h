#include <cstddef>
#include <iostream>
#include <vector>

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

int set_jump(void * address, const void * destination, std::vector<char>& binary_backup) {
    char * const function = reinterpret_cast<char *>(address);
    std::size_t function2 = reinterpret_cast<std::size_t>(destination)
        - reinterpret_cast<std::size_t>(address)
        - 5; // For jmp instruction;
    char * const function3 = reinterpret_cast<char *>(&function2);
    binary_backup = std::vector<char>(function, function + 5);
    function[0] = 0xE9;
    for (int i = 0; i < 4; ++i) {
        function[i + 1] = function3[i];
    }
    return 0;
}

void restore_jump(void * address, const std::vector<char>& binary_backup) {
    char * const function = reinterpret_cast<char *>(address);
    for (int i = 0; i < binary_backup.size(); ++i) {
        function[i] = binary_backup[i];
    }
}
