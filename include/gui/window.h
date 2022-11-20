#pragma once

#include <stdbool.h>

typedef enum
{
    GUI_WINDOW_MINIMIZED = 0,
    GUI_WINDOW_ACTIVE = 1
}
GUI_WindowFlags;

typedef struct GUI_Window GUI_Window;

struct GUI_Window
{
    int x, y, w, h;
    bool alive;
    
    GUI_WindowFlags flags;

    void (*on_update_callback)(GUI_Window*);
};

void GUI_Window_Render(GUI_Window *window);