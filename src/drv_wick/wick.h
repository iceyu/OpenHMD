#ifndef WICK_H
#define WICK_H

#include <stdint.h>
#include <stdbool.h>

#include "../openhmdi.h"

#define PNP_ID_VENDOR_ID_SOURCE 0x02
#define PNP_ID_VENDOR_ID 0x1915
#define PNP_ID_PRODUCT_ID 0xEEEE
#define PNP_ID_PRODUCT_VERSION 0x0001

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

typedef void (*frameId_detection_callback)();
typedef void (*timestamp_detection_callback)();

void register_frameId_detection(frameId_detection_callback cb);
void register_timestamp_detection(timestamp_detection_callback cb);

bool wick_sensors_decode_packet(wick_sensors_packet *pkt, const unsigned char *buffer, int size);

void update_device(ohmd_device *device);

#endif