#include "MH_Z19B.h"
#include "LCD.h"
void MH_Z19B_init(void){
	//使能串口3和引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	//配置引脚模式
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AF_PP;//TX3复用推挽输出
	settings.GPIO_Pin=GPIO_Pin_10;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);

	settings.GPIO_Mode=GPIO_Mode_IN_FLOATING;//RX3浮空输入
	settings.GPIO_Pin=GPIO_Pin_11;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
	
	//设置串口3
	USART_InitTypeDef usart3_settings;
	usart3_settings.USART_BaudRate=9600;
	usart3_settings.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart3_settings.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart3_settings.USART_Parity=USART_Parity_No;
	usart3_settings.USART_StopBits=USART_StopBits_1;
	usart3_settings.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&usart3_settings);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//串口3接收中断
	USART_Cmd(USART3,ENABLE);//开启串口3
	
	//设置串口3中断
	NVIC_InitTypeDef usart3_nvic_settings;
	usart3_nvic_settings.NVIC_IRQChannel=USART3_IRQn;
	usart3_nvic_settings.NVIC_IRQChannelCmd=ENABLE;
	usart3_nvic_settings.NVIC_IRQChannelPreemptionPriority=3;
	usart3_nvic_settings.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&usart3_nvic_settings);
}

//查询程序
const uint8_t check_code[9]={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
void MH_Z19B_update(){
	for(uint8_t i=0;i<9;i++){
		while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));//等待发送缓存准备好
		USART_SendData(USART3,check_code[i]);
	}
}



uint8_t MH_Z19B_data[9],MH_Z19B_data_static[9];//传感器接收缓存数组
//串口3中断服务程序
uint16_t C02_RAW;//二氧化碳浓度原始数据
void USART3_IRQHandler(void){
uint8_t static index=0;
	MH_Z19B_data[index++]=USART_ReceiveData(USART3);//读取数据
	if(MH_Z19B_data[0]!=0xFF && MH_Z19B_data[1]!=0x86){
	index=0;
	}
	if(index>8){
		index=0;
		//校验数据，数据正常则返回原始浓度
			//计算校验值
			uint8_t check=0;
			for(uint8_t i=1;i<=7;i++){
			check+=MH_Z19B_data[i];
			}
			check=~check+1;//取反加1
			//对比校验数据
			if(check==MH_Z19B_data[8]){
					C02_RAW=MH_Z19B_data[2]<<8|MH_Z19B_data[3];//返回原始浓度
				for(uint8_t i=0;i<9;i++){
				MH_Z19B_data_static[i]=MH_Z19B_data[i];
				}
			}
			
	}
}
