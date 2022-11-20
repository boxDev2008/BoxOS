#include <interrupt/gdt.h>

GDT g_gdt[NO_GDT_DESCRIPTORS];
GDT_PTR g_gdt_ptr;

void GDT_SetEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    GDT *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}


void GDT_Initialize(void)
{
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32_t)g_gdt;
    
    GDT_SetEntry(0, 0, 0, 0, 0);
    GDT_SetEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    GDT_SetEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    GDT_SetEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    GDT_SetEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    GDT_Load((uint32_t)&g_gdt_ptr);
}
