#pragma once
#include "wick.h"
#include <stdint.h>
#include <hidapi.h>

#pragma pack(push, 1)
typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}FirmwareVersion;
#pragma pack(pop)

#define WICK_DATA_REPORT_ID (1)

typedef enum _wick_cmd_id {
    //- common cmd
    WICK_GET_FIRMWARE_VERSION,
    WICK_GET_CMD_VERSION,
    WICK_RESET_DEVICE,

    //- device cmd
    WICK_I2C_READ,
    WICK_I2C_WRITE,
    WICK_SPI_READ,
    WICK_SPI_WRITE,
    WICK_AHB_READ,
    WICK_AHB_WRITE,
    WICK_GPIO_SET,
    WICK_GPIO_GET,

    WICK_FW_ERASE,
    WICK_FW_READ,
    WICK_FW_WRITE,

    //- function cmd

    //- imu part
    WICK_ENABLE_IMU_STREAMING,
    WICK_IS_IMU_STREAMING,
    WICK_IMU_GET_VAL,
    WICK_IMU_SET_VAL,

    //- eeprom part
    WICK_EEPROM_ERASE,
    WICK_EEPROM_READ,
    WICK_EEPROM_WRITE,

        //- temperature part
        WICK_TEMP_GET
} wick_cmd_id;

int get_firmware_version(hid_device *hmd_imu, FirmwareVersion* version);

int cmd_get_val(hid_device* hmd_imu, uint8_t type, float *val);

int cmd_i2c_read(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t *val, uint8_t val_len, uint8_t i2c_bus);

int cmd_i2c_write(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t val, uint8_t val_len, uint8_t i2c_bus);

int cmd_imu_enable_streaming(hid_device* hmd_imu, uint8_t enable);