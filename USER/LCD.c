#include "LCD.h"
#include "delay.h"
#include "font.h"

/*******************�ײ�API(���û�����)*****************************/
//ѡ��Ĵ���
void LCD_WriteIndex(uint8_t cmd){
 GPIO_ResetBits(GPIOA,CS);//ѡ��
 GPIO_ResetBits(GPIOB,DC);//��0������
 SPI_send(cmd);
 GPIO_SetBits(GPIOA,CS);//�ͷ�
}
//д8bit����
void LCD_WriteData(uint8_t data){
 GPIO_ResetBits(GPIOA,CS);//ѡ��
 GPIO_SetBits(GPIOB,DC);//��1������
 SPI_send(data);
 GPIO_SetBits(GPIOA,CS);//�ͷ�
}
//д16bit����
void LCD_WriteData_16Bit(uint16_t data){
	LCD_WriteData(data>>8);
	LCD_WriteData(data);
}
/*******************�ײ�API����**************************************/



/*******************�û���API*****************************/
//�������ܣ����û�������(x,y)
void LCD_SetXY(int16_t x,int16_t y){
	LCD_WriteIndex(0x2A);
	LCD_WriteData_16Bit(x);
	LCD_WriteIndex(0x2B);
	LCD_WriteData_16Bit(y);
}
//�������ܣ����û�������
uint8_t LCD_SetWindow(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
	//�������
	if(xStar>LCD_WEDTH || yStar>LCD_HEIGH  || xEnd> LCD_WEDTH || yEnd >LCD_HEIGH ){
	
		return 0;
	}
	LCD_WriteIndex(0x2A);
	LCD_WriteData_16Bit(xStar);
	LCD_WriteData_16Bit(xEnd);
	LCD_WriteIndex(0x2B);
	LCD_WriteData_16Bit(yStar);
	LCD_WriteData_16Bit(yEnd);
	return 1;
}
//�������ܣ���һ����
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetXY(x,y);
	LCD_WriteIndex(0x2c);	
	LCD_WriteData_16Bit(color);
}
//�������ܣ���һ��ɫ��
void LCD_DrawBlock(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color)
{
	uint32_t i;
	LCD_SetWindow(x0,y0,x0+width,y0+height);
	LCD_WriteIndex(0x2C);
	i=(x0+width-x0+1)*(y0+height-y0+1);
	for(;i>0;i--){
	LCD_WriteData_16Bit(color);
	}
}
//�������ܣ���ָ����ɫ����
void LCD_Clear(uint16_t color)               
{	
   unsigned int i;
   LCD_SetWindow(0,0,LCD_WEDTH-1,LCD_HEIGH-1);
	 LCD_WriteIndex(0x2c);
   for(i=0;i<LCD_WEDTH*LCD_HEIGH;i++)
   {	LCD_WriteData_16Bit(color);
   }   
}
//�������ܣ�������Ƕ�ֱ��
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	LCD_SetWindow(0,0,LCD_WEDTH,LCD_HEIGH);
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
//�������ܣ���Բ
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint16_t r,uint8_t parts,uint8_t fill,uint16_t color,uint16_t fill_color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		if(parts & 0x01){
		LCD_DrawPoint(x0+b,y0-a,color);             //1   
			if(fill)LCD_DrawLine(x0,y0-a,x0+b-1,y0-a,fill_color);
		}
		if(parts>>1 & 0x01){
		LCD_DrawPoint(x0+a,y0-b,color);             //2
				if(fill)LCD_DrawLine(x0,y0-b,x0+a-1,y0-b,fill_color);
		}
		if(parts>>2 & 0x01){
		LCD_DrawPoint(x0-a,y0-b,color);             //3      
				if(fill)LCD_DrawLine(x0,y0-b,x0-a+1,y0-b,fill_color);
		}
		if(parts>>3 & 0x01)  {  
		LCD_DrawPoint(x0-b,y0-a,color);             //4
				if(fill)LCD_DrawLine(x0,y0-a,x0-b+1,y0-a,fill_color);
		}
		if(parts>>4 & 0x01){
		LCD_DrawPoint(x0-b,y0+a,color);  					 //5     
				if(fill)LCD_DrawLine(x0,y0+a,x0-b+1,y0+a,fill_color);
		}
		if(parts>>5 & 0x01){
		LCD_DrawPoint(x0-a,y0+b,color);             //6    
				if(fill)LCD_DrawLine(x0,y0+b,x0-a+1,y0+b,fill_color);
		}
		if(parts>>6 & 0x01){
		LCD_DrawPoint(x0+a,y0+b,color);             //7 	 
				if(fill)LCD_DrawLine(x0,y0+b,x0+a-1,y0+b,fill_color);
		}
		if(parts>>7 & 0x01){
		LCD_DrawPoint(x0+b,y0+a,color);             //8   
				if(fill)LCD_DrawLine(x0,y0+a,x0+b-1,y0+a,fill_color);
		}			
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
//�������ܣ�������
void LCD_DrawBox(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t line_width,uint16_t line_color,uint16_t inter_color,uint8_t fill)
{
if(line_width>0){
	for(uint16_t i=0;i<line_width;i++){
	LCD_DrawLine(x0,y0+i,x0+width,y0+i,line_color);
	LCD_DrawLine(x0,y0+height-i,x0+width,y0+height-i,line_color);
	LCD_DrawLine(x0+i,y0,x0+i,y0+height,line_color);
	LCD_DrawLine(x0+width-i,y0,x0+width-i,y0+height,line_color);
	}
}
	if(fill){
	LCD_DrawBlock(x0+line_width,y0+line_width,width-2*line_width,height-2*line_width,inter_color);
	}
}
//�������ܣ���ָ��λ����ʾASCII�ַ�
void LCD_DrawWord(uint16_t x,uint16_t y,char word,uint16_t color,uint16_t back_color,uint8_t font)
{
	uint16_t i,j,k,data=0;
			if(!LCD_SetWindow(x,y,x+font/2-1,y+font-1))return;
			LCD_WriteIndex(0x2c);
	if(font==16){
		for(i=0;i<16;i++){
			data=ASCII_8x16[word-' '][i];
			for(j=0;j<8;j++){
				if(data&0x01){
				LCD_WriteData_16Bit(color);
				}	else{
				LCD_WriteData_16Bit(back_color);
				}
				data>>=1;
			}
		}
	}else if(font==32){
		for(i=0;i<4;i++){
				for(j=0;j<16;j++){
					data=ASCII_16x32[(word-' ')*4+i][j];
				for(k=0;k<8;k++){
				if(data&0x01){
				LCD_WriteData_16Bit(color);
				}	else{
				LCD_WriteData_16Bit(back_color);
				}
				data>>=1;
			}
				}
		}
	}
}
//�������ܣ���ָ��λ����ʾASCII�ַ���
void LCD_DrawWords(uint16_t x,uint16_t y,char* word_pointer,uint16_t color,uint16_t back_color,uint8_t font)
{
	while(*word_pointer!='\0'){
		
		if(*word_pointer=='^'){
			word_pointer++;
			x+=font/2;
		continue;
		}
		
		LCD_DrawWord(x,y,*word_pointer,color,back_color,font);
		word_pointer++;
		x+=font/2;
	}
}

