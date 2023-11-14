#ifndef __TCP_DEMO_H__
#define __TCP_DEMO_H__		 
/* Since this file will be included by uip.h, we cannot include uip.h
   here. But we might need to include uipopt.h if we need the u8_t and
   u16_t datatypes. */
#include "uipopt.h"
#include "psock.h"
#include "sys.h"
 #include "usart.h"
//通信程序状态字(用户可以自己定义)  
enum
{
	STATE_CMD		= 0,	//命令接收状态 
	STATE_TX_TEST	= 1,	//连续发送数据包状态(速度测试)  
	STATE_RX_TEST	= 2		//连续接收数据包状态(速度测试)  
};	 
//定义 uip_tcp_appstate_t 数据类型，用户可以添加应用程序需要用到
//成员变量。不要更改结构体类型的名字，因为这个类型名会被uip引用。
//uip.h 中定义的 	struct uip_conn  结构体中引用了 uip_tcp_appstate_t		  
struct tcp_demo_appstate
{
	u8_t state;
	u8_t *textptr;
	int textlen;
};	 
//typedef struct tcp_demo_appstate uip_tcp_appstate_t;

void tcp_demo_appcall(void);
void tcp_client_demo_appcall(void);
void tcp_server_demo_appcall(void);
void MY_UDP_APP(void) ;
//定义应用程序回调函数 
#ifndef UIP_APPCALL
#define UIP_APPCALL tcp_demo_appcall //定义回调函数为 tcp_demo_appcall 
#endif

#ifndef UIP_UDP_APPCALL
    #define UIP_UDP_APPCALL        MY_UDP_APP  
#endif

#define FLASH_SAVE_ADDR  0X0800F000 				//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)			 				   



/////////////////////////////////////TCP CLIENT/////////////////////////////////////
extern u8 wifi_name_key[128];
extern u8  tcp_conn_close;

extern u8   tcp_close_ok;
extern u8	tcp_client_ip[4] ;					//作为客户端要连接的服务端IP地址
extern u32	tcp_client_port;					//作为客户端发送数据端口

extern u32 tcp_client_reclen;   	//发送数据缓存
extern u8 tcp_ack_ok;
extern u32 tcp_client_len;
extern u8 tcp_client_databuf[];   		//发送数据缓存	 
extern u8 tcp_client_sta;				//客户端状态 
u8 tcp_client_reconnect(u32 port,u8 *buf);
//void tcp_client_reconnect(void);
void tcp_client_connected(void);
void tcp_client_aborted(void);
void tcp_client_timedout(void);
void tcp_client_closed(void);
void tcp_client_acked(void);
void tcp_client_senddata(void);
//void tcp_client_close(void);
//void tcp_client_uip_abort(void);
////////////////////////////////////////////////////////////////////////////////////
extern u8 tcp_client_recbuf[USART_REC_LEN];   	//发送数据缓存	  
extern u8 tcp_n;

extern  u8 tip_udp_senddatebuf[150];
extern  u8 tip_udp_datelen;
extern  u8 tip_udp_datebuf[USART_REC_LEN];
extern  u8 tip_udp_sata;
extern  u32 tcp_udp_port;
extern  u32 tcp_udp_time;
extern  u32	tcp_usart_time;
extern  u8	hex[10];

extern  u8  tcp_buffer_sta;
extern  u8  udp_p;
extern  u8  udp_m;
extern  u8  tcp_udp_ip[4];
#endif
























