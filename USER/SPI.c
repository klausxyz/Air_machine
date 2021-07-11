#include "SPI.h"
void SPI_init(void){
	//��ʼ��GPIOΪ��������ģʽ������SPI1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1,ENABLE);
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AF_PP;
	settings.GPIO_Pin=MOSI|MISO|SCK;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);
	
	//����SPI����ģʽ
	SPI_InitTypeDef settings1;
	settings1.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	settings1.SPI_Mode=SPI_Mode_Master;//����ģʽ
	settings1.SPI_DataSize=SPI_DataSize_8b;//һ�δ���8λ
	settings1.SPI_CPOL=SPI_CPOL_High;
	settings1.SPI_CPHA=SPI_CPHA_2Edge;
	settings1.SPI_NSS=SPI_NSS_Soft;//���Ƭѡ
	settings1.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//36M/256�ٶ�
	settings1.SPI_FirstBit=SPI_FirstBit_MSB;//��λ�ȳ�
	settings1.SPI_CRCPolynomial=0;//CRCУ�����ʽ����������
	SPI_Init(SPI1,&settings1);
	SPI_Cmd(SPI1,ENABLE);//����SPI1
}

void SPI_send(uint8_t data){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//�ȴ����ͻ�����׼����
	SPI_I2S_SendData(SPI1,data);//����
}
uint8_t SPI_read(){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);//�ȴ����ջ�����׼����
	return SPI_I2S_ReceiveData(SPI1);
}

