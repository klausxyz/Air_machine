#include "ZE08.h"
void ZE08_init(void){
	//ʹ�ܴ���2������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//��������ģʽ
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AF_PP;//TX2�����������
	settings.GPIO_Pin=GPIO_Pin_2;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);

	settings.GPIO_Mode=GPIO_Mode_IN_FLOATING;//RX2��������
	settings.GPIO_Pin=GPIO_Pin_3;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);
	
	//���ô���2
	USART_InitTypeDef usart2_settings;
	usart2_settings.USART_BaudRate=9600;
	usart2_settings.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart2_settings.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart2_settings.USART_Parity=USART_Parity_No;
	usart2_settings.USART_StopBits=USART_StopBits_1;
	usart2_settings.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&usart2_settings);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//����2�����ж�
	USART_Cmd(USART2,ENABLE);//��������2
	
	//���ô���2�ж�
	NVIC_InitTypeDef usart2_nvic_settings;
	usart2_nvic_settings.NVIC_IRQChannel=USART2_IRQn;
	usart2_nvic_settings.NVIC_IRQChannelCmd=ENABLE;
	usart2_nvic_settings.NVIC_IRQChannelPreemptionPriority=3;
	usart2_nvic_settings.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&usart2_nvic_settings);
}
//����2�жϷ������
uint8_t ZE08_data[9],ZE08_data_static[9];
uint16_t CH20_RAW;
void USART2_IRQHandler(void){
static uint8_t index=0;
	ZE08_data[index++]=USART_ReceiveData(USART2);
	if(ZE08_data[0]!=0xFF){
	index=0;
	}
	if(index>8){
	index=0;
				//У�����ݣ����������򷵻�ԭʼŨ��
			//����У��ֵ
			uint8_t check=0;
			for(uint8_t i=1;i<=7;i++){
			check+=ZE08_data[i];
			}
			check=~check+1;//ȡ����1
			//�Ա�У������
			if(check==ZE08_data[8]){
					CH20_RAW=ZE08_data[4]<<8|ZE08_data[5];//����ԭʼŨ��
				for(uint8_t i=0;i<9;i++){
				ZE08_data_static[i]=ZE08_data[i];
				}
			}

	}

}

