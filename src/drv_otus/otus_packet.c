

#include "otus.h"
#include "otus_cmd.h"
#ifdef _MSC_VER
#define inline __inline
#endif

inline static uint8_t read8(const unsigned char* buffer, uint32_t *offset)
{

    uint8_t ret = buffer[0];
    *offset += 1;
    return ret;
}

inline static int16_t read16(const unsigned char* buffer, uint32_t *offset)
{
    int16_t ret = buffer[0] | (buffer[1] << 8);
    *offset += 2;
    return ret;
}

inline static uint32_t read32(const unsigned char* buffer, uint32_t *offset)
{
    uint32_t ret = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    *offset += 4;
    return ret;
}

inline static uint64_t read64(const unsigned char* buffer, uint32_t *offset)
{
    uint64_t ret = (uint64_t)buffer[0] |
        ((uint64_t)buffer[1] << 8) |
        ((uint64_t)buffer[2] << 16) |
        ((uint64_t)buffer[3] << 24) |
        ((uint64_t)buffer[4] << 32) |
        ((uint64_t)buffer[5] << 40) |
        ((uint64_t)buffer[6] << 48) |
        ((uint64_t)buffer[7] << 56);
    *offset += 8;
    return ret;
}

bool otus_sensors_decode_packet(IMUReport* pkt, const unsigned char* buffer, int size)
{
    if (size != sizeof(IMUReport)) {
        LOGE("invalid hololens sensor packet size (expected %d but got %d)", sizeof(IMUReport), size);
        return false;
    }
    uint32_t offset = 0;
    uint8_t l_buf[IMU_REPORT_SIZE];
    memcpy(l_buf, buffer, IMU_REPORT_SIZE);

    pkt->report_id = read8(buffer + offset, &offset);
    for (int i = 0; i < 4; i++)
    {
        pkt->temperature[i] = read16(buffer + offset, &offset);
    }

    for (int i = 0; i < 8; i++)
    {
        pkt->gyro_timestamp[i] = read64(buffer + offset, &offset);
    }

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 3; i++) {
            pkt->gyro[i][j] = read16(buffer + offset, &offset);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        pkt->accel_timestamp[i] = read64(buffer + offset, &offset);
    }


    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 3; i++) {
            pkt->accel[i][j] = read16(buffer + offset, &offset);

        }
    }



    return true;
}
