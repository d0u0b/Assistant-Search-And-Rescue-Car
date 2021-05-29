#ifndef DELAY_H
#define DELAY_H

#include "hx_drv_tflm.h"
#define CPU_CLK 400000000
#define MS_CYCLE 40000
#define US_CYCLE 40

void delay_cycle(volatile uint32_t delay_time);
void delay_ms(volatile uint32_t input_ms);
void delay_us(volatile uint32_t input_us);

#endif /* DELAY_H */

