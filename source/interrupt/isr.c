#include <interrupt/isr.h>
#include <interrupt/idt.h>
#include <interrupt/8259_pic.h>

#include <kcsl.h>

#include <stddef.h>

ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];

char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt (intel reserved)",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void ISR_RegisterInterruptHandler(int num, ISR handler)
{
    
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

void ISR_EndInterrupt(int num)
{
    PIC8259_Eoi(num);
}

void ISR_IrqHandler(Registers *reg)
{
    if (g_interrupt_handlers[reg->int_no] != NULL)
    {
        ISR handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    PIC8259_Eoi(reg->int_no);
}

static void print_registers(Registers *reg)
{
    kprintf("Registers:\n");
    kprintf("err_code=%d\n", reg->err_code);
    kprintf("eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    kprintf("edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    kprintf("eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}

void ISR_ExceptionHandler(Registers reg)
{
    if (reg.int_no < 32)
    {
        kprintf("EXCEPTION: %s\n", exception_messages[reg.int_no]);
        print_registers(&reg);
    }
    if (g_interrupt_handlers[reg.int_no] != NULL)
    {
        ISR handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}
