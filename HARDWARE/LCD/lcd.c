#include "delay.h"
#include "lcd.h"
#include "font.h"
#include "spi.h"
/*
如果使用软件SPI，只需把硬件SPI的宏开关去掉即可
*/
//#define __LCD_HW_SPI_DRIVE__
#define __LCD_SW_SPI_DRIVE__

/*
LCD_PWR：PB7
LCD_RST：PA4
LCD_WR： PB4
LCD_CS： PD7
*/

//#define LCD_PWR PBout(7)// 
//#define LCD_RST PAout(4)// 
//#define LCD_WR  PBout(0)// 
//#define LCD_CS  PBout(1)// 

//#define	LCD_PWR(n)	(n ? LCD_PWR1=0 : LCD_PWR1=1)
//#define	LCD_RST(n)	(n ? LCD_RST1=1 : LCD_RST1=0)
//#define	LCD_WR(n)	(n ? LCD_WR1=1  : LCD_WR1=0 )
//#define	LCD_CS(n)	(n ? LCD_CS1=1  : LCD_CS1=0 )

#define LCD_WIDTH 	240
#define LCD_HEIHGT 	208

/* 默认LCD的画笔颜色和背景色 */
u16 POINT_COLOR =0xFFFF ;	/* 画笔颜色 */
u16 BACK_COLOR =0x0000 ;  	/* 背景色 */ 

#if defined(__LCD_HW_SPI_DRIVE__)	/* 硬件SPI驱动 */

#elif defined(__LCD_SW_SPI_DRIVE__)		/* 软件SPI驱动 */

/*
LCD_SDA：PA7
LCD_SCK：PA5
*/
//#define LCD_SDA PAout(7)//
//#define LCD_SCK PAout(5)// 

//#define	LCD_SDA(n)	(n ? LCD_SDA1=1 : LCD_SDA1=0)
//#define	LCD_SCK(n)	(n ? LCD_SCK1=1 : LCD_SCK1=0)

/**************************************************************
函数名称 : lcd_gpio_init
函数功能 : lcd gpio初始化
输入参数 : 无
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_gpio_init(void)
{
//#define LCD_PWR PBout(7)// 
//#define LCD_RST PAout(4)// 
//#define LCD_WR  PBout(0)// 
//#define LCD_CS  PBout(1)// 
//#define LCD_SDA PAout(7)//
//#define LCD_SCK PAout(5)// 

	RCC->APB2ENR|=1<<2;    //使能PORTB时钟
 	RCC->APB2ENR|=1<<3;    //使能PORTA时钟
   	 
	GPIOA->CRL&=0X0F00FFFF; 
	GPIOA->CRL|=0X30330000;	// 推挽 	    
	GPIOA->ODR|=3<<4;     	// 上拉	输出高
	GPIOA->ODR|=1<<7;     	// 上拉	输出高

	GPIOB->CRL&=0X0FFFFF00; 
	GPIOB->CRL|=0X30000033;	//推挽
	GPIOB->ODR|=3<<0;     	//上拉	输出高
	GPIOB->ODR|=1<<7;     	//上拉	输出高
	SPI1_Init();
	SPI1_SetSpeed(SPI_SPEED_2);
}

///**************************************************************
//函数名称 : lcd_write_reg
//函数功能 : 向lcd驱动芯片写一个命令
//输入参数 : reg：要写入的命令
//返回值  	 : 无
//备注		 : LCD_WR为0时表示写命令
//**************************************************************/
//void lcd_write_reg(u8 reg)
//{
//	u8 i;
//
//	LCD_CS=0;
//	LCD_WR=0;
//	
//	for(i = 0; i < 8; i++)
//	{			  
//		LCD_SCK=0;
//		if(reg & 0x80)
//			LCD_SDA=1;
//		else 
//			LCD_SDA=0;
//		LCD_SCK=1;
//		reg <<= 1;   
//	}
//	LCD_CS=1;
//	LCD_WR=1;
//}
//
///**************************************************************
//函数名称 : lcd_write_reg
//函数功能 : 向lcd驱动芯片写一个数据
//输入参数 : data：要写入的数据
//返回值  	 : 无
//备注		 : LCD_WR为1时表示写数据
//**************************************************************/
//void lcd_write_data(u8 data)
//{
//	u8 i;
//
//	LCD_CS=0;
//	LCD_WR=1;
//	
//	for(i = 0; i < 8; i++)
//	{			  
//		LCD_SCK=0;
//		if(data & 0x80)
//			LCD_SDA=1;
//		else 
//			LCD_SDA=0;
//		LCD_SCK=1;
//		data <<= 1;   
//	}
//	LCD_CS=1;
//	LCD_WR=1;
//}

