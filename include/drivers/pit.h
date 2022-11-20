#pragma once

#include <stdint.h>

void PIT_Initialize(uint32_t freq);
uint32_t PIT_GetTick(void);