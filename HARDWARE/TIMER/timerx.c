#include "timerx.h"
#include "led.h"
#include "rs485.h"	 
#include "usart2.h"	
u32  usart_time=0 ;
u32  tcp_usart_time=0 ;
u32  tcp_dns_time=0 ;


////////////////////////////////////////////////////////////////////////////////// 
u32 uip_timer=0;//uip 计时器，每10ms增加1.   	 
u32 tcp_udp_time=0;
//u32 tcp_dhcp_time=0;
u32 xt_time=0;

u8 sys_time_int=0;
//extern u8  in_rs485_timeint;	   //每隔200mS查询一次
//extern u16 WIFI_CONNECT_TIME;	//连上网络的时间计时	  1MS计时
//extern u16 WIFI_CONNECT_STA;		//连上网络标志

//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		uip_timer++;//uip计时器增加1
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
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

//使能定时器2,使能中断.
void Timer2_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;//TIM2时钟使能    
 	TIM2->ARR=arr;  //设定计数器自动重装值  
	TIM2->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM2->DIER|=1<<0;   //允许更新中断				
	TIM2->DIER|=1<<6;   //允许触发中断
		  							    
	TIM2->CR1|=0x01;    //使能定时器2
  	MY_NVIC_Init(3,3,TIM2_IRQChannel,2);//抢占3，子优先级3，组2(组2中优先级最低的)	
//	TIM2_Set(0);								 
}
//void TIM2_Set(u8 sta)
//{
//	if(sta)
//	{
//		TIM2->CR1|=1<<0;     //使能定时器4
//    	TIM2->CNT=0;         //计数器清空
//	}else TIM2->CR1&=~(1<<0);//关闭定时器4	   
//}


//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 	
	    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		uip_timer++;//uip计时器增加1
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
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM3->DIER|=1<<0;   //允许更新中断	  
	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(3,1,TIM3_IRQChannel,3);//抢占1，子优先级3，组2									 
}
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<1; 	//TIM3时钟使能    
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
	GPIOB->CRL&=0XFF00FFFF;	//PB4 PB5输出
	GPIOB->CRL|=0X00BB0000;	//复用功能输出 	  	 
	   
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]
	AFIO->MAPR|=1<<11;      //部分重映像,TIM3_CH2->PB5	 部分重映像,TIM3_CH1->PB4
	TIM3->ARR=arr;			//设定计数器自动重装值 
	TIM3->PSC=psc;			//预分频器不分频
	
	TIM3->CCMR1|=7<<12;  	//CH2 PWM2模式		 
	TIM3->CCMR1|=1<<11; 	//CH2预装载使能	   
	TIM3->CCER|=1<<4;   	//OC2 输出使能	   
	TIM3->CR1=0x8000;   	//ARPE使能 
	TIM3->CR1|=0x01;    	//使能定时器3 

	TIM3->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM3->CCMR1|=1<<3; 		//CH1 预装载使能	   
	TIM3->CCER|=1<<1;   	//OC1 低电平有效 
	TIM3->CCER|=1<<0;   	//OC1 输出使能	   
	TIM3->CR1=0x0080;   	//ARPE使能 
	TIM3->CR1|=0x01;    	//使能定时器3 											  
												  
}  	 

//定时器5通道1输入捕获配置
//arr：自动重装值
//psc：时钟预分频数
void TIM5_Cap_Init(u16 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 时钟使能 
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟  
	 
	GPIOA->CRL&=0XFFFFFFF0;	//PA0 清除之前设置  
	GPIOA->CRL|=0X00000008;	//PA0 输入   
	GPIOA->ODR|=0<<0;		//PA0 下拉
	  
 	TIM5->ARR=arr;  		//设定计数器自动重装值   
	TIM5->PSC=psc;  		//预分频器 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

	TIM5->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM5->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

	TIM5->DIER|=1<<1;   	//允许捕获中断				
	TIM5->DIER|=1<<0;   	//允许更新中断	
	TIM5->CR1|=0x01;    	//使能定时器2
	MY_NVIC_Init(2,0,TIM5_IRQChannel,2);//抢占2，子优先级0，组2	   
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//获取当前的捕获值.
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
	 			TIM5->CNT=0;					//计数器清空
	 			TIM5->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;//清除中断标志位 	    
}
//TIM4 CH1 PWM输出设置 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<2; 	//TIM4时钟使能    
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
 	  	
	GPIOB->CRL&=0X00FFFFFF;	//PB6输出
	GPIOB->CRL|=0X4B000000;	//复用功能输出 	  
 
	TIM4->ARR=arr;			//设定计数器自动重装值 
	TIM4->PSC=psc;			//预分频器分频设置
	
	TIM4->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM4->CCMR1|=1<<3; 		//CH1 预装载使能	   

	TIM4->CCER|=1<<1;   	//OC1 低电平有效 
	TIM4->CCER|=1<<0;   	//OC1 输出使能	   

	TIM4->CR1=0x0080;   	//ARPE使能 
	TIM4->CR1|=0x01;    	//使能定时器3 											  
} 
////////////////////////////////////////////////////////////////////////////////////////////		  

//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//溢出中断
	{
	//	uip_timer++;//uip计时器增加1
//		uip_timer++;//uip计时器增加1
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
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
} 
//基本定时器6中断初始化					  
//arr：自动重装值。		
//psc：时钟预分频数		 
//Tout= ((arr+1)*(psc+1))/Tclk；
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;					//TIM6时钟使能    
 	TIM6->ARR=arr;  					//设定计数器自动重装值 
	TIM6->PSC=psc;  			 		//设置预分频器.
 	TIM6->DIER|=1<<0;   				//允许更新中断				
 	TIM6->CR1|=0x01;    				//使能定时器6
  	MY_NVIC_Init(3,1,TIM6_IRQChannel,3);//抢占1，子优先级2，组2		
}






