//�������ܣ���ָ��λ����ʾ�����ַ�
void LCD_DrawChinese(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font){

	
	//���
	uint8_t index;
	for(index=0;index<CHINESE_NUMS;index++){
		if(p[0]==Chinese[index].code[0] && p[1]==Chinese[index].code[1]){
		break;
		}
	}
	if(index==CHINESE_NUMS){
		return;//û�鵽��ֱ���˳�����
	};
	//˵���鵽�ˣ����Ƹ���
	if(font==16){
		//���û�����
		if(!LCD_SetWindow(x,y,x+15,y+15))return;
		LCD_WriteIndex(0x2c);//��������
		//����16������ 32�ֽ� һ��2���ֽ�,16��
		for(uint8_t row=1;row<=16;row++){
			for(uint8_t col=1;col<=2;col++){
				/*
								c1	|		c2
						r1	0		|		1				
						r2	2		|		3
						r3	4		|		5
						r4	6		|		7
				���=2*(r-1)+c-1
				*/
				uint8_t data=Chinese[index]._16x16[2*(row-1)+col-1];//�ó�һ��8����
				for(uint8_t i=0;i<8;i++){
					if(data&0x80){
					LCD_WriteData_16Bit(color);
					}else{
					LCD_WriteData_16Bit(back_color);
					}
					data<<=1;
				}
			}
		}
	
	}else if(font ==32){
		//���û�����
		if(!LCD_SetWindow(x,y,x+31,y+31))return;
		LCD_WriteIndex(0x2c);//��������
		//����32������ 128�ֽ� һ��4�ֽ�,32��
		for(uint8_t row=1;row<=32;row++){
			for(uint8_t col=1;col<=4;col++){
				/*
								c1	|		c2
						r1	0		|		1				
						r2	2		|		3
						r3	4		|		5
						r4	6		|		7
				���=4*(r-1)+c-1
				*/
				uint8_t data=Chinese[index]._32x32[4*(row-1)+col-1];//�ó�һ��8����
				for(uint8_t i=0;i<8;i++){
					if(data&0x80){
					LCD_WriteData_16Bit(color);
					}else{
					LCD_WriteData_16Bit(back_color);
					}
					data<<=1;
				}
			}
		}
	}else{
	return;//û�и����壬ֱ���˳�
	}

}
//�������ܣ���ָ��λ����ʾ�����ַ���
void LCD_DrawChineseString(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font){
	uint16_t i=0;
	while(*p!='\0'){
	if(*p=='^'){
		p+=1;//һ��Ӣ���ַ�ռһ���ֽڣ�ָ���ƶ�1��
		i++;
		continue;
	}
	LCD_DrawChinese(x+i*font,y,p,color,back_color,font);
	p+=2;//һ����������ռ�����ֽڣ�ָ���ƶ�2��
	i++;
	}
}


