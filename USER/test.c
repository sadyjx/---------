#include "sys.h"
#include "usart.h"		
#include "delay.h"
//#include "cont.h"	
#include "led.h" 
//#include "beep.h"	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timerx.h"		 	 
//#include "tpad.h"
#include "oled.h"		 	 
#include "lcd.h"
//#include "usmart.h"	
//#include "rtc.h"	 	 
//#include "wkup.h"	
#include "adc.h" 	 
//#include "dac.h" 	 
#include "usart2.h"
//#include "enc28j60.h"
//#include "uip.h"
//#include "uip_arp.h"
//#include "tapdev.h"
//#include "timer.h"				   
//#include "math.h" 	
//#include "string.h"	 
//#include "tcp_demo.h"
//#include "dhcpc.h"
//#include "resolv.h"	
#include "stmflash.h"
#include "zigbee.h"
#include "rs485.h"	 

u8 SYS_VER[]={"V20   200814"};

u8 key_led_on_time_int=0;	 //	 ����LED������ʱ�����
u8 key_led_on_time_lan=60;	 //	 ����LED������ʱ�䳤�� 30��


u8 F0=0;// 0�¶� >= 0��  1 �¶� <  0��
u8 DpyNum[6] = {2,6,5,0,0,0}; //��ʾ������  
u8 adrr=1;
u8 df_stats=0;  //���״̬
u8 df_stats_s=3;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
u8 df_stats_f=1;  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
u16 set_t_num=260;
u8 open_on=1;     //0 ���� 1 �ػ�

u8 key_data=0;
u8 key_data_5s=0;
//u16 sys_mode_time=0; //
//u8 sys_mode_sta=0x00; // 0x00������ 0x10���ý��� 0x20�Զ����ƽ���  

u8 set_wd_sta=0; // �����¶�  
u8 set_wd_display_time=0; // �����¶�  
u8 set_c_f=0;     // 0 ���϶�  1 ���϶�
u16 wd_display=0;
void get_c_f(u8 cmd);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯


u8 set_wd_upper_limit=30; // �����¶�����  
u8 set_wd_lower_limit=16; // �����¶�����  
u8 set_wd_upper_limit_f=86; // �����¶�����  
u8 set_wd_lower_limit_f=60; // �����¶����� 

u8  timing_on=0;		   //  ��ʱ����ģʽ	0�ر� 1����
//u16 timing_on_int=200;	   //  ��ʱʱ��
//u16 set_timing_on_int=0;   //  �趨��ʱʱ��
u8  timing_min=0;		   //
u8  timing_hour=2;		   //
u8  set_timing_min=0;		   //
u8  set_timing_hour=0;		   //

u8  timing_sta=0;		   //  1 ����ʱ���趨ģʽ	 
u8  timing_sta_int=0;	   //  ����ʱ���趨ģʽʱ���ʱ �޲���10���˳�	 
u16 timing_lcd_dis=0;	   //  ����ʱ���趨ģʽʱ����˸

u8   zigbee_net=0x00;	   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
//u8   set_up_net=0;	       //  1 ��������״̬
u8   set_up_net_time=0;	   //  ���뷢������״̬��ʱ
u8   zigbee_net_sta=0x00;	//  zigbee�豸״̬  0x01���豸����״̬��	 0x02���豸�����Բ��ԣ�
u8   zigbee_net_time=0x00;	//  �豸����״̬��	 0x02���豸�����Բ��ԣ�
u8   zigbee_rssi=0x00;	    //  �豸�����ź�ǿ��
u8   zigbee_rssi_xd=11;	    //  �豸�����ŵ�
u16  zigbee_beattim=0;      //  zigbee����ʱ�䣬�ϴ����е�״̬
u8   sys_zigbee_use=0;	    //  1 ʹ��ZigBee


#define SW_L    PBout(14)// 
#define SW_M    PAout(9)// 
#define SW_H    PAout(15)// 
#define SW_HOT  PBout(13)// 
#define SW_COOL PAout(10)// 


u8  B_SW_L=0;
u8  B_SW_M=0;
u8  B_SW_H=0;
u8  B_SW_HOT=0;
u8  B_SW_COOL=0;

u16 sw_time_int=0; //�̵�������ʱ��
u8 sw_bit=0;      //�̵���������־
u8 df_stats_s_crc=0;  //���ٻ���


u8   TX_buf[]={0x02,0x06,0x01,0x00,0x02,0x06,0x01,0x00};
u8   room_card=1;   //  1 �п�
u8   wd_bc_sta=0;   //  0 ����Ҫ���� 1������ 2������
u8   wd_bc_sta_data=0;   //   �����¶�

u8   adrr_set=0;        //  1 �����ַ����ģʽ
u8   adrr_set_data=0;   //  1 �����ַ����ģʽ

void sys_wr()	//д����
{
//    u8 i,j;
	u8 buf[128];        
//	u16 n=0; 
	buf[0]=adrr;
	buf[1]=wd_bc_sta;        //  0 ����Ҫ���� 1������ 2������
	buf[2]=wd_bc_sta_data;   //   �����¶�
	buf[3]=key_led_on_time_lan/256;
	buf[4]=key_led_on_time_lan%256;
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)buf,24);
}
void sys_re()	//������
{
//    u8 i,j;
	u8 buf[128];        
//	u16 n=0; 
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)buf,24);
	if(buf[0]!=0xff)
	{
		adrr=buf[0];
		wd_bc_sta=buf[1];   //  0 ����Ҫ���� 1������ 2������
		wd_bc_sta_data=buf[2];   //   �����¶�
		if(wd_bc_sta_data>9) wd_bc_sta_data=0;
	    key_led_on_time_lan=buf[3]*256+buf[4];

		if(key_led_on_time_lan==0) key_led_on_time_lan=60;
	}
	else
	{
	   sys_wr();	//д����
	}
}

