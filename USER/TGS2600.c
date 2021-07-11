#include "TGS2600.h"
void TGS2600_init(void){
	//开启ADC1和GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	//配置GPIOA.1引脚
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	settings.GPIO_Pin=TGS2600_PIN;
	GPIO_Init(GPIOA,&settings);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);//复位ADC1
	
	//配置ADC1
	ADC_InitTypeDef adc_settings;
	adc_settings.ADC_Mode=ADC_Mode_Independent;//ADC独立模式
	adc_settings.ADC_ScanConvMode=DISABLE;//单通道模式
	adc_settings.ADC_ContinuousConvMode=DISABLE;//单次扫描模式
	adc_settings.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
	adc_settings.ADC_DataAlign=ADC_DataAlign_Right;//ADC数据右对齐
	adc_settings.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&adc_settings);
	ADC_Cmd(ADC1,ENABLE);//开启ADC1
	
	//ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	

}
uint16_t get_adc(){
ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待ADC1转换完毕
	return ADC_GetConversionValue(ADC1);//返回转换值
}
float TGS2600_read(void){
return (float)get_adc()/(0xFFF)*3.3f;
}


