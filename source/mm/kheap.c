#include <mm/dalloc.h>

#define HEAP_SIZE 1048576

uint8_t heap_array[HEAP_SIZE];
heap_t heap;

void kheap_initialize(void)
{
    heap_init(&heap, (void*)heap_array, HEAP_SIZE);
}

void *kmalloc(size_t size)
{
    void *ptr = NULL;
    dalloc(&heap, size, (void**)&ptr);
    return ptr;
}

void *krealloc(void *ptr, size_t size)
{
    dalloc(&heap, size, (void**)&ptr);
    return ptr;
}

void kfree(void *ptr)
{
    dfree(&heap, (void**)&ptr, USING_PTR_ADDRESS);
}