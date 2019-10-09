
#include <openhmd.h>
#include <stdio.h>
#include "wick.h"

int main()
{
    ohmd_context *ctx = ohmd_ctx_create();
    ohmd_driver *driver = ohmd_create_wmr_drv(ctx);
    int num_devices = ohmd_ctx_probe(ctx);

    driver->get_device_list(driver, &ctx->list);
    for (int i = 0; i < ctx->list.num_devices; i++)
    {
        ohmd_device_desc desc = ctx->list.devices[i];
        printf("%d Vendor = %s Product = %s\n", i, desc.vendor, desc.product);
        if (strcmp(desc.product, "Motion controller - Left") == 0)
        {
            printf("Found Motion Controller\n");
            ohmd_device *device = open_motion_controller_device(driver, &desc);

            while (1)
            {
                update_device(device);
            }
        }
    }

    driver->destroy(ctx);
    return 1;
}