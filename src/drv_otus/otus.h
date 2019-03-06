
#ifndef OTUS_H
#define OTUS_H

#include <stdint.h>
#include <stdbool.h>

#include "../openhmdi.h"


#define ORBBEC_VID        0x2BC5
#define OTUS_SENSORS_PID  0x00E1
#define HID_OTUS_USAGE    0x06
#define HID_OTUS_IMU_DATA_USAGE    0x76

#pragma pack(push, 1)
typedef struct  {
    uint8_t report_id;
    int16_t temperature[4];
    uint64_t gyro_timestamp[8];
    int16_t gyro[3][8];

    uint64_t accel_timestamp[8];
    int16_t accel[3][8];
}IMUReport;

#pragma pack(pop)

#define IMU_REPORT_SIZE sizeof(IMUReport)

bool otus_sensors_decode_packet(IMUReport* pkt, const unsigned char* buffer, int size);

#endif
