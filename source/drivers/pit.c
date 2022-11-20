#include <drivers/pit.h>

#include <interrupt/isr.h>

#include <io_ports.h>

uint32_t g_tick = 0;

uint32_t PIT_GetTick(void)
{
    return g_tick;
}

void PIT_Callback(Registers *regs)
{
    g_tick++;
}

void PIT_Initialize(uint32_t freq)
{
    ISR_RegisterInterruptHandler(IRQ_BASE + IRQ0_TIMER, PIT_Callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);
}
