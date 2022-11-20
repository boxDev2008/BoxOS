#include <interrupt/isr.h>
#include <interrupt/idt.h>
#include <interrupt/8259_pic.h>

#include <io_ports.h>

void PIC8259_Initialize(void)
{
    uint8_t a1, a2;
    
    a1 = inportb(PIC1_DATA);
    a2 = inportb(PIC2_DATA);
    
    outportb(PIC1_COMMAND, ICW1);
    outportb(PIC2_COMMAND, ICW1);
    
    outportb(PIC1_DATA, 0x20);
    outportb(PIC2_DATA, 0x28);
    
    outportb(PIC1_DATA, 4);
    outportb(PIC2_DATA, 2);
    
    outportb(PIC1_DATA, ICW4_8086);
    outportb(PIC2_DATA, ICW4_8086);
    
    outportb(PIC1_DATA, a1);
    outportb(PIC2_DATA, a2);
}

void PIC8259_Eoi(uint8_t irq)
{
    if(irq >= 0x28)
        outportb(PIC2, PIC_EOI);
    outportb(PIC1, PIC_EOI);
}

