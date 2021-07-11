#include "TGS2600.h"
void TGS2600_init(void){
	//����ADC1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	//����GPIOA.1����
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	settings.GPIO_Pin=TGS2600_PIN;
	GPIO_Init(GPIOA,&settings);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);//��λADC1
	
	//����ADC1
	ADC_InitTypeDef adc_settings;
	adc_settings.ADC_Mode=ADC_Mode_Independent;//ADC����ģʽ
	adc_settings.ADC_ScanConvMode=DISABLE;//��ͨ��ģʽ
	adc_settings.ADC_ContinuousConvMode=DISABLE;//����ɨ��ģʽ
	adc_settings.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�������
	adc_settings.ADC_DataAlign=ADC_DataAlign_Right;//ADC�����Ҷ���
	adc_settings.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&adc_settings);
	ADC_Cmd(ADC1,ENABLE);//����ADC1
	
	//ADCУ׼
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	

}
uint16_t get_adc(){
ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ADC1ת�����
	return ADC_GetConversionValue(ADC1);//����ת��ֵ
}
float TGS2600_read(void){
return (float)get_adc()/(0xFFF)*3.3f;
}


