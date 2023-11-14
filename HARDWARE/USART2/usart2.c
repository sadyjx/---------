#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
//#include "wifi.h"
#include "zigbee.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//串口2驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2013/2/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//串口发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节
#ifdef USART2_RX_EN   								//如果使能了接收   	  
//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.


u16 USART2_RX_TIME=0;


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA=0; 
u8  USART2_STA=0; 
extern unsigned short rx_in ;  	 
void USART2_IRQHandler(void)
{

#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif  
	u8 res;	    

	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 
        uart_receive_input(res);
//        uart_receive_input(unsigned char value)
//		if((USART2_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART2_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
//				else 
//				{
//					USART2_RX_STA|=0x8000;	//接收完成了
//					if(wifi_send_cmd_ack_sta==0)
//					{
//						wifi_connect();
//						USART2_RX_STA=0;
//					}
//				} 
//			}
//			else //还没收到0X0D
//			{	
//				if(res==0x0d)USART2_RX_STA|=0x4000;
//				else
//				{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
					USART2_RX_STA++;
					if(USART2_RX_STA>(199))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		} 
		USART2_RX_TIME=0;       //接收时间 		 									     
	}
											     
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();    
#endif  
	 											 
}   
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void USART2_Init(u32 pclk1,u32 bound)
{ 

//    wifi_io_int();	 		 
//	RCC->APB2ENR|=1<<8;   	//使能PORTG口时钟  
// 	GPIOG->CRH&=0XFFFFFF0F;	//IO状态设置
//	GPIOG->CRH|=0X00000030;	//IO状态设置
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
	GPIOA->CRL&=0XFFFF00FF;	//IO状态设置
	GPIOA->CRL|=0X00004b00;	//IO状态设置
//	GPIOA->CRL|=0X00008b00;	//IO状态设置
//	GPIOA->ODR|=1<<2;	   	//3上拉
	RCC->APB2ENR|=1<<4;   	//使能PORTA口时钟  
	GPIOC->CRH&=0XF0FFFFFF;	//IO状态设置
	GPIOC->CRH|=0X03000000;	//IO状态设置
	GPIOC->ODR|=1<<14;	   	//3上拉
	WIFI_RST=1;

	RCC->APB1ENR|=1<<17;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	USART2->CR1|=0X200C;  	//1位停止,无校验位.
	USART2->CR3=1<<7;   	//使能串口2的DMA发送
	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);//DMA1通道7,外设为串口2,存储器为USART2_TX_BUF 
	//使能接收中断
	USART2->CR1|=1<<8;    	//PE中断使能
	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,1,USART2_IRQChannel,3);//组2，最低优先级 
//	USART2_RX_STA=0;		//清零
}
//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF)); 	//通过dma发送出去

}

#endif		 
///////////////////////////////////////USART2 DMA发送配置部分//////////////////////////////////	   		    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址    
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
 	RCC->AHBENR|=1<<0;			//开启DMA1时钟
	delay_us(5);
	DMA_CHx->CPAR=cpar; 		//DMA1 外设地址 
	DMA_CHx->CMAR=cmar; 		//DMA1,存储器地址	 
	DMA_CHx->CCR=0X00000000;	//复位
	DMA_CHx->CCR|=1<<4;  		//从存储器读
	DMA_CHx->CCR|=0<<5;  		//普通模式
	DMA_CHx->CCR|=0<<6;  		//外设地址非增量模式
	DMA_CHx->CCR|=1<<7;  		//存储器增量模式
	DMA_CHx->CCR|=0<<8;  		//外设数据宽度为8位
	DMA_CHx->CCR|=0<<10; 		//存储器数据宽度8位
	DMA_CHx->CCR|=1<<12; 		//中等优先级
	DMA_CHx->CCR|=0<<14; 		//非存储器到存储器模式		  	
} 
//开启一次DMA传输
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_CHx->CCR&=~(1<<0);       //关闭DMA传输 
	DMA_CHx->CNDTR=len;          //DMA1,传输数据量 
	DMA_CHx->CCR|=1<<0;          //开启DMA传输
}

//重定义fputc函数 
void UART2_SEND(u8 ch)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   		
}


	   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 





















