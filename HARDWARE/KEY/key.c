#include "key.h"
#include "delay.h"
#include "exti.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  

u8 KEY4=1;//PE2
u8 KEY5=1;	//PA0  WK_UP
								    
//按键初始化函数
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟

	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X00088000;	//PA11设置成输入	    
	GPIOA->ODR|=3<<11;     	//PA11上拉

	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000008;	//PA11设置成输入	    
	GPIOA->ODR|=1<<8;     	//PA11上拉
	
//	GPIOA->CRL&=0XFFFFFFF0;	//PA0设置成输入	  
//	GPIOA->CRL|=0X00000008; 
//	GPIOA->ODR|=1<<0;     	//PA0上拉


	GPIOB->CRH&=0XFFFFFF00; 
	GPIOB->CRH|=0X00000088;	//PB8设置成输入	    
	GPIOB->ODR|=3<<8;     	//PB8上拉

	GPIOB->CRH&=0XFFF0FFFF; 
	GPIOB->CRH|=0X00080000;	//PB8设置成输入	    
	GPIOB->ODR|=1<<12;     	//PB8上拉

	RCC->APB2ENR|=1<<4;    	//使能PORTB时钟	

	GPIOC->CRH&=0X0F0FFFFF; 
	GPIOC->CRH|=0X80800000;	//PB8设置成输入	    
	GPIOC->ODR|=1<<13;     	//PB8上拉
	GPIOC->ODR|=1<<15;     	//PB8上拉
//#define K_A PCin(13)	    //CONNECT 	  连接
//#define K_B PBin(9)	//PE2
//#define K_C PAin(12)	//PA0  WK_UP

} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按
	if(coder_sta&0xF0)
	{
	   if(coder_sta==0x80)	KEY4=0;
	   else					KEY5=0;
	   coder_sta=0x00;
	}
	else
	{
	   KEY4=1;KEY5=1;
	}		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0))
	{
		delay_ms(20);//去抖动 
		key_up=0;
		if((KEY0==0)&&(KEY1==0)&&(KEY3==0))return 8;
		else 
		if((KEY3==0)&&(KEY1==0))return 7;
		else 
		if((KEY0==0)&&(KEY2==0))return 9;
		else 
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(KEY3==0)return 4;
		else if(KEY4==0)return 5;
		else if(KEY5==0)return 6;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1)key_up=1; 	    
 	return 0;// 无按键按下
}




















