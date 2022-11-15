#include <kcsl.h>
#include <kcvs.h>
#include <vga.h>
#include <vga_modes.h>

#include <io_ports.h>
#include <string.h>

#include <mm/kheap.h>

#include <interrupt/gdt.h>
#include <interrupt/isr.h>

#include <drivers/mouse.h>
#include <drivers/keyboard.h>
#include <drivers/timer.h>
#include <drivers/ide.h>

void kmain(void)
{
    gdt_initialize();
    idt_initialize();

    kheap_initialize();

    vga_write_regs(g_90x30_text);
    kcsl_initialize();
    
    //kcvs_initialize();
    //kcvs_clear(VGA_COLOR_BLACK);

    /*kprintf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
        "    /\\_____/\\",
        "   /         \\           Welcome to",
        "  (  x  w  x  )            BOX OS",
        "   ) =     = (",
        "  /           \\   Type \"help pls\" for help",
        " /             \\",
        "(__(__)___(__)__)");*/

    //timer_initialize(1);

    kshell_initialize();

    keyboard_initialize();
    mouse_initialize();

    ata_initialize();
    ide_write_sectors(0, 1, 0, "");

    char buffer[1028];
    ide_read_sectors(0, 1, 0, buffer);

    kprintf("\n%s\n", buffer);
}