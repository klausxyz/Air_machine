#ifndef __SPI_H__
#define __SPI_H__
#include "stm32f10x.h"                  // Device header
/***************************************************
*SPI总线驱动
*作者：刘欢
*日期：2019/12/29
***************************************************/
/*定义引脚*/
/*GPIOA*/
#define MOSI GPIO_Pin_7
#define SCK GPIO_Pin_5
#define MISO GPIO_Pin_6
//#define NSS GPIO_Pin_4

/**********************API**************************/
void SPI_init(void);
void SPI_send(uint8_t data);
uint8_t SPI_read(void);
#endif
