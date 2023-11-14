#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 

/*
	  �������� 
*/

u8 RS485_RX_DATEBUF[9];
u8 RS485_RX_DATE_STA=0;
extern u8  WIFI_CONNECT_TIME_CRC;//485�����
extern u8  adrr;


#ifdef EN_USART2_RX   	//���ʹ���˽���   	  
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;
u16 RS485_RX_TIME=0;
u8 rc_bit=0;
   
void USART3_IRQHandler(void)
{
	u8 res;	
	if(USART3->SR&(1<<5))//���յ�����
	{	 
		res=USART3->DR; 
		RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
		if(rc_bit==0)
		{
				switch (RS485_RX_CNT)
				{
					case 0:	
                          
                           if(RS485_RX_BUF[0]==0x02)  RS485_RX_CNT++;
             	           else
                           RS485_RX_CNT = 0;
						break;
					case 1:
                           	
                           if(RS485_RX_BUF[1]==0x06) RS485_RX_CNT++; 
                           else	RS485_RX_CNT = 0;
						break;
					case 2:	                           	
                           if((RS485_RX_BUF[2]==adrr)||(RS485_RX_BUF[2]==0xff)) RS485_RX_CNT++; 	
                           else RS485_RX_CNT = 0;
						break;
					case 3:	
                            RS485_RX_CNT++;	
						break;
					case 4:	
                            RS485_RX_CNT++;	
						break;
					case 5:	
                            RS485_RX_CNT++;	
						break;
					case 6:	
                            RS485_RX_CNT++;	
						break;
					case 7:	
//                           for(i = 0; i < 7; i++)  crc=crc^RS485_RX_BUF[i];
//                           if(RS485_RX_BUF[7]==crc)  rc_bit=1; //��λ���У��ֵ
//                           else RS485_RX_CNT = 0;
                          //LED1=~LED1;
						   rc_bit=1; //��λ���У��ֵ
						break;
					default:

						break;
				}

			if(RS485_RX_CNT>9)	RS485_RX_CNT = 0;
			RS485_RX_TIME=0;
		}
					 
//		if(RS485_RX_CNT<63)		//�����Խ�������
//		{
//			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ	 
//			RS485_RX_CNT++;
//			
//		}

	}  											 
}
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���3ʱ��
//	RCC->APB2ENR|=1<<0;    //��������ʱ��
//	AFIO->MAPR&=0XFFFFFFE7; //���MAPR��[26:24]
	 
	GPIOB->CRH&=0XFFFF00FF; 	 //PB11-RX,PB10-TX
	GPIOB->CRH|=0X00008B00;//IO״̬����
	GPIOB->ODR|=3<<10;     	//PB10 11����

	GPIOB->CRL&=0XFFFFF0FF; //PB2-485EN
	GPIOB->CRL|=0X00000300; //IO״̬����
	GPIOB->ODR|=1<<2;     	//PB2 11����
		  
	RCC->APB1RSTR|=1<<18;   //��λ����3��ֻ�д���1�ļĴ�����APB2RSTR��
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//USART3->CR2&=0XCFFF;  //1��ֹͣλ

	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,1,USART3_IRQChannel,3);//��2����ռ3�����ȼ�2 

	RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ	
}
//
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
	delay_ms(2);

  	for(t=0;t<len;t++)		//ѭ����������
	{
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = (u8) buf[t];      

//		while((USART3->SR&0X40)==0);//�ȴ����ͽ���		  
//		USART3->DR=buf[t];
	}	 
	delay_ms(2);
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}





