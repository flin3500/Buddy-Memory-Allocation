#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
	virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 23, 23);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 800000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+3, 8000000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+3, 10000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+3, 100);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap,virtual_heap+3);
    virtual_info(virtual_heap);
    return 0;
}