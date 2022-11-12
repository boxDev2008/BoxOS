#include <timer.h>
#include <io_ports.h>

#include <interrupt/isr.h>

uint32_t g_tick = 0;

uint32_t timer_get_tick(void)
{
    return g_tick;
}

void timer_callback(registers_t *regs)
{
    g_tick++;
}

void timer_initialize(uint32_t freq)
{
    isr_register_interrupt_handler(IRQ_BASE + IRQ0_TIMER, timer_callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);
}
