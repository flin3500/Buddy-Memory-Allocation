#include "virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
	virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 15, 12);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+4, 9000000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+4, 8);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 1);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap,virtual_heap+4+16384);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap,virtual_heap+4);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap,virtual_heap+4);
    virtual_info(virtual_heap);
    virtual_free(virtual_heap,virtual_heap+4);
    virtual_info(virtual_heap);
    
    
    return 0;
}