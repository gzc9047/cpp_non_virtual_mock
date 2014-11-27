#ifndef SET_JUMP_H
#define SET_JUMP_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

// TODO(guzuchao): support 64bit long jmp, for now just support 32bit distance.
int set_jump(void* address, const void* destination, std::vector<char>& binary_backup) {
    char* const function = reinterpret_cast<char*>(address);
    binary_backup = std::vector<char>(function, function + 5);
    function[0] = 0xE9;
    std::size_t distance = reinterpret_cast<std::size_t>(destination)
        - reinterpret_cast<std::size_t>(address)
        - 5; // For jmp instruction;
    char* const distance_bytes = reinterpret_cast<char*>(&distance);
    std::copy(distance_bytes, distance_bytes + 4, function + 1);
    return 0;
}

void restore_jump(void* address, const std::vector<char>& binary_backup) {
    std::copy(binary_backup.begin(), binary_backup.end(), reinterpret_cast<char*>(address));
}

#endif // SET_JUMP_H
