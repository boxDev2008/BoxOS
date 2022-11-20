#include <interrupt/idt.h>
#include <interrupt/isr.h>
#include <interrupt/8259_pic.h>

IDT g_idt[NO_IDT_DESCRIPTORS];
IDT_PTR g_idt_ptr;

void IDT_SetEntry(int index, uint32_t base, uint16_t seg_sel, uint8_t flags)
{
    IDT *this = &g_idt[index];

    this->base_low = base & 0xFFFF;
    this->segment_selector = seg_sel;
    this->zero = 0;
    this->type = flags | 0x60;
    this->base_high = (base >> 16) & 0xFFFF;
}

void IDT_Initialize(void)
{
    g_idt_ptr.base_address = (uint32_t)g_idt;
    g_idt_ptr.limit = sizeof(g_idt) - 1;
    PIC8259_Initialize();

    IDT_SetEntry(0, (uint32_t)exception_0, 0x08, 0x8E);
    IDT_SetEntry(1, (uint32_t)exception_1, 0x08, 0x8E);
    IDT_SetEntry(2, (uint32_t)exception_2, 0x08, 0x8E);
    IDT_SetEntry(3, (uint32_t)exception_3, 0x08, 0x8E);
    IDT_SetEntry(4, (uint32_t)exception_4, 0x08, 0x8E);
    IDT_SetEntry(5, (uint32_t)exception_5, 0x08, 0x8E);
    IDT_SetEntry(6, (uint32_t)exception_6, 0x08, 0x8E);
    IDT_SetEntry(7, (uint32_t)exception_7, 0x08, 0x8E);
    IDT_SetEntry(8, (uint32_t)exception_8, 0x08, 0x8E);
    IDT_SetEntry(9, (uint32_t)exception_9, 0x08, 0x8E);
    IDT_SetEntry(10, (uint32_t)exception_10, 0x08, 0x8E);
    IDT_SetEntry(11, (uint32_t)exception_11, 0x08, 0x8E);
    IDT_SetEntry(12, (uint32_t)exception_12, 0x08, 0x8E);
    IDT_SetEntry(13, (uint32_t)exception_13, 0x08, 0x8E);
    IDT_SetEntry(14, (uint32_t)exception_14, 0x08, 0x8E);
    IDT_SetEntry(15, (uint32_t)exception_15, 0x08, 0x8E);
    IDT_SetEntry(16, (uint32_t)exception_16, 0x08, 0x8E);
    IDT_SetEntry(17, (uint32_t)exception_17, 0x08, 0x8E);
    IDT_SetEntry(18, (uint32_t)exception_18, 0x08, 0x8E);
    IDT_SetEntry(19, (uint32_t)exception_19, 0x08, 0x8E);
    IDT_SetEntry(20, (uint32_t)exception_20, 0x08, 0x8E);
    IDT_SetEntry(21, (uint32_t)exception_21, 0x08, 0x8E);
    IDT_SetEntry(22, (uint32_t)exception_22, 0x08, 0x8E);
    IDT_SetEntry(23, (uint32_t)exception_23, 0x08, 0x8E);
    IDT_SetEntry(24, (uint32_t)exception_24, 0x08, 0x8E);
    IDT_SetEntry(25, (uint32_t)exception_25, 0x08, 0x8E);
    IDT_SetEntry(26, (uint32_t)exception_26, 0x08, 0x8E);
    IDT_SetEntry(27, (uint32_t)exception_27, 0x08, 0x8E);
    IDT_SetEntry(28, (uint32_t)exception_28, 0x08, 0x8E);
    IDT_SetEntry(29, (uint32_t)exception_29, 0x08, 0x8E);
    IDT_SetEntry(30, (uint32_t)exception_30, 0x08, 0x8E);
    IDT_SetEntry(31, (uint32_t)exception_31, 0x08, 0x8E);
    IDT_SetEntry(32, (uint32_t)irq_0, 0x08, 0x8E);
    IDT_SetEntry(33, (uint32_t)irq_1, 0x08, 0x8E);
    IDT_SetEntry(34, (uint32_t)irq_2, 0x08, 0x8E);
    IDT_SetEntry(35, (uint32_t)irq_3, 0x08, 0x8E);
    IDT_SetEntry(36, (uint32_t)irq_4, 0x08, 0x8E);
    IDT_SetEntry(37, (uint32_t)irq_5, 0x08, 0x8E);
    IDT_SetEntry(38, (uint32_t)irq_6, 0x08, 0x8E);
    IDT_SetEntry(39, (uint32_t)irq_7, 0x08, 0x8E);
    IDT_SetEntry(40, (uint32_t)irq_8, 0x08, 0x8E);
    IDT_SetEntry(41, (uint32_t)irq_9, 0x08, 0x8E);
    IDT_SetEntry(42, (uint32_t)irq_10, 0x08, 0x8E);
    IDT_SetEntry(43, (uint32_t)irq_11, 0x08, 0x8E);
    IDT_SetEntry(44, (uint32_t)irq_12, 0x08, 0x8E);
    IDT_SetEntry(45, (uint32_t)irq_13, 0x08, 0x8E);
    IDT_SetEntry(46, (uint32_t)irq_14, 0x08, 0x8E);
    IDT_SetEntry(47, (uint32_t)irq_15, 0x08, 0x8E);
    IDT_SetEntry(128, (uint32_t)exception_128, 0x08, 0x8E);

    IDT_Load((uint32_t)&g_idt_ptr);
    asm volatile("sti");
}