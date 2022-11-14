#include <kcsl.h>
#include <kcvs.h>
#include <vga.h>
#include <vga_modes.h>

#include <io_ports.h>
#include <string.h>
#include <timer.h>

#include <mm/kheap.h>

#include <interrupt/gdt.h>
#include <interrupt/isr.h>

#include <drivers/mouse.h>
#include <drivers/keyboard.h>

void kmain(void)
{
    vga_write_regs(g_90x30_text);
    kcsl_initialize();
    
    //kcvs_initialize();
    //kcvs_clear(VGA_COLOR_BLACK);

    kheap_initialize();

    gdt_initialize();
    idt_initialize();

    kprintf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
        "    /\\_____/\\",
        "   /         \\           Welcome to",
        "  (  x  w  x  )            BOX OS",
        "   ) =     = (",
        "  (           )   Type \"help pls\" for help",
        " (             )",
        "(__(__)___(__)__)");

    timer_initialize(1);

    kshell_initialize();

    keyboard_initialize();
    mouse_initialize();
}