#endif

/**************************************************************
函数名称 : lcd_write_reg
函数功能 : 向lcd驱动芯片写一个命令
输入参数 : reg：要写入的命令
返回值  	 : 无
备注		 : LCD_WR为0时表示写命令
**************************************************************/
void lcd_write_reg(u8 reg)
{
	LCD_CS=0;
	LCD_WR=0;
	SPI1_ReadWriteByte(reg);
	LCD_CS=1;
	LCD_WR=1;

//	sp3_write_byte(&reg, 1);
}
void lcd_write_reg1(u8 reg)
{
//	LCD_CS=0;
	LCD_WR=0;
	SPI1_ReadWriteByte(reg);
//	LCD_CS=1;
	LCD_WR=1;

//	sp3_write_byte(&reg, 1);
}
/**************************************************************
函数名称 : lcd_write_data
函数功能 : 向lcd驱动芯片写一个数据
输入参数 : data：要写入的数据
返回值  	 : 无
备注		 : LCD_WR为1时表示写数据
**************************************************************/
void lcd_write_data(u8 data)
{
	LCD_CS=0;
	LCD_WR=1;
	SPI1_ReadWriteByte(data);
	LCD_CS=1;
	LCD_WR=1;

//	sp3_write_byte(&data, 1);
}

void lcd_write_16data(u16 data,u16 data1)
{
//	LCD_CS=0;
//	LCD_WR=1;
	SPI1_ReadWriteByte(data>> 8);
	SPI1_ReadWriteByte(data);
	SPI1_ReadWriteByte(data1>> 8);
	SPI1_ReadWriteByte(data1);
//	LCD_CS=1;
//	LCD_WR=1;

}

/**************************************************************
函数名称 : lcd_write_ram
函数功能 : 使能写入数据到RAM
输入参数 : 无
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_write_ram(void)
{
	lcd_write_reg(0x2C);
}

/**************************************************************
函数名称 : lcd_set_address
函数功能 : 设置坐标
输入参数 : x1,y1：起始地址，x2,y2：终点地址
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_set_address(u16 x1, u16 y1, u16 x2, u16 y2)
{
	 LCD_CS=0;
    lcd_write_reg1(0x2a);
//    lcd_write_data(x1 >> 8);
//    lcd_write_data(x1);
//    lcd_write_data(x2 >> 8);
//    lcd_write_data(x2);
    lcd_write_16data(x1,x2);
    lcd_write_reg1(0x2b);
//    lcd_write_data(y1 >> 8);
//    lcd_write_data(y1);
//    lcd_write_data(y2 >> 8);
//    lcd_write_data(y2);
	lcd_write_16data(y1,y2);
	LCD_CS=1;
}

/**************************************************************
函数名称 : lcd_display_on
函数功能 : 打开显示
输入参数 : 无
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_display_on(void)
{
    //LCD_PWR(1);
	lcd_write_reg(0x29);
}

/**************************************************************
函数名称 : lcd_display_off
函数功能 : 关闭显示
输入参数 : 无
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_display_off(void)
{
   // LCD_PWR(0);
	lcd_write_reg(0x28);
}

/**************************************************************
函数名称 : lcd_clear
函数功能 : lcd清屏函数
输入参数 : color：要清屏的颜色
返回值  	 : 无
备注		 : 先关闭显示，等到所有颜色数据写入到RAM后再打开显示
**************************************************************/
void lcd_clear(u16 color)
{	
    u16 i, j;

	lcd_display_off();		/* 关闭显示 */
	lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIHGT - 1);
	lcd_write_ram();
	
	for(i = 0; i < LCD_HEIHGT; i++)
	{
		for(j = 0; j < LCD_WIDTH; j++)
		{
			lcd_write_data(color >> 8);
			lcd_write_data(color & 0x00ff);
		}
	}
	lcd_display_on();		/* 打开显示 */
}

