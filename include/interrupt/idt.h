#pragma once

#include <stdint.h>

#define NO_IDT_DESCRIPTORS 256

typedef struct
{
    uint16_t base_low;
    uint16_t segment_selector;
    uint8_t zero;
    uint8_t type;
    uint16_t base_high;
}
__attribute__((packed)) IDT;

typedef struct
{
    uint16_t limit;
    uint32_t base_address;
}
__attribute__((packed)) IDT_PTR;

extern void IDT_Load(uint32_t idt_ptr);

void IDT_SetEntry(int index, uint32_t base, uint16_t seg_sel, uint8_t flags);

void IDT_Initialize(void);