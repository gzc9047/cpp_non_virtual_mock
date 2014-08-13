#ifndef SET_FUNCTION_JUMP_H
#define SET_FUNCTION_JUMP_H

#include "set_jump.h"
#include "unprotect_memory_page.h"

#include <cstddef>
#include <vector>

// TODO(guzuchao): support 64bit long jmp, for now just support 32bit distance.
template < typename F1, typename F2 >
int SetFunctionJump(F1 address, F2 destination, std::vector<char>& binary_backup) {
    void* function = reinterpret_cast<void*>((std::size_t&)address);
    unprotect_memory_for_one_page(function);
    return set_jump(function, reinterpret_cast<void*>((std::size_t&)destination), binary_backup);
}

template < typename F1 >
void RestoreJump(F1 address, const std::vector<char>& binary_backup) {
    void* function = reinterpret_cast<void*>((std::size_t&)address);
    restore_jump(function, binary_backup);
}

#endif // SET_FUNCTION_JUMP_H
