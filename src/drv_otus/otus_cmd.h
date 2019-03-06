#pragma once
#include "otus.h"
#include <stdint.h>
#include <hidapi.h>
#pragma pack(push, 1)
typedef struct   {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}FirmwareVerion;
#pragma pack(pop)


#define OTUS_DATA_REPORT_ID (2)
#define MAX_HID_INPUT_FIELD_SIZE_BYTES (63)  //send to host
#define MAX_HID_OUTPUT_FIELD_SIZE_BYTES (63) 

#define OTUS_CMD_VERSION 0x00
typedef enum _otus_cmd_id {
    //- common cmd 0x00-0x2F
    OTUS_GET_FIRMWARE_VERSION = 0x00,
    OTUS_GET_CMD_VERSION = 0x01,
    OTUS_RESET_DEVICE = 0x02,
    //- device cmd  0x30 - 0x5F
    OTUS_I2C_READ = 0x20,
    OTUS_I2C_WRITE = 0x21,
    OTUS_SPI_READ = 0x22,
    OTUS_SPI_WRITE = 0x23,
    OTUS_AHB_READ = 0x24,
    OTUS_AHB_WRITE = 0x25,
    OTUS_GPIO_SET = 0x26,
    OTUS_GPIO_GET = 0x27,

    OTUS_FW_ERASE = 0x30,
    OTUS_FW_READ = 0x31,
    OTUS_FW_WRITE = 0x32,

    //- fucntion cmd 0x50 - 0x9F
    //- camera part 0x50 - 0x7F
    OTUS_SET_EXPOSURE_GAIN_CMD = 0x50,
    //- imu part 0x80 - 0x9F
    OTUS_ENABLE_IMU_STREAMING = 0x80,
    OUTS_IMU_GET_SENSITIVITY = 0x81,
    OUTS_IMU_SET_SENSITIVITY = 0x82,
    OUTS_IMU_GET_FULLSCALE = 0x83,
    OUTS_IMU_SET_FULLSCALE = 0x84,
    OUTS_IMU_GET_ODR = 0x85,
    OUTS_IMU_SET_ODR = 0x86,

    //- eeprom part 0xA0 - 0xAF
    OTUS_EEPROM_ERASE = 0xA0,
    OTUS_EEPROM_READ = 0xA1,
    OTUS_EEPROM_WRITE = 0xA2,
    //- temperature part
    OTUS_TEMP_GET = 0xB0,

} otus_cmd_id;

#pragma pack(push, 1)


// total 64 bytes ,send to device 
typedef struct  {
    uint8_t report_id;
    uint8_t cmd_type;
    uint8_t length;
    uint8_t data[MAX_HID_OUTPUT_FIELD_SIZE_BYTES - 3];
}HidCmdTx;

// total 64 bytes send to host 
typedef struct  {
    uint8_t report_id;
    uint8_t cmd_type;
    uint8_t status;
    uint8_t length;
    uint8_t data[MAX_HID_INPUT_FIELD_SIZE_BYTES - 4];
}HidCmdRx;


typedef struct  {
    uint8_t sensor_id;
    uint16_t exposure_us;
    uint16_t gain;
}HidCmdAec;

typedef struct  {
    uint8_t i2c_bus_id;
    uint8_t chip_address;
    uint8_t reg_len;
    uint8_t val_len;
    uint16_t reg;
    uint16_t val;
}HidCmdI2C;

typedef struct  {
    uint8_t enable; //1:enable,0 disbale
} HidCmdIMUEn;

#pragma pack(pop)



int get_firmware_version(hid_device* hmd_imu, FirmwareVerion *version);
int cmd_set_exposure_gain(hid_device* hmd_imu, uint8_t sensor_id, uint16_t exposure_us, uint16_t gain);

int cmd_i2c_read(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t *val, uint8_t val_len, uint8_t i2c_bus);

int cmd_i2c_write(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t val, uint8_t val_len, uint8_t i2c_bus);
int cmd_set_exposure_gain(hid_device* hmd_imu, uint8_t sensor_id, uint16_t exposure_us, uint16_t gain);

int cmd_imu_enable_streaming(hid_device* hmd_imu, uint8_t enable);