void display( u8 cmd )  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF  4 ON
{

//	static u8 DpyNum1[6] = {0,0,0,0,0,0}; //��ʾ������  
	static u16 wd_display1=0;
	static u8 df_stats_s1=0xff;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
	static u8 df_stats_f1=0xff;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	static u16 set_t_num1=0xff;
	static u8 cmd1=0xff;
	u16 set_t_display=260;
    u8 i;
	u8 x=0,y=0;
//	u8 dis=0;

    if(adrr_set)
	{
		   x=40;y=40;	i=24;
		   OLED_ShowString(16,0,   "  ADDR SET  ");
		   OLED_ShowChar1_48T(x,y+i*0,adrr_set_data/10,16,1); //����	
	       OLED_ShowChar1_48T(x,y+i*1,adrr_set_data%10,16,1); //����	
	}
	else
	if(zigbee_net_sta)	 //����ZigBee�������߹����Բ���ģʽ
	{
		switch (zigbee_net_sta)
		{
			case 0x01:	 //	ZigBee����
		               OLED_ShowString(16,0,   "   ZigBee   ");
					   OLED_ShowString(16+16,0," connecting ");
					   x=56;y=40;	i=24;
					   OLED_ShowChar1_48T(x,y+i*0,zigbee_net_time/10,16,1); //����	
	                   OLED_ShowChar1_48T(x,y+i*1,zigbee_net_time%10,16,1); //����	

				break;
			case 0x02:	 //	ZigBee�����Բ���
		               OLED_ShowString(16,0,   "   ZigBee   ");

					   OLED_ShowChar(16+16,8*9,zigbee_rssi_xd/10+0x30);
					   OLED_ShowChar(16+16,8*10,zigbee_rssi_xd%10+0x30);
					   OLED_ShowString(16+16,0," rssi ");

					   x=56;y=40;	i=24;
					   OLED_ShowChar1_48T(x,y+i*0,zigbee_rssi/10,16,1); //����	
	                   OLED_ShowChar1_48T(x,y+i*1,zigbee_rssi%10,16,1); //����	

				break;
			default:
				break;
		}

	}
	else
	switch (cmd)
	{
		case 0:	//  0 ��ʾ��ַ 
				   OLED_ShowString(16,0,SYS_VER);
				   x=40;y=40;	i=24;
				   OLED_ShowChar1_48T(x,y+i*0,adrr/10,16,1); //����	
                   OLED_ShowChar1_48T(x,y+i*1,adrr%10,16,1); //����	
//				   OLED_Refresh_Gram();//������ʾ
			break;
		case 1:	//  1 ��ʾ�¶Ⱥ͵�λ 

re:
			x=16;y=16;	i=24;
	
//			if(df_stats_f1!=df_stats_f)
			{
				switch (df_stats_f)	// ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
				{
					case 0:
					        OLED_ShowChar1_24T(x-0,y+i*0,7,16,1); //ͨ��ͼ��	
							OLED_ShowChar1_24T(x-3,y+i*1,8,16,1); //FAN	
						break;
					case 1:
					        OLED_ShowChar1_24T(x-0,y+i*0,0,16,1); //����ͼ��	
							OLED_ShowChar1_24T(x-3,y+i*1,1,16,1); //cool	
						break;
					case 2:
					        OLED_ShowChar1_24T(x-0,y+i*0,5,16,1); //�ȷ�ͼ��	
							OLED_ShowChar1_24T(x-3,y+i*1,6,16,1); //HOT	
						break;
					default:
						break;
				}
			}
//			if(df_stats_s1!=df_stats_s)
			{
				if(df_stats_s&0x04)	  //auto
				{
					   OLED_ShowFGJ_24T(x-4,y+i*2,4,16,1); //AT
				}
				else   OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //

				switch (df_stats_s&0x03)	   //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
				{
					case 0:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  0,16,1);   //��ܻ�0��
						break;
					case 1:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  1,16,1);   //��ܻ�1��
						break;
					case 2:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  2,16,1);   //��ܻ�2��
						break;
					case 3:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  3,16,1);   //��ܻ�3��
						break;
					case 4:
							OLED_ShowFGJ_24T(x-4,y+i*2,4,16,1); //AT
							OLED_ShowFGJ_24T(x,y+i*3,3,16,1);   //��ܻ�3��
						break;

					default:
						break;
				}
			
			}
			
			x=40;y=40;	i=24;
			get_c_f(0);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
//			if(wd_display1!=wd_display)   
			{
				OLED_ShowChar1_24T(x+24+1,16,2,16,1); //	INͼ��
				if(wd_display>999) wd_display=wd_display/10;
		        OLED_ShowChar1_48T(x,y+i*0,wd_display/100,16,1); //����	
		        OLED_ShowChar1_48T(x,y+i*1,wd_display%100/10,16,1); //����	
				
				if(set_c_f)	// 0 ���϶�  1 ���϶�
				OLED_ShowChar1_24T(x,y+i*2,4,16,1); //	��F
				else
				OLED_ShowChar1_24T(x,y+i*2,3,16,1); //	��C
				
				if(wd_display>999) 
				OLED_ShowChar1(x+31,y+i*2-13,16,16,1); //.
				else
				OLED_ShowChar1(x+31,y+i*2-13,10,16,1); //.

		        OLED_ShowSHUJ_24T(x+24,y+i*2,wd_display%10,16,1); //24*12����
			}
	
			x=94;y=8;	i=8;
	
