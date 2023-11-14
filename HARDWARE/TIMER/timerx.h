#ifndef __TIMERX_H
#define __TIMERX_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����
//V1.2 20120904
//1,����TIM5_Cap_Init����
//2,����TIM5_IRQHandler�жϷ�����	
//V1.3 20120908
//1,����TIM4_PWM_Init����						  
////////////////////////////////////////////////////////////////////////////////// 


//ͨ���ı�TIM3->CCR2��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
#define LEDR_PWM_VAL TIM3->CCR2 
#define LEDB_PWM_VAL TIM3->CCR1 
   
//TIM4 CH1��ΪPWM DAC�����ͨ�� 
#define LEDG_PWM_VAL  TIM4->CCR1 

extern u32  usart_time;
extern u32  tcp_usart_time ;
extern u32	tcp_dns_time;
extern u32	tcp_udp_time;
//extern u32 tcp_dhcp_time=0;
extern u32  xt_time;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);
void Timer2_Init(u16 arr,u16 psc);

#endif























