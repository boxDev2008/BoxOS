#include <kcvs.h>
#include <string.h>

#define CANVAS_WIDTH 320
#define CANVAS_HEIGHT 200
#define CANVAS_SIZE (CANVAS_WIDTH * CANVAS_HEIGHT)

typedef struct canvas_t
{
    uint16_t width, height, bpp;
    unsigned char *buffer;
}
canvas_t;

canvas_t kcvs;

void kcvs_initialize(void)
{
    kcvs.width = CANVAS_WIDTH;
	kcvs.height = CANVAS_HEIGHT;
	kcvs.bpp = 256;
	kcvs.buffer = (unsigned char*)0xA0000;
}

void kcvs_clear(unsigned char color)
{
    memset(kcvs.buffer, color, CANVAS_SIZE);
}

void kcvs_putpixel_at(uint16_t x, uint16_t y, unsigned char color)
{
	kcvs.buffer[CANVAS_WIDTH * y + x] = color;
}

void kcvs_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, unsigned char color)
{
    for (int yy = 0; yy < height; yy++)
    {
        kcvs.buffer[CANVAS_WIDTH * (y + yy) + x] = color;
        kcvs.buffer[CANVAS_WIDTH * (y + yy) + x + width] = color;
    }

    for (int xx = 0; xx <= width; xx++)
    {
        kcvs.buffer[CANVAS_WIDTH * (y) + x + xx] = color;
        kcvs.buffer[CANVAS_WIDTH * (y + height) + x + xx] = color;
    }
}

void kcvs_fill_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, unsigned char color)
{
	for (int xx = 0; xx < width; xx++)
        for (int yy = 0; yy < height; yy++)
            kcvs.buffer[CANVAS_WIDTH * (y + yy) + (x + xx)] = color;
}