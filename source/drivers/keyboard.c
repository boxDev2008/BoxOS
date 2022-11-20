#include <drivers/keyboard.h>

#include <interrupt/idt.h>
#include <interrupt/isr.h>

#include <io_ports.h>

#include <kcsl.h>
#include <kshell.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

bool g_shift_pressed, g_capslocked;

#define KEYBOARD_USE_CAPS ((g_shift_pressed && !g_capslocked) || (!g_shift_pressed && g_capslocked))

char Keyboard_ScancodeToChar(uint8_t scancode)
{
    switch (scancode)
    {
        case SCAN_CODE_KEY_1: return g_shift_pressed ? '!' : '1'; break;
        case SCAN_CODE_KEY_2: return g_shift_pressed ? '@' : '2'; break;
        case SCAN_CODE_KEY_3: return g_shift_pressed ? '#' : '3'; break;
        case SCAN_CODE_KEY_4: return g_shift_pressed ? '$' : '4'; break;
        case SCAN_CODE_KEY_5: return g_shift_pressed ? '%' : '5'; break;
        case SCAN_CODE_KEY_6: return g_shift_pressed ? '^' : '6'; break;
        case SCAN_CODE_KEY_7: return g_shift_pressed ? '&' : '7'; break;
        case SCAN_CODE_KEY_8: return g_shift_pressed ? '*' : '8'; break;
        case SCAN_CODE_KEY_9: return g_shift_pressed ? '(' : '9'; break;
        case SCAN_CODE_KEY_0: return g_shift_pressed ? ')' : '0'; break;
        case SCAN_CODE_KEY_MINUS: return g_shift_pressed ? '_' : '-'; break;
        case SCAN_CODE_KEY_EQUAL: return g_shift_pressed ? '+' : '='; break;
        case SCAN_CODE_KEY_Q: return KEYBOARD_USE_CAPS ? 'Q' : 'q'; break;
        case SCAN_CODE_KEY_W: return KEYBOARD_USE_CAPS ? 'W' : 'w'; break;
        case SCAN_CODE_KEY_E: return KEYBOARD_USE_CAPS ? 'E' : 'e'; break;
        case SCAN_CODE_KEY_R: return KEYBOARD_USE_CAPS ? 'R' : 'r'; break;
        case SCAN_CODE_KEY_T: return KEYBOARD_USE_CAPS ? 'T' : 't'; break;
        case SCAN_CODE_KEY_Y: return KEYBOARD_USE_CAPS ? 'Y' : 'y'; break;
        case SCAN_CODE_KEY_U: return KEYBOARD_USE_CAPS ? 'U' : 'u'; break;
        case SCAN_CODE_KEY_I: return KEYBOARD_USE_CAPS ? 'I' : 'i'; break;
        case SCAN_CODE_KEY_O: return KEYBOARD_USE_CAPS ? 'O' : 'o'; break;
        case SCAN_CODE_KEY_P: return KEYBOARD_USE_CAPS ? 'P' : 'p'; break;
        case SCAN_CODE_KEY_SQUARE_OPEN_BRACKET: return g_shift_pressed ? '{' : '['; break;
        case SCAN_CODE_KEY_SQUARE_CLOSE_BRACKET: return g_shift_pressed ? '}' : ']'; break;
        case SCAN_CODE_KEY_A: return KEYBOARD_USE_CAPS ? 'A' : 'a'; break;
        case SCAN_CODE_KEY_S: return KEYBOARD_USE_CAPS ? 'S' : 's'; break;
        case SCAN_CODE_KEY_D: return KEYBOARD_USE_CAPS ? 'D' : 'd'; break;
        case SCAN_CODE_KEY_F: return KEYBOARD_USE_CAPS ? 'F' : 'f'; break;
        case SCAN_CODE_KEY_G: return KEYBOARD_USE_CAPS ? 'G' : 'g'; break;
        case SCAN_CODE_KEY_H: return KEYBOARD_USE_CAPS ? 'H' : 'h'; break;
        case SCAN_CODE_KEY_J: return KEYBOARD_USE_CAPS ? 'J' : 'j'; break;
        case SCAN_CODE_KEY_K: return KEYBOARD_USE_CAPS ? 'K' : 'k'; break;
        case SCAN_CODE_KEY_L: return KEYBOARD_USE_CAPS ? 'L' : 'l'; break;
        case SCAN_CODE_KEY_SEMICOLON: return g_shift_pressed ? ':' : ';'; break;
        case SCAN_CODE_KEY_SINGLE_QUOTE: return g_shift_pressed ? '\"' : '\''; break;
        case SCAN_CODE_KEY_BACKSLASH: return g_shift_pressed ? '|' : '\\'; break;
        case SCAN_CODE_KEY_Z: return KEYBOARD_USE_CAPS ? 'Z' : 'z'; break;
        case SCAN_CODE_KEY_X: return KEYBOARD_USE_CAPS ? 'X' : 'x'; break;
        case SCAN_CODE_KEY_C: return KEYBOARD_USE_CAPS ? 'C' : 'c'; break;
        case SCAN_CODE_KEY_V: return KEYBOARD_USE_CAPS ? 'V' : 'v'; break;
        case SCAN_CODE_KEY_B: return KEYBOARD_USE_CAPS ? 'B' : 'b'; break;
        case SCAN_CODE_KEY_N: return KEYBOARD_USE_CAPS ? 'N' : 'n'; break;
        case SCAN_CODE_KEY_M: return KEYBOARD_USE_CAPS ? 'M' : 'm'; break;
        case SCAN_CODE_KEY_COMMA: return g_shift_pressed ? '<' : ','; break;
        case SCAN_CODE_KEY_DOT: return g_shift_pressed ? '>' : '.'; break;
        case SCAN_CODE_KEY_FORESLHASH: return g_shift_pressed ? '?' : '/'; break;
        case SCAN_CODE_KEY_SPACE: return ' '; break;
    }

    return NULL;
}

void Keyboard_Handler(Registers *regs)
{
    uint8_t scancode = inportb(KEYBOARD_DATA_PORT);

    switch (scancode)
    {
    case SCAN_CODE_KEY_CAPS_LOCK:
        g_capslocked = !g_capslocked;
        break;

    case SCAN_CODE_KEY_LEFT_SHIFT:
        g_shift_pressed = true;
        break;

    case SCAN_CODE_KEY_RIGHT_SHIFT:
        g_shift_pressed = true;
        break;

    case 0xaa:
        g_shift_pressed = false;
        break;

    case 0xb6:
        g_shift_pressed = false;
        break;

    case SCAN_CODE_KEY_BACKSPACE:
        //Kshell_Backspace();
        break;

    case SCAN_CODE_KEY_ENTER:
        //Kshell_ExecuteCurrentCommand();
        break;
    
    default:
        {
            /*char ch = Keyboard_ScancodeToChar(scancode, g_shift_pressed, g_capslocked);
            if (ch != NULL)
            {
                Kshell_WriteChar(ch);
            }
            break;*/
        }
    }
}

void Keyboard_Initialize(void)
{
    ISR_RegisterInterruptHandler(IRQ_BASE + IRQ1_KEYBOARD, Keyboard_Handler);
}