#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
	virtual_heap = sbrk(0);
    init_allocator(virtual_heap, 13, 10);
    virtual_free(virtual_heap, NULL);
    return 0;
}

