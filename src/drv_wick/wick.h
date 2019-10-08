#ifndef WICK_H
#define WICK_H

#include <stdint.h>
#include <stdbool.h>

#include "../openhmdi.h"

typedef struct
{
    uint8_t report_id;
    int16_t stick[2];
    int16_t touchpad[2];
    uint8_t buttons;

    uint8_t battery;
    int16_t trigger;
    int16_t grip;

    uint8_t frame_id;
    uint32_t timestamp;
    uint16_t temperature;
    int16_t accel[5][3];
    int16_t gyro[5][3];

} wick_sensors_packet;

bool wick_sensors_decode_packet(wick_sensors_packet *pkt, const unsigned char *buffer, int size);

#endif