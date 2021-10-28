#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
	virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 23, 10);
    virtual_info(virtual_heap);
    virtual_malloc(virtual_heap, 9000000);
    virtual_info(virtual_heap);
    virtual_realloc(virtual_heap,virtual_heap+2500, 9000000);
    virtual_info(virtual_heap);
    return 0;
}
