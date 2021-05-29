#ifndef TRANS_H
#define TRANS_H

#include "hx_drv_tflm.h"

#define TRANS_CMD_DEBUG 0x81
#define TRANS_CMD_IMG 0x82

void trans_init();
void trans_send(uint8_t *data, uint32_t data_size);
void trans_send_byte_reverse(uint8_t* data, uint32_t data_size);
static void trans_send_cmd(uint8_t cmd);
void trans_debug(uint8_t* msg, uint32_t len);
void trans_img(uint8_t* img);


#endif /* TRANS_H */