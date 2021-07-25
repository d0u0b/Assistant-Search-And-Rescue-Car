#ifndef _GPIO_H_
#define _GPIO_H_

#include "hx_drv_tflm.h"
typedef enum {
	HAL_OK = 0,
	HAL_ERROR = 1
} HAL_RETURN;


HAL_RETURN gpio_init(hx_drv_gpio_config_t *gpio, HX_DRV_GPIO_E gpio_pin, HX_DRV_GPIO_DIRCTION_E gpio_dir, uint8_t gpio_data);
HAL_RETURN gpio_set(hx_drv_gpio_config_t *gpio, uint8_t gpio_pin_data);
uint8_t gpio_get(hx_drv_gpio_config_t *gpio);
HAL_RETURN gpio_mode(hx_drv_gpio_config_t *config, HX_DRV_GPIO_DIRCTION_E gpio_dir);

#endif /* GPIO_H */
