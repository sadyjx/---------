#include "sys.h"
#include "usart.h"
#include "timerx.h"	 		   
#include "uip.h"	    
#include "enc28j60.h"
#include "led.h"
#include "wifi.h"
#include "tcp_demo.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK战舰STM32开发板
//uIP TCP测试 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/28
//版本：V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////
u8 tip_udp_datelen=0;
u8 tip_udp_datebuf[USART_REC_LEN];
u8 tip_udp_sata=0;
u8 udp_p=0;
u8 udp_m=0;
u8 tcp_udp_ip[4] ={192,168,1,23};					//作为客户端要连接的服务端IP地址 0
u8 tip_udp_senddatebuf[150];
struct uip_udp_conn *d;

//TCP应用接口函数(UIP_APPCALL)
//完成TCP服务(包括server和client)和HTTP服务
void tcp_demo_appcall(void)
{	
  	
	switch(uip_conn->lport)//本地监听端口80和1200 
	{
		case HTONS(80):
		//	httpd_appcall(); 
			break;
//		case HTONS(1200):
//		    tcp_server_demo_appcall(); 
//			break;
		default:
			tcp_client_demo_appcall();					  
		    break;
	}		    
//	switch(uip_conn->rport)	//远程连接1400端口
//	{
//	    case HTONS(1400):
//			
//	       break;
//	    default: 
//	       break;
//	}   
}
//打印日志用
void uip_log(char *m)
{			    
	printf("uIP log:%s\r\n",m);
}

