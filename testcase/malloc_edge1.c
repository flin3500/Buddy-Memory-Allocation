#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
    virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 15, 12);
    virtual_malloc(virtual_heap, 4097);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 8193);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 16384);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 8192);
    virtual_info(virtual_heap);
    return 0;
}