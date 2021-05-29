#include "hx_drv_tflm.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"

hx_drv_gpio_config_t CLK;
hx_drv_gpio_config_t SS;
hx_drv_gpio_config_t MOSI;

void spi_initial(){
	if(gpio_init(&CLK, HX_DRV_PGPIO_0, HX_DRV_GPIO_OUTPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO0 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO0 Initialized: Error\n");

	if(gpio_init(&SS, HX_DRV_PGPIO_1, HX_DRV_GPIO_OUTPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO1 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO1 Initialized: Error\n");

	if(gpio_init(&MOSI, HX_DRV_PGPIO_2, HX_DRV_GPIO_OUTPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO2 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO2 Initialized: Error\n");
}

void spi_send(uint8_t *data, uint32_t data_size){
	gpio_set(&SS, 1);
	while(data_size--) {
		// hx_drv_uart_print("Send: %d\n", data_size);
		for(int8_t b = 7; b >= 0; b--){
			gpio_set(&MOSI, *data >> b & 1);
			delay_cycle(60);
			gpio_set(&CLK, 1);
			delay_us(30);
			gpio_set(&CLK, 0);
		}
		data++;
	}
	gpio_set(&SS, 0);
}

// void spi_send_fast(volatile uint8_t *data, volatile uint32_t data_size){
// 	gpio_set(&SS, 1);
// 	hx_drv_uart_print("Fast send\n");
// 	while(data_size--) {
// 		for(int8_t b = 7; b >= 0; b--){
// 			gpio_set(&MOSI, *data >> b & 1);
// 			delay_us(50);
// 			gpio_set(&CLK, 1);
// 			delay_us(50);
// 			gpio_set(&CLK, 0);
// 		}
// 		data++;
// 	}
// 	gpio_set(&SS, 0);
// }

void send_cmd(volatile SPI_CMD cmd) {
	spi_send((uint8_t *)&cmd, 1);
}

void send_img(uint8_t *img_path) {
	send_cmd(SPI_CMD_IMG);
	spi_send(img_path, IMG_SIZE);
	hx_drv_uart_print("Img send finish\n");
}