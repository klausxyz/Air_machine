#include "DS1302.h"
#include "delay.h"
DS1302_t DS1302_data;
//�ı�IO��Ϊ���ģʽ
void IO_output(){
 GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_Out_PP;
	settings.GPIO_Pin=DS1302_IO;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
}
//�ı�IO��Ϊ����ģʽ
void IO_input(){
 GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	settings.GPIO_Pin=DS1302_IO;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
}


//����˵������DS1302�Ĵ���
uint8_t DS1302_read(uint8_t reg){
	GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
	GPIO_SetBits(GPIOB,DS1302_CE);//����Ƭѡ
	IO_output();//�л�IOΪ���ģʽ
	//д�Ĵ�����ַ����λ�ȳ���
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		if(reg&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		reg>>=1;
		GPIO_SetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
	}
	//��ȡ����(��λ����)
	IO_input();//�л�IOΪ����ģʽ
	uint8_t data=0;//������ʱ��������
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		data>>=1;
		data|=GPIO_ReadInputDataBit(GPIOB,DS1302_IO)<<7;
		GPIO_SetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
	}
	GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
	delay_us(2);
	GPIO_ResetBits(GPIOB,DS1302_CE);//����Ƭѡ
	return data;
}

//����˵����дDS1302�Ĵ���
void DS1302_write(uint8_t reg,uint8_t data){
	GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
	delay_us(2);
	GPIO_SetBits(GPIOB,DS1302_CE);//����Ƭѡ
	delay_us(2);
	IO_output();//�л�IOΪ���ģʽ
	//д�Ĵ�����ַ
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		if(reg&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		GPIO_SetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		reg>>=1;
	}
	//д�Ĵ�������
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		if(data&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		GPIO_SetBits(GPIOB,DS1302_CLK);//����ʱ��
		delay_us(2);
		data>>=1;
	}
	GPIO_ResetBits(GPIOB,DS1302_CLK);//����ʱ��
	delay_us(2);
	GPIO_ResetBits(GPIOB,DS1302_CE);//����Ƭѡ
}
void DS1302_update(){
	DS1302_data.second=DS1302_read(REG_SECOND|1);//����λΪʮλ������λΪ��λ
	DS1302_data.second=(DS1302_data.second>>4)*10+(DS1302_data.second&0x0F);
	
	DS1302_data.minute=DS1302_read(REG_MINUTE|1);//����λΪʮλ������λΪ��λ
	DS1302_data.minute=(DS1302_data.minute>>4)*10+(DS1302_data.minute&0x0F);
	
	DS1302_data.hour=DS1302_read(REG_HOUR|1);//D4λΪ��λ������λΪ��λ
	DS1302_data.hour=((DS1302_data.hour>>4)&0x01)*10+(DS1302_data.hour&0x0F);
	
	DS1302_data.day=DS1302_read(REG_DAY|1);//����λΪʮλ������λΪ��λ
	DS1302_data.day=(DS1302_data.day>>4)*10+(DS1302_data.day&0x0F);
	
	DS1302_data.month=DS1302_read(REG_MONTH|1);
	DS1302_data.month=(DS1302_data.month>>4)*10+(DS1302_data.month&0x0F);
	
	DS1302_data.week=DS1302_read(REG_WEEK|1);//���ڲ��ô���
	
	DS1302_data.year=DS1302_read(REG_YEAR|1);
	DS1302_data.year=(DS1302_data.year>>4)*10+(DS1302_data.year&0x0F);
}

void DS1302_setTime(DS1302_t * p){
	DS1302_write(REG_PROTECT,0x00);//�˳�����
	//д��Ĵ���
	p->second=(p->second/10)<<4|p->second%10;
	DS1302_write(REG_SECOND,p->second);
	
	//д�ּĴ���
	p->minute=(p->minute/10)<<4|p->minute%10;
	DS1302_write(REG_MINUTE,p->minute);
	
	//дʱ�Ĵ���
	p->hour=0x80|(p->hour/10)<<4|p->hour%10;
	DS1302_write(REG_HOUR,p->hour);
	
	//д�ռĴ���
	p->day=(p->day/10)<<4|p->day%10;
	DS1302_write(REG_DAY,p->day);
	
	//д�¼Ĵ���
	p->month=(p->month/10)<<4|p->month%10;
	DS1302_write(REG_MONTH,p->month);	
	
	//д���ڼĴ���
	p->week=(p->week/10)<<4|p->week%10;
	DS1302_write(REG_WEEK,p->week);	
	//д��Ĵ���
	p->year=(p->year/10)<<4|p->year%10;
	DS1302_write(REG_YEAR,p->year);
	
	DS1302_write(REG_PROTECT,0x80);//���뱣��
}
void DS1302_init(){
//����GPIOBʱ�ӣ�����clk��ce ����Ϊ�������ģʽ	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef settings_clk_ce;
	settings_clk_ce.GPIO_Mode=GPIO_Mode_Out_PP;
	settings_clk_ce.GPIO_Pin=DS1302_CLK|DS1302_CE;
	settings_clk_ce.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings_clk_ce);
	GPIO_ResetBits(GPIOB,DS1302_CLK);
	GPIO_ResetBits(GPIOB,DS1302_CE);
	delay_init();
}
