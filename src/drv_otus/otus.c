



#include <string.h>
#include <wchar.h>
#include <hidapi.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#include "otus.h"
#include "otus_cmd.h"




#define TICK_LEN (1.0f / 16660000.0f) // 1666 Hz ticks
void vec3f_from_OTUS_gyro(int16_t smp[3][8], int i, vec3f* out_vec);
void vec3f_from_OTUS_accel(int16_t smp[3][8], int i, vec3f* out_vec);

typedef struct {
	ohmd_device base;

	hid_device* hmd_ctrl;
    hid_device* hmd_imu;
	fusion sensor_fusion;
	vec3f raw_accel, raw_gyro;
	uint32_t last_ticks;
	uint8_t last_seq;
    IMUReport sensor;

} otus_priv;

void vec3f_from_OTUS_gyro(int16_t smp[3][8], int i, vec3f* out_vec)
{

        for (int j = 0; j < 3; j++)
        {
            float sample = (float)smp[j][i];
            out_vec->arr[j] = sample * 35.0f ;
        }
  
    return;

}

void vec3f_from_OTUS_accel(int16_t smp[3][8], int i, vec3f* out_vec)
{
   
        for (int j = 0; j < 3; j++)
        {
            float sample = (float)smp[j][i];
            out_vec->arr[j] = sample * 0.488/1000*9.8 ;
        }
   
}
static count = 0;
static void handle_tracker_sensor_msg(otus_priv* priv, unsigned char* buffer, int size)
{
	uint64_t last_sample_tick = priv->sensor.gyro_timestamp[3];

	if(!otus_sensors_decode_packet(&priv->sensor, buffer, size)){
		LOGE("couldn't decode tracker sensor message");
	}
    uint8_t arr[3][4] = { 1,2,3,4,5,6,7,8,9 };
    vec3f mag = { {0.0f, 0.0f, 0.0f} };
    IMUReport* imu_report = &priv->sensor;
    count++;
	for(int i = 0; i < 8; i++){
		uint64_t tick_delta = 1000;
		if(last_sample_tick > 0) //startup correction
			tick_delta = imu_report->gyro_timestamp[i] - last_sample_tick;

		float dt = tick_delta * TICK_LEN;

		vec3f_from_OTUS_gyro(imu_report->gyro, i, &priv->raw_gyro);
		vec3f_from_OTUS_accel(imu_report->accel, i, &priv->raw_accel);
#if 0
        if(count>500)
        {
            count > 500;
            printf("%.4f,%.4f,%.4f\n", priv->raw_accel.x, priv->raw_accel.y, priv->raw_accel.z);
            count = 0;
        }
        else
        {
            count++;
        }
#endif
		ofusion_update(&priv->sensor_fusion, dt, &priv->raw_gyro, &priv->raw_accel, &mag);

		last_sample_tick = imu_report->gyro_timestamp[i];
	}
  
}

static void update_device(ohmd_device* device)
{
    otus_priv* priv = (otus_priv*)device;

	int size = 0;
	unsigned char buffer[IMU_REPORT_SIZE];

	while(true){
		int size = hid_read(priv->hmd_imu, buffer, IMU_REPORT_SIZE);
		if(size < 0){
			LOGE("error reading from device");
			return;
		} else if(size == 0) {
			return; // No more messages, return.
		}
        handle_tracker_sensor_msg(priv, buffer, size);
		//// currently the only message type the hardware supports (I think)
		//if(buffer[0] == OTUS_IRQ_SENSORS){
		//	
		//}else if(buffer[0] != OTUS_IRQ_DEBUG){
		//	LOGE("unknown message type: %u", buffer[0]);
		//}
	}

	if(size < 0){
		LOGE("error reading from device");
	}
}

static int getf(ohmd_device* device, ohmd_float_value type, float* out)
{
	otus_priv* priv = (otus_priv*)device;

	switch(type){
	case OHMD_ROTATION_QUAT:
		*(quatf*)out = priv->sensor_fusion.orient;
		break;

	case OHMD_POSITION_VECTOR:
		out[0] = out[1] = out[2] = 0;
		break;

	case OHMD_DISTORTION_K:
		// TODO this should be set to the equivalent of no distortion
		memset(out, 0, sizeof(float) * 6);
		break;

	default:
		ohmd_set_error(priv->base.ctx, "invalid type given to getf (%ud)", type);
		return -1;
		break;
	}

	return 0;
}

