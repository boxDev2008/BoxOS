#include <gui/gui.h>
#include <gui/window.h>
#include <gui/button.h>
#include <gui/colors.h>

#include <gui/images/curosr.h>
#include <gui/images/wallpaper.h>

#include <drivers/mouse.h>
#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <io_ports.h>

#include <string.h>

GUI_Window g_windows[256];
uint8_t g_current_window;

bool g_menu_button_pressed = false;
bool g_menu_opened = false;
float g_menu_open_effect = 0.0f;

float lerp(float firstFloat, float secondFloat, float by)
{
     return firstFloat * (1 - by) + secondFloat * by;
}

void GUI_RenderCursor(void)
{
    for (int y = 0; y < 15; y++)
        for (int x = 0; x < 11; x++)
            if (g_gui_cursor[y * 11 + x] != 16711680)
                VBE_PutPixel(Mouse_GetX() + x, Mouse_GetY() + y, g_gui_cursor[y * 11 + x]);
}

void GUI_RenderWallpaper(void)
{
    for (int y = 0; y < VBE_GetHeight(); y++)
        for (int x = 0; x < VBE_GetWidth(); x++)
            VBE_PutPixel(x, y, g_gui_wallpaper[y * 1920 + x]);
}

void GUI_RenderWindows(void)
{
    for (uint8_t i = 0; i < 256; i++)
    {
        if (!g_windows[i].alive)
            continue;
        GUI_Window_Render(&g_windows[i]);
    }
}

void GUI_RenderTaskbar(void)
{
    VBE_FillRect(0, VBE_GetHeight() - 32, 1920, 32, GUI_WINDOW_FILL_COLOR);

    for (int x = 0; x < VBE_GetWidth(); x++)
        VBE_PutPixel(x, VBE_GetHeight() - 32, GUI_WINDOW_BORDER_COLOR);
    
    if (!Mouse_GetStatus().left_button)
        g_menu_button_pressed = false;
    
    if (GUI_Button_RenderWithText(0, VBE_GetHeight() - 32, "Menu"))
    {
        if (!g_menu_button_pressed)
        {
            g_menu_opened = !g_menu_opened;
            g_menu_open_effect = 0.0f;
        }
        
        g_menu_button_pressed = true;
    }
}

void GUI_RenderMenu(void)
{
    g_menu_open_effect = lerp(g_menu_open_effect, 1.0f, 0.3f);

    VBE_FillRect(0, VBE_GetHeight() - 732, 500 * g_menu_open_effect, 700 * g_menu_open_effect, GUI_WINDOW_FILL_COLOR);
    VBE_DrawRect(0, VBE_GetHeight() - 732, 500 * g_menu_open_effect, 700 * g_menu_open_effect, GUI_WINDOW_BORDER_COLOR);

    if (GUI_Button_RenderWithText(8, VBE_GetHeight() - 716, "Power off"))
    {
        asm volatile("cli");
        asm volatile("hlt");
    }
   GUI_Button_RenderWithText(160, VBE_GetHeight() - 716, "Restart");
}

void GUI_Render(void)
{
    //VBE_ClearBackBuffer();

    GUI_RenderWallpaper();
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_Window_Render(&g_windows[0]);
    GUI_RenderTaskbar();

    if (g_menu_opened)
        GUI_RenderMenu();

    GUI_RenderCursor();
}

void GUI_KeyboardEvent(uint8_t scancode)
{

}