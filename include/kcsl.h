#pragma once

#include <stdint.h>

void kcsl_initialize(void);

void kcsl_set_color(uint8_t color);
void kcsl_set_column(uint16_t column);
void kcsl_set_row(uint16_t row);

uint8_t kcsl_get_color(void);
uint16_t kcsl_get_column(void);
uint16_t kcsl_get_row(void);

void kcsl_set_cursor_position(uint8_t x, uint8_t y);

void kcsl_putentryat(char c, uint8_t color, uint16_t x, uint16_t y);

void kputchar(char c);
void kprintf(const char *format, ...);