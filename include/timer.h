#pragma once

#include <stdint.h>

void timer_initialize(uint32_t freq);

uint32_t timer_get_tick(void);