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

static uint8_t mouse_bitmap[64] =
{
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 0, 0,
};

void kmain(void)
{
    gdt_initialize();
    idt_initialize();

    bios32_initialize();

    vesa_initialize(1280, 1024, 32);

    keyboard_initialize();
    mouse_initialize();

    int x = 0;
    int y = 0;

    int velocity_x = 3, velocity_y = 3;

    vbe_set_background(VBE_RGB(26, 26, 26));

    while (1)
    {
        vbe_clear_backbuffer();

        for (int i = 0; i < 255; i++)
            for (int j = 0; j < 255; j++)
                vbe_putpixel(i + x, j + y, VBE_RGB(i, j, 255));

        if (x < 0)
            velocity_x = 3;
        else if (x + 255 > 1280)
            velocity_x = -3;

        if (y < 0)
            velocity_y = 3;
        else if (y + 255 > 1024)
            velocity_y = -3;

        x += velocity_x;
        y += velocity_y;

        vbe_draw_string("Welcome to BoxOS!", 1280 / 2, 1024 / 2, VBE_RGB(255, 255, 255));

        for (int x = 0; x < 8; x++)
            for (int y = 0; y < 8; y++)
                if (mouse_bitmap[x + y * 8] == 1)
                    vbe_putpixel((int)mouse_get_x() + x, (int)mouse_get_y() + y, VBE_RGB(255, 255, 255));

        vbe_swapbuffers();
    }

    //Switch back to VBA
    /*registers16_t in = {0};

    in.ax = 0x4F02;
    in.bx = 0x0;
    int86(0x10, &in, (registers16_t*){ 0 });*/

    //kheap_initialize();

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