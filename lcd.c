#include "oled.h"
#include "stdlib.h"
#include "spi.h"
#include "gpio.h"
#include "HanZi.h"

u16 BACK_COLOR=GRAY, POINT_COLOR=WHITE;  //这是显示汉字时的背景颜色与字体颜色，若想更改背景颜色需要使用清屏

u8  ref=0;                //刷新显示
u16 vx=15542,vy=11165;    //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx=140,chy=146;      //默认像素点坐标为0时的AD起始值

//------------------------------------  基础命令写入程序-开始  ---------------------------------//
/**********************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
***********************************************************/
void LCD_Writ_Bus(uint8_t dat)   //串行数据写入
{				  
	// 控制CS引脚（若需手动管理）
  // OLED_CS_Clr();                       // CS拉低,spi片选启动
	HAL_SPI_Transmit(&hspi1, &dat, 1, 0x00FF);
  // OLED_CS_Set();                       // CS拉高,spi片选结束
}
/**********************************************************
      函数说明：LCD写入数据8位
      入口数据：dat 写入的数据
      返回值：  无
***********************************************************/
void LCD_WR_DATA8(uint8_t dat) //发送数据-8位参数
{	
    OLED_DC_Set();         // DC=1（数据模式）
	  LCD_Writ_Bus(dat);      // 数据内容
}  
/**********************************************************
      函数说明：LCD写入数据16位
      入口数据：da 写入的数据
			返回值：  无
***********************************************************/
 void LCD_WR_DATA(uint16_t dat)   
{
    OLED_DC_Set();        // DC=1（数据模式）
//	  LCD_Writ_Bus(dat >> 8);  // 发送高8位
//    LCD_Writ_Bus(dat & 0xFF);     // 发送低8位
	dat = (dat >> 8) | (dat << 8);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&dat, 2, 0x00FF);
}	  
/**********************************************************
      函数说明：LCD写入命令
      入口数据：da 写入的命令
      返回值：  无
***********************************************************/
void LCD_WR_REG(uint8_t dat)	  //发送命令
{	  
    OLED_DC_Clr();           // DC=0（命令模式）
  	LCD_Writ_Bus(dat);       // 命令内容
}
/**********************************************************
      函数说明：LCD写入命令及16位参数
      入口数据：reg  8位命令，da  16位参数
      返回值：  无
***********************************************************/
void LCD_WR_REG_DATA(uint8_t reg,uint16_t dat)   
{	
    LCD_WR_REG(reg);     // 发送命令
	  LCD_WR_DATA(dat);    // 发送16位数据
} 

 //------------------------------------  基础命令写入程序-结束  ---------------------------------//

