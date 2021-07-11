#include "DHT11.h"
#include "delay.h"
#include "stdio.h"
void DHT11_output(){
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_Out_PP;
	settings.GPIO_Pin=DHT11_PIN_DATA;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);
}
void DHT11_input(){
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	settings.GPIO_Pin=DHT11_PIN_DATA;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);
}

void DHT11_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	DHT11_output();
	GPIO_SetBits(GPIOA,DHT11_PIN_DATA);//DHT11��������
	delay_init();
}

void DHT11_begin(){
	GPIO_ResetBits(GPIOA,DHT11_PIN_DATA);//DHT11��������20ms
	delay_ms(25);
	GPIO_SetBits(GPIOA,DHT11_PIN_DATA);//DHT11��������30us
	delay_us(30);
}
void DHT11_waitAck(){
	uint8_t time_out=200;
	while(GPIO_ReadInputDataBit(GPIOA,DHT11_PIN_DATA) && time_out-->0)delay_us(1);//�ȴ�DHT11��������
	if(time_out==0)return;
	time_out=200;
	while(!GPIO_ReadInputDataBit(GPIOA,DHT11_PIN_DATA) && time_out-->0)delay_us(1);//�ȴ�DHT11��������
	if(time_out==0)return;
}
uint8_t DHT11_buffer[5];//��������ԭʼ����
void DHT11_receive(){
	uint8_t time_out;
	for(uint8_t index=0;index<5;index++){
			for(uint8_t index_bit=0;index_bit<8;index_bit++){
				DHT11_buffer[index]<<=1;
				time_out=200;
				while(GPIO_ReadInputDataBit(GPIOA,DHT11_PIN_DATA) && time_out-->0)delay_us(1);//�ȴ�DHT11��������
					if(time_out==0)return;
				time_out=200;
				while(!GPIO_ReadInputDataBit(GPIOA,DHT11_PIN_DATA) && time_out-->0)delay_us(1);//�ȴ�DHT11��������
					if(time_out==0)return;
					delay_us(50);
					if(GPIO_ReadInputDataBit(GPIOA,DHT11_PIN_DATA)){
						DHT11_buffer[index]|=1;
					}else{
						DHT11_buffer[index]&=~0x01;
					}
			}
	}
}
uint8_t DHT11_update(){
	DHT11_output();//�л�Ϊ���ģʽ
	DHT11_begin();//��ʼ�ź�
	DHT11_input();//�л�Ϊ����ģʽ
	DHT11_waitAck();//�ȴ�DHT11��Ӧ
	DHT11_receive();//����DHT11����
	DHT11_output();//�л�Ϊ���ģʽ
	GPIO_SetBits(GPIOA,DHT11_PIN_DATA);//��������
	//У������: 8bitУ���=8bitʪ����������+8bitʪ��С������+8bi�¶���������+8bit�¶�С������
	if(((DHT11_buffer[0]+DHT11_buffer[1]+DHT11_buffer[2]+DHT11_buffer[3])&0xff)==DHT11_buffer[4]){
	return 1;
	}else{
	return 0;
	}
}
float DHT11_readTemp(){
	float decimal=0;
	decimal=DHT11_buffer[3];//�¶�8bitС��
	while(decimal>=1){
	decimal/=10.0f;
	}
	return (float)DHT11_buffer[2]+decimal;
}

float DHT11_readHum(){
	float decimal=0;
	decimal=DHT11_buffer[1];//�¶�8bitС��
	while(decimal>=1){
	decimal/=10.0f;
	}
	return (float)DHT11_buffer[0]+decimal;
}

