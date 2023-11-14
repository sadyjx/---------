#include "tcp_demo.h"
#include "led.h"
#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	   
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK战舰STM32开发板
//uIP TCP Client测试 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/28
//版本：V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////
u32 tcp_client_len;   	//发送数据个数
u32 tcp_client_reclen;   	//接收数据个数
u8 tcp_client_databuf[200];   	//发送数据缓存	  
u8 tcp_client_sta;				//客户端状态
u8 tcp_client_recbuf[USART_REC_LEN];   	//发送数据缓存	  
u8 tcp_ack_ok=1;
u8 tcp_close_ok=0;

//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//这是一个TCP 客户端应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1400),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void tcp_client_demo_appcall(void)
{		  
 	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(uip_aborted())tcp_client_aborted();		//连接终止	   
	if(uip_timedout())tcp_client_timedout();	//连接超时   
	if(uip_closed())tcp_client_closed();		//连接关闭	   
 	if(uip_connected())tcp_client_connected();	//连接成功	    
	if(uip_acked())tcp_client_acked();			//发送的数据成功送达 
 	//接收到一个新的TCP数据包 
	if (uip_newdata())
	{
		if((tcp_client_sta&(1<<6))==0)//还未收到数据
		{
			if(uip_len>199)
			{		   
				((u8*)uip_appdata)[199]=0;
			}
			tcp_client_reclen=uip_len;		    
	    	strcpy((char*)tcp_client_databuf,uip_appdata);				   	  		  
			tcp_client_sta|=1<<6;//表示收到客户端数据
		}				  
	}else if(tcp_client_sta&(1<<5))//有数据需要发送
	{
		//s->textptr=tcp_client_databuf;
		s->textptr=tcp_client_recbuf;
		s->textlen=tcp_client_len;
		tcp_client_sta&=~(1<<5);//清除标记
	}  
	//当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
	{
		tcp_client_senddata();
	}											   
}
//这里我们假定Server端的IP地址为:192.168.1.103
//这个IP必须根据Server端的IP修改.
//尝试重新连接
u8 tcp_client_reconnect(u32 port,u8 *buf)
{
	uip_ipaddr_t ipaddr;
	uip_ipaddr(&ipaddr,buf[0],buf[1],buf[2],buf[3]);	//设置IP为192.168.1.103
	uip_connect(&ipaddr,htons(port)); 	//端口为1400
	//uip_ipaddr(&ipaddr,tcp_client_ip[0],tcp_client_ip[1],tcp_client_ip[2],tcp_client_ip[3]);	//设置IP为192.168.1.103
	//uip_connect(&ipaddr,htons(tcp_client_port)); 	//端口为1400
}
//终止连接				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接
//    if(tcp_close_ok!=1)
//	{
//	 tcp_client_reconnect();		//尝试重新连接
//	}
	//tcp_client_reconnect();		//尝试重新连接
	//uip_log("tcp_client aborted!\r\n");//打印log
}
//连接超时
void tcp_client_timedout(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接
 
  //	uip_log("tcp_client timeout!\r\n");//打印log
}
//连接关闭
void tcp_client_closed(void)
{
	tcp_client_sta&=~(1<<7);	//标志没有连接
//   if(tcp_close_ok!=1)
//	{
//	 tcp_client_reconnect();		//尝试重新连接
//	}
	//uip_log("tcp_client closed!\r\n");//打印log
}	 
//连接建立
void tcp_client_connected(void)
{ 
//	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=1<<7;		//标志连接成功
  	//uip_log("tcp_client connected!\r\n");//打印log
//	s->state=STATE_CMD; 		//指令状态
//	s->textlen=0;
//	s->textptr="connected\r\n";//回应消息
//	s->textlen=strlen((char *)s->textptr);	  
}
//发送的数据成功送达
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//发送清零
    if(tcp_buffer_sta)
	{
	tcp_ack_ok=1;
	tcp_buffer_sta=0;
	}
	if(tcp_close_ok==1)
	{
	//uip_close();
		tcp_close_ok=0;
	    tcp_client_sta&=~(1<<7);	//标志没有连接
	    uip_abort();
//		
	}

	//uip_log("tcp_client acked!\r\n");//表示成功发送		 
}
//发送数据给服务端
void tcp_client_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//s->textptr:发送的数据包缓冲区指针
	//s->textlen:数据包的大小（单位字节）		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//发送TCP数据包	 
}
//void tcp_client_close(void)
//{
//	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
//	tcp_client_sta&=~(1<<7);	//标志没有连接
//  	//uip_log("tcp_client connected!\r\n");//打印log
//	s->textlen=0;
//	s->textptr=" \r\n";//回应消息
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
