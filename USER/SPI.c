#include "SPI.h"
void SPI_init(void){
	//初始化GPIO为复用推挽模式并开启SPI1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1,ENABLE);
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AF_PP;
	settings.GPIO_Pin=MOSI|MISO|SCK;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&settings);
	
	//设置SPI工作模式
	SPI_InitTypeDef settings1;
	settings1.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工
	settings1.SPI_Mode=SPI_Mode_Master;//主机模式
	settings1.SPI_DataSize=SPI_DataSize_8b;//一次传输8位
	settings1.SPI_CPOL=SPI_CPOL_High;
	settings1.SPI_CPHA=SPI_CPHA_2Edge;
	settings1.SPI_NSS=SPI_NSS_Soft;//软件片选
	settings1.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//36M/256速度
	settings1.SPI_FirstBit=SPI_FirstBit_MSB;//高位先出
	settings1.SPI_CRCPolynomial=0;//CRC校验多项式，基本不用
	SPI_Init(SPI1,&settings1);
	SPI_Cmd(SPI1,ENABLE);//开启SPI1
}

void SPI_send(uint8_t data){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//等待发送缓存器准备好
	SPI_I2S_SendData(SPI1,data);//发送
}
uint8_t SPI_read(){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);//等待接收缓存器准备好
	return SPI_I2S_ReceiveData(SPI1);
}

