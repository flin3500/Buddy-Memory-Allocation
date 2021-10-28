#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
    virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 15, 12);
    virtual_realloc(virtual_heap,NULL, 0);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,NULL, 10000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+4, 0);
    virtual_info(virtual_heap);
    return 0;
}