#include "delay.h"


void delay_cycle(volatile uint32_t delay_time)
{
	volatile uint32_t j = 0;
	for(; j < delay_time; j++);
}

void delay_us(volatile uint32_t input_us)
{
    delay_cycle(US_CYCLE * input_us);
}

void delay_ms(volatile uint32_t input_ms)
{
	delay_cycle(MS_CYCLE * input_ms);
}