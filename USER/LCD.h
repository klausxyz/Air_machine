#ifndef __LCD_H__
#define __LCD_H__
#include "stm32f10x.h"                  // Device header
#include "SPI.h"
/***************************************************
*LI9341_TFT_LCD驱动
*作者：刘欢
*日期：2019/12/30
***************************************************/
#define LCD_HEIGH 240
#define LCD_WEDTH 320

#define WHITE 0xFFFF
#define RED 0x8802
#define RED_ 0xE8C4
#define RED__ 0xE800

#define YELLOW  0xFE41
#define YELLOW_ 0xFF80
#define YELLOW__ 0xBBCA

#define BLUE 0x001F
#define BLUE_ 0x7497
#define BLUE__  0x051D
#define BLUE___   0x3A39
#define BLUE____ 0x9EDD

#define GREEN   0x2589
#define GREEN_   0xB723
#define GREEN__   0x07E0

#define BLACK   0x0000
#define GRAY 0x78EF
#define GRAY_ 0xC618
#define ORINGE 0xFBE4
#define PURPLE 0xA254
#define PINK 0xFD79
/*定义引脚*/

/*GPIOA*/
#define CS GPIO_Pin_4

/*GPIOB*/
#define RST GPIO_Pin_0
#define DC GPIO_Pin_1

/*GPIOC*/
#define LED GPIO_Pin_15

/**********************API**************************/
void LCD_init(void);
void LCD_SetXY(int16_t x,int16_t y);
uint8_t LCD_SetWindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawBlock(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color);
void LCD_Clear(uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint16_t r,uint8_t parts,uint8_t fill,uint16_t color,uint16_t fill_color);
void LCD_DrawBox(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t line_width,uint16_t line_color,uint16_t inter_color,uint8_t fill);
void LCD_DrawWord(uint16_t x,uint16_t y,char word,uint16_t color,uint16_t back_color,uint8_t font);
void LCD_DrawWords(uint16_t x,uint16_t y,char* word_pointer,uint16_t color,uint16_t back_color,uint8_t font);
void LCD_DrawChinese(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font);
void LCD_DrawChineseString(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font);

//功能强大的^标记式函数，支持中英文混合输入
void LCD_DrawString(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font);
#endif
