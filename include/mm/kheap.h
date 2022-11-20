#pragma once

#include <stddef.h>

void Kheap_Initialize(void);

void kmalloc(size_t size, void **ptr);
void krealloc(size_t size, void **ptr);
void kfree(void **ptr);