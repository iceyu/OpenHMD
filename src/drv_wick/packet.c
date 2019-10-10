#include "wick.h"

#ifdef _MSC_VER
#define inline __inline
#endif

frameId_detection_callback frameId_callback = NULL;
timestamp_detection_callback timestamp_callback = NULL;

inline static uint8_t read8(const unsigned char **buffer)
{
    uint8_t ret = **buffer;
    *buffer += 1;
    return ret;
}

inline static int16_t read16(const unsigned char **buffer)
{
    int16_t ret = **buffer | (*(*buffer + 1) << 8);
    *buffer += 2;
    return ret;
}

inline static int32_t read_i32(const unsigned char **buffer)
{
    int32_t ret = **buffer | (*(*buffer + 1) << 8) | (*(*buffer + 2) << 16) | (*(*buffer + 3) << 24);
    *buffer += 4;
    return ret;
}

inline static int32_t read24(const unsigned char **buffer)
{
    int32_t ret = (**buffer << 8) | (*(*buffer + 1) << 16) | (*(*buffer + 2) << 24);
    *buffer += 3;
    return ret >> 8;
}

inline static int32_t read_u32(const unsigned char **buffer)
{
    int32_t ret = **buffer | (*(*buffer + 1) << 8) | (*(*buffer + 2) << 16) | (*(*buffer + 3) << 24);
    *buffer += 4;
    return ret;
}

inline static uint64_t read64(const unsigned char **buffer)
{
    uint64_t ret = (uint64_t) * *buffer |
                   ((uint64_t) * (*buffer + 1) << 8) |
                   ((uint64_t) * (*buffer + 2) << 16) |
                   ((uint64_t) * (*buffer + 3) << 24) |
                   ((uint64_t) * (*buffer + 4) << 32) |
                   ((uint64_t) * (*buffer + 5) << 40) |
                   ((uint64_t) * (*buffer + 6) << 48) |
                   ((uint64_t) * (*buffer + 7) << 56);
    *buffer += 8;
    return ret;
}

void register_frameId_detection(frameId_detection_callback cb)
{
    frameId_callback = cb;
}
void register_timestamp_detection(timestamp_detection_callback cb)
{
    timestamp_callback = cb;
}

bool wick_sensors_decode_packet(wick_sensors_packet *pkt, const unsigned char *buffer, int size)
{
    static int last_frame_id = 0;
    if (size != 82)
    {
        LOGE("invalid wick sensor packet size (expected 82 but got %d)", size);
        return false;
    }
    pkt->report_id = read8(&buffer);
    pkt->stick[0] = read16(&buffer);
    pkt->stick[1] = read16(&buffer);
    pkt->touchpad[0] = read16(&buffer);
    pkt->touchpad[1] = read16(&buffer);
    pkt->buttons = read8(&buffer);
    pkt->battery = read8(&buffer);
    pkt->trigger = read16(&buffer);
    pkt->grip = read16(&buffer);
    pkt->frame_id = read8(&buffer);
    pkt->timestamp = read_i32(&buffer);
    pkt->temperature = read16(&buffer);
    if ((pkt->frame_id - last_frame_id == -255) || (pkt->frame_id - last_frame_id == 1))
    {
        LOGI("frame id = %d\n", pkt->frame_id);
    }
    else
    {
        LOGE("Frame drop... ");
        if (frameId_callback != NULL)
        {
            frameId_callback();
        }
    }
    last_frame_id = pkt->frame_id;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pkt->accel[i][j] = read16(&buffer);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pkt->gyro[i][j] = read16(&buffer);
        }
    }
    return true;
}
