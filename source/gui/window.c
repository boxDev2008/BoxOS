#include <gui/window.h>
#include <gui/colors.h>
#include <gui/colors.h>

#include <drivers/vesa.h>

#include <stddef.h>

void GUI_Window_Render(GUI_Window *window)
{
    if (window->flags & GUI_WINDOW_MINIMIZED)
        return;
    
    VBE_FillRect(window->x, window->y, window->w, window->h, GUI_WINDOW_FILL_COLOR);
    VBE_DrawRect(window->x, window->y, window->w, window->h, GUI_WINDOW_BORDER_COLOR);

    if (window->flags & GUI_WINDOW_ACTIVE)
        VBE_DrawRect(window->x, window->y, window->w, window->h, GUI_WINDOW_ACTIVE_BORDER_COLOR);

    if (window->on_update_callback != NULL)
        window->on_update_callback(window);
}