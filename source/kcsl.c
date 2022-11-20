#include <kcsl.h>
#include <string.h>

#include <drivers/vga.h>

#define CONSOLE_WIDTH 90
#define CONSOLE_HEIGHT 30
#define CONSOLE_SIZE (CONSOLE_WIDTH * CONSOLE_HEIGHT)

typedef struct
{
    uint16_t row, column;
    uint8_t color;
    uint16_t *buffer;
}
Console;

Console kcsl;

void Kcsl_Initialize(void)
{
    kcsl.row = 0;
	kcsl.column = 0;
	kcsl.color = VGA_COLOR_LIGHT_GREY;
	kcsl.buffer = (uint16_t*)0xB8000;

    Kcsl_Clear();
}

void Kcsl_SetColor(uint8_t color) 
{
	kcsl.color = color;
}

void Kcsl_SetColumn(uint16_t column)
{
    kcsl.column = column;
}

void Kcsl_SetRow(uint16_t row)
{
    kcsl.row = row;
}

uint8_t Kcsl_GetColor(void) 
{
    return kcsl.color;
}

uint16_t Kcsl_GetColumn(void)
{
    return kcsl.column;
}

uint16_t Kcsl_GetRow(void)
{
    return kcsl.row;
}

void Kcsl_SetCursorPosition(uint8_t x, uint8_t y)
{
	uint16_t pos = y * CONSOLE_WIDTH + x;

	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void Kcsl_PutEntryAt(char c, uint8_t color, uint16_t x, uint16_t y)
{
	const uint32_t index = y * CONSOLE_WIDTH + x;
	kcsl.buffer[index] = VGA_Entry(c, color);
}

void Kcsl_Backspace(void)
{
    kcsl.column--;
    Kcsl_PutEntryAt(' ', VGA_EntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), kcsl.column, kcsl.row);
}

void Kcsl_Clear(void)
{
    for (int i = 0; i < CONSOLE_SIZE; i++)
        kcsl.buffer[i] = VGA_Entry(0x20, VGA_EntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void Kcsl_Scroll(void)
{
    memmove(kcsl.buffer, kcsl.buffer + CONSOLE_WIDTH, CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1) * sizeof(uint16_t));

    size_t index = (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH;
    for(size_t x = 0; x < CONSOLE_WIDTH; ++x)
        kcsl.buffer[index + x] = VGA_Entry(' ', VGA_EntryColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void kputchar(char ch)
{
    if (ch == '\n')
    {
		kcsl.column = 0;
		kcsl.row++;

        if (kcsl.row >= CONSOLE_HEIGHT)
        {
            Kcsl_Scroll();

            kcsl.column = 0;
            kcsl.row--;
        }
        
        return;
    }

    if (kcsl.row >= CONSOLE_HEIGHT)
    {
        Kcsl_Scroll();

		kcsl.column = 0;
        kcsl.row--;
    }

	Kcsl_PutEntryAt(ch, VGA_EntryColor(kcsl.color, VGA_COLOR_BLACK), kcsl.column, kcsl.row);

	if (++kcsl.column == CONSOLE_WIDTH)
    {
		kcsl.column = 0;
		if (++kcsl.row == CONSOLE_HEIGHT)
			kcsl.row = 0;
	}
}

void kprintf(const char *format, ...)
{
    char **arg = (char**)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0)
    {
        if (c != '%')
            kputchar(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0')
            {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9')
            {
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        kputchar(pad0 ? '0' : ' ');
                    while (*p)
                        kputchar(*p++);
                    break;

                default:
                    kputchar(*((int *)arg++));
                    break;
            }
        }
    }
}