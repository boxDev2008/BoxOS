#include <drivers/keyboard.h>

#include <interrupt/idt.h>
#include <interrupt/isr.h>

#include <io_ports.h>

#include <kcsl.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

bool g_shift_pressed, g_capslocked;

char keyboard_scancode_to_char(uint8_t scancode, bool shift_pressed, bool capslocked)
{
    switch (scancode)
    {
        case SCAN_CODE_KEY_1: return '1'; break;
        case SCAN_CODE_KEY_2: return '2'; break;
        case SCAN_CODE_KEY_3: return '3'; break;
        case SCAN_CODE_KEY_4: return '4'; break;
        case SCAN_CODE_KEY_5: return '5'; break;
        case SCAN_CODE_KEY_6: return '6'; break;
        case SCAN_CODE_KEY_7: return '7'; break;
        case SCAN_CODE_KEY_8: return '8'; break;
        case SCAN_CODE_KEY_9: return '9'; break;
        case SCAN_CODE_KEY_0: return '0'; break;
        case SCAN_CODE_KEY_MINUS: return '-'; break;
        case SCAN_CODE_KEY_EQUAL: return '='; break;
        case SCAN_CODE_KEY_Q: return 'q'; break;
        case SCAN_CODE_KEY_W: return 'w'; break;
        case SCAN_CODE_KEY_E: return 'e'; break;
        case SCAN_CODE_KEY_R: return 'r'; break;
        case SCAN_CODE_KEY_T: return 't'; break;
        case SCAN_CODE_KEY_Y: return 'y'; break;
        case SCAN_CODE_KEY_U: return 'u'; break;
        case SCAN_CODE_KEY_I: return 'i'; break;
        case SCAN_CODE_KEY_O: return 'o'; break;
        case SCAN_CODE_KEY_P: return 'p'; break;
        case SCAN_CODE_KEY_SQUARE_OPEN_BRACKET: return '['; break;
        case SCAN_CODE_KEY_SQUARE_CLOSE_BRACKET: return ']'; break;
    }

    return NULL;
}

void keyboard_handler(registers_t *regs)
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

    case 0xaa:
        g_shift_pressed = false;
        break;

    case SCAN_CODE_KEY_ENTER:
        // Shell new line
        break;
    
    default:
        char c = keyboard_scancode_to_char(scancode, g_shift_pressed, g_capslocked);
        if (c != NULL)
        {
            // Write char to shell
            kprintf("%c", c);
        }
        break;
    }
}

void keyboard_initialize(void)
{
    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, keyboard_handler);
}