//			if(set_t_num1!=set_t_num)
			{
				OLED_ShowChar1(x,y+i*0,11,16,1); // 2
				OLED_ShowChar1(x,y+i*1,12,16,1); // 2
				OLED_ShowChar1(x,y+i*2,13,16,1); // 2
		
				OLED_Fill(94,6,94,33,1);
				OLED_Fill(110,6,110,33,1);
				 
				OLED_Fill(95,5,109,5,1);
				OLED_Fill(95,34,109,34,1);
			    x=95;y=48;	i=8;
				OLED_ShowChar1(x,y+i*0,  set_t_num/100,16,1); // 2
				OLED_ShowChar1(x,y+i*1+2,set_t_num%100/10,16,1); // 6
				OLED_ShowChar1(x,y+i*2+2,10,16,1); // .
				OLED_ShowChar1(x,y+i*3+2,set_t_num%10,16,1); // 5

				if(set_c_f)	// 0 ���϶�  1 ���϶�
				OLED_ShowChar1(x-4,y+i*4+6,15,16,1); // ��F
				else
				OLED_ShowChar1(x-4,y+i*4+6,14,16,1); // ��c
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//������ʾ

			wd_display1=wd_display;
			df_stats_s1=df_stats_s;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
			df_stats_f1=df_stats_f;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
			set_t_num1=set_t_num;


			break;
		case 2:	//   2�趨�¶�
			
	
			if(set_t_num1!=set_t_num);
			{
				x=40;y=40;	i=24;
				OLED_ShowChar1_24T(x+24+1,16,9,16,1); //	INͼ��

				set_t_display=set_t_num;
				if(set_t_num>999) set_t_display=set_t_num/10;

		        OLED_ShowChar1_48T(x,y+i*0,set_t_display/100,16,1); //����	
		        OLED_ShowChar1_48T(x,y+i*1,set_t_display%100/10,16,1); //����
				
				if(set_c_f)	// 0 ���϶�  1 ���϶�
				OLED_ShowChar1_24T(x,y+i*2,4,16,1); //	��F
				else	
				OLED_ShowChar1_24T(x,y+i*2,3,16,1); //	��C
			
				if(set_t_num>999)
				OLED_ShowChar1(x+31,y+i*2-13,16,16,1); //.
				else
				OLED_ShowChar1(x+31,y+i*2-13,10,16,1); //.

		        OLED_ShowSHUJ_24T(x+24,y+i*2,set_t_display%10,16,1); //24*12����

				x=94;y=8;	i=8;
				OLED_ShowChar1(x,y+i*0,11,16,1); // 2
				OLED_ShowChar1(x,y+i*1,12,16,1); // 2
				OLED_ShowChar1(x,y+i*2,13,16,1); // 2
		
				OLED_Fill(94,6,94,33,1);
				OLED_Fill(110,6,110,33,1);
				 
				OLED_Fill(95,5,109,5,1);
				OLED_Fill(95,34,109,34,1);
			    x=95;y=48;	i=8;
				OLED_ShowChar1(x,y+i*0,  set_t_display/100,16,1); // 2
				OLED_ShowChar1(x,y+i*1+2,set_t_display%100/10,16,1); // 6
				
				if(set_t_num>999)
				OLED_ShowChar1(x,y+i*2+2,16,16,1); // .
				else
				OLED_ShowChar1(x,y+i*2+2,10,16,1); // .

				OLED_ShowChar1(x,y+i*3+2,set_t_display%10,16,1); // 5

				if(set_c_f)	// 0 ���϶�  1 ���϶�
				OLED_ShowChar1(x-4,y+i*4+6,15,16,1); // ��F
				else
				OLED_ShowChar1(x-4,y+i*4+6,14,16,1); // ��c
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//������ʾ
			wd_display1=set_t_num;
//			DpyNum1[0] =set_t_num/100 ; //��ʾ������  
//			DpyNum1[1] =set_t_num%100/10 ; //��ʾ������  
//			DpyNum1[2] =set_t_num%10 ; //��ʾ������  
			set_t_num1=set_t_num;

			break;
		case 3:	//   3 OFF
			if(cmd1!=cmd)
			{
				OLED_Clear(); 
				x=40;y=30;	i=24;
		        OLED_ShowChar1_48T(x,y+i*0,0,16,1); //����	
		        OLED_ShowChar1_48T(x,y+i*1,10,16,1); //����
				OLED_ShowChar1_48T(x,y+i*2,10,16,1); //����	
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//������ʾ

			break;
		case 4:	//   ��ʾ�¶Ⱥ͵�λ 

			wd_display1=0;
			df_stats_s1=0;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
			df_stats_f1=0;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
			set_t_num1=0;
			goto re;

//			break;

		default:
			break;
	}
	OLED_Refresh_Gram();//������ʾ
	cmd1=cmd;

}



void get_wd(u8 cmd)  //  �õ�ntc�¶�
{
	u16 ADCValue = 0;
	u8 temp = 0;       //����
    static u16 sum=0;
    static u8  nn=0;
//	u8 buf[6];
	
//	ADCValue = Get_Adc(0)+16;         //��ȡADת��ֵ	
	ADCValue =Get_Adc_Average(0,100)+14;         //��ȡADת��ֵ	
	ADCValue=(ADCValue/((float)(4096-ADCValue)/10000))/10;
//	buf[0]=ADCValue/1000+0x30;
//	buf[1]=ADCValue%1000/100+0x30;
//	buf[2]=ADCValue%1000%100/10+0x30;
//	buf[3]=ADCValue%10+0x30;
//	buf[4]=0x0d;
//	buf[5]=0x0a;
//
//	RS485_Send_Data(buf,6);

	ADCValue=get_temperature(ADCValue); //�����¶�ֵ 		   
	ADCValue=500-ADCValue;
	if(sum==ADCValue) nn++;
	else nn=0; 
	sum=ADCValue;
	
	if((nn>5)||cmd)              
	{ 
		nn=0;  
	       
		if(wd_bc_sta==1)//  0 ����Ҫ���� 1������ 2������ 
		ADCValue=ADCValue+(wd_bc_sta_data*10);
		else
		if(wd_bc_sta==2)//  0 ����Ҫ���� 1������ 2������ 
		ADCValue=ADCValue-(wd_bc_sta_data*10);

		temp = ADCValue%1000/100;
		DpyNum[0]=temp;//��ʾ��λ						
		temp = (ADCValue%100)/10;
		DpyNum[1]=temp;//��ʾʮλ				
		temp = (ADCValue%100)%10;
		DpyNum[2]=temp;//��ʾ��λ 
		get_c_f(0);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
	}



}


void get_c_f(u8 cmd)  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
{
   	u16 adcx;
	float temp;  
	static u16  wd=0;

	if(cmd)
	{
		if(set_c_f)	// 0 ���϶�  1 ���϶�
		{
			 if(set_t_num<400)
			 {
	 		  adcx=set_t_num;
			  temp=(float)adcx/10;
	          temp=(32+temp*1.8);
			  set_t_num=temp*10;
			 }
		}
		else
		{
			 if(set_t_num>400)
			 {
//				printf("set_t_num F =%d\r\n",set_t_num);
				temp=(float)set_t_num/10;
				temp=(float)((temp-32)*5/9);
//				printf("set_t_num C0=%f\r\n",temp);
			 	set_t_num=temp*10;
//			 	printf("set_t_num C1=%d\r\n",set_t_num);
			 }

		}
		
	}
	else
	{
		if(set_c_f)	// 0 ���϶�  1 ���϶�
		{
			  adcx=DpyNum[0]*100+DpyNum[1]*10+DpyNum[2];
			  temp=(float)adcx/10;
//			  printf("temp0=%04f\r\n",temp);
	          temp=(32+temp*1.8);
//			  printf("temp1=%f\r\n",temp);
			  wd_display=temp*10;
		}
		else
		{
			  wd_display=DpyNum[0]*100+DpyNum[1]*10+DpyNum[2];
		}
		
	}

	if(wd!=wd_display)
	{
      one_data_update(2);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
	  wd=wd_display;
	}

}



u16 get_c_f_data1(u16 cmd_data,u8 cmd)  // cmd_data �¶���ֵ 0���϶�ת���϶� 1 ���϶�ת���϶�  
{
//   	u16 adcx;
	float temp; 
	switch (cmd)
	{
		case 0:	//   0���϶�ת���϶�
				if(cmd_data>400)
				if(set_c_f)	// 0 ���϶�  1 ���϶�
				{
					temp=(float)cmd_data/10;
					temp=(float)((temp-32)*5/9);
				 	cmd_data=temp*10;
				}
			break;
		case 1:	//1 ���϶�ת���϶� 
				if(cmd_data<400)
				if(set_c_f)	// 0 ���϶�  1 ���϶�
				{
					  temp=(float)cmd_data/10;
			          temp=(32+temp*1.8);
					  cmd_data=temp*10;
				}
			break;
		default:
			break;
	}
	return cmd_data;
}

u8 cmd_crc=0xff;
void PWM_RGB_INT(u8 mode,u8 cmd)  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
{
//    static u16 sum=0;
    

	if(cmd_crc!=cmd)
	{
		if(mode==0)
		{
//				TIM3->CCER|=1<<4;   	//OC2 ���ʹ��	 LEDR_PWM_VAL TIM3->CCR2  
				TIM3->CCER&=~(1<<4);   	//OC2 ���ʹ��	 LEDR_PWM_VAL TIM3->CCR2 
//				TIM3->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDB_PWM_VAL TIM3->CCR1  
				TIM3->CCER&=~(1<<0);   	//OC1 ���ʹ��	 LEDB_PWM_VAL TIM3->CCR1  
//	            TIM4->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDG_PWM_VAL  TIM4->CCR1  
	            TIM4->CCER&=~(1<<0);   	//OC1 ���ʹ��	 LEDG_PWM_VAL  TIM4->CCR1  

		  switch (cmd)
		  {
		  	case 0:	   // 0 ȫ���� 

				break;
			case 1:	   // 1���� 

				TIM3->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDB_PWM_VAL TIM3->CCR1  
				LEDB_PWM_VAL=1000;		//��ֵԽС������ԽС 
				break;
			case 2:	   // 2����

				TIM3->CCER|=1<<4;   	//OC2 ���ʹ��	 LEDR_PWM_VAL TIM3->CCR2  
	            TIM4->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDG_PWM_VAL  TIM4->CCR1  
				LEDR_PWM_VAL=5000;		//��ֵԽС������Խ��
				LEDG_PWM_VAL=1000;
				break;
			case 3:	   // 3�ͷ�
				  
	            TIM4->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDG_PWM_VAL  TIM4->CCR1  
				LEDG_PWM_VAL=1000;		//��ֵԽС������ԽС
				break;
			case 8:	   // ��������ģʽ
//				TIM3->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDB_PWM_VAL TIM3->CCR1  
//				LEDB_PWM_VAL=4000;
//
//				TIM3->CCER|=1<<4;   	//OC2 ���ʹ��	 LEDR_PWM_VAL TIM3->CCR2  
//	            TIM4->CCER|=1<<0;   	//OC1 ���ʹ��	 LEDG_PWM_VAL  TIM4->CCR1  
//				LEDR_PWM_VAL=400;
//				LEDG_PWM_VAL=400;

				break;

			default:

				break;
		  }
		}

		cmd_crc=cmd;
	}
}




void get_download_handle(u8 cmd)  //�ж��Ƿ������ݱ仯
{	 
	static u8 df_stats_s1=0xff;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
	static u8 df_stats_f1=0xff;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	static u16 set_t_num1=0xff;
	static u8 open_on1=0xff;
	static u8  set_c_f1=0XFF;
	u8 dis=0;

    if(download_handle_sta||cmd)
	{
	  if(open_on1!=open_on)
	  {
	  	 if(open_on==0)
		 {
			OLED_Clear(); 
		 }		  
		 else 	PWM_RGB_INT(0,0);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
		 dis=1;
	  }

	  if(df_stats_s1!=df_stats_s)dis=1;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
	  if(df_stats_f1!=df_stats_f)dis=1;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	  if(set_t_num1!=set_t_num)  dis=1;
	  if(set_c_f1!=set_c_f)
	  {	
		 get_c_f(1);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
	  	 dis=1;
	  }
      download_handle_sta=0;   //�в�����Ϣ
	}

	df_stats_s1=df_stats_s;  //����	 ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
	df_stats_f1=df_stats_f;  //����	 ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	set_t_num1=set_t_num;
	open_on1=open_on;
	set_c_f1=set_c_f;

	if(dis)
	{
		 		  LED0=1;					 //  ����LED��
				  if(open_on==0)   //�Ѿ�����
				  {
					  OLED_Display_On();LCD_PWR=0;	//��lcd����
					  if(zigbee_net_sta)   //  1 ��������״̬
					  {
						 PWM_RGB_INT(0,8);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�			  
					  }
					  else
					  if(df_stats_f==0)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
					  {
						 PWM_RGB_INT(0,3);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  }
					  else
					  if(df_stats_f==1)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
					  {
						 PWM_RGB_INT(0,1);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  }
					  else
					  if(df_stats_f==2)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
					  {
						 PWM_RGB_INT(0,2);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  }
					  display(4);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
				  }
				  else
				  {
				  	OLED_Display_On(); LCD_PWR=0;	//��lcd����
					display(3);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
				  }
				key_led_on_time_int=5;	 //	 ����LED������ʱ�����
	}

}





void sys_sw_init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
   	 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X00000330;	//PA9 10 ���� 	    
	GPIOA->ODR|=3<<9;     	//PA9 10 ����	�����

	GPIOA->CRH&=0X0FFFFFFF; 
	GPIOA->CRH|=0X30000000;	//PA15 ���� 	    
	GPIOA->ODR|=1<<15;     	//PA15 ����	�����
	GPIOB->CRH&=0XF00FFFFF; 
	GPIOB->CRH|=0X03300000;	//PB13 14  ���� 	    
	GPIOB->ODR|=3<<13;     	//PB13 14  ����	�����

	SW_L=0; 
	SW_M=0; 
	SW_H=0; 
	SW_HOT=0; 
	SW_COOL=0;  
}
void sw_init(void)
{
    u16 d_t_num=0;
    d_t_num=wd_display;
/*

?	��4λ��3~0��������
?	0x00  ? ͣ
?	0x01  ? ��
?	0x02  ? ��
?	0x03  ? ��
?	������Ч
?	��5~4��λ������״̬
?	0x00  ? ͨ��
?	0x01  ? �䷧
?	0x02  ? �ȷ�

*/
	if(zigbee_net_sta)   //  1 ��������״̬
    {
			B_SW_L=0;
			B_SW_M=0;
			B_SW_H=0;
			
			B_SW_COOL=0;
			B_SW_HOT=0;
           goto back;
    }
    if(open_on==1) 
    {
			B_SW_L=0;
			B_SW_M=0;
			B_SW_H=0;
			
			B_SW_COOL=0;
			B_SW_HOT=0;
           goto back;
    }



     if((df_stats_s_crc!=df_stats_s)||sw_time_int)
     {   
        if((df_stats_s_crc!=df_stats_s)) sw_time_int=0;

        if(sw_time_int==0)
        {
			B_SW_L=0;
			B_SW_M=0;
			B_SW_H=0;
            sw_bit=1;
        }
        sw_time_int++;
        if(sw_time_int>2)
        {
            sw_time_int=0;
            sw_bit=0;
			switch (df_stats_s&0x03)
			{
			 	case 1:  //��
						B_SW_L=1;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 2:  //��
						B_SW_L=0;
						B_SW_M=1;
						B_SW_H=0;				
					break;
			 	case 3:  //��
						B_SW_L=0;
						B_SW_M=0;
						B_SW_H=1;					
					break;	
				default:
			
					break;
			}

        }

         df_stats_s_crc=df_stats_s;
         goto back;
     }

	  if(sw_bit==0)
	  {


			switch (df_stats_f)
			{
			 	case 0:  //ͨ�� 
                     
					   B_SW_HOT=0;
					   B_SW_COOL=0;
                       goto re;
            			
//					break;
			 	case 1:  //�䷧
					   B_SW_HOT=0;
					   B_SW_COOL=1;
				       if(df_stats_s&0x04) //AUTO 
				       {
                           	if(((set_t_num*1)-5)>d_t_num)  //��ǰ�¶�С���趨�¶�0.5�� ֹͣ����
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0; 
								df_stats_s=0x04;                                
                            }

			                if(((set_t_num*1)+40)<d_t_num)  //��ǰ�¶ȴ����趨�¶�4��  ��������
	                        {
                               df_stats_s=3+0x04; 
                               goto re;  
                            }
                            else
			                if(((set_t_num*1)+20)<d_t_num)  //��ǰ�¶ȴ����趨�¶�1��  ��������
	                        {
                               goto re;
                            }
                            else
			                if(((set_t_num*1)+10)<d_t_num)  //��ǰ�¶ȴ����趨�¶�1��  ��������
	                        {
                               if((df_stats_s&0x03)==3)
                               {
                                  df_stats_s=1+0x04;
                               }
                               goto re;
                            }

				       }
				       else 
				       {    
			                if(((set_t_num*1)-5)>d_t_num)
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0;                                
                            }
			                if(((set_t_num*1)+5)<d_t_num)
	                        {
                              goto re;
                            }
                       }            
					break;
			 	case 2:  //�ȷ�  
                     
					   B_SW_HOT=1;
					   B_SW_COOL=0;

				       if(df_stats_s&0x04) //AUTO 
				       {
                           	if(d_t_num>((set_t_num*1)+5))//��ǰ�¶ȴ����趨�¶�0.5�� ֹͣ����
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0; 
								df_stats_s=0x04;                                
                            }

			                if(d_t_num<((set_t_num*1)-40))//��ǰ�¶�С���趨�¶�-4�� ���ټ���
	                        {
                               df_stats_s=3+0x04;  
                               goto re; 
                            }
                            else
			                if(d_t_num<((set_t_num*1)-20))//��ǰ�¶�С���趨�¶�-2�� ���ټ���
	                        {
//                               df_stats_s=3+0x04;  
                               goto re; 
                            }
                            else
			                if(d_t_num<((set_t_num*1)-10))//��ǰ�¶�С���趨�¶�-1�� ���ټ���
	                        {
                               if((df_stats_s&0x03)==3)
                               {
                                  df_stats_s=1+0x04;
                               }
                               goto re;
                            }
                          
				       }
				       else 
				       {    
			                if(d_t_num<((set_t_num*1)-5))	//��ǰ�¶�С���趨�¶�-0.5�� ��ʼ����
	                        {
                               goto re;
                            }
			                if(d_t_num>((set_t_num*1)+5)) //��ǰ�¶ȴ����趨�¶�+0.5�� ֹͣ����
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0;                               
                            }
                       }            			
					break;
			 	case 3:  //�䷧+�ȷ� 
                     
					   B_SW_HOT=0;
					   B_SW_COOL=0;

                       goto re;
            			
