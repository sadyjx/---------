#include "sys.h"
#include "timerx.h"
#include "usart.h"
//#include "tcp_demo.h"
#include "rs485.h"	 

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

////////////////////////////////////////////////////////////////////////////////// 	  
u8  tcp_clientusart_datacount=0;
u8  tcp_clientusart_int=0;
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
u16 USART_RX_count=0;       //����״̬���
u16 USART_RX_STA=0;       //����״̬���	  
u16 tcp_USART_RX_STA=0;       //����״̬��� 
extern u16 usart_ip_time;


void USART1_IRQHandler(void)
{
	u8 res;	

	if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 

		RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
		RS485_RX_CNT++;						//������������1 
		usart_time=0;
		if(RS485_RX_CNT>12)	RS485_RX_CNT=0;						//������������1 
	}		 

} 
#endif										 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
//091209
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1,1,USART1_IRQChannel,3);//��2��������ȼ� 
#endif
}
