#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���
extern u8 RS485_RX_DATEBUF[9];
extern u8 RS485_RX_DATE_STA;
extern u16 RS485_RX_TIME;
extern u8 rc_bit;
//ģʽ����
#define RS485_TX_EN		PBout(2)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART2_RX 	1			//0,������;1,����.

														 
void RS485_Init(u32 pclk2,u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);		 
#endif	   
















