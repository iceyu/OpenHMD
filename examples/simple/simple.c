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

void detecte_frameId()
{
	LOGE("Lost frame\n");
}

void detecte_timestamp()
{
	LOGE("Time interval is not stabile\n");
}

void test_wick()
{
	register_frameId_detection(detecte_frameId);
	register_timestamp_detection(detecte_timestamp);

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
			if (index == 0)
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

#endif
	return 1;
}
