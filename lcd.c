#include "lcd.h"
#include "stdlib.h"
#include "spi.h"
#include "gpio.h"
#include "HanZi.h"

u16 BACK_COLOR=GRAY, POINT_COLOR=WHITE;  //������ʾ����ʱ�ı�����ɫ��������ɫ��������ı�����ɫ��Ҫʹ������

u8  ref=0;                //ˢ����ʾ
u16 vx=15542,vy=11165;    //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
u16 chx=140,chy=146;      //Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ


//------------------------------------  ��������д�����-��ʼ  ---------------------------------//
 
 
/**********************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
***********************************************************/
void LCD_Writ_Bus(uint8_t dat)   //��������д��
{				  
	// ����CS���ţ������ֶ�����
  // OLED_CS_Clr();                       // CS����,spiƬѡ����
	HAL_SPI_Transmit(&hspi1, &dat, 1, 0x00FF);
  // OLED_CS_Set();                       // CS����,spiƬѡ����
}
/**********************************************************
      ����˵����LCDд������8λ
      ������ݣ�dat д�������
      ����ֵ��  ��
***********************************************************/
void LCD_WR_DATA8(uint8_t dat) //��������-8λ����
{	
    OLED_DC_Set();         // DC=1������ģʽ��
	  LCD_Writ_Bus(dat);      // ��������
}  
/**********************************************************
      ����˵����LCDд������16λ
      ������ݣ�da д�������
			����ֵ��  ��
***********************************************************/
 void LCD_WR_DATA(uint16_t dat)   
{
    OLED_DC_Set();        // DC=1������ģʽ��
//	  LCD_Writ_Bus(dat >> 8);  // ���͸�8λ
//    LCD_Writ_Bus(dat & 0xFF);     // ���͵�8λ
	dat = (dat >> 8) | (dat << 8);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&dat, 2, 0x00FF);
}	  
/**********************************************************
      ����˵����LCDд������
      ������ݣ�da д�������
      ����ֵ��  ��
***********************************************************/
void LCD_WR_REG(uint8_t dat)	  //��������
{	  
    OLED_DC_Clr();           // DC=0������ģʽ��
  	LCD_Writ_Bus(dat);       // ��������
}
/**********************************************************
      ����˵����LCDд�����16λ����
      ������ݣ�reg  8λ���da  16λ����
      ����ֵ��  ��
***********************************************************/
void LCD_WR_REG_DATA(uint8_t reg,uint16_t dat)   
{	
    LCD_WR_REG(reg);     // ��������
	  LCD_WR_DATA(dat);    // ����16λ����
} 

 //------------------------------------  ��������д�����-����  ---------------------------------//




/***********************************************************
      ����˵�������ý��в�������ʼ�ͽ�����ַ  
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
               y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
***********************************************************/
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) 
{ 
    // �����е�ַ��Χ (X����)
    LCD_WR_REG(0x2a);          // �����е�ַ��������
    LCD_WR_DATA8(x1 >> 8);     // ��ʼ�и�8λ (ͨ��Ϊ0)
    LCD_WR_DATA8(x1 & 0xFF);   // ��ʼ�е�8λ
    LCD_WR_DATA8(x2 >> 8);     // �����и�8λ (ͨ��Ϊ0)
    LCD_WR_DATA8(x2 & 0xFF);   // �����е�8λ

    // �����е�ַ��Χ (Y����)
    LCD_WR_REG(0x2b);          // �����е�ַ��������
    LCD_WR_DATA8(y1 >> 8);     // ��ʼ�и�8λ
    LCD_WR_DATA8(y1 & 0xFF);   // ��ʼ�е�8λ
    LCD_WR_DATA8(y2 >> 8);     // �����и�8λ
    LCD_WR_DATA8(y2 & 0xFF);   // �����е�8λ

    // ����GRAMд��
    LCD_WR_REG(0x2C);          // �����ڴ�д������
}
								
