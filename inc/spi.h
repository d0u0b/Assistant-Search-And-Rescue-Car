#ifndef SPI_H
#define SPI_H

#include "hx_drv_tflm.h"
#define CPU_CLK 400000000

#define IMG_SIZE 307200

typedef enum __attribute__((packed)) {
    SPI_CMD_IMG = 0x82
} SPI_CMD;

void spi_initial();
void spi_send(uint8_t *data, uint32_t data_size);
void spi_send_fast(volatile uint8_t *data, volatile uint32_t data_size);
void send_cmd(volatile SPI_CMD cmd);
void send_img(uint8_t *img_path);


#endif /* SPI_H */