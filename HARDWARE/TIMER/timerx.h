#ifndef __TIMERX_H
#define __TIMERX_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽
//V1.2 20120904
//1,新增TIM5_Cap_Init函数
//2,新增TIM5_IRQHandler中断服务函数	
//V1.3 20120908
//1,新增TIM4_PWM_Init函数						  
////////////////////////////////////////////////////////////////////////////////// 


//通过改变TIM3->CCR2的值来改变占空比，从而控制LED0的亮度
#define LEDR_PWM_VAL TIM3->CCR2 
#define LEDB_PWM_VAL TIM3->CCR1 
   
//TIM4 CH1作为PWM DAC的输出通道 
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