void Lcd_Init(void)
{
	//MX_GPIO_Init();
  //	OLED_DC_Clr() ;  //��Ƭѡʹ��
  //	HAL_Delay(20);
	
 	OLED_RES_Clr();
	HAL_Delay(20);
	OLED_RES_Set();
	HAL_Delay(150);
//************* st7789**********// 
	  
LCD_WR_REG(0x11); // Sleep Out���˳�˯��ģʽ��
HAL_Delay(120);   //Delay 120ms 	
	
//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36); 
LCD_WR_DATA8(0x00);  //����
//LCD_WR_DATA8(0xC0);//����
//LCD_WR_DATA8(0x70);//����
// ����������
                        // [MY:0][MX:0][MV:0][ML:0][RGB:0][MH:0][0][0]
                        // MY=0: �е�ַ������
                        // MX=0: �е�ַ������
                        // MV=0: ��/�в�����������ģʽ��
                        // �������������0xC0��MV=1,���н�����
LCD_WR_REG(0x3A);   // COLMOD: �ӿ����ظ�ʽ
LCD_WR_DATA8(0x55); // 16λ/pixel (RGB565)�ֲ����°汾�Ƽ�ֵ��

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
 //------------------------------------ST7735S Gamma Sequence---------------------------------//
 	//************* Start Initial Sequence **********//
//	LCD_WR_REG(0x11); //Sleep out
//	HAL_Delay(120);
//	LCD_WR_REG(0xE0);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x22);
//	LCD_WR_DATA8(0x07);
//	LCD_WR_DATA8(0x0A);
//	LCD_WR_DATA8(0x2E);
//	LCD_WR_DATA8(0x30);
//	LCD_WR_DATA8(0x25);
//	LCD_WR_DATA8(0x2A);
//	LCD_WR_DATA8(0x28);
//	LCD_WR_DATA8(0x26);
//	LCD_WR_DATA8(0x2E);
//	LCD_WR_DATA8(0x3A);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x01);
//	LCD_WR_DATA8(0x03);
//	LCD_WR_DATA8(0x13);
//	LCD_WR_REG(0xE1);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x16);
//	LCD_WR_DATA8(0x06);
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x2D);
//	LCD_WR_DATA8(0x26);
//	LCD_WR_DATA8(0x23);
//	LCD_WR_DATA8(0x27);
//	LCD_WR_DATA8(0x27);
//	LCD_WR_DATA8(0x25);
//	LCD_WR_DATA8(0x2D);
//	LCD_WR_DATA8(0x3B);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x01);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
//	LCD_WR_REG(0x3A); //65k mode
//	LCD_WR_DATA8(0x05);
//	LCD_WR_REG(0x29); //Display on
} 


//*************************************************************


/******************************************************************************
      ����˵������ȫ�������ɫ
      ������ݣ�color       Ҫ������ɫ
        ����ֵ����
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
      ����˵�����Ե������ʽ��ʾ����
      ������ݣ�����X ����Y �ֿ���� ���ִ�С ��8�ı��� 16 24 32 40��
      ����ֵ����
******************************************************************************/
void showhanzi(unsigned int x,unsigned int y,unsigned char index,unsigned char a)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
  Address_set(x,y,x+a-1,y+a-1); //��������      
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
      ����˵������ĳһ���껭һ��һ���ص�С�㣬
      ������ݣ�����
      ����ֵ����
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	
	LCD_WR_DATA(POINT_COLOR); 
  
} 	
/******************************************************************************
      ����˵������ĳһ���������·���һ���Զ����С�ĵ�
      ������ݣ�����  ��С
      ����ֵ����
******************************************************************************/
void LCD_DrawPoint_big(u16 x,u16 y,u8 z)
{
  Address_set(x,y,x+z-1,y+z-1);//���ù��λ�� 
	for(u8 i=0;i<z*z;i++)
	{
		LCD_WR_DATA(POINT_COLOR); 
	}
}
/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
               xend,yend   ��ֹ����
							 color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}  
