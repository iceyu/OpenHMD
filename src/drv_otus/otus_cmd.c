
#include "otus_cmd.h"



//int cmd_set_exposure_gain((hid_device* hmd_imu, uint8_t sensor_id, uint16_t exposure_us, uint16_t gain);

int send_cmd(hid_device* hmd_imu, HidCmdTx *cmd_tx, HidCmdRx *cmd_rx);


int send_cmd(hid_device* hmd_imu, HidCmdTx *cmd_tx, HidCmdRx *cmd_rx)
{
    int ret;
    int status = 0;
    int len = sizeof(HidCmdRx);
    ret = hid_write(hmd_imu,(uint8_t *)cmd_tx, sizeof(HidCmdTx));
    ret = hid_read_timeout(hmd_imu,(uint8_t *)cmd_rx, sizeof(HidCmdRx), 5000);

    status = cmd_rx->status;
    if (status != 0)
    {
        return status;
    }
    return status;
}

int get_firmware_version(hid_device* hmd_imu, FirmwareVerion *version)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    memset(&cmd_tx, 0, sizeof(cmd_tx));

    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OTUS_GET_FIRMWARE_VERSION;
    cmd_tx.length = 0;

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }
    FirmwareVerion *ver_prt = (FirmwareVerion *)cmd_rx.data;
    version->major = ver_prt->major;
    version->minor = ver_prt->minor;
    version->patch = ver_prt->patch;

    return ret;


}


int cmd_i2c_read(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t *val, uint8_t val_len, uint8_t i2c_bus)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    HidCmdI2C cmd_i2c;
    HidCmdI2C cmd_i2c_rx;
    memset(&cmd_tx, 0, sizeof(cmd_tx));


    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OTUS_I2C_WRITE;
    cmd_tx.length = sizeof(cmd_i2c);

    cmd_i2c.chip_address = addr_7bit << 1;
    cmd_i2c.i2c_bus_id = i2c_bus;
    cmd_i2c.reg_len = reg_len;
    cmd_i2c.val_len = val_len;
    cmd_i2c.reg = reg;
    cmd_i2c.val = 0;

    memcpy(cmd_tx.data, &cmd_i2c, sizeof(cmd_i2c));

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }

    memcpy(&cmd_i2c_rx, cmd_rx.data, sizeof(cmd_i2c));

    *val = cmd_i2c_rx.reg;
    return ret;
}

int cmd_i2c_write(hid_device* hmd_imu, uint8_t addr_7bit,
    uint16_t reg, uint8_t reg_len,
    uint16_t val, uint8_t val_len, uint8_t i2c_bus)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    HidCmdI2C cmd_i2c;
    HidCmdI2C cmd_i2c_rx;
    memset(&cmd_tx, 0, sizeof(cmd_tx));


    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OTUS_I2C_WRITE;
    cmd_tx.length = sizeof(cmd_i2c);

    cmd_i2c.chip_address = addr_7bit << 1;
    cmd_i2c.i2c_bus_id = i2c_bus;
    cmd_i2c.reg_len = reg_len;
    cmd_i2c.val_len = val_len;
    cmd_i2c.reg = reg;
    cmd_i2c.val = val;

    memcpy(cmd_tx.data, &cmd_i2c, sizeof(cmd_i2c));

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }
    return ret;
}



int cmd_set_exposure_gain(hid_device* hmd_imu, uint8_t sensor_id, uint16_t exposure_us, uint16_t gain)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    HidCmdAec cmd_aec;
    memset(&cmd_tx, 0, sizeof(cmd_tx));


    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OTUS_SET_EXPOSURE_GAIN_CMD;
    cmd_tx.length = sizeof(cmd_aec);

    cmd_aec.sensor_id = sensor_id;
    cmd_aec.exposure_us = exposure_us;
    cmd_aec.gain = gain;

    memcpy(cmd_tx.data, &cmd_aec, sizeof(cmd_aec));

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }
    return ret;
}

int cmd_imu_enable_streaming(hid_device* hmd_imu, uint8_t enable)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    HidCmdIMUEn cmd_imu_en;
    memset(&cmd_tx, 0, sizeof(cmd_tx));


    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OTUS_ENABLE_IMU_STREAMING;
    cmd_tx.length = sizeof(cmd_imu_en);

    cmd_imu_en.enable = enable;

    memcpy(cmd_tx.data, &cmd_imu_en, sizeof(cmd_imu_en));

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }
    return ret;
}

int cmd_get_val(hid_device* hmd_imu, uint8_t type, float *val)
{
    int ret = 0;
    HidCmdTx cmd_tx;
    HidCmdRx cmd_rx;
    HidCmdIMUVal cmd_imu_val_tx;
    HidCmdIMUVal cmd_imu_val_rx;
    memset(&cmd_tx, 0, sizeof(cmd_tx));


    cmd_tx.report_id = OTUS_DATA_REPORT_ID;
    cmd_tx.cmd_type = OUTS_IMU_GET_VAL;
    cmd_tx.length = sizeof(cmd_imu_val_tx);

    cmd_imu_val_tx.type = type;
    cmd_imu_val_tx.val = 0.0;
   

    memcpy(cmd_tx.data, &cmd_imu_val_tx, sizeof(cmd_imu_val_tx));

    ret = send_cmd(hmd_imu, &cmd_tx, &cmd_rx);
    if (ret != 0)
    {
        return ret;
    }

    memcpy(&cmd_imu_val_rx, cmd_rx.data, sizeof(cmd_imu_val_rx));

    *val = cmd_imu_val_rx.val;
    return ret;



}
