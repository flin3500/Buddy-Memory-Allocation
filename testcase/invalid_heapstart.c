#include "../virtual_alloc.h"

void * virtual_heap = NULL;

void * virtual_sbrk(int32_t increment) {
    return sbrk(increment);

}

int main() {
    init_allocator(virtual_heap, 23, 10);
	virtual_malloc(virtual_heap, 1024);
	virtual_free(virtual_heap, virtual_heap+2+2048);
	virtual_realloc(virtual_heap, virtual_heap+2+2048, 4096);
	virtual_info(virtual_heap);
    return 0;
}
