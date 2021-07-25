#include "camera.h"

hx_drv_sensor_image_config_t img_config;

void camera_initialize() {
    if(hx_drv_sensor_initial(&img_config) == HX_DRV_LIB_PASS)
	  hx_drv_uart_print("Camera Initialize Successful\n");
}

void camera_capture() {
    hx_drv_sensor_capture(&img_config);
}