//					break;	
				default:
			
					break;
			}
	     
	  }

goto back;

re:
			switch (df_stats_s&0x03)
			{
			 	case 0:  //��ֹͣ
						B_SW_L=0;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 1:  //��
						B_SW_L=1;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 2:  //��
						B_SW_L=0;
						B_SW_M=1;
						B_SW_H=0;				
					break;
			 	case 3:  //��
						B_SW_L=0;
						B_SW_M=0;
						B_SW_H=1;					
					break;	
				default:
			
					break;
			}
    goto back;
back:

    if((B_SW_L!=SW_L)||(B_SW_M!=SW_M)||(B_SW_H!=SW_H)||(B_SW_HOT!=SW_HOT)||(B_SW_COOL!=SW_COOL))
    {
//        ad_t=0;  
    }
		if(sw_bit==0)
        if((B_SW_L==0)&&(B_SW_M==0)&&(B_SW_H==0))
        {
			B_SW_HOT=0;
			B_SW_COOL=0;            
        }
		SW_L=B_SW_L;
		SW_M=B_SW_M;
		SW_H=B_SW_H;
		SW_HOT=B_SW_HOT;
		SW_COOL=B_SW_COOL;


   ;;;

}


void uart3_485_rx()
{
//	u8 udata1=0;
//	u8 udata2=0;
	u8 crc=0;
    u8 i=0;
//	u8 j=0;
//	u8 n=0;
//	if(RS485_RX_TIME>10)
	{
	     
		if(rc_bit)//���յ�������
		{
		    if(RS485_RX_BUF[0]==0x02)
            if(RS485_RX_BUF[1]==0x06)
			{
               for(i = 0; i < 7; i++)  crc=crc^RS485_RX_BUF[i];
               if(RS485_RX_BUF[7]==crc)  //��λ���У��ֵ
               {
                  if(RS485_RX_BUF[2]==adrr)
                  {
                        for(i = 0; i < 7; i++)  TX_buf[i]=RS485_RX_BUF[i];
                        if(RS485_RX_BUF[3]==0x00)//����ǰ�¿���״̬
                        { 
 //                          if(df_stats_s) open_on=0; //0 ���� 1 �ػ�  
                           df_stats=df_stats_f*16+df_stats_s;
                           if(open_on)     //0 ���� 1 �ػ�
                           TX_buf[4]=df_stats|0x80;  //���״̬
                           else
                           TX_buf[4]=df_stats;  //���״̬

                           TX_buf[5]=get_c_f_data1(set_t_num,0)/10;  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯;//�趨���¶�
                           TX_buf[6]=get_c_f_data1(wd_display,0)/10;//�趨��ǰ���¶�  
	                        crc=0;
	                        for(i = 0; i < 7; i++)  crc=crc^TX_buf[i];
	                        TX_buf[7]=crc; 
	                        RS485_Send_Data(TX_buf, 8); 

                        }
                        else
                        if(RS485_RX_BUF[3]==0x01)//���÷��״̬���¶�
                        {
                           TX_buf[3]=0x00;  //���״̬
                           if(RS485_RX_BUF[4]&0x80) 
						   {
						     open_on=1;     //0 ���� 1 �ػ�
                             df_stats=RS485_RX_BUF[4]&0x7f;  //���״̬                            
					       }
                           else  
                           {              
                           open_on=0;     //0 ���� 1 �ػ�
                           df_stats=RS485_RX_BUF[4];  //���״̬                            
                           } 

						   if((df_stats&0x0f)==0x04)
						   {
                             df_stats_s=df_stats_s|0x04;
						   }
						   else
						   df_stats_s=df_stats&0x0f;

                           df_stats_f=df_stats/16;

                           set_t_num=get_c_f_data1(RS485_RX_BUF[5]*10,1);//�趨���¶�  
						    
                           TX_buf[4]=RS485_RX_BUF[4];  //���״̬
                           TX_buf[5]=RS485_RX_BUF[5];//�趨���¶�
                           TX_buf[6]=get_c_f_data1(wd_display,0)/10;//�趨��ǰ���¶�  
						   get_download_handle(1);
	                        crc=0;
	                        for(i = 0; i < 7; i++)  crc=crc^TX_buf[i];
	                        TX_buf[7]=crc; 
	                        RS485_Send_Data(TX_buf, 8); 

						   if(open_on) 	//0 ���� 1 �ػ�
						   {
							  display( 3 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
						   }
						   else
						   {
						   display( 4 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
                           }
						   sw_time_int=0;
				           sw_bit=0;
						   sw_init(); 

						}
                  }
                  else
                  if(RS485_RX_BUF[2]==0xff)  //�㲥
                  {                       
                        if(RS485_RX_BUF[3]==0x06)//����ǰ�¿����忨ȡ��״̬
                        { 
                           if(RS485_RX_BUF[4]==0x01)//�п�
                           room_card=1;   //  1 �п�
                           else
                           room_card=0;   //  0 �޿�
                        }
                        else
                        if(RS485_RX_BUF[3]==0x15)//�¶Ȳ���
                        {
                            
                           if(RS485_RX_BUF[4]==0x01) //������
						   {
						       wd_bc_sta=1;   //  0 ����Ҫ���� 1������ 2������                        
					       }
                           else  
                           {              
                               wd_bc_sta=2;   //  0 ����Ҫ���� 1������ 2������           
                           } 
                           wd_bc_sta_data=RS485_RX_BUF[5];   //   �����¶�

  						   get_wd(1);  //  �õ�ntc�¶�
						   if(open_on) 	//0 ���� 1 �ػ�
						   {
							  display( 3 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
						   }
						   else
						   {
						   display( 4 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
                           }

//                           i=DpyNum[0]*10+DpyNum[1];
//                           if(wd_bc_sta==1)//  0 ����Ҫ���� 1������ 2������ 
//                           i=i+wd_bc_sta_data;
//                           else
//                           if(wd_bc_sta==2)//  0 ����Ҫ���� 1������ 2������ 
//                           i=i-wd_bc_sta_data;
//                           DpyNum[0]=i/10;
//                           DpyNum[1]=i%10;
//						   display_num (DpyNum[0],DpyNum[1],DpyNum[2]);                   
                        }
                  }
               }
			}
            for(i = 0; i < 7; i++)  RS485_RX_BUF[i]=0;
            RS485_RX_CNT=0;
			rc_bit=0;
	    }
	RS485_RX_TIME=0;
  }
}



u8 display_wd()  
{

    u8 t;
	u8 t1;

    u8 i;
	u8 x=0,y=0;
//	u8 dis=0;

	OLED_Clear();

	x=40;y=40;	i=24;

//	 wd_bc_sta=0;   //  0 ����Ҫ���� 1������ 2������
//     wd_bc_sta_data=0;   //   �����¶�
    re:
	if(wd_bc_sta==1)
    OLED_ShowString(x+16,y-12,"+");
	else
	if(wd_bc_sta==2)
    OLED_ShowString(x+16,y-12,"-");
	else
    OLED_ShowString(x+16,y-12," ");
		   	
	OLED_ShowChar1_48T(x,y+i*1,wd_bc_sta_data,16,1); //����	

	OLED_Refresh_Gram();//������ʾ
	while (1)
	{
	   IWDG_Feed();//ι��
		t=KEY_Scan(1);

		if(t)
		{
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //ģʽ
					  sys_wr();	//д����
					  PWM_RGB_INT(0,2);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  LED0=0;					 //  ����LED��
                      delay_ms(500);
					  LED0=1;					 //  ����LED��
					  return 0;
				break;
			case 2:	  //��λ

				break;
		  	case 3:	  //����
					  return 0;
				break;
			case 4:	  //���϶�/���϶� 
				break;
			case 5:	  //˳ʱ��ת 
					  if(wd_bc_sta==2)
					  {
						  wd_bc_sta_data--;
						  if(wd_bc_sta_data==0) 
						  wd_bc_sta=0;
					  }
					  else
					  {
						  wd_bc_sta_data++;
						  if(wd_bc_sta_data>9) wd_bc_sta_data=9;
					  	  wd_bc_sta=1;
					  }
					  goto re;
//				break;
			case 6:	  //��ʱ��ת 
					  if(wd_bc_sta==1)
					  {
						  wd_bc_sta_data--;
						  if(wd_bc_sta_data==0) 
						  wd_bc_sta=0;
					  }
					  else
					  {
						  wd_bc_sta_data++;
						  if(wd_bc_sta_data>9) wd_bc_sta_data=9;
					  	  wd_bc_sta=2;
					  }
					  goto re;

			default:
				break;
		  }
		  }
		  t1=t;
		}
		else
		{
		  t1=t;
		}
	}
   return 0;
}

u8 oled_open_display_time()  
{

    u8 t;
	u8 t1;

    u8 i;
	u8 x=0,y=0;
	u16 lan=0;

	lan=key_led_on_time_lan;
	OLED_Clear();

	x=40;y=40;	i=24;

    re:
	   x=40;y=28;	i=24;
	   OLED_ShowString(16,4,   " OLED TIME  ");
	   OLED_ShowChar1_48T(x,y+i*0,lan/100,16,1); //����	
	   OLED_ShowChar1_48T(x,y+i*1,lan%100/10,16,1); //����	
	   OLED_ShowChar1_48T(x,y+i*2,lan%10,16,1); //����	

	OLED_Refresh_Gram();//������ʾ
	while (1)
	{
	   IWDG_Feed();//ι��
		t=KEY_Scan(1);

		if(t)
		{
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //ģʽ
			          key_led_on_time_lan=lan;
					  sys_wr();	//д����
					  PWM_RGB_INT(0,2);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  LED0=0;					 //  ����LED��
                      delay_ms(500);
					  LED0=1;					 //  ����LED��
					  return 1;
				break;
			case 2:	  //��λ

				break;
		  	case 3:	  //����
					  return 1;
				break;
			case 4:	  //���϶�/���϶� 
				break;
			case 5:	  //˳ʱ��ת 
						  lan++;
						  if(lan>999) lan=999;
					  goto re;
//				break;
			case 6:	  //��ʱ��ת 
						  if(lan==0)  lan=1;
						  lan--;
					  goto re;

			default:
				break;
		  }
		  }
		  t1=t;
		}
		else
		{
		  t1=t;
		}
	}

   return 0;
}



