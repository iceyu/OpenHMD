#include "wick_cmd.h"

int get_firmware_version(hid_device *hmd_imu, FirmwareVersion *version)
{
    return 1;
}

int cmd_get_val(hid_device *hmd_imu, uint8_t type, float *val)
{
    return 1;
}

int cmd_i2c_read(hid_device *hmd_imu, uint8_t addr_7bit,
                 uint16_t reg, uint8_t reg_len,
                 uint16_t *val, uint8_t val_len, uint8_t i2c_bus)
{
    return 1;
}

int cmd_i2c_write(hid_device *hmd_imu, uint8_t addr_7bit,
                  uint16_t reg, uint8_t reg_len,
                  uint16_t val, uint8_t val_len, uint8_t i2c_bus)
{
    return 1;
}

int cmd_imu_enable_streaming(hid_device *hmd_imu, uint8_t enable)
{
    return 1;
}