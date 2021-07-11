#include "delay.h"
#include "stm32f10x.h"                  // Device header
void delay_init(void){
	SysTick->CTRL=0x00;//�����־λ�������ⲿʱ��9Mhz
}
void delay_ms(uint32_t ms){
	SysTick->LOAD=9000;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;//�����δ�ʱ��
	while(ms>0){
	while(!(SysTick->CTRL&(1<<16)));//�ȴ���ʱ����ʱ1ms����
	ms--;
	}
	SysTick->CTRL=0x00;//�رն�ʱ��
}
void delay_us(uint32_t us){
	SysTick->LOAD=9;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;//�����δ�ʱ��
	while(us>0){
	while(!(SysTick->CTRL&(1<<16)));//�ȴ���ʱ����ʱ1ms����
	us--;
	}
	SysTick->CTRL=0x00;//�رն�ʱ��

}