/**************************************************************
函数名称 : lcd_draw_point
函数功能 : lcd画一个点
输入参数 : x,y	--> 画点坐标，color --> 点的颜色
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_draw_point(u16 x, u16 y, u16 color)
{
	LCD_CS=0;
	LCD_WR=0;
	SPI1_ReadWriteByte(0x2a);
//	LCD_CS=1;
	LCD_WR=1;


	SPI1_ReadWriteByte(x>> 8);
	SPI1_ReadWriteByte(x);
	SPI1_ReadWriteByte(x>> 8);
	SPI1_ReadWriteByte(x);

//    lcd_write_16data(x1,x2);
//    lcd_write_reg1(0x2b);
	LCD_WR=0;
	SPI1_ReadWriteByte(0x2b);
//	LCD_CS=1;
	LCD_WR=1;

	SPI1_ReadWriteByte(y>> 8);
	SPI1_ReadWriteByte(y);
	SPI1_ReadWriteByte(y>> 8);
	SPI1_ReadWriteByte(y);

//	lcd_write_16data(y1,y2);



//	LCD_CS=1;

//	LCD_CS=0;
	LCD_WR=0;
	SPI1_ReadWriteByte(0x2c);
	LCD_WR=1;

	SPI1_ReadWriteByte(color >> 8);
	SPI1_ReadWriteByte(color & 0x00ff);
	LCD_CS=1;

//	lcd_set_address(x, y, x, y);

//	lcd_write_ram();
//	lcd_write_data(color >> 8);
//    lcd_write_data(color & 0x00ff); 
}

/**************************************************************
函数名称 : lcd_show_char
函数功能 : lcd显示一个字符
输入参数 : x,y:起始坐标
		   num:要显示的字符:" "--->"~"
		   size:字体大小
		   mode:叠加方式(1)还是非叠加方式(0)
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_show_char(u16 x, u16 y, u8 num, u8 size, u8 mode)
{  							  
    u16 temp, t1, t;
	u16 y0 = y;
	u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size/2);	/* 得到字体一个字符对应点阵集所占的字节数	 */
