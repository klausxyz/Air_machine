#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "LCD.h"
#include "G7.h"
#include "stdio.h"
#include "ctype.h"
#include "DHT11.h"
#include "string.h"
#include "DS1302.h"
#include "MH_Z19B.h"
#include "ZE08.h"
#include "TGS2600.h"
char lcd_buffer[50];//屏幕字符缓存数组必须大于50。否则绘图会不正确。很奇葩的BUG
extern uint16_t PM2_5,PM10;
extern uint16_t C02_RAW;
extern uint16_t CH20_RAW;

//定义背景色
#define BACKCOLOR BLUE

uint16_t PM2_5_diagram_data[15];
uint16_t PM10_diagram_data[15];
uint8_t PM_diagram_data_index=0;//图表数据索引
void UI_pm(){
		//显示PM2.5
		sprintf(lcd_buffer,"PM2.5:%d ",PM2_5);
		if(PM2_5<100){
			LCD_DrawString(16,16,lcd_buffer,GREEN,BACKCOLOR,32);
		}else if(PM2_5<300){
		LCD_DrawString(16,16,lcd_buffer,YELLOW,BACKCOLOR,32);
		}else{
		LCD_DrawString(16,16,lcd_buffer,RED,BACKCOLOR,32);
		}
		sprintf(lcd_buffer,"PM10 :%d  ",PM10);
		
		if(PM10<100){
			LCD_DrawString(16,48,lcd_buffer,GREEN,BACKCOLOR,32);
		}else if(PM10<300){
		LCD_DrawString(16,48,lcd_buffer,YELLOW,BACKCOLOR,32);
		}else{
		LCD_DrawString(16,48,lcd_buffer,RED,BACKCOLOR,32);
		}
		LCD_DrawBlock(186,16,2,64,WHITE);//竖线
		LCD_DrawBlock(0,16+64,LCD_WEDTH,2,WHITE);//横线
		if(PM2_5+PM10<200){
		LCD_DrawString(194,32,"AQI: ^优",GREEN,BACKCOLOR,32);
		}else if(PM2_5+PM10<300){
		LCD_DrawString(194,32,"AQI: ^良",YELLOW,BACKCOLOR,32);
		}else{
		LCD_DrawString(194,32,"AQI: ^差",RED,BACKCOLOR,32);
		
		}
		
}

void UI_C02(){
	if(C02_RAW<2000){
			LCD_DrawString(16-16,84,"CO",GREEN,BACKCOLOR,32);
			LCD_DrawString(48-16,97,"2",GREEN,BACKCOLOR,16);
			sprintf(lcd_buffer,":%d  ",C02_RAW);
			LCD_DrawString(60-16,84,lcd_buffer,GREEN,BACKCOLOR,32);
			LCD_DrawString(150-16,95,"ppm",GREEN,BACKCOLOR,16);
	}else{
			LCD_DrawString(16-16,84,"CO",RED,BACKCOLOR,32);
			LCD_DrawString(48-16,97,"2",RED,BACKCOLOR,16);
		sprintf(lcd_buffer,":%d  ",C02_RAW);
			LCD_DrawString(60-16,84,lcd_buffer,RED,BACKCOLOR,32);
		LCD_DrawString(150-16,95,"ppm",RED,BACKCOLOR,16);
	}
}
void UI_CH20(){
	sprintf(lcd_buffer,"^甲^醛:%d  ",CH20_RAW);
	if(CH20_RAW<100){
			LCD_DrawString(180-16,84,lcd_buffer,GREEN,BACKCOLOR,32);
			LCD_DrawString(292,95,"ppb",GREEN,BACKCOLOR,16);
	}else if(CH20_RAW<200){
			LCD_DrawString(180-16,84,lcd_buffer,YELLOW,BACKCOLOR,32);
		LCD_DrawString(292,95,"ppb",YELLOW,BACKCOLOR,16);
	}else{
			LCD_DrawString(180-16,84,lcd_buffer,RED,BACKCOLOR,32);
		LCD_DrawString(292,95,"ppb",RED,BACKCOLOR,16);
	}
}
#define DIAGRAMCOLOR WHITE
#define DIAGRAM_BACKCOLOR BLACK
void UI_diagram(){
	LCD_DrawBlock(0,159,LCD_WEDTH,LCD_HEIGH-159,DIAGRAM_BACKCOLOR);
	LCD_DrawBlock(0,156,LCD_WEDTH,2,WHITE);
	LCD_DrawString(0,160,"500",DIAGRAMCOLOR,DIAGRAM_BACKCOLOR,16);
	LCD_DrawString(0,LCD_HEIGH-16,"  0",DIAGRAMCOLOR,DIAGRAM_BACKCOLOR,16);
	LCD_DrawString(0,192,"250",DIAGRAMCOLOR,DIAGRAM_BACKCOLOR,16);
	
	//绘制PM2.5曲线
	for(uint8_t index=0;index<15-1;index++){
		if(PM2_5_diagram_data[index+1]!=0){
			uint16_t data_a=PM2_5_diagram_data[index]*0.16f;
			uint16_t data_b=PM2_5_diagram_data[index+1]*0.16f;
			if(data_a>80){
			data_a=80;
			}
			if(data_b>80){
			data_b=80;
			}
		LCD_DrawLine(26+20*index,LCD_HEIGH-data_a,26+20*(index+1),LCD_HEIGH-data_b,GREEN);
		}	
	}
	
		//绘制PM10曲线
	for(uint8_t index=0;index<15-1;index++){
		if(PM10_diagram_data[index+1]!=0){
			uint16_t data_a=PM10_diagram_data[index]*0.16f;
			uint16_t data_b=PM10_diagram_data[index+1]*0.16f;
			if(data_a>80){
			data_a=80;
			}
			if(data_b>80){
			data_b=80;
			}
		LCD_DrawLine(26+20*index,LCD_HEIGH-data_a,26+20*(index+1),LCD_HEIGH-data_b,RED);
		}	
	}
	LCD_DrawLine(26,159,26,LCD_HEIGH,DIAGRAMCOLOR);//x轴
	LCD_DrawLine(26,LCD_HEIGH,LCD_WEDTH,LCD_HEIGH,DIAGRAMCOLOR);//y轴
	LCD_DrawString(280,162,"PM10-",RED,DIAGRAM_BACKCOLOR,16);
	LCD_DrawString(225,162,"PM2.5-",GREEN,DIAGRAM_BACKCOLOR,16);
	//x轴刻度
	for(uint8_t i=1;i<=80;i++){
		if(i%20==0){
			LCD_DrawLine(26,160+i,26+4,160+i,DIAGRAMCOLOR);
		}
	}
	
	//y轴刻度
	for(uint16_t i=26+5;i<=LCD_WEDTH;i++){
		if(i%20==0){
			LCD_DrawLine(i,LCD_HEIGH-4,i,LCD_HEIGH,DIAGRAMCOLOR);
		}
	}
	

	
}

