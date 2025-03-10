#ifndef __LCD_H__
#define __LCD_H__

////////////////////////
//begin
#include "stdlib.h"	
#include "main.h"
/******************************************************************************
简化数据类型字符
******************************************************************************/
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
/******************************************************************************
设置屏幕大小
******************************************************************************/
#define LCD_W 240
#define LCD_H 240
/******************************************************************************
通信模式，具体如下：
Mode0：CPOL=0，CPHA=0
Mode1：CPOL=0，CPHA=1
Mode2：CPOL=1，CPHA=0
Mode3：CPOL=1，CPHA=1
时钟极性CPOL是用来配置SCLK的电平出于哪种状态时是空闲态或者有效态，时钟相位CPHA
是用来配置数据采样是在第几个边沿：
CPOL=0，表示当SCLK=0时处于空闲态，所以有效状态就是SCLK处于高电平时
CPOL=1，表示当SCLK=1时处于空闲态，所以有效状态就是SCLK处于低电平时
CPHA=0，表示数据采样是在第1个边沿，数据发送在第2个边沿
CPHA=1，表示数据采样是在第2个边沿，数据发送在第1个边沿
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

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据



//-----------------OLED端口定义----------------  
#define OLED_RES_Clr()   (GPIOB->BSRR = (1 << (0+16)))    //HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)//RES
#define OLED_RES_Set()   (GPIOB->BSRR = (1 << 0))         //HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr()    (GPIOB->BSRR = (1 << (1+16)))    //HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)//DC
#define OLED_DC_Set()    (GPIOB->BSRR = (1 << 1))         //HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET) 
//
/************************************************************************************************************/
extern  u16 BACK_COLOR, POINT_COLOR;

extern u8 ref;          //刷新显示
extern u16 vx,vy;       //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 chx,chy;     //默认像素点坐标为0时的AD起始值
/*********************************************************************************************************/


void Lcd_Init(void); 
void LCD_WR_DATA8(uint8_t da); //发送数据-8位参数
void LCD_WR_DATA(uint16_t da);
void LCD_WR_REG(uint8_t  da);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void LCD_Clear(u16 Color);

void showhanzi(unsigned int x,unsigned int y,unsigned char index,unsigned char a);

void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_DrawPoint_big(u16 x,u16 y,u8 z);//画一个大点
void LCD_ReadPoint(u16 x,u16 y); //读点
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//显示数字
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//显示2个数字
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //显示一个字符串,16字体

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

//画笔颜色
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
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
//end
////////////////////


#endif 