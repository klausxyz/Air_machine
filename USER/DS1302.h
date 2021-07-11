#ifndef __DS1302_H__
#define __DS1302_H__
#include "stm32f10x.h"                  // Device header
#define DS1302_CLK GPIO_Pin_12
#define DS1302_IO GPIO_Pin_13
#define DS1302_CE GPIO_Pin_14
//DS1302Ö÷Òª¼Ä´æÆ÷Ð´µØÖ·£¬¶ÁµØÖ·£ºÐ´µØÖ·|1
#define REG_SECOND 0x80
#define REG_MINUTE 0x82
#define REG_HOUR	 0x84
#define REG_DAY 	 0x86
#define REG_MONTH	 0x88
#define REG_WEEK 	 0x8A
#define REG_YEAR	 0x8C
#define REG_PROTECT 0X8E

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t week;
	uint8_t year;	
}DS1302_t;
extern DS1302_t DS1302_data;
void DS1302_init(void);
void DS1302_update(void);
void DS1302_setTime(DS1302_t * p);
#endif


