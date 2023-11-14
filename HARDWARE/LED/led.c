#include "led.h"
#include "rs485.h"	 

//LED IO��ʼ��
void LED_Init(void)
{

//#define LED0 PBout(3)// close	   ϵͳ���е�
//#define LED1 PAout(12)// connect   ����״ָ̬ʾ��
//#define LED2 PAout(15)// connect   ����״ָ̬ʾ��
//
//#define LED_B PBout(4)// connect   ����״ָ̬ʾ��
//#define LED_Y PBout(5)// connect   ����״ָ̬ʾ��
//#define LED_R PBout(6)// connect   ����״ָ̬ʾ��


	RCC->APB2ENR|=1<<2;    //ʹ��PORTBʱ��
 	RCC->APB2ENR|=1<<3;    //ʹ��PORTAʱ��
	
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
   	 
	GPIOA->CRH&=0XFF0FFFFF; 
	GPIOA->CRH|=0X00300000;	//PB9 ���� 	    
	GPIOA->ODR|=1<<15;     	//PB9 ����	�����

	GPIOA->CRH&=0XFFF0FFFF; 
	GPIOA->CRH|=0X00030000;	//PB9 ���� 	    
	GPIOA->ODR|=1<<12;     	//PB9 ����	�����

	GPIOA->CRH&=0XFFFF0FFF; 
	GPIOA->CRH|=0X00003000;	//PB9 ���� 	    
	GPIOA->ODR|=1<<11;     	//PB9 ����	�����

	
	GPIOB->CRL&=0XFFFF0FFF; 
	GPIOB->CRL|=0X00003000;	//PA12 ���� 	    
	GPIOB->ODR|=1<<3;     	//PA12 ����	�����

	GPIOB->CRL&=0XF000FFFF; 
	GPIOB->CRL|=0X03330000;	//PA12 ���� 	    
	GPIOB->ODR|=7<<4;     	//PA12 ����	�����
	LED_Y=0;
	LED_B=0;
	LED_R=0;
   	 
	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;	//PA11 ���� 	    
	GPIOC->ODR|=1<<13;     	//PA11 ����	�����
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
 									  									  
}






