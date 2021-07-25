#include "gpio.h"


HAL_RETURN gpio_init(hx_drv_gpio_config_t * config, HX_DRV_GPIO_E gpio_pin, HX_DRV_GPIO_DIRCTION_E gpio_dir, uint8_t gpio_data)
{
  config->gpio_data = gpio_data;
  config->gpio_direction = gpio_dir;
  config->gpio_pin = gpio_pin;
  if(hx_drv_gpio_initial(config) != HX_DRV_LIB_PASS)return HAL_ERROR;
  return HAL_OK;
}

HAL_RETURN gpio_mode(hx_drv_gpio_config_t * config, HX_DRV_GPIO_DIRCTION_E gpio_dir) {
  config->gpio_direction = gpio_dir;
  config->gpio_data = 0;
  if(hx_drv_gpio_initial(config) != HX_DRV_LIB_PASS)return HAL_ERROR;
  return HAL_OK;
}

HAL_RETURN gpio_set(hx_drv_gpio_config_t * config, uint8_t gpio_pin_data)
{
  config->gpio_data = gpio_pin_data;
  if(hx_drv_gpio_set(config) != HX_DRV_LIB_PASS)return HAL_ERROR;
  return HAL_OK;
}

uint8_t gpio_get(hx_drv_gpio_config_t * config)
{
  if(hx_drv_gpio_get(config) != HX_DRV_LIB_PASS)return 2;
  return config->gpio_data;
}