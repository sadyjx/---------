#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//LED�˿ڶ���
#define LED0 PBout(3)// close	   ϵͳ���е�
#define LED1 PAout(12)// connect   ����״ָ̬ʾ��
#define LED2 PAout(15)// connect   ����״ָ̬ʾ��

#define LED_B PBout(4)// connect   ����״ָ̬ʾ��
#define LED_Y PBout(5)// connect   ����״ָ̬ʾ��
#define LED_R PBout(6)// connect   ����״ָ̬ʾ��
	
//

// //LED�˿ڶ���
//#define LED0 PAout(12)// close	   ϵͳ���е�
//#define LED1 PAout(11)// connect   ����״ָ̬ʾ��	

void LED_Init(void);//��ʼ��		 				    
#endif

















