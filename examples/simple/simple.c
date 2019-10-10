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

#include "wick.h"

void ohmd_sleep(double);

// gets float values from the device and prints them
void print_infof(ohmd_device *hmd, const char *name, int len, ohmd_float_value val)
{
	float *f = (float *)malloc(len * sizeof(float));
	ohmd_device_getf(hmd, val, f);
	printf("%-25s", name);
	for (int i = 0; i < len; i++)
		printf("%f ", f[i]);
	printf("\n");
}

// gets int values from the device and prints them
void print_infoi(ohmd_device *hmd, const char *name, int len, ohmd_int_value val)
{
	int *iv = (int *)malloc(len * sizeof(int));
	ohmd_device_geti(hmd, val, iv);
	printf("%-25s", name);
	for (int i = 0; i < len; i++)
		printf("%d ", iv[i]);
	printf("\n");
}

void test_wick()
{
	ohmd_context *ctx = ohmd_ctx_create();
	ohmd_driver *driver = ohmd_create_wick_drv(ctx);
	int num_devices = ohmd_ctx_probe(ctx);
	driver->get_device_list(driver, &ctx->list);
	// printf("Device Num = %d\n", ctx->list.num_devices);
	// for (int i = 0; i < ctx->list.num_devices; i++)
	// {
	// 	ohmd_device_desc desc = ctx->list.devices[i];
	// 	printf("%d Vendor = %s Product = %s\n", i, desc.vendor, desc.product);
	// 	if (strcmp(desc.product, "Motion controller - Left") == 0)
	// 	{
	// 		printf("Found Motion Controller\n");
	// 		//ohmd_device *device = open_motion_controller_device(driver, &desc);

	// 		while (1)
	// 		{
	// 			//update_device(device);
	// 		}
	// 	}
	// }

	// driver->destroy(ctx);
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

	// 		ohmd_list_geti(ctx, i, OHMD_DEVICE_CLASS, &device_class);
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
	return 0;
}
