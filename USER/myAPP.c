#include "myAPP.h"
#include "uip.h"
#include "string.h"
#include "dhcpc.h"

void UDP_6677_APP(void);


void MY_MAIN_APP(void)
{
	
}

void MY_UDP_APP(void)
{
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

void UDP_6677_APP(void)
{
	/* 判断当前状态 */
	if (uip_poll())
	{
		char *tmp_dat = "the auto send!\r\n";
		uip_send((char *)tmp_dat,strlen(tmp_dat));
	}
	if (uip_newdata())
	{
		char *tmp_dat = "receive the data!\r\n";
		/* 收到新数据 */
		printf("%d",uip_len);
		printf("%s",(char *)uip_appdata);
		uip_send((char *)tmp_dat,strlen(tmp_dat));
	}

}


