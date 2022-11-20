#include <drivers/vesa.h>

#include <bios32.h>
#include <string.h>
#include <fonts.h>

#include <interrupt/isr.h>

#include <mm/kheap.h>

VBE20_INFOBLOCK g_vbe_info_block;
VBE20_MODEINFOBLOCK g_vbe_mode_info_block;

int g_selected_mode = -1;

uint32_t g_width = 0, g_height = 0;

uint32_t *g_vbe_buffer = NULL;
uint32_t *g_back_buffer = NULL;

int g_vbe_background_color = 0;

uint32_t VBE_GetWidth(void) 
{
    return g_width;
}

uint32_t VBE_GetHeight(void) 
{
    return g_height;
}

int VBE_GetInfo(void)
{
    Registers16 in = {0}, out = {0};
    
    in.ax = 0x4F00;
    
    in.di = BIOS_CONVENTIONAL_MEMORY;
    int86(0x10, &in, &out);
    
    memcpy(&g_vbe_info_block, (void *)BIOS_CONVENTIONAL_MEMORY, sizeof(VBE20_INFOBLOCK));
    return (out.ax == 0x4F);
    
}

void VBE_GetModeInfo(uint16_t mode, VBE20_MODEINFOBLOCK *mode_info)
{
    Registers16 in = {0}, out = {0};
    
    in.ax = 0x4F01;
    in.cx = mode; 
    in.di = BIOS_CONVENTIONAL_MEMORY + 1024;  
    int86(0x10, &in, &out);  
    
    memcpy(mode_info, (void *)BIOS_CONVENTIONAL_MEMORY + 1024, sizeof(VBE20_MODEINFOBLOCK));
}

void VBE_SetMode(uint32_t mode)
{
    Registers16 in = {0}, out = {0};
    
    in.ax = 0x4F02;
    in.bx = mode;
    int86(0x10, &in, &out);  
}

uint32_t VBE_FindMode(uint32_t width, uint32_t height, uint32_t bpp)
{
    uint16_t *mode_list = (uint16_t *)g_vbe_info_block.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff)
    {
        VBE_GetModeInfo(mode, &g_vbe_mode_info_block);
        if (g_vbe_mode_info_block.XResolution == width && g_vbe_mode_info_block.YResolution == height && g_vbe_mode_info_block.BitsPerPixel == bpp)
            return mode;
        mode = *mode_list++;
    }
    return -1;
}


void VBE_PrintAvailableModes(void)
{
    VBE20_MODEINFOBLOCK modeinfoblock;

    uint16_t *mode_list = (uint16_t *)g_vbe_info_block.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff) {
        VBE_GetModeInfo(mode, &modeinfoblock);
        
        mode = *mode_list++;
    }
}

uint32_t VBE_RGB(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}

uint32_t *VBE_GetBackBuffer(void)
{
    return g_back_buffer;
}

void VBE_PutPixel(int x, int y, int color)
{
    uint32_t i = y * g_width + x;
    *(g_back_buffer + i) = color;
}

uint32_t VBE_GetPixel(int x, int y)
{
    uint32_t i = y * g_width + x;
    uint32_t color = *(g_back_buffer + i);
    return color;
}

void VBE_ClearBackBuffer(void)
{
    memset(g_back_buffer, g_vbe_background_color, g_width * g_height * sizeof(uint32_t));
}

void VBE_SwapBuffers(void)
{
    for (int i = 0; i < g_width * g_height; i++)
        g_vbe_buffer[i] = g_back_buffer[i];
}

void VBE_SetBackground(int color)
{
    g_vbe_background_color = color;
}

void VBE_DrawRect(int x, int y, int w, int h, int color)
{
    int i;

    for (i = 0; i <= w; i++)
    {
        VBE_PutPixel(x + i, y, color);
        VBE_PutPixel(x + i, y + h, color);
    }

    for (i = 0; i < h; i++)
    {
        VBE_PutPixel(x, y + i, color);
        VBE_PutPixel(x + w, y + i, color);
    }
}

void VBE_FillRect(int x, int y, int w, int h, int color)
{
    for (int xx = 0; xx < w; xx++)
        for (int yy = 0; yy < h; yy++)
            VBE_PutPixel(x + xx, y + yy, color);
}

void VBE_DrawChar(char ch, int x, int y, int color)
{
    int temp = 0, pix_data = 0;

    for (int i = 0; i < 16; i++)
    {
        temp = x;
        x = 0;
        pix_data = g_8x16_font[(int)ch * 16 + i];
        while (pix_data > 0)
        {
            if (pix_data & 1)
            {
                VBE_PutPixel(temp + 8 - x, y, color);
            }
            pix_data >>= 1;
            x++;
        }
        x = temp;
        y++;
    }
}

void VBE_DrawString(const char *str, int x, int y, int color)
{
    int new_x = x;
    int new_y = y;
    while (*str)
    {
        VBE_DrawChar(*str, new_x, new_y, color);
        str++;
        new_x += 9;
        if (new_x > (int)g_width)
        {
            new_y += 17;
            new_x = x;
        }
    }
}

void VBE_FreeBuffer(void)
{
    kfree((void**)&g_back_buffer);
}

int VBE_InitializeBuffer(uint32_t width, uint32_t height, uint32_t bpp)
{    
    if (!VBE_GetInfo())
        return -1;

    g_selected_mode = VBE_FindMode(width, height, bpp);
        
    if (g_selected_mode == -1)
        return -1;

    g_width = g_vbe_mode_info_block.XResolution;
    g_height = g_vbe_mode_info_block.YResolution;

    g_vbe_buffer = (uint32_t *)g_vbe_mode_info_block.PhysBasePtr;

    kmalloc(g_width * g_height * sizeof(uint32_t), (void**)&g_back_buffer);
 
    VBE_SetMode(g_selected_mode);

    return 0;
}