#include <gui/window.h>

#include <mm/kheap.h>

#include <drivers/vesa.h>

gui_window_t *create_gui_window(int w, int h)
{
    gui_window_t *window = (gui_window_t*)kmalloc(sizeof(gui_window_t));

    window->x = 0;
    window->y = 0;
    window->w = w;
    window->h = h;

    window->minimized = false;

    return window;
}

void destroy_gui_window(gui_window_t *window)
{
    kfree(window);
}

void gui_window_update(gui_window_t *window)
{
    vbe_fill_rect(window->x, window->y, window->w, window->h, VBE_RGB(41, 44, 53));
    vbe_draw_rect(window->x, window->y, window->w, window->h, VBE_RGB(34, 36, 38));
}