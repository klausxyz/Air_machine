#include "DS1302.h"
#include "delay.h"
DS1302_t DS1302_data;
//改变IO口为输出模式
void IO_output(){
 GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_Out_PP;
	settings.GPIO_Pin=DS1302_IO;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
}
//改变IO口为输入模式
void IO_input(){
 GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	settings.GPIO_Pin=DS1302_IO;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
}


//函数说明：读DS1302寄存器
uint8_t DS1302_read(uint8_t reg){
	GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
	GPIO_SetBits(GPIOB,DS1302_CE);//拉高片选
	IO_output();//切换IO为输出模式
	//写寄存器地址（低位先出）
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
		delay_us(2);
		if(reg&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		reg>>=1;
		GPIO_SetBits(GPIOB,DS1302_CLK);//拉高时钟
		delay_us(2);
	}
	//读取数据(低位先入)
	IO_input();//切换IO为输入模式
	uint8_t data=0;//用来临时保存数据
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
		delay_us(2);
		data>>=1;
		data|=GPIO_ReadInputDataBit(GPIOB,DS1302_IO)<<7;
		GPIO_SetBits(GPIOB,DS1302_CLK);//拉高时钟
		delay_us(2);
	}
	GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
	delay_us(2);
	GPIO_ResetBits(GPIOB,DS1302_CE);//拉低片选
	return data;
}

//函数说明：写DS1302寄存器
void DS1302_write(uint8_t reg,uint8_t data){
	GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
	delay_us(2);
	GPIO_SetBits(GPIOB,DS1302_CE);//拉高片选
	delay_us(2);
	IO_output();//切换IO为输出模式
	//写寄存器地址
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
		delay_us(2);
		if(reg&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		GPIO_SetBits(GPIOB,DS1302_CLK);//拉高时钟
		delay_us(2);
		reg>>=1;
	}
	//写寄存器数据
	for(uint8_t i=0;i<8;i++){
		GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
		delay_us(2);
		if(data&0x01){
		GPIO_SetBits(GPIOB,DS1302_IO);
		}else{
		GPIO_ResetBits(GPIOB,DS1302_IO);
		}
		GPIO_SetBits(GPIOB,DS1302_CLK);//拉高时钟
		delay_us(2);
		data>>=1;
	}
	GPIO_ResetBits(GPIOB,DS1302_CLK);//拉低时钟
	delay_us(2);
	GPIO_ResetBits(GPIOB,DS1302_CE);//拉高片选
}
void DS1302_update(){
	DS1302_data.second=DS1302_read(REG_SECOND|1);//高四位为十位，低四位为个位
	DS1302_data.second=(DS1302_data.second>>4)*10+(DS1302_data.second&0x0F);
	
	DS1302_data.minute=DS1302_read(REG_MINUTE|1);//高四位为十位，低四位为个位
	DS1302_data.minute=(DS1302_data.minute>>4)*10+(DS1302_data.minute&0x0F);
	
	DS1302_data.hour=DS1302_read(REG_HOUR|1);//D4位为个位，低四位为个位
	DS1302_data.hour=((DS1302_data.hour>>4)&0x01)*10+(DS1302_data.hour&0x0F);
	
	DS1302_data.day=DS1302_read(REG_DAY|1);//高四位为十位，低四位为个位
	DS1302_data.day=(DS1302_data.day>>4)*10+(DS1302_data.day&0x0F);
	
	DS1302_data.month=DS1302_read(REG_MONTH|1);
	DS1302_data.month=(DS1302_data.month>>4)*10+(DS1302_data.month&0x0F);
	
	DS1302_data.week=DS1302_read(REG_WEEK|1);//星期不用处理
	
	DS1302_data.year=DS1302_read(REG_YEAR|1);
	DS1302_data.year=(DS1302_data.year>>4)*10+(DS1302_data.year&0x0F);
}

void DS1302_setTime(DS1302_t * p){
	DS1302_write(REG_PROTECT,0x00);//退出保护
	//写秒寄存器
	p->second=(p->second/10)<<4|p->second%10;
	DS1302_write(REG_SECOND,p->second);
	
	//写分寄存器
	p->minute=(p->minute/10)<<4|p->minute%10;
	DS1302_write(REG_MINUTE,p->minute);
	
	//写时寄存器
	p->hour=0x80|(p->hour/10)<<4|p->hour%10;
	DS1302_write(REG_HOUR,p->hour);
	
	//写日寄存器
	p->day=(p->day/10)<<4|p->day%10;
	DS1302_write(REG_DAY,p->day);
	
	//写月寄存器
	p->month=(p->month/10)<<4|p->month%10;
	DS1302_write(REG_MONTH,p->month);	
	
	//写星期寄存器
	p->week=(p->week/10)<<4|p->week%10;
	DS1302_write(REG_WEEK,p->week);	
	//写年寄存器
	p->year=(p->year/10)<<4|p->year%10;
	DS1302_write(REG_YEAR,p->year);
	
	DS1302_write(REG_PROTECT,0x80);//进入保护
}
void DS1302_init(){
//开启GPIOB时钟，配置clk、ce 引脚为推挽输出模式	
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
