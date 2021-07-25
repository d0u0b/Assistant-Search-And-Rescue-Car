#include "connect.h"

hx_drv_gpio_config_t CLK;
hx_drv_gpio_config_t SS;
hx_drv_gpio_config_t DATA;

const uint8_t send_head = 0x81;


void connect_initial() {
    if(gpio_init(&CLK, HX_DRV_PGPIO_0, HX_DRV_GPIO_INPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO0 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO0 Initialized: Error\n");

	if(gpio_init(&SS, HX_DRV_PGPIO_1, HX_DRV_GPIO_INPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO1 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO1 Initialized: Error\n");

	if(gpio_init(&DATA, HX_DRV_PGPIO_2, HX_DRV_GPIO_INPUT, 0) == HAL_OK)
		hx_drv_uart_print("GPIO2 Initialized: OK\n");
	else
		hx_drv_uart_print("GPIO2 Initialized: Error\n");
}

CMD get_cmd(){
    uint8_t buffer = 0;
    // while(gpio_get(&SS));
    // while(!gpio_get(&SS));
    if(!gpio_get(&SS)) return None;
    for(uint8_t i = 0; i < 8; i++) {
        buffer <<= 1;
        while(!gpio_get(&CLK));
        buffer += gpio_get(&DATA);
        while(gpio_get(&CLK));
    }

    return CMD(buffer);
}

void hang_up() {
    gpio_set(&DATA, 1);
    gpio_mode(&DATA, HX_DRV_GPIO_OUTPUT);
    hx_drv_mic_off();
}

void send_data(volatile uint8_t data) {
    hx_drv_uart_print("send\n");
    gpio_set(&DATA, 0);
    int8_t i;
    for(i = 7; i >= 0; i--) {
        while(gpio_get(&CLK))
            if(!gpio_get(&SS)) return;
        gpio_set(&DATA, data >> i & 1);
        while(!gpio_get(&CLK))
            if(!gpio_get(&SS)) return;
    }
    while(gpio_get(&SS));
    gpio_mode(&DATA, HX_DRV_GPIO_INPUT);
    hx_drv_mic_on();
}

bool send_data(uint8_t* data, uint len, bool start, bool end) {
    if(start) gpio_set(&DATA, 0);
    int8_t i;
    while(len--) {
        for(i = 7; i >= 0; i--) {
            while(gpio_get(&CLK))
                if(!gpio_get(&SS)){
                    hx_drv_uart_print("%d\n", len);
                     return false;
                }
            gpio_set(&DATA, *data >> i & 1);
            while(!gpio_get(&CLK))
                if(!gpio_get(&SS)){
                    hx_drv_uart_print("%d\n", len);
                     return false;
                }
        }
        data++;
    }
    if(!end) return true;
    while(gpio_get(&SS));
    gpio_mode(&DATA, HX_DRV_GPIO_INPUT);
    hx_drv_mic_on();
    return true;
}