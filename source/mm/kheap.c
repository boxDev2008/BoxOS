#include <mm/dalloc.h>

#define HEAP_SIZE 1048576 * 16

uint8_t heap_array[HEAP_SIZE];
heap_t heap;

void Kheap_Initialize(void)
{
    heap_init(&heap, (void*)heap_array, HEAP_SIZE);
}

void kmalloc(size_t size, void **ptr)
{
    dalloc(&heap, size, ptr);
}

void krealloc(size_t size, void **ptr)
{
    drealloc(&heap, size, ptr);
}

void kfree(void **ptr)
{
    dfree(&heap, ptr, USING_PTR_ADDRESS);
}