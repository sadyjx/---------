#include "key.h"
#include "delay.h"
#include "exti.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  

u8 KEY4=1;//PE2
u8 KEY5=1;	//PA0  WK_UP
								    
//������ʼ������
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��

	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X00088000;	//PA11���ó�����	    
	GPIOA->ODR|=3<<11;     	//PA11����

	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000008;	//PA11���ó�����	    
	GPIOA->ODR|=1<<8;     	//PA11����
	
//	GPIOA->CRL&=0XFFFFFFF0;	//PA0���ó�����	  
//	GPIOA->CRL|=0X00000008; 
//	GPIOA->ODR|=1<<0;     	//PA0����


	GPIOB->CRH&=0XFFFFFF00; 
	GPIOB->CRH|=0X00000088;	//PB8���ó�����	    
	GPIOB->ODR|=3<<8;     	//PB8����

	GPIOB->CRH&=0XFFF0FFFF; 
	GPIOB->CRH|=0X00080000;	//PB8���ó�����	    
	GPIOB->ODR|=1<<12;     	//PB8����

	RCC->APB2ENR|=1<<4;    	//ʹ��PORTBʱ��	

	GPIOC->CRH&=0X0F0FFFFF; 
	GPIOC->CRH|=0X80800000;	//PB8���ó�����	    
	GPIOC->ODR|=1<<13;     	//PB8����
	GPIOC->ODR|=1<<15;     	//PB8����
//#define K_A PCin(13)	    //CONNECT 	  ����
//#define K_B PBin(9)	//PE2
//#define K_C PAin(12)	//PA0  WK_UP

} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������
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
		delay_ms(20);//ȥ���� 
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
 	return 0;// �ް�������
}




















