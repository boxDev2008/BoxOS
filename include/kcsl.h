#pragma once

#include <stdint.h>

void Kcsl_Initialize(void);

void Kcsl_SetColor(uint8_t color);
void Kcsl_SetColumn(uint16_t column);
void Kcsl_SetRow(uint16_t row);

uint8_t Kcsl_GetColor(void);
uint16_t Kcsl_GetColumn(void);
uint16_t Kcsl_GetRow(void);

void Kcsl_SetCursorPosition(uint8_t x, uint8_t y);

void Kcsl_PutEntryAt(char c, uint8_t color, uint16_t x, uint16_t y);
void Kcsl_Backspace(void);
void Kcsl_Clear(void);
void Kcsl_Scroll(void);

void kputchar(char c);
void kprintf(const char *format, ...);