void UDP_6677_APP(void)
{  
   u8 i;

   u8 tip_udp_datebuf1[256];
   uip_ipaddr_t ipaddr;
	   		char tmp_dat1[21];// = "G1C5E5B6";

	 	           tmp_dat1[0]= 0x47;
				   tmp_dat1[1]= 0x31;
				   tmp_dat1[2]= 0x43;
				   tmp_dat1[3]= 0x35;
				    for(i=4;i<17;i++)
		           {
		            tmp_dat1[i]=tip_udp_senddatebuf[i];
		           }
				   tmp_dat1[17]= 0x45;
				   tmp_dat1[18]= 0x35;
				   tmp_dat1[19]= 0x42;
				   tmp_dat1[20]= 0x36;

//	/* 判断当前状态 */
//	if (uip_poll())
//	{	
//	   if(tcp_udp_time>200)
//	   {
//	         	   tcp_udp_time=0;
//	               uip_send((char *)tmp_dat1,21);	
//	   }
//	}
   
	if (uip_newdata())
	{
	    char *tmp_dat =  "D1C5B5C6";
		char *tmp_dat_1 =  "D1C5B5C5";
		/* 收到新数据 */
		if(uip_len>0)
		{
		 //LED1=0;
		  tip_udp_datelen=uip_len;
//		  strcpy(tip_udp_datebuf1,uip_appdata);
		  memcpy(tip_udp_datebuf1,uip_appdata,tip_udp_datelen);
//		  for(i=0;i<tip_udp_datelen;i++)
//		  {
//			USART1->DR=tip_udp_datebuf1[i];
//			while((USART1->SR&0X40)==0);//等待发送结束
//	      }

	     // uip_send((char *)tmp_dat1,strlen(tmp_dat1));	
		}
		   if(tip_udp_datebuf1[0]==0x41)	       //写参数配置
		   if(tip_udp_datebuf1[1]==0x35)
		   if(tip_udp_datebuf1[2]==0x45)
		   if(tip_udp_datebuf1[3]==0x36)
		   if((tip_udp_datebuf1[4]==0x30)||(tip_udp_datebuf1[4]==0x31))
		   if((tip_udp_datebuf1[59]==0x30)||(tip_udp_datebuf1[59]==0x31))
		   if(tip_udp_datebuf1[tip_udp_datelen-4]==0x42)
		   if(tip_udp_datebuf1[tip_udp_datelen-3]==0x35) 
		   if(tip_udp_datebuf1[tip_udp_datelen-2]==0x43) 
		   if(tip_udp_datebuf1[tip_udp_datelen-1]==0x36) 
		   {
			   for(i=0;i<tip_udp_datelen;i++)
			   {
			   	 tip_udp_datebuf[i]=tip_udp_datebuf1[i];

			   }
//				for(i=0;i<tip_udp_datelen;i++)
//			    {
//				USART1->DR=tip_udp_datebuf[i];
//				while((USART1->SR&0X40)==0);//等待发送结束
//		    	}

			   tip_udp_sata=1; 
			   tcp_usart_time=0;
			   uip_send((char *)tmp_dat,strlen(tmp_dat));
		   }

		   if(tip_udp_datebuf1[0]==0x41)	       //写wifi参数配置
		   if(tip_udp_datebuf1[1]==0x35)
		   if(tip_udp_datebuf1[2]==0x45)
		   if(tip_udp_datebuf1[3]==0x35)		 
		   if(tip_udp_datebuf1[64+4]==0x42)
		   if(tip_udp_datebuf1[64+5]==0x35) 
		   if(tip_udp_datebuf1[64+6]==0x43) 
		   if(tip_udp_datebuf1[64+7]==0x36) 
		   {

				for(i=0;i<72;i++)
			    {
//				wifi_name_key[i]=tip_udp_datebuf1[i];
//				USART1->DR=tip_udp_datebuf1[i];
//				while((USART1->SR&0X40)==0);//等待发送结束
		    	}
				STMFLASH_Write(FLASH_SAVE_ADDR+256,(u16*)(tip_udp_datebuf1+4),64);
				memcpy(wifi_name,tip_udp_datebuf1+4,32);
				memcpy(wifi_key,tip_udp_datebuf1+32+4,32);

			   uip_send((char *)tmp_dat_1,strlen(tmp_dat_1));
		   }
		   	
	       if(tip_udp_datebuf1[0]==0x41)	   //读参数配置
		   if(tip_udp_datebuf1[1]==0x35)
		   if(tip_udp_datebuf1[2]==0x45)
		   if(tip_udp_datebuf1[3]==0x36)
		   if(tip_udp_datebuf1[4]==0x45)
		   if(tip_udp_datebuf1[5]==0x37) 
		   if(tip_udp_datebuf1[6]==0x35) 
		   if(tip_udp_datebuf1[7]==0x41) 
		   {
		           tcp_usart_time=0;
//	   								for(i=0;i<strlen(tip_udp_senddatebuf);i++)
//								    {
//									USART1->DR=tip_udp_senddatebuf[i];
//									while((USART1->SR&0X40)==0);//等待发送结束
//							    	}

		   		   uip_send((char *)tip_udp_senddatebuf,strlen(tip_udp_senddatebuf));
		   }
	       if(tip_udp_datebuf1[0]==0x41)	   //读wifi参数配置
		   if(tip_udp_datebuf1[1]==0x35)
		   if(tip_udp_datebuf1[2]==0x45)
		   if(tip_udp_datebuf1[3]==0x35)
		   if(tip_udp_datebuf1[4]==0x45)
		   if(tip_udp_datebuf1[5]==0x37) 
		   if(tip_udp_datebuf1[6]==0x35) 
		   if(tip_udp_datebuf1[7]==0x41) 
		   {
		           tcp_usart_time=0;
				   wifi_name_key[0]=0x44;
				   wifi_name_key[1]=0x31;
				   wifi_name_key[2]=0x43;
				   wifi_name_key[3]=0x35;
				   wifi_name_key[68]=0x45;
				   wifi_name_key[69]=0x37;
				   wifi_name_key[70]=0x35;
				   wifi_name_key[71]=0x42;
				   memcpy(wifi_name_key+4,wifi_name,32);
				   memcpy(wifi_name_key+32+4,wifi_key,32);
		   		   uip_send((char *)wifi_name_key,72);
		   }
		   if(tip_udp_datebuf1[0]==0x43) //C5E6E89A	   搜索包
		   if(tip_udp_datebuf1[1]==0x35)
		   if(tip_udp_datebuf1[2]==0x45)
		   if(tip_udp_datebuf1[3]==0x36)
		   if(tip_udp_datebuf1[16]==0x45)
		   if(tip_udp_datebuf1[17]==0x38) 
		   if(tip_udp_datebuf1[18]==0x39) 
		   if(tip_udp_datebuf1[19]==0x41) 
		   {	 
				tcp_usart_time=0;
//				udp_m=1;
//				tcp_udp_ip[0]=hex[(tip_udp_datebuf1[4]-0x30)]*100+hex[(tip_udp_datebuf1[5]-0x30)]*10+hex[(tip_udp_datebuf1[6]-0x30)];
//	            tcp_udp_ip[1]=hex[(tip_udp_datebuf1[7]-0x30)]*100+hex[(tip_udp_datebuf1[8]-0x30)]*10+hex[(tip_udp_datebuf1[9]-0x30)];
//	            tcp_udp_ip[2]=hex[(tip_udp_datebuf1[10]-0x30)]*100+hex[(tip_udp_datebuf1[11]-0x30)]*10+hex[(tip_udp_datebuf1[12]-0x30)];
//	            tcp_udp_ip[3]=hex[(tip_udp_datebuf1[13]-0x30)]*100+hex[(tip_udp_datebuf1[14]-0x30)]*10+hex[(tip_udp_datebuf1[15]-0x30)];
//				udp_p++;
//				if(udp_p==2)
//				{
				udp_p=0;
				tcp_dns_time=0;
				uip_send((char *)tmp_dat1,21);	 //搜索包返回
//				}

		   }
		    for(i=0;i<200;i++)
			   {
			   	 tip_udp_datebuf1[i]=0x00;
			   }
	}

}





void MY_UDP_APP(void)
{

	if((uip_udp_conn->lport)==(HTONS(6677)))
	{
	  UDP_6677_APP();
	}
	switch (uip_udp_conn->rport)
	{
    case HTONS(6677):       	
		UDP_6677_APP();
		break;
	case HTONS(67):
		dhcpc_appcall();
		break;
	case HTONS(68):
		dhcpc_appcall();
		break;
	case HTONS(53):
		resolv_appcall();
	}
	
}























