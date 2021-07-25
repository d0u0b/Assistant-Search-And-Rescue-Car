#ifndef _CONNECT_H_
#define _CONNECT_H_

#include "gpio.h"
#include "config.h"

void connect_initial();
CMD get_cmd();
void send_data(uint8_t data);
bool send_data(uint8_t* data, uint len, bool start, bool end);
void hang_up();

#endif