static void close_device(ohmd_device* device)
{
    otus_priv* priv = (otus_priv*)device;

	LOGD("closing Otus Sensors device");

    if (priv->hmd_imu != NULL)
    {
        hid_close(priv->hmd_imu);
    }
    if (priv->hmd_ctrl != NULL)
    {
        hid_close(priv->hmd_ctrl);
    }

	free(device);
}

static int  open_device_idx(otus_priv* priv,int manufacturer, int product)
{
	struct hid_device_info* devs = hid_enumerate(manufacturer, product);
	struct hid_device_info* cur_dev = devs;
    int ret = 0;

	while (cur_dev) {
		
        if(cur_dev->usage== HID_OTUS_USAGE)
        {
            LOGI("%04x:%04x %s\n", manufacturer, product, cur_dev->path);
            priv->hmd_ctrl = hid_open_path(cur_dev->path);
			LOGI("opening\n");
		
        }
        if (cur_dev->usage == HID_OTUS_IMU_DATA_USAGE)
        {
            LOGI("%04x:%04x %s\n", manufacturer, product, cur_dev->path);
            priv->hmd_imu = hid_open_path(cur_dev->path);
            LOGI("opening\n");

        }
		cur_dev = cur_dev->next;

	}

	hid_free_enumeration(devs);
    if ((priv->hmd_imu != NULL) && (priv->hmd_ctrl != NULL))
    {
        ret = 0;
    }
    else
    {
        if (priv->hmd_imu != NULL)
        {
            hid_close(priv->hmd_imu);
        }
        if (priv->hmd_ctrl != NULL)
        {
            hid_close(priv->hmd_ctrl);
        }
        ret = -1;
    }
	return ret;
}

static ohmd_device* open_device(ohmd_driver* driver, ohmd_device_desc* desc)
{
    otus_priv* priv = ohmd_alloc(driver->ctx, sizeof(otus_priv));
	unsigned char *config;
	bool samsung = false;
    int ret = 0;
	if(!priv)
		return NULL;

	priv->base.ctx = driver->ctx;

	int idx = atoi(desc->path);

	// Open the HMD device
    ret = open_device_idx(priv,ORBBEC_VID, OTUS_SENSORS_PID);

	if(ret!=0)
		goto cleanup;

    // TODO read config information from device
	//config = read_config(priv);


	if(hid_set_nonblocking(priv->hmd_imu, 1) == -1){
		ohmd_set_error(driver->ctx, "failed to set non-blocking on device");
		goto cleanup;
	}
    if (hid_set_nonblocking(priv->hmd_ctrl, 1) == -1) {
        ohmd_set_error(driver->ctx, "failed to set non-blocking on device");
        goto cleanup;
    }

    // set IMU configuration
    // turn the IMU on
    cmd_imu_enable_streaming(priv->hmd_ctrl, 1);
    
	// set up device callbacks
	priv->base.update = update_device;
	priv->base.close = close_device;
	priv->base.getf = getf;

	ofusion_init(&priv->sensor_fusion);

	return (ohmd_device*)priv;

cleanup:
	if(priv)
		free(priv);

	return NULL;
}

static void get_device_list(ohmd_driver* driver, ohmd_device_list* list)
{
	struct hid_device_info* devs = hid_enumerate(ORBBEC_VID, OTUS_SENSORS_PID);
	struct hid_device_info* cur_dev = devs;

	int idx = 0;
	while (cur_dev) {
		
        if(cur_dev->usage== HID_OTUS_USAGE)
        {
            ohmd_device_desc* desc = &list->devices[list->num_devices++];
		    strcpy(desc->driver, "OpenHMD Otus Driver");
		    strcpy(desc->vendor, "ORBBEC");
		    strcpy(desc->product, "Otus Sensors");

		    desc->revision = 0;

		    snprintf(desc->path, OHMD_STR_SIZE, "%d", idx);

		    desc->driver_ptr = driver;

		    desc->device_class = OHMD_DEVICE_CLASS_HMD;
		    desc->device_flags = OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING;
            idx++;
        }
		
		
        cur_dev = cur_dev->next;
	}

	hid_free_enumeration(devs);
}

static void destroy_driver(ohmd_driver* drv)
{
	LOGD("shutting down Otus driver");
	free(drv);
}

ohmd_driver* ohmd_create_otus_drv(ohmd_context* ctx)
{
	ohmd_driver* drv = ohmd_alloc(ctx, sizeof(ohmd_driver));

	if(!drv)
		return NULL;

	drv->get_device_list = get_device_list;
	drv->open_device = open_device;
	drv->destroy = destroy_driver;
	drv->ctx = ctx;

	return drv;
}
