#include <unistd.h>
#include <sys/mman.h>

// PaegSize just use for calling page unprotect API with alignment address.
// TODO(guzuchao): get page size from runtime.
size_t PageSize = getpagesize();

int unprotect_memory(const void * address, size_t length) {
    void * const page = reinterpret_cast<void*>(reinterpret_cast<long long>(address) & (~(PageSize - 1)));
    // void * const page = const_cast<void*>(address);
    return mprotect(page, length, PROT_READ | PROT_WRITE | PROT_EXEC);
}

int unprotect_memory_for_one_page(void * address) {
    return unprotect_memory(address, PageSize);
}
