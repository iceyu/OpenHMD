
#ifndef OTUS_H
#define OTUS_H

#include <stdint.h>
#include <stdbool.h>

#include "../openhmdi.h"

typedef enum
{
	OTUS_IRQ_SENSORS = 1,
    OTUS_IRQ_CONTROL = 2,
    OTUS_IRQ_DEBUG = 3,
} otus_sensors_irq_cmd;

typedef struct
{
        uint8_t id;
        uint16_t temperature[4];
        uint64_t gyro_timestamp[4];
        int16_t gyro[3][32];
        uint64_t accel_timestamp[4];
        int32_t accel[3][4];
        uint64_t video_timestamp[4];
} otus_sensors_packet;

static const unsigned char otus_sensors_imu_on[64] = {
	0x02, 0x07
};

bool otus_sensors_decode_packet(otus_sensors_packet* pkt, const unsigned char* buffer, int size);

#endif