//	if(48 != size)
// 	num = num - ' ';	/* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */
	
	for(t = 0; t < csize; t++)	/*遍历打印所有像素点到LCD */
	{   
//     	if(16 == size)
//		{
////			temp = asc2_1608[num][t];	/* 调用1608字体 */
//     	}
//		else 
		if(24 == size)
		{
			temp = asc2_2412[num][t];	/* 调用2412字体 */
		}
		else if(32 == size)
		{
			temp = asc2_3216[num][t];	/* 调用3216数码管字体 */
		}
		else if(48 == size)
		{
			temp = asc2_4824[num][t];	/* 调用3216数码管字体 */
		}
		else
		{	
			return;		/* 没有找到对应的字库 */
		}
		for(t1 = 0; t1 < 8; t1++)	/* 打印一个像素点到液晶 */
		{			    
			if(temp & 0x80)
			{
				lcd_draw_point(x, y, POINT_COLOR);
			}
			else if(0 == mode)
			{
				lcd_draw_point(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			
//			if(y >= LCD_HEIHGT)
//			{
//				return;		/* 超区域了 */
//			}
			if((y - y0) == size)
			{
				y = y0;
				x++;
//				if(x >= LCD_WIDTH)
//				{
//					return;	/* 超区域了 */
//				}
				break;
			}
		}  	 
	}  	    	   	 	  
} 

void lcd_show_char_data(u16 x, u16 y, u8 num, u8 size, u8 mode)
{  							  
    u16 temp, t1, t;
	u16 y0 = y;
	u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size/2);	/* 得到字体一个字符对应点阵集所占的字节数	 */

 	num = num - ' ';	/* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */
	
	for(t = 0; t < csize; t++)	/*遍历打印所有像素点到LCD */
	{   
//     	if(16 == size)
//		{
//			temp = oled_asc2_1608[num][t];	/* 调用1608字体 */
//     	}
//		else 
		if(24 == size)
		{
			temp = tft_asc2_2412[num][t];	/* 调用2412字体 */
		}
//		else if(32 == size)
//		{
//			temp = asc2_3216[num][t];	/* 调用3216数码管字体 */
//		}
//		else if(48 == size)
//		{
//			temp = asc2_4824[num][t];	/* 调用3216数码管字体 */
//		}
		else
		{	
			return;		/* 没有找到对应的字库 */
		}
		for(t1 = 0; t1 < 8; t1++)	/* 打印一个像素点到液晶 */
		{			    
			if(temp & 0x80)
			{
				lcd_draw_point(x, y, POINT_COLOR);
			}
			else if(0 == mode)
			{
				lcd_draw_point(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			
//			if(y >= LCD_HEIHGT)
//			{
//				return;		/* 超区域了 */
//			}
			if((y - y0) == size)
			{
				y = y0;
				x++;
//				if(x >= LCD_WIDTH)
//				{
//					return;	/* 超区域了 */
//				}
				break;
			}
		}  	 
	}  	    	   	 	  
} 


void lcd_show_char96(u16 x, u16 y, u8 num, u8 size, u8 mode)
{  							  
    u16 temp, t1, t;
	u16 y0 = y;
	u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size/2);	/* 得到字体一个字符对应点阵集所占的字节数	 */
	
// 	num = num - ' ';	/* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */
	
	for(t = 0; t < csize; t++)	/*遍历打印所有像素点到LCD */
	{   
		temp = asc2_9648[num][t];	/* 调用3216数码管字体 */
		for(t1 = 0; t1 < 8; t1++)	/* 打印一个像素点到液晶 */
		{			    
			if(temp & 0x80)
			{
				lcd_draw_point(x, y, POINT_COLOR);
			}
			else if(0 == mode)
			{
				lcd_draw_point(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			
//			if(y >= LCD_HEIHGT)
//			{
//				return;		/* 超区域了 */
//			}
			if((y - y0) == size)
			{
				y = y0;
				x++;
//				if(x >= LCD_WIDTH)
//				{
//					return;	/* 超区域了 */
//				}
				break;
			}
		}  	 
	}  	    	   	 	  
} 

/**************************************************************
函数名称 : lcd_show_string
函数功能 : lcd显示字符串
输入参数 : x,y:起始坐标
		   width,height：区域大小
		   *p:字符串起始地址
		   size:字体大小
		   mode:叠加方式(1)还是非叠加方式(0)
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_show_string(u16 x, u16 y, u16 width, u16 height, u8 *p, u8 size, u8 mode)
{         
	u16 x0 = x;
	width += x;
	height += y;
	
    while((*p<='~') && (*p>=' '))		/* 判断是不是非法字符! */
    {       
        if(x >= width)
		{
			x = x0; 
			y += size;
		}
		
        if(y >= height)
		{	
			break;
        }
        lcd_show_char_data(x, y, *p, size, mode);
        x += size/2;
        p++;
    }  
}

/**************************************************************
函数名称 : lcd_show_chinese
函数功能 : lcd显示字符串
输入参数 : x,y:起始坐标
		   *ch:汉字字符串起始地址
		   size:字体大小
		   n:汉字个数
		   mode:叠加方式(1)还是非叠加方式(0)
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_show_chinese(u16 x, u16 y, const u8 * ch, u8 size, u8 n, u8 mode)
{
	u32 temp, t, t1;
    u16 y0 = y;
    u32 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size) * n;	/* 得到字体字符对应点阵集所占的字节数 */
    
    for(t = 0; t < csize; t++)
	{   												   
		temp = ch[t];	/* 得到点阵数据 */
		
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)
			{
				lcd_draw_point(x, y, POINT_COLOR);
			}
			else if(mode==0)
			{
				lcd_draw_point(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			if((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}  	 
	}  
}

void lcd_show_mode(u16 x, u16 y, u8 num)	// num=0 出风 1 制冷 2 制热  
{  	
    u8 size=48;
	u8 mode=0;						  
    u16 temp, t1, t;
	u16 y0 = y;
	u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size);	/* 得到字体一个字符对应点阵集所占的字节数	 */
	
	
	for(t = 0; t < csize; t++)	/*遍历打印所有像素点到LCD */
	{   
		temp = mode_4848[num][t];	/* 调用3216数码管字体 */
		for(t1 = 0; t1 < 8; t1++)	/* 打印一个像素点到液晶 */
		{			    
			if(temp & 0x80)
			{
				lcd_draw_point(x, y, POINT_COLOR);
			}
			else if(0 == mode)
			{
				lcd_draw_point(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			
//			if(y >= LCD_HEIHGT)
//			{
//				return;		/* 超区域了 */
//			}
			if((y - y0) == size)
			{
				y = y0;
				x++;
//				if(x >= LCD_WIDTH)
//				{
//					return;	/* 超区域了 */
//				}
				break;
			}
		}  	 
	}  	    	   	 	  
} 

/**************************************************************
函数名称 : lcd_init
函数功能 : lcd初始化
输入参数 : 无
返回值  	 : 无
备注		 : 无
**************************************************************/
void lcd_init(void)
{
	lcd_gpio_init();

	LCD_RST=0;
    delay_ms(120);
    LCD_RST=1;
	delay_ms(120);

	/* Sleep Out */
    lcd_write_reg(0x11);
    /* wait for power stability */
    delay_ms(120);

    /* Memory Data Access Control */
    lcd_write_reg(0x36);
    lcd_write_data(0x00);

	lcd_write_reg(0x36);			
	lcd_write_data(0x00);
	
	lcd_write_reg(0x3A);			
	lcd_write_data(0x05);
	
	lcd_write_reg(0xB2);			
	lcd_write_data(0x46); //59Hz
	lcd_write_data(0x46); //59Hz
	lcd_write_data(0x00); 
	lcd_write_data(0x33); 
	lcd_write_data(0x33); 			
	
	lcd_write_reg(0xB7);			
	lcd_write_data(0x03); //12.2, -8.87
	
	lcd_write_reg(0xBB);	//VCOM		
	lcd_write_data(0x02); //
													  	
	lcd_write_reg(0xC2);			
	lcd_write_data(0x01);
	
	lcd_write_reg(0xC3);			
	lcd_write_data(0x11); //4.4V, 0Dh: 4.2V
														  
	lcd_write_reg(0xC4);			
	lcd_write_data(0x20);
	
	lcd_write_reg(0xC6);	//Inversion 		
	lcd_write_data(0x0F); //0F: Dot, EF: Column  	
	
	lcd_write_reg(0xD0);			
	lcd_write_data(0xA4);
	lcd_write_data(0x30);											  												  																								
	
	lcd_write_reg(0xE4);			
	lcd_write_data(0x19); //Gate=208
		
	lcd_write_reg(0xE8);   //Power Control 2
	lcd_write_data(0xFF); 
		    
	lcd_write_reg(0xE0);			
	lcd_write_data(0xF3);
	lcd_write_data(0x11); 
	lcd_write_data(0x19); 
	lcd_write_data(0x0A); 
	lcd_write_data(0x09); 
	lcd_write_data(0x06); 
	lcd_write_data(0x3D); 
	lcd_write_data(0x33); 
	lcd_write_data(0x50); 
	lcd_write_data(0x36); 
	lcd_write_data(0x13); 
	lcd_write_data(0x13); 
	lcd_write_data(0x2D); 
	lcd_write_data(0x32); 
	
	lcd_write_reg(0xE1);			
	lcd_write_data(0xF3);
	lcd_write_data(0x0F); 
	lcd_write_data(0x15); 
	lcd_write_data(0x0B); 
	lcd_write_data(0x0B); 
	lcd_write_data(0x17); 
	lcd_write_data(0x3A); 
	lcd_write_data(0x54); 
	lcd_write_data(0x4E); 
	lcd_write_data(0x38); 
	lcd_write_data(0x14); 
	lcd_write_data(0x14); 
	lcd_write_data(0x2E); 
	lcd_write_data(0x34);  
	
	lcd_write_reg(0x29);
	
	lcd_write_reg(0x21);
//    /* RGB 5-6-5-bit  */
//    lcd_write_reg(0x3A);
//    lcd_write_data(0x05);
//
//    /* Porch Setting */
//    lcd_write_reg(0xB2);
//    lcd_write_data(0x0C);
//    lcd_write_data(0x0C);
//    lcd_write_data(0x00);
//    lcd_write_data(0x33);
//    lcd_write_data(0x33);
//
//    /*  Gate Control */
//    lcd_write_reg(0xB7);
//    lcd_write_data(0x35);
//
//    /* VCOM Setting */
//    lcd_write_reg(0xBB);
//    lcd_write_data(0x1A);   //Vcom=1.625V
//
//    /* LCM Control */
//    lcd_write_reg(0xC0);
//    lcd_write_data(0x2C);
//
//    /* VDV and VRH Command Enable */
//    lcd_write_reg(0xC2);
//    lcd_write_data(0x01);
//
//    /* VRH Set */
//    lcd_write_reg(0xC3);
//    lcd_write_data(0x0B);
//
//    /* VDV Set */
//    lcd_write_reg(0xC4);
//    lcd_write_data(0x20);
//
//    /* Frame Rate Control in Normal Mode */
//    lcd_write_reg(0xC6);
//    lcd_write_data(0x0F);	//60MHZ
//
//    /* Power Control 1 */
//    lcd_write_reg(0xD0);
//    lcd_write_data(0xA4);
//    lcd_write_data(0xA1);
//
//	lcd_write_reg(0x21); 
//
//    /* Positive Voltage Gamma Control */
//    lcd_write_reg(0xE0);
//    lcd_write_data(0x00);
//    lcd_write_data(0x19);
//    lcd_write_data(0x1E);
//    lcd_write_data(0x0A);
//    lcd_write_data(0x09);
//    lcd_write_data(0x15);
//	lcd_write_data(0x3D);   
//	lcd_write_data(0x44);   
//	lcd_write_data(0x51);   
//	lcd_write_data(0x12);   
//	lcd_write_data(0x03);   
//	lcd_write_data(0x00);   
//	lcd_write_data(0x3F);   
//	lcd_write_data(0x3F); 
//
//    /* Negative Voltage Gamma Control */
//    lcd_write_reg(0xE1);
//	lcd_write_data(0x00);   
//	lcd_write_data(0x18);   
//	lcd_write_data(0x1E);   
//	lcd_write_data(0x0A);   
//	lcd_write_data(0x09);   
//	lcd_write_data(0x25);   
//	lcd_write_data(0x3F);   
//	lcd_write_data(0x43);   
//	lcd_write_data(0x52);   
//	lcd_write_data(0x33);   
//	lcd_write_data(0x03);   
//	lcd_write_data(0x00);   
//	lcd_write_data(0x3F);   
//	lcd_write_data(0x3F); 
//
////WriteComm(0x35); 
////Delay(10);  
////WriteComm(0x29); 
//    /* Display Inversion On */
//    lcd_write_reg(0x35);
//	delay_ms(10);
//    lcd_write_reg(0x29);

	lcd_clear(BACK_COLOR);
	/*打开显示*/
	LCD_PWR=0;	
}
//void lcd_gImage()
//{	
//    u16 i, j;
//	u16 n=0;
//	lcd_display_off();		/* 关闭显示 */
//	lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIHGT - 1);
//	lcd_write_ram();
//	
//	for(i = 0; i < LCD_HEIHGT; i++)
//	{
//		for(j = 0; j < LCD_WIDTH; j++)
//		{
////			gImage_back[97920];
//			lcd_write_data(gImage_back[i*j*2]);
//			lcd_write_data(gImage_back[i*j*2+1]);
//		}
//	}
//	lcd_display_on();		/* 打开显示 */
//}
//


