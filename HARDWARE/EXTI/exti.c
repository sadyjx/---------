#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
//#include "usart.h"

u8   coder_sta=0;	 //高4位 0x80左转触发  0x10 往左触发	低4位 0x08右转触发  0x01 往右触发    

//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//消抖
//	if(KEY3==1)	 //WK_UP按键
//	{				 
//		BEEP=!BEEP;	
//	}		 
//	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
//}
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键KEY2
	{
		LED0=!LED0;
	}		 
	EXTI->PR=1<<2;  //清除LINE2上的中断标志位  
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI->PR=1<<3;  //清除LINE3上的中断标志位  
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{
		LED0=!LED0;
		LED1=!LED1;
	}		 
	EXTI->PR=1<<4;  //清除LINE4上的中断标志位  
}
void EXTI9_5_IRQHandler(void)
{
   if(K_B==0)	 // K_B触发
   {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x01)	// K_A已经触发
		   {
		   	   coder_sta=0x40;	 //往左
		   }
		   else
		   if(coder_sta==0x04)	// K_C已经触发
		   {
		   	  coder_sta=0x80;	//往右
		   }
		   else
		   coder_sta=0x02;	   // b触发
		}
   }
   EXTI->PR=1<<9;  //清除LINE4上的中断标志位  
}
void EXTI15_10_IRQHandler(void)
{

  if(K_A==0)  // K_A触发
  {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x02)	// K_B已经触发
		   {
		   	   coder_sta=0x80;	 //往右
		   }
		   else
		   if(coder_sta==0x04)	// K_C已经触发
		   {
		   	  coder_sta=0x40;	//往左
		   }
		   else
		   coder_sta=0x01;	   // b触发
		}
  }
  else
  if(K_C==0)  // K_C触发
  {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x01)	// K_A已经触发
		   {
		   	   coder_sta=0x80;	 //往右
		   }
		   else
		   if(coder_sta==0x02)	// K_B已经触发
		   {
		   	  coder_sta=0x40;	//往左
		   }
		   else
		   coder_sta=0x04;	   // b触发
		}
  }
  EXTI->PR=1<<12;  //清除LINE4上的中断标志位  
  EXTI->PR=1<<13;  //清除LINE4上的中断标志位 
}
		   
//外部中断初始化程序
//初始化PA0/PE2/PE3/PE4为中断输入.
void EXTIX_Init(void)
{
//	KEY_Init();
//#define K_A PCin(13)	    //CONNECT 	  连接
//#define K_B PBin(9)	//PE2
//#define K_C PAin(12)	//PA0  WK_UP

//	Ex_NVIC_Config(GPIO_A,0,RTIR); 			//上升沿触发
	Ex_NVIC_Config(GPIO_A,12,FTIR); 			//下降沿触发
	Ex_NVIC_Config(GPIO_B,9, FTIR); 			//下降沿触发
 	Ex_NVIC_Config(GPIO_C,13,FTIR); 			//下降沿触发

	MY_NVIC_Init(2,3,EXTI9_5_IRQChannel,2);	//抢占2，子优先级3，组2
	MY_NVIC_Init(2,2,EXTI15_10_IRQChannel,2);	//抢占2，子优先级2，组2	   
//	MY_NVIC_Init(2,1,EXTI3_IRQChannel,2);	//抢占2，子优先级1，组2	   
//	MY_NVIC_Init(2,0,EXTI4_IRQChannel,2);	//抢占2，子优先级0，组2	   
}












