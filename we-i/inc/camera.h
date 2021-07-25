#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "hx_drv_tflm.h"

extern hx_drv_sensor_image_config_t img_config;

void camera_initialize(void);
void camera_capture(void);

#endif