void Array_move_font(uint16_t *p,uint16_t size){
	for(uint16_t i=0;i<size-1;i++){
		p[i]=p[i+1];
	}
}
int main(){
	LCD_init();
	LCD_DrawString(55,LCD_HEIGH/2-8,"Sensors are initializing...",BLACK,WHITE,16);
	delay_init();
	G7_init();
	DHT11_init();
	DS1302_init();
	MH_Z19B_init();
	ZE08_init();
	TGS2600_init();
//	DS1302_t settings;
//	settings.hour=13;
//	settings.minute=22;
//	settings.second=0;
//	settings.year=20;
//	settings.month=1;
//	settings.day=7;
//	settings.week=2;
//	DS1302_setTime(&settings);
	delay_ms(2000);
	LCD_Clear(BACKCOLOR);
	memset(PM2_5_diagram_data,0,15);
	memset(PM10_diagram_data,0,15);
	while(1){
		static uint8_t time=0;
		//PM2.5 PM10显示
		 if(time%10==0){
		 UI_pm();
		 }
		 
		 if(time%20==0){
			 if(PM_diagram_data_index<15){
			 PM2_5_diagram_data[PM_diagram_data_index]=PM2_5;
			 PM10_diagram_data[PM_diagram_data_index]=PM10;
				PM_diagram_data_index++;
			 }else{//说明数据满了
			 	Array_move_font(PM2_5_diagram_data,15);//数据前移，丢弃首元素
				Array_move_font(PM10_diagram_data,15);//数据前移，丢弃首元素
				PM2_5_diagram_data[PM_diagram_data_index-1]=PM2_5;
				PM10_diagram_data[PM_diagram_data_index-1]=PM10;
			 }
			 UI_diagram();
		 }
		
		//每5秒更新一次温度
		if(time%5==0){
		DHT11_update();
		}
		//二氧化碳浓度每3秒更新一次
		if(time%8==0){
		MH_Z19B_update();
			 UI_C02();
		}
		
		if(time%10==0){
		UI_CH20();
		}
		
		if(time%4==0){
		sprintf(lcd_buffer,"^空^气^质^量: %.3f ",TGS2600_read());
		LCD_DrawString(16,120,lcd_buffer,WHITE,BACKCOLOR,32);
		}
		
//		//间隔3s
		DS1302_update();//获取时间																							|
		sprintf(lcd_buffer,"20%d^年%d^月%d^日 %d:%d:%d  ^温^度:%d ^湿^度:%d%%    ",DS1302_data.year,DS1302_data.month,DS1302_data.day,DS1302_data.hour,DS1302_data.minute,DS1302_data.second,(int)DHT11_readTemp(),(int)DHT11_readHum());
		LCD_DrawString(0,0,lcd_buffer,BACKCOLOR,~BACKCOLOR,16);
		time++;
		delay_ms(1000);
	};
}
