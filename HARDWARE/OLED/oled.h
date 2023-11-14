
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 

#define USE_OLED_TFT	0   //定义系统显示使用的是OLED还是TFT 0使用OLED 1使用TFT

   	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	96
#define Max_Row		96
#define	Brightness	0xFF 
#define X_WIDTH 	96
#define Y_WIDTH 	96	    

//-----------------测试LED端口定义---------------- 
//#define LED_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DC
//#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)

//-----------------OLED端口定义----------------  					   
//-----------------OLED端口定义----------------  					   
#define OLED_CS PBout(1)
#define OLED_RST  PAout(4)//在MINISTM32上直接接到了STM32的复位脚！	
#define OLED_DC PBout(0)

//PB0~7,作为数据线
//#define DATAOUT(x) GPIOB->ODR=(GPIOB->ODR&0xff00)|(x&0x00FF); //输出
  
//使用4线串行接口时使用 
#define OLED_SCLK PAout(5)
#define OLED_SDIN PAout(7)


#define OLED_SCLK_Clr() OLED_SCLK=0//CLK
#define OLED_SCLK_Set() OLED_SCLK=1

#define OLED_SDIN_Clr() OLED_SDIN=0//DIN
#define OLED_SDIN_Set() OLED_SDIN=1

#define OLED_RST_Clr() OLED_RST=0//RES
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0//DC
#define OLED_DC_Set() OLED_DC=1
 		     
#define OLED_CS_Clr()  OLED_CS=0//CS
#define OLED_CS_Set()  OLED_CS=1

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ShowChar1_48T(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowChar1_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode);


void OLED_ShowCHinese_FEN(u8 x,u8 y,u8 no);
void OLED_ShowCHinese_T(u8 x,u8 y,u8 no);

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowFGJ_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowChar1(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowSHUJ_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_Refresh_Gram(void);

#endif  
	 



