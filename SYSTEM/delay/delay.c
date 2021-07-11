#include "delay.h"
#include "stm32f10x.h"                  // Device header
void delay_init(void){
	SysTick->CTRL=0x00;//清楚标志位，设置外部时钟9Mhz
}
void delay_ms(uint32_t ms){
	SysTick->LOAD=9000;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;//开启滴答定时器
	while(ms>0){
	while(!(SysTick->CTRL&(1<<16)));//等待定时器计时1ms结束
	ms--;
	}
	SysTick->CTRL=0x00;//关闭定时器
}
void delay_us(uint32_t us){
	SysTick->LOAD=9;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;//开启滴答定时器
	while(us>0){
	while(!(SysTick->CTRL&(1<<16)));//等待定时器计时1ms结束
	us--;
	}
	SysTick->CTRL=0x00;//关闭定时器

}

