#ifndef __DELAY_H__
#define __DELAY_H__
#include "stm32f10x.h"                  // Device header
void delay_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
#endif