//------------------------------------  绘图程序-开始  ---------------------------------//
/***********************************************************
      函数说明：设置进行操作的起始和结束地址  
      入口数据：x1,x2 设置列的起始和结束地址
               y1,y2 设置行的起始和结束地址
      返回值：  无
***********************************************************/
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) 
{ 
    // 设置列地址范围 (X方向)
    LCD_WR_REG(0x2a);          // 发送列地址设置命令
    LCD_WR_DATA8(x1 >> 8);     // 起始列高8位 (通常为0)
    LCD_WR_DATA8(x1 & 0xFF);   // 起始列低8位
    LCD_WR_DATA8(x2 >> 8);     // 结束列高8位 (通常为0)
    LCD_WR_DATA8(x2 & 0xFF);   // 结束列低8位
    // 设置行地址范围 (Y方向)
    LCD_WR_REG(0x2b);          // 发送行地址设置命令
    LCD_WR_DATA8(y1 >> 8);     // 起始行高8位
    LCD_WR_DATA8(y1 & 0xFF);   // 起始行低8位
    LCD_WR_DATA8(y2 >> 8);     // 结束行高8位
    LCD_WR_DATA8(y2 & 0xFF);   // 结束行低8位
    // 启动GRAM写入
    LCD_WR_REG(0x2C);          // 发送内存写入命令
}
void Lcd_Init(void)
{
	//MX_GPIO_Init();
  //	OLED_DC_Clr() ;  //打开片选使能
  //	HAL_Delay(20);
	
OLED_RES_Clr();
HAL_Delay(20);
OLED_RES_Set();
HAL_Delay(150);
//************* st7789**********// 
LCD_WR_REG(0x11); // Sleep Out（退出睡眠模式）
HAL_Delay(120);   //Delay 120ms 	
//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36); 
LCD_WR_DATA8(0x00);  //竖屏
//LCD_WR_DATA8(0xC0);//横屏
//LCD_WR_DATA8(0x70);//横屏
// 参数解析：
                        // [MY:0][MX:0][MV:0][ML:0][RGB:0][MH:0][0][0]
                        // MY=0: 行地址不反向
                        // MX=0: 列地址不反向
                        // MV=0: 行/列不交换（竖屏模式）
                        // 若需横屏，可用0xC0（MV=1,行列交换）
LCD_WR_REG(0x3A);   // COLMOD: 接口像素格式
LCD_WR_DATA8(0x55); // 16位/pixel (RGB565)手册最新版本推荐值
LCD_WR_REG(0xB2);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x33);
LCD_WR_REG(0xB7); 
LCD_WR_DATA8(0x35);  
LCD_WR_REG(0xBB);
LCD_WR_DATA8(0x19);
LCD_WR_REG(0xC0);
LCD_WR_DATA8(0x2C);
LCD_WR_REG(0xC2);
LCD_WR_DATA8(0x01);
LCD_WR_REG(0xC3);
LCD_WR_DATA8(0x12);   
LCD_WR_REG(0xC4);
LCD_WR_DATA8(0x20);  
LCD_WR_REG(0xC6); 
LCD_WR_DATA8(0x0F);    
LCD_WR_REG(0xD0); 
LCD_WR_DATA8(0xA4);
LCD_WR_DATA8(0xA1);
LCD_WR_REG(0xE0);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2B);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x54);
LCD_WR_DATA8(0x4C);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x0B);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x23);
LCD_WR_REG(0xE1);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2C);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x44);
LCD_WR_DATA8(0x51);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x20);
LCD_WR_DATA8(0x23);
LCD_WR_REG(0x21); 
LCD_WR_REG(0x11); 
HAL_Delay (121); 
LCD_WR_REG(0x29); 
LCD_Clear(LIGHTGREEN);
} 
/******************************************************************************
      函数说明：在全屏填充颜色
      入口数据：color       要填充的颜色
        返回值：无
******************************************************************************/
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
  Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }
	  }
}
/******************************************************************************
      函数说明：以点阵的形式显示汉字
      入口数据：坐标X 坐标Y 字库序号 汉字大小 （8的倍数 16 24 32 40）
      返回值：无
******************************************************************************/
void showhanzi(unsigned int x,unsigned int y,unsigned char index,unsigned char a)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
  Address_set(x,y,x+a-1,y+a-1); //设置区域      
	temp+=index*a*a/8;	
	for(j=0;j<a*a/8;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)  LCD_WR_DATA(POINT_COLOR);
			else                   LCD_WR_DATA(BACK_COLOR);
		}
		temp++;
	 }
}
/******************************************************************************
      函数说明：在某一坐标画一个一像素的小点，
      入口数据：坐标
      返回值：无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//设置光标位置 
	
	LCD_WR_DATA(POINT_COLOR); 
  
} 
/******************************************************************************
      函数说明：在某一坐标向右下方画一个自定义大小的点
      入口数据：坐标  大小
      返回值：无
******************************************************************************/
void LCD_DrawPoint_big(u16 x,u16 y,u8 z)
{
  Address_set(x,y,x+z-1,y+z-1);//设置光标位置 
	for(u8 i=0;i<z*z;i++)
	{
		LCD_WR_DATA(POINT_COLOR); 
	}
}
/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
               xend,yend   终止坐标
							 color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}  
