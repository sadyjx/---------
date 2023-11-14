#include "led.h"
#include "rs485.h"	 

//LED IO初始化
void LED_Init(void)
{

//#define LED0 PBout(3)// close	   系统运行灯
//#define LED1 PAout(12)// connect   连接状态指示灯
//#define LED2 PAout(15)// connect   连接状态指示灯
//
//#define LED_B PBout(4)// connect   连接状态指示灯
//#define LED_Y PBout(5)// connect   连接状态指示灯
//#define LED_R PBout(6)// connect   连接状态指示灯


	RCC->APB2ENR|=1<<2;    //使能PORTB时钟
 	RCC->APB2ENR|=1<<3;    //使能PORTA时钟
	
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
   	 
	GPIOA->CRH&=0XFF0FFFFF; 
	GPIOA->CRH|=0X00300000;	//PB9 推挽 	    
	GPIOA->ODR|=1<<15;     	//PB9 上拉	输出高

	GPIOA->CRH&=0XFFF0FFFF; 
	GPIOA->CRH|=0X00030000;	//PB9 推挽 	    
	GPIOA->ODR|=1<<12;     	//PB9 上拉	输出高

	GPIOA->CRH&=0XFFFF0FFF; 
	GPIOA->CRH|=0X00003000;	//PB9 推挽 	    
	GPIOA->ODR|=1<<11;     	//PB9 上拉	输出高

	
	GPIOB->CRL&=0XFFFF0FFF; 
	GPIOB->CRL|=0X00003000;	//PA12 推挽 	    
	GPIOB->ODR|=1<<3;     	//PA12 上拉	输出高

	GPIOB->CRL&=0XF000FFFF; 
	GPIOB->CRL|=0X03330000;	//PA12 推挽 	    
	GPIOB->ODR|=7<<4;     	//PA12 上拉	输出高
	LED_Y=0;
	LED_B=0;
	LED_R=0;
   	 
	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;	//PA11 推挽 	    
	GPIOC->ODR|=1<<13;     	//PA11 上拉	输出高
	RS485_TX_EN=0;				//设置为接收模式	
 									  									  
}






