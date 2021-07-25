#include "hx_drv_tflm.h"
#include "connect.h"
#include "person_detection.h"
#include "camera.h"
#include "config.h"
#include "gpio.h"
#include "mic.h"

hx_drv_gpio_config_t hal_led_r;
hx_drv_gpio_config_t hal_led_g;

META_AUDIO_t audio_circular_queue[QUEUE_LEN * AUD_BLK_100MS_SZ];

// int8_t img_send_buffer[];

int main(int argc, char *argv[])
{
  // uint8_t key_data;
  hx_drv_uart_initial(UART_BR_115200);
  connect_initial();
  detection_initialize();
  camera_initialize();

  Mic mic(audio_circular_queue);

  if (gpio_init(&hal_led_r, (HX_DRV_GPIO_E)HX_DRV_LED_RED, HX_DRV_GPIO_OUTPUT, 0) == HAL_OK)
    hx_drv_uart_print("RED LED Initialized: OK\n");
  else
    hx_drv_uart_print("RED LED Initialized: Error\n");

  if (gpio_init(&hal_led_g, (HX_DRV_GPIO_E)HX_DRV_LED_GREEN, HX_DRV_GPIO_OUTPUT, 0) == HAL_OK)
    hx_drv_uart_print("GREEN LED Initialized: OK\n");
  else
    hx_drv_uart_print("GREEN LED Initialized: Error\n");

  // if(hx_drv_sensor_initial(&pimg_configa) == HX_DRV_LIB_PASS)
  //   hx_drv_uart_print("Camera Initialize Successful\n");

  gpio_set(&hal_led_r, 1);

  hx_drv_uart_print("start receive");
  while (1)
  {
    switch (get_cmd())
    {
    case PERSON_DETECT:
      hang_up();
      gpio_set(&hal_led_g, 1);
      hx_drv_uart_print("person detect\n");
      send_data((uint8_t)person_detect());
      break;
    case GET_IMAGE:
      hang_up();
      gpio_set(&hal_led_g, 1);
      hx_drv_uart_print("get image\n");
      send_data((uint8_t *)img_config.raw_address, 640 * 480, true, true);
      break;
    case GET_DOA:
      hang_up();
      gpio_set(&hal_led_g, 1);

      // hx_drv_uart_print("send audio\n");
      // hx_drv_uart_print("%d\t%d\n", mic.audio_circular_queue[mic.queue_index * AUD_BLK_100MS_SZ].left, mic.audio_circular_queue[mic.queue_index * AUD_BLK_100MS_SZ].right);
      hx_drv_uart_print("%d\n", (QUEUE_LEN - mic.queue_index) * AUD_BLK_100MS_SZ * sizeof(META_AUDIO_t));
      if (!send_data(
              (uint8_t *)&(audio_circular_queue[mic.queue_index * AUD_BLK_100MS_SZ]),
              (QUEUE_LEN - mic.queue_index) * AUD_BLK_100MS_SZ * sizeof(META_AUDIO_t), true, false))
      {
        hx_drv_uart_print("send fail\n");
      }
      hx_drv_uart_print("%d\n", mic.queue_index * AUD_BLK_100MS_SZ * sizeof(META_AUDIO_t));
      if (!send_data(
              (uint8_t *)(audio_circular_queue),
              mic.queue_index * AUD_BLK_100MS_SZ * sizeof(META_AUDIO_t), false, true))
      {
        hx_drv_uart_print("send fail\n");
      }
      hx_drv_uart_print("send audio end\n");
      break;
    default:
      break;
    }
    gpio_set(&hal_led_g, 0);
    mic.record();
  }

  // detection_initialize();

  // hx_drv_uart_print("Start detection\n");

  // while (1)
  // {
  //   hx_drv_uart_getchar(&key_data);
  //   hx_drv_uart_print("%c", (char)key_data);
  //   if(key_data == 'A')
  //   {
  //     if(hx_drv_sensor_capture(&pimg_configa) == HX_DRV_LIB_PASS){
  //       hx_drv_uart_print("Camera Get Data Success\n");

  //       send_img((uint8_t *)pimg_configa.raw_address);
  //     }
  //     else hx_drv_uart_print("Camera Get Data Fail\n");
  //   }

  // 	key_data = 0;
  //   // person_detect();
  // }
}