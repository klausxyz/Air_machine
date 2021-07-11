#ifndef __DHT11_H__
#define __DHT11_H__
#include "stm32f10x.h"                  // Device header
#define DHT11_PIN_DATA GPIO_Pin_1
void DHT11_init(void);
uint8_t DHT11_update(void);
float DHT11_readTemp(void);
float DHT11_readHum(void);
#endif


