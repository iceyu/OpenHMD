/*
 * OpenHMD - Free and Open Source API and drivers for immersive technology.
 * Copyright (C) 2013 Fredrik Hultin.
 * Copyright (C) 2013 Jakob Bornecrantz.
 * Distributed under the Boost 1.0 licence, see LICENSE for full text.
 */

/* Simple Test */

#include <openhmd.h>
#include <stdio.h>
#include <stdlib.h>
#include <hidapi.h>
#include "wick.h"

typedef struct
{
	ohmd_device base;

	hid_device *hmd_handle;
	hid_device *hmd_control;

	vec3f raw_accel[5][3];
	vec3f raw_gyro[5][3];
	wick_sensors_packet sensor;

} wick_priv;

#define FEATURE_BUFFER_SIZE 256

static void handle_tracker_sensor_msg(wick_priv *priv, unsigned char *buffer,
									  int size)
{
	if (!wick_sensors_decode_packet(&priv->sensor, buffer, size))
	{
		LOGE("couldn't decode tracker sensor message");
	}

	wick_sensors_packet *s = &priv->sensor;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//accel_from_psvr_vec(s->accel[i][j], &priv->raw_accel[i][j]);
			//gyro_from_psvr_vec(s->gyro[i][j], &priv->raw_gyro[i][j]);
		}
	}
}

static void update_device(ohmd_device *device)
{
	wick_priv *priv = (wick_priv *)device;
	int size = 0;
	unsigned char buffer[FEATURE_BUFFER_SIZE];

	while (true)
	{
		int size = hid_read(priv->hmd_handle, buffer, FEATURE_BUFFER_SIZE);

		if (size < 0)
		{
			LOGE("error reading from device");
			return;
		}
		else if (size == 0)
		{
			return; // No more messages, return.
		}
		handle_tracker_sensor_msg(priv, buffer, size);
	}

	if (size < 0)
	{
		LOGE("error reading from device");
	}
}

void test_wick()
{
	ohmd_context *ctx = ohmd_ctx_create();
	ohmd_driver *driver = ohmd_create_wick_drv(ctx);
	int num_devices = ohmd_ctx_probe(ctx);
	driver->get_device_list(driver, &ctx->list);
	int index = 0;
	for (int i = 0; i < ctx->list.num_devices; i++)
	{
		ohmd_device_desc desc = ctx->list.devices[i];
		LOGI("%d Vendor = %s Product = %s\n", i, desc.vendor, desc.product);
		if (strcmp(desc.product, "NRF52840-DK") == 0)
		{
			if (index == 1)
			{
				printf("Found Wick Game Controller\n");

				ohmd_device *device = driver->open_device(driver, &desc);

				while (true)
				{
					update_device(device);
				}
			}
			index++;
		}
	}

	driver->destroy(ctx);
}

int main(int argc, char **argv)
{
#if DRIVER_WICK
	test_wick();
#else
	// 	int device_idx = 0;

	// 	if (argc > 1)
	// 		device_idx = atoi(argv[1]);

	// 	ohmd_require_version(0, 3, 0);

	// 	int major, minor, patch;
	// 	ohmd_get_version(&major, &minor, &patch);

	// 	printf("OpenHMD version: %d.%d.%d\n", major, minor, patch);

	// 	ohmd_context *ctx = ohmd_ctx_create();

	// 	// Probe for devices
	// 	int num_devices = ohmd_ctx_probe(ctx);
	// 	if (num_devices < 0)
	// 	{
	// 		printf("failed to probe devices: %s\n", ohmd_ctx_get_error(ctx));
	// 		return -1;
	// 	}

	// 	printf("num devices: %d\n\n", num_devices);

	// 	// Print device information
	// 	for (int i = 0; i < num_devices; i++)
	// 	{
	// 		int device_class = 0, device_flags = 0;
	// 		const char *device_class_s[] = {"HMD", "Controller", "Generic Tracker", "Unknown"};

	// 		ohmd_list_geti(ctx, i, OHMD_DEVICE_FLAGS, &device_flags);

	// 		printf("device %d\n", i);
	// 		printf("  vendor:  %s\n", ohmd_list_gets(ctx, i, OHMD_VENDOR));
	// 		printf("  product: %s\n", ohmd_list_gets(ctx, i, OHMD_PRODUCT));
	// 		printf("  path:    %s\n", ohmd_list_gets(ctx, i, OHMD_PATH));
	// 		printf("  class:   %s\n", device_class_s[device_class > OHMD_DEVICE_CLASS_GENERIC_TRACKER ? 4 : device_class]);
	// 		printf("  flags:   %02x\n", device_flags);
	// 		printf("    null device:         %s\n", device_flags & OHMD_DEVICE_FLAGS_NULL_DEVICE ? "yes" : "no");
	// 		printf("    rotational tracking: %s\n", device_flags & OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING ? "yes" : "no");
	// 		printf("    positional tracking: %s\n", device_flags & OHMD_DEVICE_FLAGS_POSITIONAL_TRACKING ? "yes" : "no");
	// 		printf("    left controller:     %s\n", device_flags & OHMD_DEVICE_FLAGS_LEFT_CONTROLLER ? "yes" : "no");
	// 		printf("    right controller:    %s\n\n", device_flags & OHMD_DEVICE_FLAGS_RIGHT_CONTROLLER ? "yes" : "no");
	// 	}

	// 	// Open specified device idx or 0 (default) if nothing specified
	// 	printf("opening device: %d\n", device_idx);
	// 	ohmd_device *hmd = ohmd_list_open_device(ctx, device_idx);

	// 	if (!hmd)
	// 	{
	// 		printf("failed to open device: %s\n", ohmd_ctx_get_error(ctx));
	// 		return -1;
	// 	}

	// 	// Print hardware information for the opened device

	// 	int device_class = 0;
	// 	ohmd_list_geti(ctx, device_idx, OHMD_DEVICE_CLASS, &device_class);
	// 	// Ask for n rotation quaternions and position vectors
	// 	int limit = 0;
	// 	while (1)
	// 	{
	// 		ohmd_ctx_update(ctx);
	// 		//print_infof(hmd, "rotation quat:", 4, OHMD_ROTATION_QUAT);
	// 		//print_infof(hmd, "position vec: ", 3, OHMD_POSITION_VECTOR);
	// #if 1
	// 		if (count > 1000)
	// 		{

	// 			//print_infof(hmd, "Quat:", 4, OHMD_ROTATION_QUAT);
	// 			//print_infof(hmd, "position vec : ", 3, OHMD_POSITION_VECTOR);
	// 			count = 0;
	// 		}
	// 		else
	// 		{
	// 			count++;
	// 		}
	// #endif
	// 		limit++;
	// 	}

	// 	ohmd_ctx_destroy(ctx);

#endif
	return 1;
}
