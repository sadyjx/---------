#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 

/*
	  发送命令 
*/

u8 RS485_RX_DATEBUF[9];
u8 RS485_RX_DATE_STA=0;
extern u8  WIFI_CONNECT_TIME_CRC;//485随机数
extern u8  adrr;


#ifdef EN_USART2_RX   	//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;
u16 RS485_RX_TIME=0;
u8 rc_bit=0;
   
void USART3_IRQHandler(void)
{
	u8 res;	
	if(USART3->SR&(1<<5))//接收到数据
	{	 
		res=USART3->DR; 
		RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
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
//                           if(RS485_RX_BUF[7]==crc)  rc_bit=1; //按位异或校验值
//                           else RS485_RX_CNT = 0;
                          //LED1=~LED1;
						   rc_bit=1; //按位异或校验值
						break;
					default:

						break;
				}

			if(RS485_RX_CNT>9)	RS485_RX_CNT = 0;
			RS485_RX_TIME=0;
		}
					 
//		if(RS485_RX_CNT<63)		//还可以接收数据
//		{
//			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值	 
//			RS485_RX_CNT++;
//			
//		}

	}  											 
}
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口3时钟
//	RCC->APB2ENR|=1<<0;    //开启辅助时钟
//	AFIO->MAPR&=0XFFFFFFE7; //清除MAPR的[26:24]
	 
	GPIOB->CRH&=0XFFFF00FF; 	 //PB11-RX,PB10-TX
	GPIOB->CRH|=0X00008B00;//IO状态设置
	GPIOB->ODR|=3<<10;     	//PB10 11上拉

	GPIOB->CRL&=0XFFFFF0FF; //PB2-485EN
	GPIOB->CRL|=0X00000300; //IO状态设置
	GPIOB->ODR|=1<<2;     	//PB2 11上拉
		  
	RCC->APB1RSTR|=1<<18;   //复位串口3，只有串口1的寄存器在APB2RSTR中
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//USART3->CR2&=0XCFFF;  //1个停止位

	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,1,USART3_IRQChannel,3);//组2，抢占3，优先级2 

	RS485_TX_EN=0;			//默认为接收模式	
}
//
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
	delay_ms(2);

  	for(t=0;t<len;t++)		//循环发送数据
	{
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = (u8) buf[t];      

//		while((USART3->SR&0X40)==0);//等待发送结束		  
//		USART3->DR=buf[t];
	}	 
	delay_ms(2);
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}





