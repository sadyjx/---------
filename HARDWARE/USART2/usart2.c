#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
//#include "wifi.h"
#include "zigbee.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//����2��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2013/2/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
#ifdef USART2_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


u16 USART2_RX_TIME=0;


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART2_RX_STA=0; 
u8  USART2_STA=0; 
extern unsigned short rx_in ;  	 
void USART2_IRQHandler(void)
{

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif  
	u8 res;	    

	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 
        uart_receive_input(res);
//        uart_receive_input(unsigned char value)
//		if((USART2_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART2_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
//				else 
//				{
//					USART2_RX_STA|=0x8000;	//���������
//					if(wifi_send_cmd_ack_sta==0)
//					{
//						wifi_connect();
//						USART2_RX_STA=0;
//					}
//				} 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(res==0x0d)USART2_RX_STA|=0x4000;
//				else
//				{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
					USART2_RX_STA++;
					if(USART2_RX_STA>(199))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		} 
		USART2_RX_TIME=0;       //����ʱ�� 		 									     
	}
											     
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();    
#endif  
	 											 
}   
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART2_Init(u32 pclk1,u32 bound)
{ 

//    wifi_io_int();	 		 
//	RCC->APB2ENR|=1<<8;   	//ʹ��PORTG��ʱ��  
// 	GPIOG->CRH&=0XFFFFFF0F;	//IO״̬����
//	GPIOG->CRH|=0X00000030;	//IO״̬����
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
	GPIOA->CRL&=0XFFFF00FF;	//IO״̬����
	GPIOA->CRL|=0X00004b00;	//IO״̬����
//	GPIOA->CRL|=0X00008b00;	//IO״̬����
//	GPIOA->ODR|=1<<2;	   	//3����
	RCC->APB2ENR|=1<<4;   	//ʹ��PORTA��ʱ��  
	GPIOC->CRH&=0XF0FFFFFF;	//IO״̬����
	GPIOC->CRH|=0X03000000;	//IO״̬����
	GPIOC->ODR|=1<<14;	   	//3����
	WIFI_RST=1;

	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=(pclk1*1000000)/(bound);// ����������	 
	USART2->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	USART2->CR3=1<<7;   	//ʹ�ܴ���2��DMA����
	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);//DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF 
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    	//PE�ж�ʹ��
	USART2->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,1,USART2_IRQChannel,3);//��2��������ȼ� 
//	USART2_RX_STA=0;		//����
}
//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF)); 	//ͨ��dma���ͳ�ȥ

}

#endif		 
///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ    
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
 	RCC->AHBENR|=1<<0;			//����DMA1ʱ��
	delay_us(5);
	DMA_CHx->CPAR=cpar; 		//DMA1 �����ַ 
	DMA_CHx->CMAR=cmar; 		//DMA1,�洢����ַ	 
	DMA_CHx->CCR=0X00000000;	//��λ
	DMA_CHx->CCR|=1<<4;  		//�Ӵ洢����
	DMA_CHx->CCR|=0<<5;  		//��ͨģʽ
	DMA_CHx->CCR|=0<<6;  		//�����ַ������ģʽ
	DMA_CHx->CCR|=1<<7;  		//�洢������ģʽ
	DMA_CHx->CCR|=0<<8;  		//�������ݿ��Ϊ8λ
	DMA_CHx->CCR|=0<<10; 		//�洢�����ݿ��8λ
	DMA_CHx->CCR|=1<<12; 		//�е����ȼ�
	DMA_CHx->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ		  	
} 
//����һ��DMA����
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
{
	DMA_CHx->CCR&=~(1<<0);       //�ر�DMA���� 
	DMA_CHx->CNDTR=len;          //DMA1,���������� 
	DMA_CHx->CCR|=1<<0;          //����DMA����
}

//�ض���fputc���� 
void UART2_SEND(u8 ch)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   		
}


	   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 





















