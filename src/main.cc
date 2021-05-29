#include "hx_drv_tflm.h"
#include "spi.h"
#include "delay.h"
// #include "synopsys_wei_uart.h"
char send[] = "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttes\n";

hx_drv_sensor_image_config_t pimg_config;

int main(int argc, char* argv[])
{
  uint8_t key_data;
  hx_drv_uart_initial(UART_BR_115200);
  spi_initial();

  // while (1) {
  //   spi_send((uint8_t *)&send, sizeof(send));
  //   delay_ms(500);
  // }

  if(hx_drv_sensor_initial(&pimg_config) == HX_DRV_LIB_PASS)
	  hx_drv_uart_print("Camera Initialize Successful\n");

  hx_drv_uart_print("Wait for user press key: [A] \n");

  while (1) 
  {
    hx_drv_uart_getchar(&key_data);
    if(key_data == 'A')
    {
      if(hx_drv_sensor_capture(&pimg_config) == HX_DRV_LIB_PASS){
        hx_drv_uart_print("Camera Get Data Success\n");

        send_img((uint8_t *)pimg_config.raw_address);
      }
      
    }

		key_data = 0;
  }
}