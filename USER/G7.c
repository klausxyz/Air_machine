#include "G7.h"
#include "LCD.h"
#include "stdio.h"
#include "delay.h"
void G7_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	//初始化PA9/TX1 PA10/RX1
	GPIO_InitTypeDef settings0,settings2;
	settings0.GPIO_Mode=GPIO_Mode_AF_PP;
	settings0.GPIO_Pin=GPIO_Pin_9;
	settings0.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings0);
	
	settings2.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	settings2.GPIO_Pin=GPIO_Pin_10;
	settings2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings2);
	
	//串口1初始化
	USART_InitTypeDef settings;
	settings.USART_BaudRate=9600;
	settings.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	settings.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	settings.USART_Parity=USART_Parity_No;
	settings.USART_StopBits=USART_StopBits_1;
	settings.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&settings);
	//开启串口
	USART_Cmd(USART1,ENABLE);
	
	//配置串口中断
	NVIC_InitTypeDef settings1;
	settings1.NVIC_IRQChannel=USART1_IRQn;
	settings1.NVIC_IRQChannelCmd=ENABLE;
	settings1.NVIC_IRQChannelPreemptionPriority=2;
	settings1.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&settings1);//初始化串口中断
	
	//开启串口1接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

uint8_t G7_buffer_dyn[32],G7_buffer_static[32];
uint16_t PM2_5,PM10;
char buffer2[10];
void USART1_IRQHandler(){
	static uint8_t index=0;
	G7_buffer_dyn[index++]=USART_ReceiveData(USART1);
	if(G7_buffer_dyn[0]!=0x42 && G7_buffer_dyn[1]!=0x4d){
	index=0;
	}
	if(index>31){
	index=0;
		PM2_5=G7_buffer_dyn[12]<<8|G7_buffer_dyn[13];
		PM10=G7_buffer_dyn[14]<<8|G7_buffer_dyn[15];
		
			for(uint8_t i=0;i<32;i++){
			G7_buffer_static[i]=G7_buffer_dyn[i];
			}
	}
}

