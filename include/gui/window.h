#pragma once

#include <stdbool.h>

typedef struct
{
    int x, y, w, h;
    bool minimized;
}
gui_window_t;

gui_window_t *create_gui_window(int w, int h);
void destroy_gui_window(gui_window_t *window);

void gui_window_update(gui_window_t *window);