#include <drivers/keyboard.h>

#include <interrupt/idt.h>
#include <interrupt/isr.h>

#include <io_ports.h>

#include <kcsl.h>
#include <string.h>

#include <stdint.h>

void keyboard_handler(registers_t *regs)
{
    inportb(KEYBOARD_DATA_PORT);
}

void keyboard_initialize(void)
{
    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, keyboard_handler);
}