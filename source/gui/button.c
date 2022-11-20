#include <gui/button.h>
#include <gui/colors.h>

#include <drivers/vesa.h>
#include <drivers/mouse.h>

#include <string.h>

bool GUI_Button_RenderWithText(int x, int y, const char *text)
{
    bool result = false;
    int mouseX = Mouse_GetX(), mouseY = Mouse_GetY();
    int textLength = strlen(text);

    if (mouseX > x && mouseX < x + textLength * 8 + 32 &&
        mouseY > y && mouseY < y + 32)
    {
        if (Mouse_GetStatus().left_button)
        {
            VBE_FillRect(x, y, textLength * 8 + 32, 32, GUI_BUTTON_PRESS_COLOR);
            result = true;
        }
        else
            VBE_FillRect(x, y, textLength * 8 + 32, 32, GUI_BUTTON_HOVER_COLOR);
    }
    else
        VBE_FillRect(x, y, textLength * 8 + 32, 32, GUI_BUTTON_FILL_COLOR);
    
    VBE_DrawRect(x, y, textLength * 8 + 32, 32, GUI_BUTTON_BORDER_COLOR);

    VBE_DrawString(text, x + 16, y + 8, GUI_BUTTON_TEXT_COLOR);

    return result;
}