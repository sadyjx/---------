#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
//#include "usart.h"

u8   coder_sta=0;	 //��4λ 0x80��ת����  0x10 ���󴥷�	��4λ 0x08��ת����  0x01 ���Ҵ���    

//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(KEY3==1)	 //WK_UP����
//	{				 
//		BEEP=!BEEP;	
//	}		 
//	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
//}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����KEY2
	{
		LED0=!LED0;
	}		 
	EXTI->PR=1<<2;  //���LINE2�ϵ��жϱ�־λ  
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI->PR=1<<3;  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0==0)	 //����KEY0
	{
		LED0=!LED0;
		LED1=!LED1;
	}		 
	EXTI->PR=1<<4;  //���LINE4�ϵ��жϱ�־λ  
}
void EXTI9_5_IRQHandler(void)
{
   if(K_B==0)	 // K_B����
   {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x01)	// K_A�Ѿ�����
		   {
		   	   coder_sta=0x40;	 //����
		   }
		   else
		   if(coder_sta==0x04)	// K_C�Ѿ�����
		   {
		   	  coder_sta=0x80;	//����
		   }
		   else
		   coder_sta=0x02;	   // b����
		}
   }
   EXTI->PR=1<<9;  //���LINE4�ϵ��жϱ�־λ  
}
void EXTI15_10_IRQHandler(void)
{

  if(K_A==0)  // K_A����
  {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x02)	// K_B�Ѿ�����
		   {
		   	   coder_sta=0x80;	 //����
		   }
		   else
		   if(coder_sta==0x04)	// K_C�Ѿ�����
		   {
		   	  coder_sta=0x40;	//����
		   }
		   else
		   coder_sta=0x01;	   // b����
		}
  }
  else
  if(K_C==0)  // K_C����
  {
		if((coder_sta&0xf0)==0)
		{
		   if(coder_sta==0x01)	// K_A�Ѿ�����
		   {
		   	   coder_sta=0x80;	 //����
		   }
		   else
		   if(coder_sta==0x02)	// K_B�Ѿ�����
		   {
		   	  coder_sta=0x40;	//����
		   }
		   else
		   coder_sta=0x04;	   // b����
		}
  }
  EXTI->PR=1<<12;  //���LINE4�ϵ��жϱ�־λ  
  EXTI->PR=1<<13;  //���LINE4�ϵ��жϱ�־λ 
}
		   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0/PE2/PE3/PE4Ϊ�ж�����.
void EXTIX_Init(void)
{
//	KEY_Init();
//#define K_A PCin(13)	    //CONNECT 	  ����
//#define K_B PBin(9)	//PE2
//#define K_C PAin(12)	//PA0  WK_UP

//	Ex_NVIC_Config(GPIO_A,0,RTIR); 			//�����ش���
	Ex_NVIC_Config(GPIO_A,12,FTIR); 			//�½��ش���
	Ex_NVIC_Config(GPIO_B,9, FTIR); 			//�½��ش���
 	Ex_NVIC_Config(GPIO_C,13,FTIR); 			//�½��ش���

	MY_NVIC_Init(2,3,EXTI9_5_IRQChannel,2);	//��ռ2�������ȼ�3����2
	MY_NVIC_Init(2,2,EXTI15_10_IRQChannel,2);	//��ռ2�������ȼ�2����2	   
//	MY_NVIC_Init(2,1,EXTI3_IRQChannel,2);	//��ռ2�������ȼ�1����2	   
//	MY_NVIC_Init(2,0,EXTI4_IRQChannel,2);	//��ռ2�������ȼ�0����2	   
}












