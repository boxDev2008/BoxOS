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
#include <drivers/pit.h>
#include <drivers/ide.h>
#include <drivers/rtc.h>

#include <gui/gui.h>
#include <gui/window.h>

DateTime dt;

void on_update_terminal(GUI_Window *window)
{
    VBE_DrawString("Welcome to BoxOS!", window->x + 8, window->y + 6, VBE_RGB(200, 200, 200));
    VBE_DrawString("root@boxos $ ", window->x + 8, window->y + 6 + 18, VBE_RGB(200, 200, 200));
}

void kmain(void)
{
    GDT_Initialize();
    IDT_Initialize();

    Kheap_Initialize();

    Bios32_Initialize();

    VBE_InitializeBuffer(1280, 1024, 32);

    Keyboard_Initialize();
    Mouse_Initialize();

    while (1)
    {
        //VBE_ClearBackBuffer();

        GUI_Render();

        VBE_SwapBuffers();
    }

    VBE_FreeBuffer();

    //Switch back to VBA
    /*Registers16 in = {0};

    in.ax = 0x4F02;
    in.bx = 0x0;
    int86(0x10, &in, (Registers16*){ 0 });*/

    //VGA_WriteRegs(g_90x30_text);
    //Kcsl_Initialize();
    
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

    //Kshell_Initialize();

    //ATA_Initialize();
}