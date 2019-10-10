// Copyright 2013, Fredrik Hultin.
// Copyright 2013, Jakob Bornecrantz.
// Copyright 2016, Joey Ferwerda.
// SPDX-License-Identifier: BSL-1.0
/*
 * OpenHMD - Free and Open Source API and drivers for immersive technology.
 */

/* Sony PSVR Driver */

#define FEATURE_BUFFER_SIZE 256

#define TICK_LEN (1.0f / 1000000.0f) // 1 MHz ticks

#include <string.h>
#include <wchar.h>
#include <hidapi.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

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

void accel_from_wick_vec(const int16_t *smp, vec3f *out_vec)
{
  out_vec->x = (float)smp[1] * (9.81 / 16384);
  out_vec->y = (float)smp[0] * (9.81 / 16384);
  out_vec->z = (float)smp[2] * -(9.81 / 16384);
}

void gyro_from_wick_vec(const int16_t *smp, vec3f *out_vec)
{
  out_vec->x = (float)smp[1] * 0.00105f;
  out_vec->y = (float)smp[0] * 0.00105f;
  out_vec->z = (float)smp[2] * 0.00105f * -1.0f;
}

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
      //accel_from_wick_vec(s->accel[i][j], &priv->raw_accel[i][j]);
      //gyro_from_wick_vec(s->gyro[i][j], &priv->raw_gyro[i][j]);
    }
  }
}

void update_device(ohmd_device *device)
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

static int getf(ohmd_device *device, ohmd_float_value type, float *out)
{
  wick_priv *priv = (wick_priv *)device;

  switch (type)
  {
    // case OHMD_ROTATION_QUAT:
    //     *(quatf *)out = priv->sensor_fusion.orient;
    //     break;

    // case OHMD_POSITION_VECTOR:
    //     out[0] = out[1] = out[2] = 0;
    //     break;

    // case OHMD_DISTORTION_K:
    //     // TODO this should be set to the equivalent of no distortion
    //     memset(out, 0, sizeof(float) * 6);
    //     break;

    // case OHMD_CONTROLS_STATE:
    //     out[0] = (priv->buttons & PSVR_BUTTON_VOLUME_PLUS) != 0;
    //     out[1] = (priv->buttons & PSVR_BUTTON_VOLUME_MINUS) != 0;
    //     out[2] = (priv->buttons & PSVR_BUTTON_MIC_MUTE) != 0;
    //     break;

  default:
    ohmd_set_error(priv->base.ctx, "invalid type given to getf (%ud)", type);
    return -1;
    break;
  }

  return 0;
}

static void close_device(ohmd_device *device)
{
  wick_priv *priv = (wick_priv *)device;

  LOGD("closing WICK device");

  hid_close(priv->hmd_handle);
  hid_close(priv->hmd_control);

  free(device);
}

static hid_device *open_device_idx(int manufacturer, int product)
{
  struct hid_device_info *devs = hid_enumerate(manufacturer, product);
  struct hid_device_info *cur_dev = devs;

  int idx = 0;
  hid_device *ret = NULL;

  while (cur_dev)
  {
    LOGI("%04x:%04x %s\n", manufacturer, product, cur_dev->path);

    if (cur_dev->vendor_id == manufacturer && cur_dev->product_id == product)
    {
      ret = hid_open_path(cur_dev->path);
      LOGI("opening\n");
      if (NULL == ret)
      {
        LOGE("Open failed, try open with 'sudo'");
      }
      else
      {
        LOGI("Open %s successfully\n", cur_dev->path);
      }
    }

    cur_dev = cur_dev->next;
  }

  hid_free_enumeration(devs);

  return ret;
}

static ohmd_device *open_device(ohmd_driver *driver, ohmd_device_desc *desc)
{
  wick_priv *priv = ohmd_alloc(driver->ctx, sizeof(wick_priv));
  unsigned char *config;
  bool samsung = false;

  if (!priv)
    return NULL;

  priv->base.ctx = driver->ctx;

  int idx = atoi(desc->path);

  // Open the HMD device
  priv->hmd_handle = open_device_idx(PNP_ID_VENDOR_ID, PNP_ID_PRODUCT_ID);

  if (!priv->hmd_handle)
    goto cleanup;

  // set up device callbacks
  priv->base.update = update_device;
  priv->base.close = close_device;
  priv->base.getf = getf;

cleanup:
  if (priv)
    free(priv);

  return (ohmd_device *)priv;
}

static void get_device_list(ohmd_driver *driver, ohmd_device_list *list)
{
  struct hid_device_info *devs = hid_enumerate(PNP_ID_VENDOR_ID, PNP_ID_PRODUCT_ID);
  struct hid_device_info *cur_dev = devs;

  int idx = 0;
  while (cur_dev)
  {

    printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
    printf("\n");
    printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
    printf("  Product:      %ls\n", cur_dev->product_string);
    printf("  Release:      %hx\n", cur_dev->release_number);
    printf("  Interface:    %d\n", cur_dev->interface_number);
    printf("\n");

    ohmd_device_desc *desc = &list->devices[list->num_devices++];

    strcpy(desc->driver, "OpenHMD Wick Driver");
    strcpy(desc->vendor, "Segger");
    strcpy(desc->product, "NRF52840-DK");

    desc->revision = 0;

    snprintf(desc->path, OHMD_STR_SIZE, "%d", idx);

    desc->driver_ptr = driver;

    //TODO : device_class ? device_flags ?
    desc->device_class = OHMD_DEVICE_CLASS_HMD;
    desc->device_flags = OHMD_DEVICE_FLAGS_ROTATIONAL_TRACKING;

    cur_dev = cur_dev->next;
    idx++;
  }
  hid_free_enumeration(devs);
}

static void destroy_driver(ohmd_driver *drv)
{
  LOGD("shutting down Sony PSVR driver");
  free(drv);
}

ohmd_driver *ohmd_create_wick_drv(ohmd_context *ctx)
{
  ohmd_driver *drv = ohmd_alloc(ctx, sizeof(ohmd_driver));

  if (!drv)
    return NULL;

  drv->get_device_list = get_device_list;
  drv->open_device = open_device;
  drv->destroy = destroy_driver;
  drv->ctx = ctx;

  return drv;
}
