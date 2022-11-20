#include <bios32.h>
#include <kcsl.h>
#include <string.h>

#include <interrupt/gdt.h>
#include <interrupt/idt.h>

IDT_PTR g_real_mode_gdt;
IDT_PTR g_real_mode_idt;

extern GDT g_gdt[NO_GDT_DESCRIPTORS];

void (*exec_bios32_function)() = (void *)0x7c00;

void Bios32_Initialize(void)
{
    GDT_SetEntry(6, 0, 0xffffffff, 0x9A, 0x0f);
    GDT_SetEntry(7, 0, 0xffffffff, 0x92, 0x0f);
    g_real_mode_gdt.base_address = (uint32_t)g_gdt;
    g_real_mode_gdt.limit = sizeof(g_gdt) - 1;
    g_real_mode_idt.base_address = 0;
    g_real_mode_idt.limit = 0x3ff;
}

/**
 copy data to assembly bios32_call.asm and execute code from 0x7c00 address
*/
void Bios32_Service(uint8_t interrupt, Registers16 *in, Registers16 *out)
{
    void *new_code_base = (void *)0x7c00;

    // copy our GDT entries g_gdt to bios32_gdt_entries(bios32_call.asm)
    memcpy(&bios32_gdt_entries, g_gdt, sizeof(g_gdt));
    // set base_address of bios32_gdt_entries(bios32_call.asm) starting from 0x7c00
    g_real_mode_gdt.base_address = (uint32_t)REBASE_ADDRESS((&bios32_gdt_entries));
    // copy g_real_mode_gdt to bios32_gdt_ptr(bios32_call.asm)
    memcpy(&bios32_gdt_ptr, &g_real_mode_gdt, sizeof(IDT_PTR));
    // copy g_real_mode_idt to bios32_idt_ptr(bios32_call.asm)
    memcpy(&bios32_idt_ptr, &g_real_mode_idt, sizeof(IDT_PTR));
    // copy all 16 bit in registers to bios32_in_reg16_ptr(bios32_call.asm)
    memcpy(&bios32_in_reg16_ptr, in, sizeof(Registers16));
    // get out registers address defined in bios32_call.asm starting from 0x7c00
    void *in_reg16_address = REBASE_ADDRESS(&bios32_in_reg16_ptr);
    // copy bios interrupt number to bios32_int_number_ptr(bios32_call.asm)
    memcpy(&bios32_int_number_ptr, &interrupt, sizeof(uint8_t));

    // copy bios32_call.asm code to new_code_base address
    uint32_t size = (uint32_t)BIOS32_END - (uint32_t)BIOS32_START;
    memcpy(new_code_base, BIOS32_START, size);
    // execute the code from 0x7c00
    exec_bios32_function();
    // copy output registers to out
    in_reg16_address = REBASE_ADDRESS(&bios32_out_reg16_ptr);
    memcpy(out, in_reg16_address, sizeof(Registers16));

    // re-initialize the gdt and idt
    GDT_Initialize();
    IDT_Initialize();
}

// bios interrupt call
void int86(uint8_t interrupt, Registers16 *in, Registers16 *out)
{
    Bios32_Service(interrupt, in, out);
}