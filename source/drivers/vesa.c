#include <drivers/vesa.h>

#include <bios32.h>
#include <string.h>
#include <fonts.h>

#include <interrupt/isr.h>

VBE20_INFOBLOCK g_vbe_infoblock;
VBE20_MODEINFOBLOCK g_vbe_modeinfoblock;

int g_selected_mode = -1;

uint32_t g_width = 0, g_height = 0;

uint32_t *g_vbe_buffer = NULL;
uint32_t *g_back_buffer = NULL;

int g_vbe_background_color = 0;

uint32_t vbe_get_width(void) 
{
    return g_width;
}

uint32_t vbe_get_height(void) 
{
    return g_height;
}

int get_vbe_info(void)
{
    registers16_t in = {0}, out = {0};
    
    in.ax = 0x4F00;
    
    in.di = BIOS_CONVENTIONAL_MEMORY;
    int86(0x10, &in, &out);
    
    memcpy(&g_vbe_infoblock, (void *)BIOS_CONVENTIONAL_MEMORY, sizeof(VBE20_INFOBLOCK));
    return (out.ax == 0x4F);
    
}

void get_vbe_mode_info(uint16_t mode, VBE20_MODEINFOBLOCK *mode_info)
{
    registers16_t in = {0}, out = {0};
    
    in.ax = 0x4F01;
    in.cx = mode; 
    in.di = BIOS_CONVENTIONAL_MEMORY + 1024;  
    int86(0x10, &in, &out);  
    
    memcpy(mode_info, (void *)BIOS_CONVENTIONAL_MEMORY + 1024, sizeof(VBE20_MODEINFOBLOCK));
}

void vbe_set_mode(uint32_t mode)
{
    registers16_t in = {0}, out = {0};
    
    in.ax = 0x4F02;
    in.bx = mode;
    int86(0x10, &in, &out);  
}

uint32_t vbe_find_mode(uint32_t width, uint32_t height, uint32_t bpp)
{
    uint16_t *mode_list = (uint16_t *)g_vbe_infoblock.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff)
    {
        get_vbe_mode_info(mode, &g_vbe_modeinfoblock);
        if (g_vbe_modeinfoblock.XResolution == width && g_vbe_modeinfoblock.YResolution == height && g_vbe_modeinfoblock.BitsPerPixel == bpp)
            return mode;
        mode = *mode_list++;
    }
    return -1;
}


void vbe_print_available_modes()
{
    VBE20_MODEINFOBLOCK modeinfoblock;

    uint16_t *mode_list = (uint16_t *)g_vbe_infoblock.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff) {
        get_vbe_mode_info(mode, &modeinfoblock);
        
        mode = *mode_list++;
    }
}

uint32_t vbe_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}

void vbe_putpixel(int x, int y, int color)
{
    uint32_t i = y * g_width + x;
    *(g_back_buffer + i) = color;
}

uint32_t vbe_getpixel(int x, int y)
{
    uint32_t i = y * g_width + x;
    uint32_t color = *(g_back_buffer + i);
    return color;
}

void vbe_clear_backbuffer(void)
{
    memset(g_back_buffer, g_vbe_background_color, g_width * g_height * sizeof(uint32_t));
}

void vbe_swapbuffers(void)
{
    for (int i = 0; i < g_width * g_height; i++)
        g_vbe_buffer[i] = g_back_buffer[i];
}

void vbe_set_background(int color)
{
    g_vbe_background_color = color;
}

void vbe_draw_char(char ch, int x, int y, int color)
{
    int temp = 0, pix_data = 0;

    for (int i = 0; i < 16; i++)
    {
        temp = x;
        x += 16;
        pix_data = g_8x16_font[(int)ch * 16 + i];
        while (pix_data > 0)
        {
            if (pix_data & 1)
            {
                vbe_putpixel(temp * 2 - x, y, color);
            }
            pix_data >>= 1;
            x++;
        }
        x = temp;
        y++;
    }
}

void vbe_draw_string(const char *str, int x, int y, int color)
{
    int new_x = x;
    int new_y = y;
    while (*str)
    {
        vbe_draw_char(*str, new_x, new_y, color);
        str++;
        new_x += 9;
        if (new_x > (int)g_width)
        {
            new_y += 17;
            new_x = x;
        }
    }
}

int vesa_initialize(uint32_t width, uint32_t height, uint32_t bpp)
{
    bios32_initialize();
    
    if (!get_vbe_info())
        return -1;

    g_selected_mode = vbe_find_mode(width, height, bpp);
        
    if (g_selected_mode == -1)
        return -1;

    g_width = g_vbe_modeinfoblock.XResolution;
    g_height = g_vbe_modeinfoblock.YResolution;

    g_vbe_buffer = (uint32_t *)g_vbe_modeinfoblock.PhysBasePtr;

    uint32_t back_buffer[g_width * g_height];
    g_back_buffer = back_buffer;
 
    vbe_set_mode(g_selected_mode);

    return 0;
}