/******************************************************************************
      函数说明：画线  若要改变粗细需要更改画点函数大小
      入口数据：x1,y1:起点坐标
               x2,y2:终点坐标  
							 color 要填充的颜色
      返回值：  无
******************************************************************************/


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
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
/******************************************************************************
      函数说明：画矩形  若要改变粗细需要去更改画点函数大小
      入口数据：x1,y1:起点坐标
               x2,y2:终点坐标  
							 color 要填充的颜色
      返回值：  无
******************************************************************************/

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
/******************************************************************************
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
/******************************************************************************
//在指定位置显示一个字符
//num:要显示的字符:" "--->"~"
//mode:叠加方式(1)还是非叠加方式(0)
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
  u8 temp;        							// 临时存储字模字节
	u8 pos, t;      							// 循环计数器（行、位）
	u16 x0 = x;     							// 保存初始X坐标
	u16 colortemp = POINT_COLOR;  // 保存当前颜色  
  if(x>LCD_W-16||y>LCD_H-16)	return;	    	
	
	num=num-' ';									// 转换为字库数组索引
																			//	  字库从空格字符（ASCII 32）开始存储，例如：
																			//		'A' (65) → 索引 65 - 32 = 33
																			//		'0' (48) → 索引 48 - 32 = 16
	//设置窗口	
	Address_set(x,y,x+8-1,y+16-1);//设置光标位置 
	
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)					       // 遍历16行
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)										 // 遍历8列
		    {                 
		        if(temp&0x01)	POINT_COLOR=colortemp;
						else 					POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1;                         // 右移检查下一位
				x++;                              // 移动到下一列
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/*
	计算长数字位数
*/
int count_digits(int num) {
    if (num == 0) return 1; // 处理0的特殊情况
    int count = 0;
    num = abs(num); // 处理负数
    
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}
//显示多个数字
//x,y :起点坐标	 
//len :数字的位数
//color:颜色
//num:数值(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 

//显示2个数字
//x,y:起点坐标
//num:数值(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 

void showimage1() //显示40*40图片
{
	u8 ref=0;
  int i,j,k; 
	for(k=2;k<4;k++)
	{
	   	for(j=2;j<4;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 { 					 		
			  	 LCD_WR_DATA8(image1[i*2+1]);	 
				   LCD_WR_DATA8(image1[i*2]);				
			 }	
		 }
	}
	ref=0;				
}



/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width)
{
	u16 i,j;
	u32 k=0;
	Address_set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(image1[k*2]);
			LCD_WR_DATA8(image1[k*2+1]);
			k++;
		}
	}			
}
/**
  * 显示单色图像
  * img: 单色图像数组（240x240/8=7200字节）
  * fg_color: 前景色（16位RGB565）
  * bg_color: 背景色（16位RGB565）
  * 宽度
  * 高度
  * 坐标x
  * 坐标y
  * 如果x,y任意超过屏幕大小，默认为居中模式
  */

void DisplayMonochromeImage(const uint8_t *img, 
                           uint16_t fg_color,
                           uint16_t bg_color,
                           int width,
                           int height,
													 int x,
													 int y) 
{
    // 参数有效性检查
    if(!img || width <=0 || height <=0) return;

    // 计算每行实际数据字节数（包含补位）
    const int bytes_per_row = (width + 7) / 8;
    
    // 根据LCD的字节序调整颜色（示例为大端序）
    const uint8_t fg_bytes[] = { (uint8_t)(fg_color >> 8), (uint8_t)(fg_color & 0xFF) };
    const uint8_t bg_bytes[] = { (uint8_t)(bg_color >> 8), (uint8_t)(bg_color & 0xFF) };

    // 设置逐行刷新区域（优化显示速度）
		if(x<=LCD_H && y<=LCD_H)  Address_set(x, y, x+width-1, y+height-1);
    else Address_set((LCD_W-width)/2,(LCD_H-height)/2, LCD_W/2+width/2-1, LCD_H/2+height/2-1);

    // 使用堆内存避免栈溢出（适用于大尺寸显示）
    uint8_t *line_buf = malloc(width * 2);
    if(!line_buf) return;

    for(int y=0; y<height; y++)
    {
        const uint8_t *img_row = img + y * bytes_per_row;
        uint8_t *buf_ptr = line_buf;

        // 逐字节处理原始图像数据
        for(int x=0; x<bytes_per_row; x++)
        {
            const uint8_t byte = img_row[x];
            const int valid_bits = (x == bytes_per_row-1) ? 
                                 (width % 8 ? width % 8 : 8) : 8;

            // 从最高位开始处理有效位
            for(int b=7; b>=8-valid_bits; b--)
            {
                const uint8_t *color = (byte & (1<<b)) ? fg_bytes : bg_bytes;
                *buf_ptr++ = color[0];
                *buf_ptr++ = color[1];
            }
        }

        // 逐字节传输整行数据（无DMA版本）
        for(int i=0; i<width*2; i++)
        {
            LCD_WR_DATA8(line_buf[i]);
        }
    }
    
    free(line_buf);
}
