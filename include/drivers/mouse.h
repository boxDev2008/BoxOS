#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MOUSE_CMD_RESET     0xFF
#define MOUSE_CMD_RESEND     0xFE
#define MOUSE_CMD_SET_DEFAULTS     0xF6
#define MOUSE_CMD_DISABLE_PACKET_STREAMING     0xF5
#define MOUSE_CMD_ENABLE_PACKET_STREAMING     0xF4
#define MOUSE_CMD_SAMPLE_RATE     0xF3
#define MOUSE_CMD_MOUSE_ID     0xF2
#define MOUSE_CMD_REQUEST_SINGLE_PACKET     0xEB
#define MOUSE_CMD_STATUS     0xE9
#define MOUSE_CMD_RESOLUTION     0xE8

#define MOUSE_ACKNOWLEDGE      0xFA

#define PS2_CMD_PORT      0x64
#define MOUSE_DATA_PORT     0x60

typedef struct
{
    uint8_t left_button: 1;
    uint8_t right_button: 1;
    uint8_t middle_button: 1;
    uint8_t always_1: 1;
    uint8_t x_sign: 1;
    uint8_t y_sign: 1;
    uint8_t x_overflow: 1;
    uint8_t y_overflow: 1;
}
MouseStatus;

void Mouse_Initialize(void);

int Mouse_GetX(void);
int Mouse_GetY(void);

MouseStatus Mouse_GetStatus(void);