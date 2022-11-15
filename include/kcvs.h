#pragma once

#include <stdint.h>

void kcvs_initialize(void);
void kcvs_clear(unsigned char color);
void kcvs_putpixel_at(uint16_t x, uint16_t y, unsigned char color);
void kcvs_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, unsigned char color);
void kcvs_fill_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, unsigned char color);