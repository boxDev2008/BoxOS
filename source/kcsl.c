#include <kcsl.h>
#include <vga.h>
#include <string.h>

typedef struct
{
    uint16_t row, column;
    uint8_t color;
    uint16_t *buffer;
}
console_t;

console_t kcsl;

void kcsl_initialize(void)
{
    kcsl.row = 0;
	kcsl.column = 0;
	kcsl.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	kcsl.buffer = (uint16_t*)0xB8000;

    memset(kcsl.buffer, 0, 80 * 25 * 2);
}

void kcsl_set_color(uint8_t color) 
{
	kcsl.color = color;
}

void kcsl_set_column(uint16_t column)
{
    kcsl.column = column;
}

void kcsl_set_row(uint16_t row)
{
    kcsl.row = row;
}

uint8_t kcsl_get_color(void) 
{
    return kcsl.color;
}

uint16_t kcsl_get_column(void)
{
    return kcsl.column;
}

uint16_t kcsl_get_row(void)
{
    return kcsl.row;
}

void kcsl_set_cursor_position(uint8_t x, uint8_t y)
{
	uint16_t pos = y * 80 + x;

	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void kcsl_putentryat(char c, uint8_t color, uint16_t x, uint16_t y)
{
	const uint32_t index = y * 80 + x;
	kcsl.buffer[index] = vga_entry(c, color);
}

void kputchar(char c)
{
    if (kcsl.row > 25)
    {
        memset(kcsl.buffer, 0, 80 * 25 * 2);
        kcsl.row = 0;
    }

    if (c == '\n')
    {
		kcsl.column = 0;
		kcsl.row++;
        
        return;
    }

	kcsl_putentryat(c, kcsl.color, kcsl.column, kcsl.row);

	if (++kcsl.column == 80)
    {
		kcsl.column = 0;
		if (++kcsl.row == 25)
			kcsl.row = 0;
	}
}

void kprintf(const char *format, ...)
{
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            kputchar(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
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