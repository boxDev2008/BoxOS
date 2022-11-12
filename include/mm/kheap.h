#pragma once

#include <stddef.h>

void kheap_initialize(void);
void *kmalloc(size_t size);
void *krealloc(void *ptr, size_t size);
void kfree(void *ptr);