#ifndef __LCD_H__
#define __LCD_H__

////////////////////////
//begin
#include "stdlib.h"	
#include "main.h"
/******************************************************************************
�����������ַ�
******************************************************************************/
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
/******************************************************************************
������Ļ��С
******************************************************************************/
#define LCD_W 240
#define LCD_H 240
/******************************************************************************
ͨ��ģʽ���������£�
Mode0��CPOL=0��CPHA=0
Mode1��CPOL=0��CPHA=1
Mode2��CPOL=1��CPHA=0
Mode3��CPOL=1��CPHA=1
ʱ�Ӽ���CPOL����������SCLK�ĵ�ƽ��������״̬ʱ�ǿ���̬������Ч̬��ʱ����λCPHA
�������������ݲ������ڵڼ������أ�
CPOL=0����ʾ��SCLK=0ʱ���ڿ���̬��������Ч״̬����SCLK���ڸߵ�ƽʱ
CPOL=1����ʾ��SCLK=1ʱ���ڿ���̬��������Ч״̬����SCLK���ڵ͵�ƽʱ
CPHA=0����ʾ���ݲ������ڵ�1�����أ����ݷ����ڵ�2������
CPHA=1����ʾ���ݲ������ڵ�2�����أ����ݷ����ڵ�1������
******************************************************************************/
#define OLED_MODE 0

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����



//-----------------OLED�˿ڶ���----------------  
#define OLED_RES_Clr()   (GPIOB->BSRR = (1 << (0+16)))    //HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)//RES
#define OLED_RES_Set()   (GPIOB->BSRR = (1 << 0))         //HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr()    (GPIOB->BSRR = (1 << (1+16)))    //HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)//DC
#define OLED_DC_Set()    (GPIOB->BSRR = (1 << 1))         //HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET) 
//
/************************************************************************************************************/
extern  u16 BACK_COLOR, POINT_COLOR;

extern u8 ref;          //ˢ����ʾ
extern u16 vx,vy;       //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 chx,chy;     //Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
/*********************************************************************************************************/


void Lcd_Init(void); 
void LCD_WR_DATA8(uint8_t da); //��������-8λ����
void LCD_WR_DATA(uint16_t da);
void LCD_WR_REG(uint8_t  da);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void LCD_Clear(u16 Color);

void showhanzi(unsigned int x,unsigned int y,unsigned char index,unsigned char a);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_big(u16 x,u16 y,u8 z);//��һ�����
void LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //��ʾһ���ַ���,16����

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width);
//void showimage();
void showimage1();

//////////////////
void DisplayMonochromeImage(const uint8_t *img, 
                           uint16_t fg_color,
                           uint16_t bg_color,
                           int width,
                           int height,
													 int x,
													 int y) 														 ;
int count_digits(int num);

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
//end
////////////////////


#endif 