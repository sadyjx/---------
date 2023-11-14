#include "timerx.h"
#include "led.h"
#include "rs485.h"	 
#include "usart2.h"	
u32  usart_time=0 ;
u32  tcp_usart_time=0 ;
u32  tcp_dns_time=0 ;


////////////////////////////////////////////////////////////////////////////////// 
u32 uip_timer=0;//uip ��ʱ����ÿ10ms����1.   	 
u32 tcp_udp_time=0;
//u32 tcp_dhcp_time=0;
u32 xt_time=0;

u8 sys_time_int=0;
//extern u8  in_rs485_timeint;	   //ÿ��200mS��ѯһ��
//extern u16 WIFI_CONNECT_TIME;	//���������ʱ���ʱ	  1MS��ʱ
//extern u16 WIFI_CONNECT_STA;		//���������־

//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		uip_timer++;//uip��ʱ������1
		sys_time_int++;
		usart_time++;
		RS485_RX_TIME++;
		if(sys_time_int>9)
		{
			sys_time_int=0;
//			tcp_usart_time++;
//			tcp_dns_time++;	
//			tcp_udp_time++;	 
	 		xt_time++;
//			in_rs485_timeint++;
		}
		USART2_RX_TIME++;       //???? 	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

//ʹ�ܶ�ʱ��2,ʹ���ж�.
void Timer2_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;//TIM2ʱ��ʹ��    
 	TIM2->ARR=arr;  //�趨�������Զ���װֵ  
	TIM2->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM2->DIER|=1<<0;   //��������ж�				
	TIM2->DIER|=1<<6;   //�������ж�
		  							    
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
  	MY_NVIC_Init(3,3,TIM2_IRQChannel,2);//��ռ3�������ȼ�3����2(��2�����ȼ���͵�)	
//	TIM2_Set(0);								 
}
//void TIM2_Set(u8 sta)
//{
//	if(sta)
//	{
//		TIM2->CR1|=1<<0;     //ʹ�ܶ�ʱ��4
//    	TIM2->CNT=0;         //���������
//	}else TIM2->CR1&=~(1<<0);//�رն�ʱ��4	   
//}


//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 	
	    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		uip_timer++;//uip��ʱ������1
		sys_time_int++;
		usart_time++;
//		if(WIFI_CONNECT_STA==0)
//		{
//		   WIFI_CONNECT_TIME++;
//		}
		if(sys_time_int>9)
		{
			sys_time_int=0;
//			tcp_usart_time++;
//			tcp_dns_time++;	
//			tcp_udp_time++;	 
	 		xt_time++;
//			in_rs485_timeint++;
		}
				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM3->DIER|=1<<0;   //��������ж�	  
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(3,1,TIM3_IRQChannel,3);//��ռ1�������ȼ�3����2									 
}
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<1; 	//TIM3ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	
	GPIOB->CRL&=0XFF00FFFF;	//PB4 PB5���
	GPIOB->CRL|=0X00BB0000;	//���ù������ 	  	 
	   
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XFFFFF3FF; //���MAPR��[11:10]
	AFIO->MAPR|=1<<11;      //������ӳ��,TIM3_CH2->PB5	 ������ӳ��,TIM3_CH1->PB4
	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR1|=7<<12;  	//CH2 PWM2ģʽ		 
	TIM3->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	   
	TIM3->CCER|=1<<4;   	//OC2 ���ʹ��	   
	TIM3->CR1=0x8000;   	//ARPEʹ�� 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 

	TIM3->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM3->CCMR1|=1<<3; 		//CH1 Ԥװ��ʹ��	   
	TIM3->CCER|=1<<1;   	//OC1 �͵�ƽ��Ч 
	TIM3->CCER|=1<<0;   	//OC1 ���ʹ��	   
	TIM3->CR1=0x0080;   	//ARPEʹ�� 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 											  
												  
}  	 

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM5_Cap_Init(u16 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��  
	 
	GPIOA->CRL&=0XFFFFFFF0;	//PA0 ���֮ǰ����  
	GPIOA->CRL|=0X00000008;	//PA0 ����   
	GPIOA->ODR|=0<<0;		//PA0 ����
	  
 	TIM5->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM5->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM5->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM5->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM5->DIER|=1<<1;   	//�������ж�				
	TIM5->DIER|=1<<0;   	//��������ж�	
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
	MY_NVIC_Init(2,0,TIM5_IRQChannel,2);//��ռ2�������ȼ�0����2	   
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//���
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
	 			TIM5->CNT=0;					//���������
	 			TIM5->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;//����жϱ�־λ 	    
}
//TIM4 CH1 PWM������� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<2; 	//TIM4ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	
 	  	
	GPIOB->CRL&=0X00FFFFFF;	//PB6���
	GPIOB->CRL|=0X4B000000;	//���ù������ 	  
 
	TIM4->ARR=arr;			//�趨�������Զ���װֵ 
	TIM4->PSC=psc;			//Ԥ��Ƶ����Ƶ����
	
	TIM4->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM4->CCMR1|=1<<3; 		//CH1 Ԥװ��ʹ��	   

	TIM4->CCER|=1<<1;   	//OC1 �͵�ƽ��Ч 
	TIM4->CCER|=1<<0;   	//OC1 ���ʹ��	   

	TIM4->CR1=0x0080;   	//ARPEʹ�� 
	TIM4->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 											  
} 
////////////////////////////////////////////////////////////////////////////////////////////		  

//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//����ж�
	{
	//	uip_timer++;//uip��ʱ������1
//		uip_timer++;//uip��ʱ������1
		sys_time_int++;
		usart_time++;
//		if(WIFI_CONNECT_STA==0)
//		{
//		   WIFI_CONNECT_TIME++;
//		}
		if(sys_time_int>9)
		{
			sys_time_int=0;
//			tcp_usart_time++;
//			tcp_dns_time++;	
//			tcp_udp_time++;	 
	 		xt_time++;
//			in_rs485_timeint++;
		}
		       				   				     	    	
	}				   
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
} 
//������ʱ��6�жϳ�ʼ��					  
//arr���Զ���װֵ��		
//psc��ʱ��Ԥ��Ƶ��		 
//Tout= ((arr+1)*(psc+1))/Tclk��
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;					//TIM6ʱ��ʹ��    
 	TIM6->ARR=arr;  					//�趨�������Զ���װֵ 
	TIM6->PSC=psc;  			 		//����Ԥ��Ƶ��.
 	TIM6->DIER|=1<<0;   				//��������ж�				
 	TIM6->CR1|=0x01;    				//ʹ�ܶ�ʱ��6
  	MY_NVIC_Init(3,1,TIM6_IRQChannel,3);//��ռ1�������ȼ�2����2		
}






