void uart2_rx()
{

   if(USART2_RX_TIME>20)
   {
	    zigbee_uart_service();
        rx_in = 0;	
		USART2_RX_TIME=0;
		USART2_RX_STA=0;//????,????		
   }

}
int main(void)
{	 
//    u32 i,j;
    u8 t;
	u8 t1;
	u8 key_time=0;
//	u8 x=0,y=0;
//    u8 buf[30];
//	u8 dns_int=0;
	u8 k_on=0;
//	u8 client=0;
//	u16 adcx;
//	float temp;  
	u8 net=0;

	u8 TOUCH_IN_CRC=0;	
#if USE_OLED_TFT	// 1 ʹ��tft
	u8 clock=11;
#else//0ʹ��OLED
	u8 clock=9;
#endif



	Stm32_Clock_Init(clock);	//ϵͳʱ������
	delay_init(clock*8);	   	 	//��ʱ��ʼ�� 
	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();
	sys_sw_init();
//    rs485_send_incidentrecord_int();
//	TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�����100��Ϊ10ms 
//	TIM3_Int_Init(99,719);//10Khz�ļ���Ƶ�ʣ�����100��Ϊ10ms 
//    TIM3_Int_Init(999,71);//1ms  
	Timer2_Init(999,(clock*8-1));
//    uart_init(72,115200);
	zigbee_protocol_init();
	USART2_Init((clock*8)/2,9600);	//��ʼ������2
	RS485_Init((clock*8)/2,4800);
									    
	TIM3_PWM_Init(8999,0);	//R ����Ƶ��PWMƵ��=72000/(899+1)=80Khz
	TIM4_PWM_Init(8999,0);	//G
	PWM_RGB_INT(0,0);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
	EXTIX_Init();
	Adc_Init();

#if USE_OLED_TFT	// 1 ʹ��tft
	lcd_init();
#else//0ʹ��OLED
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
#endif
    sys_re();	//������ 
	WIFI_RST=1;
	display( 0 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
	delay_ms(500);
	WIFI_RST=0;
//	IWDG_Feed();//ι��
	delay_ms(500);

#if USE_OLED_TFT	// 1 ʹ��tft
	lcd_clear(BACK_COLOR);
#else//0ʹ��OLED
	OLED_Clear(); 
#endif


//	display( 1 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
	WIFI_RST=1;
//	printf("RST\r\n");

	OLED_Display_Off();
	LCD_PWR=1;				 //�ر�lcd����
	LED0=0;					 //  ����LED����
	get_wd(1);  //  �õ�ntc�¶�
	IWDG_Init(4,625*2);    	//���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	  

//  zigbee_rst_cmd_handle(0x00);//����zigbeeģ��   0x00����ģ����λ�������ջ���ݣ���������״̬��0x01����ģ������Ϊ��ʼ����״̬��
	   

	while (1)
	{
	   IWDG_Feed();//ι��
	   uart2_rx();
//	   zigbee_uart_service();
	   get_download_handle(0);  //�ж��Ƿ������ݱ仯
	   uart3_485_rx();

		if(room_card)   //  1 �п�
		t=KEY_Scan(1);

		if(t)
		{
		  key_led_on_time_int=0;	 //	 ����LED������ʱ�����
//		  printf("KEY=%d\r\n",t);
		  
		  if((open_on==0)||((t==3)))
		  if(k_on)
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //ģʽ
				   if(adrr_set)
				   {
			          adrr_set=0;   //  1 �����ַ����ģʽ
					  adrr=adrr_set_data;   //  1 �����ַ����ģʽ
					  sys_wr();	//д����
				   }
				   else
				   {
	               df_stats_f++;
	               if(df_stats_f>2) df_stats_f=0;
				   display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
				   one_data_update(3);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
				   one_data_update(7);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
				   }
				break;
			case 3:	  //��λ
                   if(df_stats_s==3) //�Զ���λ
				   {
				   	 df_stats_s=df_stats_s|0x04;
				   }
				   else
				   {
					   df_stats_s++;
					   if(df_stats_s>3) 
					   df_stats_s=1;
				   }
				   display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
				   one_data_update(4);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
				   adrr_set=0;   //  1 �����ַ����ģʽ
				break;
		  	case 2:	  //����
					 if(adrr_set==0) //  1 �����ַ����ģʽ
					 {
					  if(open_on)
					  {
					    OLED_Clear(); 
					    display( 4 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
						OLED_Display_On(); LCD_PWR=0;	//��lcd����
					  	open_on=0;     //0 ���� 1 �ػ�
					 	cmd_crc=0xff;
					  }
					  else
					  {
						zigbee_net_sta=0;	       //  1 ��������״̬
						OLED_Display_Off();LCD_PWR=1;				 //�ر�lcd����
					  	open_on=1;     //0 ���� 1 �ػ�
						PWM_RGB_INT(0,0);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
					  }
					  display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
					  one_data_update(0);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
					 }   
				break;
			case 4:	  //���϶�/���϶� 
					  if(set_c_f) set_c_f=0;	// 0 ���϶�  1 ���϶�
					  else 	      set_c_f=1;	// 0 ���϶�  1 ���϶�
					  get_c_f(1);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯

						//u8 set_wd_upper_limit=30; // �����¶�����  
						//u8 set_wd_lower_limit=16; // �����¶�����  

					  display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
//					  one_data_update(5);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
//					  one_data_update(6);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
					  all_data_update();
					  adrr_set=0;   //  1 �����ַ����ģʽ
				break;
			case 5:	  //˳ʱ��ת 
				   if(adrr_set)
				   {
					  adrr_set_data=adrr_set_data+1;   //  1 �����ַ����ģʽ
				   	  if(adrr_set_data>99) adrr_set_data=99;
					  display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
				   }
				   else
				   {
					  set_t_num=set_t_num+10;
					  if(set_c_f) 	// 0 ���϶�  1 ���϶�
					  {
					  	if(set_t_num>(set_wd_upper_limit_f*10)) set_t_num=(set_wd_upper_limit_f*10);
					  }
					  else
					  {
					    if(set_t_num>(set_wd_upper_limit*10)) set_t_num=(set_wd_upper_limit*10);
					  }
					  set_t_num=(set_t_num/10)*10;
//					  if(set_t_num>(set_wd_upper_limit*10)) set_t_num=set_wd_upper_limit*10;

					  display(2);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
					  set_wd_sta=1; // �����¶�  
					  set_wd_display_time=0; // �����¶�  
					  one_data_update(1);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
				  }
				break;
			case 6:	  //��ʱ��ת 
				   if(adrr_set)
				   {
				   	  if(adrr_set_data==0) adrr_set_data=1;
					  adrr_set_data=adrr_set_data-1;   //  1 �����ַ����ģʽ
					  display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
				   }
				   else
				   {
					  set_t_num=set_t_num-10;
					  if(set_c_f) 	// 0 ���϶�  1 ���϶�
					  {
						if(set_t_num<(set_wd_lower_limit_f*10)) set_t_num=set_wd_lower_limit_f*10;
					  }
					  else
					  if(set_t_num<(set_wd_lower_limit*10)) set_t_num=set_wd_lower_limit*10;

					  set_t_num=(set_t_num/10)*10;
//					  if(set_t_num<(set_wd_lower_limit*10)) set_t_num=set_wd_lower_limit*10;
					  display(2);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
					  set_wd_sta=1; // �����¶�  
					  set_wd_display_time=0; // �����¶�  
					  one_data_update(1);	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
				   }
				break;
			default:
				break;
		  }
		   sw_time_int=0;
           sw_bit=0;
		   sw_init(); 
		  }
		  t1=t;
 		  LED0=1;					 //  ����LED��
		  if(open_on==0)   //�Ѿ�����
		  {
			  OLED_Display_On(); LCD_PWR=0;	//��lcd����

				  if(key_time==3)	//����3S
				  {
				  	   key_time++;
					   if(sys_zigbee_use)	    //  1 ʹ��ZigBee
				  	   if(t==7)		
					   {
	                      zigbee_rst_cmd_handle(0x01);//����zigbeeģ��   0x00����ģ����λ�������ջ���ݣ���������״̬��0x01����ģ������Ϊ��ʼ����״̬��
	                      zigbee_net=0x00;		   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
						  zigbee_net_sta=1;	       //  1 ��������״̬
						  zigbee_net_time=0;	   //  ��������״̬ ��ʱ
						  OLED_Clear(); 
						  display(4);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
					   }

				  }
				  if(key_time==6)	//����3S
				  {
				  	   key_time++;
				  	   if(sys_zigbee_use)	    //  1 ʹ��ZigBee
					   if(t==8)		//����ģʽ+��λ��3�����zigbee
					   {
		                  zigbee_net=0x00;		   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
						  zigbee_rssi_xd=11;	    //  �豸�����ŵ�
						  zigbee_rf_test_cmd_handle(zigbee_rssi_xd);//ZigBeeģ��RF���ܲ���   cmd �ŵ� 11-26 						  
						  zigbee_net_sta=0x02;	//  zigbee�豸״̬  0x01���豸����״̬��	 0x02���豸�����Բ��ԣ�
						  zigbee_net_time=0x00;	//  �豸����״̬��	 0x02���豸�����Բ��ԣ�
					   	  zigbee_rssi=99;	    //  �豸�����ź�ǿ��
	                      zigbee_net=0x00;		   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
						  OLED_Clear(); 
						  display(4);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
//					   	  JTAG_Set(SWD_ENABLE); 
					   }
				  }

			  if(zigbee_net_sta)   //  1 ��������״̬
			  {
				 PWM_RGB_INT(0,8);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�			  
			  }
			  else
			  if(df_stats_f==0)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
			  {
				 PWM_RGB_INT(0,3);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
			  }
			  else
			  if(df_stats_f==1)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
			  {
				 PWM_RGB_INT(0,1);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
			  }
			  else
			  if(df_stats_f==2)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
			  {
				 PWM_RGB_INT(0,2);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
			  }
		  }
		  else
		  {
		  	OLED_Display_On();LCD_PWR=0;	//��lcd����
			display(3);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
				  if(key_time==5)	//����3S
				  {
				  	   key_time++;
					   if(t==2)		//������λ��5������¶�ƫ�����
					   {
						 display_wd();
					   }
					   if(t==4)		//������λ��5����뱳����������
					   {
						 oled_open_display_time();
					   }
					   display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
				       display(3);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
				  }

		  }
		  if(key_time==5)	//����3S
		  {
		  	   key_time++;
			   if(t==9)		//����ģʽ+������5������ַ����
			   {
		          adrr_set=1;   //  1 �����ַ����ģʽ
				  adrr_set_data=adrr;   //  1 �����ַ����ģʽ
				  OLED_Clear(); 
				  display( 4 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
			      OLED_Display_On(); LCD_PWR=0;	//��lcd����
				  open_on=0;     //0 ���� 1 �ػ�
				  cmd_crc=0xff;
			   }
		  }

//		  k_on=1;

//		  if(zigbee_net==0x01)	all_data_update();	   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
		}
		else
		{
		  if(t1)  k_on=1;
		  t1=t;
		  key_time=0;
		}


	   if((xt_time%20)==0)
	   get_wd(0);  //  �õ�ntc�¶�


	   if(xt_time>99)
	   {
		  xt_time=0;
		  key_time++;
		  sw_init();

		  if(sys_zigbee_use)	    //  1 ʹ��ZigBee
		  {
			  if(zigbee_net_sta)  //  1 ��������״̬
			  {
			  	  zigbee_net_time++;	   //  ��������״̬ ��ʱ
			  	  if(zigbee_net_time>99)
				  { 
				    zigbee_net_time=0;
					if(zigbee_net_sta==0x01) 
					{
					  zigbee_net_sta=0x00;
					  OLED_Clear(); 
					}
				  }
					if(zigbee_net_sta==0x02) 
					{
					  if(zigbee_net_time>19)
					  {
					  	zigbee_net_time=0;
						zigbee_rssi_xd++;
						zigbee_rf_test_cmd_handle(zigbee_rssi_xd);//ZigBeeģ��RF���ܲ���   cmd �ŵ� 11-26 						  
					  }
					}
	
				  if(zigbee_rssi_xd==26)
				  {
					  zigbee_net_sta=0x00;
					  while(1); 
				  }
	
				  if(zigbee_net==0x01)
				  {
					OLED_Clear(); 
	//				display( 4 );  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
					zigbee_net_sta=0;	       //  1 ��������״̬
					zigbee_net_time=0;	   //  ��������״̬ ��ʱ
				  	all_data_update();	   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
				  }
			  }
			  else
			  {
				  if(open_on==0)   //�Ѿ�����
				  {
					  if(zigbee_net==0x01)
					  {
						#if USE_OLED_TFT	// 1 ʹ��tft
				            lcd_show_mode(0,72, 4);	//���� num= ���� 1 ���� 2 ����	 3��ʪ 4 WiFi 5����	 6 ��C	7��λ0   8��λ1  9��λ2 10��λ3	 11�Զ�
						#else//0ʹ��OLED
							OLED_ShowChar1_24T(40+1,16,10,16,1); //	WiFiͼ��
						#endif

					  }
					  else
					  {
					     zigbee_net_time++;	   //  ��������״̬ ��ʱ
					  	 if(zigbee_net_time>1) zigbee_net_time=0;	   //  ��������״̬ ��ʱ

						 #if USE_OLED_TFT	// 1 ʹ��tft
							 if(zigbee_net_time)
							 {
				                   lcd_show_mode(0,72, 4);	//���� num= ���� 1 ���� 2 ����	 3��ʪ 4 WiFi 5����	 6 ��C	7��λ0   8��λ1  9��λ2 10��λ3	 11�Զ�
							 }
							 else  lcd_show_mode(0,72, 12);	//���� num= ���� 1 ���� 2 ����	 3��ʪ 4 WiFi 5����	 6 ��C	7��λ0   8��λ1  9��λ2 10��λ3	 11�Զ�
						
						 #else//0ʹ��OLED
							 if(zigbee_net_time)
							 {
							  OLED_ShowChar1_24T(40+1,16,10,16,1); //	WiFiͼ��
							 }
							 else  OLED_ShowChar1_24T(40+1,16,11,16,1); //	�ո�ͼ��
						 #endif
					  }
				  }
			  }
	
			  if(net!=zigbee_net)
			  {
				  if(zigbee_net==0x01)
				  {
				  	all_data_update();	   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
				  }
				  net=zigbee_net;
			  }
			 if(zigbee_net==0x01)
			 {
			    set_up_net_time++;
				if(set_up_net_time>5) set_up_net_time=10;
			 }
			 zigbee_beattim++;      //  zigbee����ʱ�䣬�ϴ����е�״̬
			 if((zigbee_beattim>600)||(set_up_net_time<5))
			 {
			 	 zigbee_beattim=0;      //  zigbee����ʱ�䣬�ϴ����е�״̬
				 if(zigbee_net==0x01)
				 all_data_update();	   //  zigbee�豸����״̬  0x00���豸Ϊδ����״̬  0x01���豸Ϊ������״̬��	 0x02���豸����״̬�쳣��
			 }
		 }

		  if(open_on==0) //�Ѿ�����
		  {
			  if(set_wd_sta) //���������¶���ʾ
			  {
				set_wd_display_time++;     //  
				if(set_wd_display_time>4)  //5��
				set_wd_sta=0;              //   
			  }
			  else
			  display(1);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
		  }		 
		  else 	PWM_RGB_INT(0,0);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�

		  if(key_led_on_time_int>key_led_on_time_lan)
		  {
			adrr_set=0;   //  1 �����ַ����ģʽ
		  	LED0=0;					 //  ����LED��
			if(key_led_on_time_int==key_led_on_time_lan+1)
			{
//			  printf("OLED_Display_Off\r\n");
			  if(open_on==1) 
			  display(3);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�
			  else
			  display(4);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�

			  OLED_Display_Off();LCD_PWR=1;				 //�ر�lcd����
			}
			key_led_on_time_int=key_led_on_time_lan+2;	 //	 ����LED������ʱ�����
		  	k_on=0;
//			LED_B=0;
//			LED_Y=0;
			PWM_RGB_INT(0,0);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
		  }
		  if(zigbee_net_sta==0)   //  1 ��������״̬
		  key_led_on_time_int++;	 //	 ����LED������ʱ�����

	   }

       #if USE_OLED_TFT==0	// 0 ʹ��oled
			if(TOUCH_IN==0)	  //�ӽ���Ӧ��Ч
			{
				if(TOUCH_IN_CRC!=TOUCH_IN)
				{
					  k_on=1;
			 		  LED0=1;					 //  ����LED��
					  if(open_on==0)   //�Ѿ�����
					  {
						  OLED_Display_On(); LCD_PWR=0;	//��lcd����
						  if(zigbee_net_sta)   //  1 ��������״̬
						  {
							 PWM_RGB_INT(0,8);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�			  
						  }
						  else
						  if(df_stats_f==0)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
						  {
							 PWM_RGB_INT(0,3);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
						  }
						  else
						  if(df_stats_f==1)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
						  {
							 PWM_RGB_INT(0,1);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
						  }
						  else
						  if(df_stats_f==2)  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�)
						  {
							 PWM_RGB_INT(0,2);  // mode 0 �¿�ģʽ��u8 cmd 0 ȫ����  1����  2���� 3 �ͷ�
						  }
					  }
					  else
					  {
					  	OLED_Display_On(); LCD_PWR=0;	//��lcd����
						display(3);  //  0 ��ʾ��ַ 1 ��ʾ�¶Ⱥ͵�λ 2�趨�¶�  3 OFF
					  }
					key_led_on_time_int=5;	 //	 ����LED������ʱ�����
					TOUCH_IN_CRC=TOUCH_IN;
				}
			}
			else
			TOUCH_IN_CRC=TOUCH_IN;

		 #endif

	}  
} 


