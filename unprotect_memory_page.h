#include <sys/mman.h>

// PaegSize just use for calling page unprotect API with alignment address.
// TODO(guzuchao): get page size from runtime.
const long long PageSize = 0x1000;

int unprotect_memory(const void * address, size_t length) {
    void * const page = reinterpret_cast<void*>(reinterpret_cast<long long>(address) & (~(PageSize - 1)));
    return mprotect(page, length, PROT_READ | PROT_WRITE | PROT_EXEC);
}

int unprotect_memory_for_one_page(void * address) {
    return unprotect_memory(address, PageSize);
}