/******************************************************************************
      ����˵��������  ��Ҫ�ı��ϸ��Ҫ���Ļ��㺯����С
      ������ݣ�x1,y1:�������
               x2,y2:�յ�����  
							 color Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
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
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
      ����˵����������  ��Ҫ�ı��ϸ��Ҫȥ���Ļ��㺯����С
      ������ݣ�x1,y1:�������
               x2,y2:�յ�����  
							 color Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
/******************************************************************************
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
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
//��ָ��λ����ʾһ���ַ�
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
  u8 temp;        							// ��ʱ�洢��ģ�ֽ�
	u8 pos, t;      							// ѭ�����������С�λ��
	u16 x0 = x;     							// �����ʼX����
	u16 colortemp = POINT_COLOR;  // ���浱ǰ��ɫ  
  if(x>LCD_W-16||y>LCD_H-16)	return;	    	
	
	num=num-' ';									// ת��Ϊ�ֿ���������
																			//	  �ֿ�ӿո��ַ���ASCII 32����ʼ�洢�����磺
																			//		'A' (65) �� ���� 65 - 32 = 33
																			//		'0' (48) �� ���� 48 - 32 = 16
	//���ô���	
	Address_set(x,y,x+8-1,y+16-1);//���ù��λ�� 
	
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)					       // ����16��
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)										 // ����8��
		    {                 
		        if(temp&0x01)	POINT_COLOR=colortemp;
						else 					POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1;                         // ���Ƽ����һλ
				x++;                              // �ƶ�����һ��
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/*
	���㳤����λ��
*/
int count_digits(int num) {
    if (num == 0) return 1; // ����0���������
    int count = 0;
    num = abs(num); // ������
    
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}
//��ʾ�������
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
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

//��ʾ�������
//x,y:�������
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}



void showimage1() //��ʾ40*40ͼƬ
{
	u8 ref=0;
  int i,j,k; 
	for(k=2;k<4;k++)
	{
	   	for(j=2;j<4;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//��������
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
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
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
  * @brief  ��ʾ240x240��ɫͼ��
  * @param  img: ��ɫͼ�����飨240x240/8=7200�ֽڣ�
  * @param  fg_color: ǰ��ɫ��16λRGB565��
  * @param  bg_color: ����ɫ��16λRGB565��
  */

void DisplayMonochromeImage(const uint8_t *img, 
                           uint16_t fg_color,
                           uint16_t bg_color,
                           int width,
                           int height,
													 int x,
													 int y) 
{
    // ������Ч�Լ��
    if(!img || width <=0 || height <=0) return;

    // ����ÿ��ʵ�������ֽ�����������λ��
    const int bytes_per_row = (width + 7) / 8;
    
    // ����LCD���ֽ��������ɫ��ʾ��Ϊ�����
    const uint8_t fg_bytes[] = { (uint8_t)(fg_color >> 8), (uint8_t)(fg_color & 0xFF) };
    const uint8_t bg_bytes[] = { (uint8_t)(bg_color >> 8), (uint8_t)(bg_color & 0xFF) };

    // ��������ˢ�������Ż���ʾ�ٶȣ�
		if(x<=LCD_H && y<=LCD_H)  Address_set(x, y, x+width-1, y+height-1);
    else Address_set((LCD_W-width)/2,(LCD_H-height)/2, LCD_W/2+width/2-1, LCD_H/2+height/2-1);

    // ʹ�ö��ڴ����ջ����������ڴ�ߴ���ʾ��
    uint8_t *line_buf = malloc(width * 2);
    if(!line_buf) return;

    for(int y=0; y<height; y++)
    {
        const uint8_t *img_row = img + y * bytes_per_row;
        uint8_t *buf_ptr = line_buf;

        // ���ֽڴ���ԭʼͼ������
        for(int x=0; x<bytes_per_row; x++)
        {
            const uint8_t byte = img_row[x];
            const int valid_bits = (x == bytes_per_row-1) ? 
                                 (width % 8 ? width % 8 : 8) : 8;

            // �����λ��ʼ������Чλ
            for(int b=7; b>=8-valid_bits; b--)
            {
                const uint8_t *color = (byte & (1<<b)) ? fg_bytes : bg_bytes;
                *buf_ptr++ = color[0];
                *buf_ptr++ = color[1];
            }
        }

        // ���ֽڴ����������ݣ���DMA�汾��
        for(int i=0; i<width*2; i++)
        {
            LCD_WR_DATA8(line_buf[i]);
        }
    }
    
    free(line_buf);
}