//����ǿ���^���ʽ������֧����Ӣ�Ļ������
void LCD_DrawString(uint16_t x,uint16_t y,char *p,uint16_t color,uint16_t back_color,uint8_t font){
	uint16_t i=0;//λ�ü�����һ��iռ���8�����ص�
	while(*p!='\0'){
		if(*p=='^'){
			p++;//��һ���ַ��ض��������ַ�
			LCD_DrawChinese(x+i*8,y,p,color,back_color,font);
			i+=font/8;//����ռfont/8��i���
			p+=2;//����վ2���ֽڣ�ָ���ƶ�2��
		}else{
		//���粻��^����ǰ��Ӣ���ַ�
			LCD_DrawWord(x+i*8,y,*p,color,back_color,font);
			i+=font/16;//Ӣ��ռfont/16��i���
			p++;//Ӣ��ռ1���ֽڣ�ָ���ƶ�1��
		}
	}
}
/*******************�û���API����*****************************/

//��Ļ��ʼ������
void LCD_init_cmd(){
//************* Reset LCD Driver ****************//
	GPIO_SetBits(GPIOB,RST);
	delay_ms(1); // Delay 1ms
	GPIO_ResetBits(GPIOB,RST);
	delay_ms(10); // Delay 10ms // This delay time is necessary
	GPIO_SetBits(GPIOB,RST);
	delay_ms(120); // Delay 120 ms
//************* Start Initial Sequence **********//
	LCD_WriteIndex(0x11);  
	LCD_WriteData(0x00); 
	LCD_WriteIndex(0xCF);  
	LCD_WriteData(0X00); 
	LCD_WriteData(0XC1); 
	LCD_WriteData(0X30);
	LCD_WriteIndex(0xED);  
	LCD_WriteData(0X64); 
	LCD_WriteData(0X03); 
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);
	LCD_WriteIndex(0xE8);  
	LCD_WriteData(0X85); 
	LCD_WriteData(0X11); 
	LCD_WriteData(0X78);
	LCD_WriteIndex(0xF6);  
	LCD_WriteData(0X01); 
	LCD_WriteData(0X30); 
	LCD_WriteData(0X00);
	LCD_WriteIndex(0xCB);  
	LCD_WriteData(0X39); 
	LCD_WriteData(0X2C); 
	LCD_WriteData(0X00);
	LCD_WriteData(0X34);
	LCD_WriteData(0X05);
	LCD_WriteIndex(0xF7);  
	LCD_WriteData(0X20); 
	LCD_WriteIndex(0xEA);  
	LCD_WriteData(0X00); 
	LCD_WriteData(0X00); 
	LCD_WriteIndex(0xC0);  
	LCD_WriteData(0X20); 
	LCD_WriteIndex(0xC1);  
	LCD_WriteData(0X11); 
	LCD_WriteIndex(0xC5);  
	LCD_WriteData(0X31); 
	LCD_WriteData(0X3C); 
	LCD_WriteIndex(0xC7);  
	LCD_WriteData(0XA9);
	LCD_WriteIndex(0x3A);  
	LCD_WriteData(0X55); 
  LCD_WriteIndex(0x36);  
	LCD_WriteData(0xE8);//��������
	LCD_WriteIndex(0xB1);  
	LCD_WriteData(0X00); 
	LCD_WriteData(0X18); 
	LCD_WriteIndex(0xB4);  
	LCD_WriteData(0X00); 
	LCD_WriteData(0X00); 
	LCD_WriteIndex(0xF2);  
	LCD_WriteData(0X00); 
	LCD_WriteIndex(0x26);  
	LCD_WriteData(0X01); 
	LCD_WriteIndex(0xE0);  
	LCD_WriteData(0X0F); 
	LCD_WriteData(0X17); 
	LCD_WriteData(0X14); 
	LCD_WriteData(0X09); 
	LCD_WriteData(0X0C); 
	LCD_WriteData(0X06); 
	LCD_WriteData(0X43); 
	LCD_WriteData(0X75); 
	LCD_WriteData(0X36); 
	LCD_WriteData(0X08); 
	LCD_WriteData(0X13); 
	LCD_WriteData(0X05); 
	LCD_WriteData(0X10); 
	LCD_WriteData(0X0B); 
	LCD_WriteData(0X08); 
	LCD_WriteIndex(0xE1);  
	LCD_WriteData(0X00); 
	LCD_WriteData(0X1F); 
	LCD_WriteData(0X23); 
	LCD_WriteData(0X03); 
	LCD_WriteData(0X0E); 
	LCD_WriteData(0X04); 
	LCD_WriteData(0X39); 
	LCD_WriteData(0X25); 
	LCD_WriteData(0X4D); 
	LCD_WriteData(0X06); 
	LCD_WriteData(0X0D); 
	LCD_WriteData(0X0B); 
	LCD_WriteData(0X33); 
	LCD_WriteData(0X37); 
	LCD_WriteData(0X0F); 
	LCD_WriteIndex(0x29);  	
}

void LCD_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//����GPIO A.B.C
	GPIO_InitTypeDef settings;
	settings.GPIO_Mode=GPIO_Mode_Out_PP;
	settings.GPIO_Pin=RST|DC;
	settings.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&settings);
	settings.GPIO_Pin=LED;
	GPIO_Init(GPIOC,&settings);
	settings.GPIO_Pin=CS;
	GPIO_Init(GPIOA,&settings);
	GPIO_ResetBits(GPIOC,LED);//�ر���Ļ����
	GPIO_SetBits(GPIOA,CS);//Ĭ���ͷ�Ƭѡ
	SPI_init();//��ʼ��SPI�ӿ�
	delay_init();//��ʼ���ӳٺ���
	LCD_init_cmd();//������Ļ��ʼ��ָ��
	LCD_Clear(WHITE);
	GPIO_SetBits(GPIOC,LED);//������Ļ����
	//����Ϊ���Դ���
}
