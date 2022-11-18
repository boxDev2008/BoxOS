#include <kcsl.h>
#include <drivers/vga.h>
#include <vga_modes.h>

#include <io_ports.h>
#include <string.h>

#include <bios32.h>
#include <drivers/vesa.h>

#include <mm/kheap.h>

#include <interrupt/gdt.h>
#include <interrupt/isr.h>

#include <drivers/mouse.h>
#include <drivers/keyboard.h>
#include <drivers/timer.h>
#include <drivers/ide.h>

#include <gui/window.h>

void kmain(void)
{
    gdt_initialize();
    idt_initialize();

    kheap_initialize();

    bios32_initialize();

    vesa_initialize(1280, 1024, 32);

    keyboard_initialize();
    mouse_initialize();

    vbe_set_background(VBE_RGB(24, 24, 24));

    gui_window_t *window = create_gui_window(400, 240);

    while (1)
    {
        vbe_clear_backbuffer();

        vbe_draw_string("Welcome to BoxOS!", 1280 / 2, 1024 / 2, VBE_RGB(255, 255, 255));

        gui_window_update(window);

        for (int x = 0; x < 1280; x++)
            for (int y = 0; y < 32; y++)
            {
                vbe_putpixel(x, 1024 - 32 + y, VBE_RGB(14, 14, 14));
                vbe_putpixel(x, 1024 - 32, VBE_RGB(42, 42, 42));
            }

        vbe_swapbuffers();
    }

    destroy_gui_window(window);

    //Switch back to VBA
    /*registers16_t in = {0};

    in.ax = 0x4F02;
    in.bx = 0x0;
    int86(0x10, &in, (registers16_t*){ 0 });*/

    //vga_write_regs(g_90x30_text);
    //kcsl_initialize();
    
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

    //kshell_initialize();

    //ata_initialize();
}