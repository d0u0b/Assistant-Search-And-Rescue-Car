#ifndef GPIO_H
#define GPIO_H

#include "hx_drv_tflm.h"
typedef enum
{
    HAL_OK = 0,
    HAL_ERROR = 1
}
HAL_RETURN;

extern hx_drv_gpio_config_t gpio_0;
extern hx_drv_gpio_config_t gpio_1;
extern hx_drv_gpio_config_t gpio_2;
extern hx_drv_gpio_config_t led_r;
extern hx_drv_gpio_config_t led_g;

HAL_RETURN gpio_init(hx_drv_gpio_config_t * gpio, HX_DRV_GPIO_E gpio_pin, HX_DRV_GPIO_DIRCTION_E gpio_dir, uint8_t gpio_data);
HAL_RETURN gpio_set(hx_drv_gpio_config_t * gpio, uint8_t gpio_pin_data);
uint8_t gpio_get(hx_drv_gpio_config_t * gpio);

#endif /* GPIO_H */

