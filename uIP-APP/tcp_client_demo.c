#include "tcp_demo.h"
#include "led.h"
#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	   
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEKս��STM32������
//uIP TCP Client���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/28
//�汾��V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////
u32 tcp_client_len;   	//�������ݸ���
u32 tcp_client_reclen;   	//�������ݸ���
u8 tcp_client_databuf[200];   	//�������ݻ���	  
u8 tcp_client_sta;				//�ͻ���״̬
u8 tcp_client_recbuf[USART_REC_LEN];   	//�������ݻ���	  
u8 tcp_ack_ok=1;
u8 tcp_close_ok=0;

//[7]:0,������;1,�Ѿ�����;
//[6]:0,������;1,�յ��ͻ�������
//[5]:0,������;1,��������Ҫ����

//����һ��TCP �ͻ���Ӧ�ûص�������
//�ú���ͨ��UIP_APPCALL(tcp_demo_appcall)����,ʵ��Web Client�Ĺ���.
//��uip�¼�����ʱ��UIP_APPCALL�����ᱻ����,���������˿�(1400),ȷ���Ƿ�ִ�иú�����
//���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
void tcp_client_demo_appcall(void)
{		  
 	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(uip_aborted())tcp_client_aborted();		//������ֹ	   
	if(uip_timedout())tcp_client_timedout();	//���ӳ�ʱ   
	if(uip_closed())tcp_client_closed();		//���ӹر�	   
 	if(uip_connected())tcp_client_connected();	//���ӳɹ�	    
	if(uip_acked())tcp_client_acked();			//���͵����ݳɹ��ʹ� 
 	//���յ�һ���µ�TCP���ݰ� 
	if (uip_newdata())
	{
		if((tcp_client_sta&(1<<6))==0)//��δ�յ�����
		{
			if(uip_len>199)
			{		   
				((u8*)uip_appdata)[199]=0;
			}
			tcp_client_reclen=uip_len;		    
	    	strcpy((char*)tcp_client_databuf,uip_appdata);				   	  		  
			tcp_client_sta|=1<<6;//��ʾ�յ��ͻ�������
		}				  
	}else if(tcp_client_sta&(1<<5))//��������Ҫ����
	{
		//s->textptr=tcp_client_databuf;
		s->textptr=tcp_client_recbuf;
		s->textlen=tcp_client_len;
		tcp_client_sta&=~(1<<5);//������
	}  
	//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
	{
		tcp_client_senddata();
	}											   
}
//�������Ǽٶ�Server�˵�IP��ַΪ:192.168.1.103
//���IP�������Server�˵�IP�޸�.
//������������
u8 tcp_client_reconnect(u32 port,u8 *buf)
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(&ipaddr,buf[0],buf[1],buf[2],buf[3]);	//����IPΪ192.168.1.103
	uip_connect(&ipaddr,htons(port)); 	//�˿�Ϊ1400
	//uip_ipaddr(&ipaddr,tcp_client_ip[0],tcp_client_ip[1],tcp_client_ip[2],tcp_client_ip[3]);	//����IPΪ192.168.1.103
	//uip_connect(&ipaddr,htons(tcp_client_port)); 	//�˿�Ϊ1400
}
//��ֹ����				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������
//    if(tcp_close_ok!=1)
//	{
//	 tcp_client_reconnect();		//������������
//	}
	//tcp_client_reconnect();		//������������
	//uip_log("tcp_client aborted!\r\n");//��ӡlog
}
//���ӳ�ʱ
void tcp_client_timedout(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������
 
  //	uip_log("tcp_client timeout!\r\n");//��ӡlog
}
//���ӹر�
void tcp_client_closed(void)
{
	tcp_client_sta&=~(1<<7);	//��־û������
//   if(tcp_close_ok!=1)
//	{
//	 tcp_client_reconnect();		//������������
//	}
	//uip_log("tcp_client closed!\r\n");//��ӡlog
}	 
//���ӽ���
void tcp_client_connected(void)
{ 
//	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=1<<7;		//��־���ӳɹ�
  	//uip_log("tcp_client connected!\r\n");//��ӡlog
//	s->state=STATE_CMD; 		//ָ��״̬
//	s->textlen=0;
//	s->textptr="connected\r\n";//��Ӧ��Ϣ
//	s->textlen=strlen((char *)s->textptr);	  
}
//���͵����ݳɹ��ʹ�
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//��������
    if(tcp_buffer_sta)
	{
	tcp_ack_ok=1;
	tcp_buffer_sta=0;
	}
	if(tcp_close_ok==1)
	{
	//uip_close();
		tcp_close_ok=0;
	    tcp_client_sta&=~(1<<7);	//��־û������
	    uip_abort();
//		
	}

	//uip_log("tcp_client acked!\r\n");//��ʾ�ɹ�����		 
}
//�������ݸ������
void tcp_client_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//s->textptr:���͵����ݰ�������ָ��
	//s->textlen:���ݰ��Ĵ�С����λ�ֽڣ�		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//����TCP���ݰ�	 
}
//void tcp_client_close(void)
//{
//	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
//	tcp_client_sta&=~(1<<7);	//��־û������
//  	//uip_log("tcp_client connected!\r\n");//��ӡlog
//	s->textlen=0;
//	s->textptr=" \r\n";//��Ӧ��Ϣ
//	s->textlen=strlen((char *)s->textptr);
//	//uip_close();
//	LED0=0;
//
//}
//void tcp_client_uip_abort(void)
//{
//	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
//	uip_stop();
//	LED0=0;
//
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
