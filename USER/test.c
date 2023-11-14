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

u8 key_led_on_time_int=0;	 //	 按键LED灯亮的时间计数
u8 key_led_on_time_lan=60;	 //	 按键LED灯亮的时间长度 30秒


u8 F0=0;// 0温度 >= 0度  1 温度 <  0度
u8 DpyNum[6] = {2,6,5,0,0,0}; //显示缓冲区  
u8 adrr=1;
u8 df_stats=0;  //风机状态
u8 df_stats_s=3;  //风速  低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
u8 df_stats_f=1;  //阀门  阀门状态0x00通风0x01冷阀0x02热阀
u16 set_t_num=260;
u8 open_on=1;     //0 开机 1 关机

u8 key_data=0;
u8 key_data_5s=0;
//u16 sys_mode_time=0; //
//u8 sys_mode_sta=0x00; // 0x00主界面 0x10设置界面 0x20自动控制界面  

u8 set_wd_sta=0; // 设置温度  
u8 set_wd_display_time=0; // 设置温度  
u8 set_c_f=0;     // 0 摄氏度  1 华氏度
u16 wd_display=0;
void get_c_f(u8 cmd);  //  0 得到温度变化  1 设定温度变化


u8 set_wd_upper_limit=30; // 设置温度上限  
u8 set_wd_lower_limit=16; // 设置温度下限  
u8 set_wd_upper_limit_f=86; // 设置温度上限  
u8 set_wd_lower_limit_f=60; // 设置温度下限 

u8  timing_on=0;		   //  定时开启模式	0关闭 1开启
//u16 timing_on_int=200;	   //  定时时间
//u16 set_timing_on_int=0;   //  设定定时时间
u8  timing_min=0;		   //
u8  timing_hour=2;		   //
u8  set_timing_min=0;		   //
u8  set_timing_hour=0;		   //

u8  timing_sta=0;		   //  1 进入时间设定模式	 
u8  timing_sta_int=0;	   //  进入时间设定模式时间计时 无操作10秒退出	 
u16 timing_lcd_dis=0;	   //  进入时间设定模式时间闪烁

u8   zigbee_net=0x00;	   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
//u8   set_up_net=0;	       //  1 进入配网状态
u8   set_up_net_time=0;	   //  进入发送网络状态计时
u8   zigbee_net_sta=0x00;	//  zigbee设备状态  0x01：设备配网状态；	 0x02：设备功能性测试；
u8   zigbee_net_time=0x00;	//  设备配网状态；	 0x02：设备功能性测试；
u8   zigbee_rssi=0x00;	    //  设备网络信号强度
u8   zigbee_rssi_xd=11;	    //  设备网络信道
u16  zigbee_beattim=0;      //  zigbee心跳时间，上传所有的状态
u8   sys_zigbee_use=0;	    //  1 使用ZigBee


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

u16 sw_time_int=0; //继电器启动时间
u8 sw_bit=0;      //继电器启动标志
u8 df_stats_s_crc=0;  //风速缓存


u8   TX_buf[]={0x02,0x06,0x01,0x00,0x02,0x06,0x01,0x00};
u8   room_card=1;   //  1 有卡
u8   wd_bc_sta=0;   //  0 不需要补偿 1正补偿 2负补偿
u8   wd_bc_sta_data=0;   //   补偿温度

u8   adrr_set=0;        //  1 进入地址设置模式
u8   adrr_set_data=0;   //  1 进入地址设置模式

void sys_wr()	//写数据
{
//    u8 i,j;
	u8 buf[128];        
//	u16 n=0; 
	buf[0]=adrr;
	buf[1]=wd_bc_sta;        //  0 不需要补偿 1正补偿 2负补偿
	buf[2]=wd_bc_sta_data;   //   补偿温度
	buf[3]=key_led_on_time_lan/256;
	buf[4]=key_led_on_time_lan%256;
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)buf,24);
}
void sys_re()	//读数据
{
//    u8 i,j;
	u8 buf[128];        
//	u16 n=0; 
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)buf,24);
	if(buf[0]!=0xff)
	{
		adrr=buf[0];
		wd_bc_sta=buf[1];   //  0 不需要补偿 1正补偿 2负补偿
		wd_bc_sta_data=buf[2];   //   补偿温度
		if(wd_bc_sta_data>9) wd_bc_sta_data=0;
	    key_led_on_time_lan=buf[3]*256+buf[4];

		if(key_led_on_time_lan==0) key_led_on_time_lan=60;
	}
	else
	{
	   sys_wr();	//写数据
	}
}

void display( u8 cmd )  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF  4 ON
{

//	static u8 DpyNum1[6] = {0,0,0,0,0,0}; //显示缓冲区  
	static u16 wd_display1=0;
	static u8 df_stats_s1=0xff;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
	static u8 df_stats_f1=0xff;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
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
		   OLED_ShowChar1_48T(x,y+i*0,adrr_set_data/10,16,1); //数据	
	       OLED_ShowChar1_48T(x,y+i*1,adrr_set_data%10,16,1); //数据	
	}
	else
	if(zigbee_net_sta)	 //进入ZigBee配网或者功能性测试模式
	{
		switch (zigbee_net_sta)
		{
			case 0x01:	 //	ZigBee配网
		               OLED_ShowString(16,0,   "   ZigBee   ");
					   OLED_ShowString(16+16,0," connecting ");
					   x=56;y=40;	i=24;
					   OLED_ShowChar1_48T(x,y+i*0,zigbee_net_time/10,16,1); //数据	
	                   OLED_ShowChar1_48T(x,y+i*1,zigbee_net_time%10,16,1); //数据	

				break;
			case 0x02:	 //	ZigBee功能性测试
		               OLED_ShowString(16,0,   "   ZigBee   ");

					   OLED_ShowChar(16+16,8*9,zigbee_rssi_xd/10+0x30);
					   OLED_ShowChar(16+16,8*10,zigbee_rssi_xd%10+0x30);
					   OLED_ShowString(16+16,0," rssi ");

					   x=56;y=40;	i=24;
					   OLED_ShowChar1_48T(x,y+i*0,zigbee_rssi/10,16,1); //数据	
	                   OLED_ShowChar1_48T(x,y+i*1,zigbee_rssi%10,16,1); //数据	

				break;
			default:
				break;
		}

	}
	else
	switch (cmd)
	{
		case 0:	//  0 显示地址 
				   OLED_ShowString(16,0,SYS_VER);
				   x=40;y=40;	i=24;
				   OLED_ShowChar1_48T(x,y+i*0,adrr/10,16,1); //数据	
                   OLED_ShowChar1_48T(x,y+i*1,adrr%10,16,1); //数据	
//				   OLED_Refresh_Gram();//更新显示
			break;
		case 1:	//  1 显示温度和档位 

re:
			x=16;y=16;	i=24;
	
//			if(df_stats_f1!=df_stats_f)
			{
				switch (df_stats_f)	// 阀门状态0x00通风0x01冷阀0x02热阀
				{
					case 0:
					        OLED_ShowChar1_24T(x-0,y+i*0,7,16,1); //通风图标	
							OLED_ShowChar1_24T(x-3,y+i*1,8,16,1); //FAN	
						break;
					case 1:
					        OLED_ShowChar1_24T(x-0,y+i*0,0,16,1); //制冷图标	
							OLED_ShowChar1_24T(x-3,y+i*1,1,16,1); //cool	
						break;
					case 2:
					        OLED_ShowChar1_24T(x-0,y+i*0,5,16,1); //热阀图标	
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

				switch (df_stats_s&0x03)	   //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
				{
					case 0:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  0,16,1);   //风管机0挡
						break;
					case 1:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  1,16,1);   //风管机1挡
						break;
					case 2:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  2,16,1);   //风管机2挡
						break;
					case 3:
//							OLED_ShowFGJ_24T(x-4,y+i*2,5,16,1); //
							OLED_ShowFGJ_24T(x,y+i*3,  3,16,1);   //风管机3挡
						break;
					case 4:
							OLED_ShowFGJ_24T(x-4,y+i*2,4,16,1); //AT
							OLED_ShowFGJ_24T(x,y+i*3,3,16,1);   //风管机3挡
						break;

					default:
						break;
				}
			
			}
			
			x=40;y=40;	i=24;
			get_c_f(0);  //  0 得到温度变化  1 设定温度变化
//			if(wd_display1!=wd_display)   
			{
				OLED_ShowChar1_24T(x+24+1,16,2,16,1); //	IN图标
				if(wd_display>999) wd_display=wd_display/10;
		        OLED_ShowChar1_48T(x,y+i*0,wd_display/100,16,1); //数据	
		        OLED_ShowChar1_48T(x,y+i*1,wd_display%100/10,16,1); //数据	
				
				if(set_c_f)	// 0 摄氏度  1 华氏度
				OLED_ShowChar1_24T(x,y+i*2,4,16,1); //	°F
				else
				OLED_ShowChar1_24T(x,y+i*2,3,16,1); //	°C
				
				if(wd_display>999) 
				OLED_ShowChar1(x+31,y+i*2-13,16,16,1); //.
				else
				OLED_ShowChar1(x+31,y+i*2-13,10,16,1); //.

		        OLED_ShowSHUJ_24T(x+24,y+i*2,wd_display%10,16,1); //24*12数字
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

				if(set_c_f)	// 0 摄氏度  1 华氏度
				OLED_ShowChar1(x-4,y+i*4+6,15,16,1); // °F
				else
				OLED_ShowChar1(x-4,y+i*4+6,14,16,1); // °c
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//更新显示

			wd_display1=wd_display;
			df_stats_s1=df_stats_s;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
			df_stats_f1=df_stats_f;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
			set_t_num1=set_t_num;


			break;
		case 2:	//   2设定温度
			
	
			if(set_t_num1!=set_t_num);
			{
				x=40;y=40;	i=24;
				OLED_ShowChar1_24T(x+24+1,16,9,16,1); //	IN图标

				set_t_display=set_t_num;
				if(set_t_num>999) set_t_display=set_t_num/10;

		        OLED_ShowChar1_48T(x,y+i*0,set_t_display/100,16,1); //数据	
		        OLED_ShowChar1_48T(x,y+i*1,set_t_display%100/10,16,1); //数据
				
				if(set_c_f)	// 0 摄氏度  1 华氏度
				OLED_ShowChar1_24T(x,y+i*2,4,16,1); //	°F
				else	
				OLED_ShowChar1_24T(x,y+i*2,3,16,1); //	°C
			
				if(set_t_num>999)
				OLED_ShowChar1(x+31,y+i*2-13,16,16,1); //.
				else
				OLED_ShowChar1(x+31,y+i*2-13,10,16,1); //.

		        OLED_ShowSHUJ_24T(x+24,y+i*2,set_t_display%10,16,1); //24*12数字

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

				if(set_c_f)	// 0 摄氏度  1 华氏度
				OLED_ShowChar1(x-4,y+i*4+6,15,16,1); // °F
				else
				OLED_ShowChar1(x-4,y+i*4+6,14,16,1); // °c
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//更新显示
			wd_display1=set_t_num;
//			DpyNum1[0] =set_t_num/100 ; //显示缓冲区  
//			DpyNum1[1] =set_t_num%100/10 ; //显示缓冲区  
//			DpyNum1[2] =set_t_num%10 ; //显示缓冲区  
			set_t_num1=set_t_num;

			break;
		case 3:	//   3 OFF
			if(cmd1!=cmd)
			{
				OLED_Clear(); 
				x=40;y=30;	i=24;
		        OLED_ShowChar1_48T(x,y+i*0,0,16,1); //数据	
		        OLED_ShowChar1_48T(x,y+i*1,10,16,1); //数据
				OLED_ShowChar1_48T(x,y+i*2,10,16,1); //数据	
			}

//		    if(dis)
//		    OLED_Refresh_Gram();//更新显示

			break;
		case 4:	//   显示温度和档位 

			wd_display1=0;
			df_stats_s1=0;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
			df_stats_f1=0;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
			set_t_num1=0;
			goto re;

//			break;

		default:
			break;
	}
	OLED_Refresh_Gram();//更新显示
	cmd1=cmd;

}



void get_wd(u8 cmd)  //  得到ntc温度
{
	u16 ADCValue = 0;
	u8 temp = 0;       //变量
    static u16 sum=0;
    static u8  nn=0;
//	u8 buf[6];
	
//	ADCValue = Get_Adc(0)+16;         //读取AD转换值	
	ADCValue =Get_Adc_Average(0,100)+14;         //读取AD转换值	
	ADCValue=(ADCValue/((float)(4096-ADCValue)/10000))/10;
//	buf[0]=ADCValue/1000+0x30;
//	buf[1]=ADCValue%1000/100+0x30;
//	buf[2]=ADCValue%1000%100/10+0x30;
//	buf[3]=ADCValue%10+0x30;
//	buf[4]=0x0d;
//	buf[5]=0x0a;
//
//	RS485_Send_Data(buf,6);

	ADCValue=get_temperature(ADCValue); //计算温度值 		   
	ADCValue=500-ADCValue;
	if(sum==ADCValue) nn++;
	else nn=0; 
	sum=ADCValue;
	
	if((nn>5)||cmd)              
	{ 
		nn=0;  
	       
		if(wd_bc_sta==1)//  0 不需要补偿 1正补偿 2负补偿 
		ADCValue=ADCValue+(wd_bc_sta_data*10);
		else
		if(wd_bc_sta==2)//  0 不需要补偿 1正补偿 2负补偿 
		ADCValue=ADCValue-(wd_bc_sta_data*10);

		temp = ADCValue%1000/100;
		DpyNum[0]=temp;//显示百位						
		temp = (ADCValue%100)/10;
		DpyNum[1]=temp;//显示十位				
		temp = (ADCValue%100)%10;
		DpyNum[2]=temp;//显示个位 
		get_c_f(0);  //  0 得到温度变化  1 设定温度变化
	}



}


void get_c_f(u8 cmd)  //  0 得到温度变化  1 设定温度变化
{
   	u16 adcx;
	float temp;  
	static u16  wd=0;

	if(cmd)
	{
		if(set_c_f)	// 0 摄氏度  1 华氏度
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
		if(set_c_f)	// 0 摄氏度  1 华氏度
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
      one_data_update(2);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
	  wd=wd_display;
	}

}



u16 get_c_f_data1(u16 cmd_data,u8 cmd)  // cmd_data 温度数值 0华氏度转摄氏度 1 摄氏度转华氏度  
{
//   	u16 adcx;
	float temp; 
	switch (cmd)
	{
		case 0:	//   0华氏度转摄氏度
				if(cmd_data>400)
				if(set_c_f)	// 0 摄氏度  1 华氏度
				{
					temp=(float)cmd_data/10;
					temp=(float)((temp-32)*5/9);
				 	cmd_data=temp*10;
				}
			break;
		case 1:	//1 摄氏度转华氏度 
				if(cmd_data<400)
				if(set_c_f)	// 0 摄氏度  1 华氏度
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
void PWM_RGB_INT(u8 mode,u8 cmd)  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
{
//    static u16 sum=0;
    

	if(cmd_crc!=cmd)
	{
		if(mode==0)
		{
//				TIM3->CCER|=1<<4;   	//OC2 输出使能	 LEDR_PWM_VAL TIM3->CCR2  
				TIM3->CCER&=~(1<<4);   	//OC2 输出使能	 LEDR_PWM_VAL TIM3->CCR2 
//				TIM3->CCER|=1<<0;   	//OC1 输出使能	 LEDB_PWM_VAL TIM3->CCR1  
				TIM3->CCER&=~(1<<0);   	//OC1 输出使能	 LEDB_PWM_VAL TIM3->CCR1  
//	            TIM4->CCER|=1<<0;   	//OC1 输出使能	 LEDG_PWM_VAL  TIM4->CCR1  
	            TIM4->CCER&=~(1<<0);   	//OC1 输出使能	 LEDG_PWM_VAL  TIM4->CCR1  

		  switch (cmd)
		  {
		  	case 0:	   // 0 全部关 

				break;
			case 1:	   // 1制冷 

				TIM3->CCER|=1<<0;   	//OC1 输出使能	 LEDB_PWM_VAL TIM3->CCR1  
				LEDB_PWM_VAL=1000;		//数值越小，亮度越小 
				break;
			case 2:	   // 2制热

				TIM3->CCER|=1<<4;   	//OC2 输出使能	 LEDR_PWM_VAL TIM3->CCR2  
	            TIM4->CCER|=1<<0;   	//OC1 输出使能	 LEDG_PWM_VAL  TIM4->CCR1  
				LEDR_PWM_VAL=5000;		//数值越小，亮度越大
				LEDG_PWM_VAL=1000;
				break;
			case 3:	   // 3送风
				  
	            TIM4->CCER|=1<<0;   	//OC1 输出使能	 LEDG_PWM_VAL  TIM4->CCR1  
				LEDG_PWM_VAL=1000;		//数值越小，亮度越小
				break;
			case 8:	   // 进入配置模式
//				TIM3->CCER|=1<<0;   	//OC1 输出使能	 LEDB_PWM_VAL TIM3->CCR1  
//				LEDB_PWM_VAL=4000;
//
//				TIM3->CCER|=1<<4;   	//OC2 输出使能	 LEDR_PWM_VAL TIM3->CCR2  
//	            TIM4->CCER|=1<<0;   	//OC1 输出使能	 LEDG_PWM_VAL  TIM4->CCR1  
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




void get_download_handle(u8 cmd)  //判定是否有数据变化
{	 
	static u8 df_stats_s1=0xff;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
	static u8 df_stats_f1=0xff;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
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
		 else 	PWM_RGB_INT(0,0);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
		 dis=1;
	  }

	  if(df_stats_s1!=df_stats_s)dis=1;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
	  if(df_stats_f1!=df_stats_f)dis=1;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
	  if(set_t_num1!=set_t_num)  dis=1;
	  if(set_c_f1!=set_c_f)
	  {	
		 get_c_f(1);  //  0 得到温度变化  1 设定温度变化
	  	 dis=1;
	  }
      download_handle_sta=0;   //有操作信息
	}

	df_stats_s1=df_stats_s;  //风速	 低4位（3~0）：风速  0x00停 0x01低0x02中0x03高
	df_stats_f1=df_stats_f;  //阀门	 阀门状态0x00通风0x01冷阀0x02热阀
	set_t_num1=set_t_num;
	open_on1=open_on;
	set_c_f1=set_c_f;

	if(dis)
	{
		 		  LED0=1;					 //  按键LED灯
				  if(open_on==0)   //已经开机
				  {
					  OLED_Display_On();LCD_PWR=0;	//打开lcd背光
					  if(zigbee_net_sta)   //  1 进入配网状态
					  {
						 PWM_RGB_INT(0,8);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风			  
					  }
					  else
					  if(df_stats_f==0)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
					  {
						 PWM_RGB_INT(0,3);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  }
					  else
					  if(df_stats_f==1)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
					  {
						 PWM_RGB_INT(0,1);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  }
					  else
					  if(df_stats_f==2)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
					  {
						 PWM_RGB_INT(0,2);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  }
					  display(4);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
				  }
				  else
				  {
				  	OLED_Display_On(); LCD_PWR=0;	//打开lcd背光
					display(3);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
				  }
				key_led_on_time_int=5;	 //	 按键LED灯亮的时间计数
	}

}





void sys_sw_init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
   	 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X00000330;	//PA9 10 推挽 	    
	GPIOA->ODR|=3<<9;     	//PA9 10 上拉	输出高

	GPIOA->CRH&=0X0FFFFFFF; 
	GPIOA->CRH|=0X30000000;	//PA15 推挽 	    
	GPIOA->ODR|=1<<15;     	//PA15 上拉	输出高
	GPIOB->CRH&=0XF00FFFFF; 
	GPIOB->CRH|=0X03300000;	//PB13 14  推挽 	    
	GPIOB->ODR|=3<<13;     	//PB13 14  上拉	输出高

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

?	低4位（3~0）：风速
?	0x00  ? 停
?	0x01  ? 低
?	0x02  ? 中
?	0x03  ? 高
?	其他无效
?	（5~4）位：阀门状态
?	0x00  ? 通风
?	0x01  ? 冷阀
?	0x02  ? 热阀

*/
	if(zigbee_net_sta)   //  1 进入配网状态
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
			 	case 1:  //低
						B_SW_L=1;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 2:  //中
						B_SW_L=0;
						B_SW_M=1;
						B_SW_H=0;				
					break;
			 	case 3:  //高
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
			 	case 0:  //通风 
                     
					   B_SW_HOT=0;
					   B_SW_COOL=0;
                       goto re;
            			
//					break;
			 	case 1:  //冷阀
					   B_SW_HOT=0;
					   B_SW_COOL=1;
				       if(df_stats_s&0x04) //AUTO 
				       {
                           	if(((set_t_num*1)-5)>d_t_num)  //当前温度小于设定温度0.5度 停止制冷
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0; 
								df_stats_s=0x04;                                
                            }

			                if(((set_t_num*1)+40)<d_t_num)  //当前温度大于设定温度4度  高速制冷
	                        {
                               df_stats_s=3+0x04; 
                               goto re;  
                            }
                            else
			                if(((set_t_num*1)+20)<d_t_num)  //当前温度大于设定温度1度  慢速制冷
	                        {
                               goto re;
                            }
                            else
			                if(((set_t_num*1)+10)<d_t_num)  //当前温度大于设定温度1度  慢速制冷
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
			 	case 2:  //热阀  
                     
					   B_SW_HOT=1;
					   B_SW_COOL=0;

				       if(df_stats_s&0x04) //AUTO 
				       {
                           	if(d_t_num>((set_t_num*1)+5))//当前温度大于设定温度0.5度 停止加热
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0; 
								df_stats_s=0x04;                                
                            }

			                if(d_t_num<((set_t_num*1)-40))//当前温度小于设定温度-4度 快速加热
	                        {
                               df_stats_s=3+0x04;  
                               goto re; 
                            }
                            else
			                if(d_t_num<((set_t_num*1)-20))//当前温度小于设定温度-2度 快速加热
	                        {
//                               df_stats_s=3+0x04;  
                               goto re; 
                            }
                            else
			                if(d_t_num<((set_t_num*1)-10))//当前温度小于设定温度-1度 慢速加热
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
			                if(d_t_num<((set_t_num*1)-5))	//当前温度小于设定温度-0.5度 开始加热
	                        {
                               goto re;
                            }
			                if(d_t_num>((set_t_num*1)+5)) //当前温度大于设定温度+0.5度 停止加热
	                        {
					 			B_SW_L=0;
								B_SW_M=0;
								B_SW_H=0;                               
                            }
                       }            			
					break;
			 	case 3:  //冷阀+热阀 
                     
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
			 	case 0:  //低停止
						B_SW_L=0;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 1:  //低
						B_SW_L=1;
						B_SW_M=0;
						B_SW_H=0;			         	
					break;
			 	case 2:  //中
						B_SW_L=0;
						B_SW_M=1;
						B_SW_H=0;				
					break;
			 	case 3:  //高
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
	     
		if(rc_bit)//接收到有数据
		{
		    if(RS485_RX_BUF[0]==0x02)
            if(RS485_RX_BUF[1]==0x06)
			{
               for(i = 0; i < 7; i++)  crc=crc^RS485_RX_BUF[i];
               if(RS485_RX_BUF[7]==crc)  //按位异或校验值
               {
                  if(RS485_RX_BUF[2]==adrr)
                  {
                        for(i = 0; i < 7; i++)  TX_buf[i]=RS485_RX_BUF[i];
                        if(RS485_RX_BUF[3]==0x00)//读当前温控器状态
                        { 
 //                          if(df_stats_s) open_on=0; //0 开机 1 关机  
                           df_stats=df_stats_f*16+df_stats_s;
                           if(open_on)     //0 开机 1 关机
                           TX_buf[4]=df_stats|0x80;  //风机状态
                           else
                           TX_buf[4]=df_stats;  //风机状态

                           TX_buf[5]=get_c_f_data1(set_t_num,0)/10;  //  0 得到温度变化  1 设定温度变化;//设定的温度
                           TX_buf[6]=get_c_f_data1(wd_display,0)/10;//设定当前的温度  
	                        crc=0;
	                        for(i = 0; i < 7; i++)  crc=crc^TX_buf[i];
	                        TX_buf[7]=crc; 
	                        RS485_Send_Data(TX_buf, 8); 

                        }
                        else
                        if(RS485_RX_BUF[3]==0x01)//设置风机状态和温度
                        {
                           TX_buf[3]=0x00;  //风机状态
                           if(RS485_RX_BUF[4]&0x80) 
						   {
						     open_on=1;     //0 开机 1 关机
                             df_stats=RS485_RX_BUF[4]&0x7f;  //风机状态                            
					       }
                           else  
                           {              
                           open_on=0;     //0 开机 1 关机
                           df_stats=RS485_RX_BUF[4];  //风机状态                            
                           } 

						   if((df_stats&0x0f)==0x04)
						   {
                             df_stats_s=df_stats_s|0x04;
						   }
						   else
						   df_stats_s=df_stats&0x0f;

                           df_stats_f=df_stats/16;

                           set_t_num=get_c_f_data1(RS485_RX_BUF[5]*10,1);//设定的温度  
						    
                           TX_buf[4]=RS485_RX_BUF[4];  //风机状态
                           TX_buf[5]=RS485_RX_BUF[5];//设定的温度
                           TX_buf[6]=get_c_f_data1(wd_display,0)/10;//设定当前的温度  
						   get_download_handle(1);
	                        crc=0;
	                        for(i = 0; i < 7; i++)  crc=crc^TX_buf[i];
	                        TX_buf[7]=crc; 
	                        RS485_Send_Data(TX_buf, 8); 

						   if(open_on) 	//0 开机 1 关机
						   {
							  display( 3 );  //  0 显示地址 1 显示温度和档位 2设定温度
						   }
						   else
						   {
						   display( 4 );  //  0 显示地址 1 显示温度和档位 2设定温度
                           }
						   sw_time_int=0;
				           sw_bit=0;
						   sw_init(); 

						}
                  }
                  else
                  if(RS485_RX_BUF[2]==0xff)  //广播
                  {                       
                        if(RS485_RX_BUF[3]==0x06)//读当前温控器插卡取电状态
                        { 
                           if(RS485_RX_BUF[4]==0x01)//有卡
                           room_card=1;   //  1 有卡
                           else
                           room_card=0;   //  0 无卡
                        }
                        else
                        if(RS485_RX_BUF[3]==0x15)//温度补偿
                        {
                            
                           if(RS485_RX_BUF[4]==0x01) //正补偿
						   {
						       wd_bc_sta=1;   //  0 不需要补偿 1正补偿 2负补偿                        
					       }
                           else  
                           {              
                               wd_bc_sta=2;   //  0 不需要补偿 1正补偿 2负补偿           
                           } 
                           wd_bc_sta_data=RS485_RX_BUF[5];   //   补偿温度

  						   get_wd(1);  //  得到ntc温度
						   if(open_on) 	//0 开机 1 关机
						   {
							  display( 3 );  //  0 显示地址 1 显示温度和档位 2设定温度
						   }
						   else
						   {
						   display( 4 );  //  0 显示地址 1 显示温度和档位 2设定温度
                           }

//                           i=DpyNum[0]*10+DpyNum[1];
//                           if(wd_bc_sta==1)//  0 不需要补偿 1正补偿 2负补偿 
//                           i=i+wd_bc_sta_data;
//                           else
//                           if(wd_bc_sta==2)//  0 不需要补偿 1正补偿 2负补偿 
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

//	 wd_bc_sta=0;   //  0 不需要补偿 1正补偿 2负补偿
//     wd_bc_sta_data=0;   //   补偿温度
    re:
	if(wd_bc_sta==1)
    OLED_ShowString(x+16,y-12,"+");
	else
	if(wd_bc_sta==2)
    OLED_ShowString(x+16,y-12,"-");
	else
    OLED_ShowString(x+16,y-12," ");
		   	
	OLED_ShowChar1_48T(x,y+i*1,wd_bc_sta_data,16,1); //数据	

	OLED_Refresh_Gram();//更新显示
	while (1)
	{
	   IWDG_Feed();//喂狗
		t=KEY_Scan(1);

		if(t)
		{
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //模式
					  sys_wr();	//写数据
					  PWM_RGB_INT(0,2);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  LED0=0;					 //  按键LED灯
                      delay_ms(500);
					  LED0=1;					 //  按键LED灯
					  return 0;
				break;
			case 2:	  //档位

				break;
		  	case 3:	  //开关
					  return 0;
				break;
			case 4:	  //摄氏度/华氏度 
				break;
			case 5:	  //顺时针转 
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
			case 6:	  //逆时针转 
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
	   OLED_ShowChar1_48T(x,y+i*0,lan/100,16,1); //数据	
	   OLED_ShowChar1_48T(x,y+i*1,lan%100/10,16,1); //数据	
	   OLED_ShowChar1_48T(x,y+i*2,lan%10,16,1); //数据	

	OLED_Refresh_Gram();//更新显示
	while (1)
	{
	   IWDG_Feed();//喂狗
		t=KEY_Scan(1);

		if(t)
		{
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //模式
			          key_led_on_time_lan=lan;
					  sys_wr();	//写数据
					  PWM_RGB_INT(0,2);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  LED0=0;					 //  按键LED灯
                      delay_ms(500);
					  LED0=1;					 //  按键LED灯
					  return 1;
				break;
			case 2:	  //档位

				break;
		  	case 3:	  //开关
					  return 1;
				break;
			case 4:	  //摄氏度/华氏度 
				break;
			case 5:	  //顺时针转 
						  lan++;
						  if(lan>999) lan=999;
					  goto re;
//				break;
			case 6:	  //逆时针转 
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
#if USE_OLED_TFT	// 1 使用tft
	u8 clock=11;
#else//0使用OLED
	u8 clock=9;
#endif



	Stm32_Clock_Init(clock);	//系统时钟设置
	delay_init(clock*8);	   	 	//延时初始化 
	
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();
	sys_sw_init();
//    rs485_send_incidentrecord_int();
//	TIM3_Int_Init(99,7199);//10Khz的计数频率，计数100次为10ms 
//	TIM3_Int_Init(99,719);//10Khz的计数频率，计数100次为10ms 
//    TIM3_Int_Init(999,71);//1ms  
	Timer2_Init(999,(clock*8-1));
//    uart_init(72,115200);
	zigbee_protocol_init();
	USART2_Init((clock*8)/2,9600);	//初始化串口2
	RS485_Init((clock*8)/2,4800);
									    
	TIM3_PWM_Init(8999,0);	//R 不分频。PWM频率=72000/(899+1)=80Khz
	TIM4_PWM_Init(8999,0);	//G
	PWM_RGB_INT(0,0);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
	EXTIX_Init();
	Adc_Init();

#if USE_OLED_TFT	// 1 使用tft
	lcd_init();
#else//0使用OLED
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
#endif
    sys_re();	//读数据 
	WIFI_RST=1;
	display( 0 );  //  0 显示地址 1 显示温度和档位 2设定温度
	delay_ms(500);
	WIFI_RST=0;
//	IWDG_Feed();//喂狗
	delay_ms(500);

#if USE_OLED_TFT	// 1 使用tft
	lcd_clear(BACK_COLOR);
#else//0使用OLED
	OLED_Clear(); 
#endif


//	display( 1 );  //  0 显示地址 1 显示温度和档位 2设定温度
	WIFI_RST=1;
//	printf("RST\r\n");

	OLED_Display_Off();
	LCD_PWR=1;				 //关闭lcd背光
	LED0=0;					 //  按键LED灯灭
	get_wd(1);  //  得到ntc温度
	IWDG_Init(4,625*2);    	//与分频数为64,重载值为625,溢出时间为1s	  

//  zigbee_rst_cmd_handle(0x00);//配置zigbee模块   0x00：将模块软复位，清除堆栈数据，保存网络状态；0x01：将模块配置为开始配网状态；
	   

	while (1)
	{
	   IWDG_Feed();//喂狗
	   uart2_rx();
//	   zigbee_uart_service();
	   get_download_handle(0);  //判定是否有数据变化
	   uart3_485_rx();

		if(room_card)   //  1 有卡
		t=KEY_Scan(1);

		if(t)
		{
		  key_led_on_time_int=0;	 //	 按键LED灯亮的时间计数
//		  printf("KEY=%d\r\n",t);
		  
		  if((open_on==0)||((t==3)))
		  if(k_on)
		  if(t!=t1)
		  {
		  switch (t)
		  {
		  	case 1:	  //模式
				   if(adrr_set)
				   {
			          adrr_set=0;   //  1 进入地址设置模式
					  adrr=adrr_set_data;   //  1 进入地址设置模式
					  sys_wr();	//写数据
				   }
				   else
				   {
	               df_stats_f++;
	               if(df_stats_f>2) df_stats_f=0;
				   display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
				   one_data_update(3);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
				   one_data_update(7);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
				   }
				break;
			case 3:	  //档位
                   if(df_stats_s==3) //自动档位
				   {
				   	 df_stats_s=df_stats_s|0x04;
				   }
				   else
				   {
					   df_stats_s++;
					   if(df_stats_s>3) 
					   df_stats_s=1;
				   }
				   display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
				   one_data_update(4);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
				   adrr_set=0;   //  1 进入地址设置模式
				break;
		  	case 2:	  //开关
					 if(adrr_set==0) //  1 进入地址设置模式
					 {
					  if(open_on)
					  {
					    OLED_Clear(); 
					    display( 4 );  //  0 显示地址 1 显示温度和档位 2设定温度
						OLED_Display_On(); LCD_PWR=0;	//打开lcd背光
					  	open_on=0;     //0 开机 1 关机
					 	cmd_crc=0xff;
					  }
					  else
					  {
						zigbee_net_sta=0;	       //  1 进入配网状态
						OLED_Display_Off();LCD_PWR=1;				 //关闭lcd背光
					  	open_on=1;     //0 开机 1 关机
						PWM_RGB_INT(0,0);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
					  }
					  display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
					  one_data_update(0);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
					 }   
				break;
			case 4:	  //摄氏度/华氏度 
					  if(set_c_f) set_c_f=0;	// 0 摄氏度  1 华氏度
					  else 	      set_c_f=1;	// 0 摄氏度  1 华氏度
					  get_c_f(1);  //  0 得到温度变化  1 设定温度变化

						//u8 set_wd_upper_limit=30; // 设置温度上限  
						//u8 set_wd_lower_limit=16; // 设置温度下限  

					  display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
//					  one_data_update(5);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
//					  one_data_update(6);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
					  all_data_update();
					  adrr_set=0;   //  1 进入地址设置模式
				break;
			case 5:	  //顺时针转 
				   if(adrr_set)
				   {
					  adrr_set_data=adrr_set_data+1;   //  1 进入地址设置模式
				   	  if(adrr_set_data>99) adrr_set_data=99;
					  display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
				   }
				   else
				   {
					  set_t_num=set_t_num+10;
					  if(set_c_f) 	// 0 摄氏度  1 华氏度
					  {
					  	if(set_t_num>(set_wd_upper_limit_f*10)) set_t_num=(set_wd_upper_limit_f*10);
					  }
					  else
					  {
					    if(set_t_num>(set_wd_upper_limit*10)) set_t_num=(set_wd_upper_limit*10);
					  }
					  set_t_num=(set_t_num/10)*10;
//					  if(set_t_num>(set_wd_upper_limit*10)) set_t_num=set_wd_upper_limit*10;

					  display(2);  //  0 显示地址 1 显示温度和档位 2设定温度
					  set_wd_sta=1; // 设置温度  
					  set_wd_display_time=0; // 设置温度  
					  one_data_update(1);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
				  }
				break;
			case 6:	  //逆时针转 
				   if(adrr_set)
				   {
				   	  if(adrr_set_data==0) adrr_set_data=1;
					  adrr_set_data=adrr_set_data-1;   //  1 进入地址设置模式
					  display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
				   }
				   else
				   {
					  set_t_num=set_t_num-10;
					  if(set_c_f) 	// 0 摄氏度  1 华氏度
					  {
						if(set_t_num<(set_wd_lower_limit_f*10)) set_t_num=set_wd_lower_limit_f*10;
					  }
					  else
					  if(set_t_num<(set_wd_lower_limit*10)) set_t_num=set_wd_lower_limit*10;

					  set_t_num=(set_t_num/10)*10;
//					  if(set_t_num<(set_wd_lower_limit*10)) set_t_num=set_wd_lower_limit*10;
					  display(2);  //  0 显示地址 1 显示温度和档位 2设定温度
					  set_wd_sta=1; // 设置温度  
					  set_wd_display_time=0; // 设置温度  
					  one_data_update(1);	   //0	 开机/关机  1 //当前温度设置   2当前当前温度	3当前模式  4当前档位	5当前温标切换
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
 		  LED0=1;					 //  按键LED灯
		  if(open_on==0)   //已经开机
		  {
			  OLED_Display_On(); LCD_PWR=0;	//打开lcd背光

				  if(key_time==3)	//长按3S
				  {
				  	   key_time++;
					   if(sys_zigbee_use)	    //  1 使用ZigBee
				  	   if(t==7)		
					   {
	                      zigbee_rst_cmd_handle(0x01);//配置zigbee模块   0x00：将模块软复位，清除堆栈数据，保存网络状态；0x01：将模块配置为开始配网状态；
	                      zigbee_net=0x00;		   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
						  zigbee_net_sta=1;	       //  1 进入配网状态
						  zigbee_net_time=0;	   //  进入配网状态 计时
						  OLED_Clear(); 
						  display(4);  //  0 显示地址 1 显示温度和档位 2设定温度
					   }

				  }
				  if(key_time==6)	//长按3S
				  {
				  	   key_time++;
				  	   if(sys_zigbee_use)	    //  1 使用ZigBee
					   if(t==8)		//长按模式+档位键3秒进入zigbee
					   {
		                  zigbee_net=0x00;		   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
						  zigbee_rssi_xd=11;	    //  设备网络信道
						  zigbee_rf_test_cmd_handle(zigbee_rssi_xd);//ZigBee模块RF功能测试   cmd 信道 11-26 						  
						  zigbee_net_sta=0x02;	//  zigbee设备状态  0x01：设备配网状态；	 0x02：设备功能性测试；
						  zigbee_net_time=0x00;	//  设备配网状态；	 0x02：设备功能性测试；
					   	  zigbee_rssi=99;	    //  设备网络信号强度
	                      zigbee_net=0x00;		   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
						  OLED_Clear(); 
						  display(4);  //  0 显示地址 1 显示温度和档位 2设定温度
//					   	  JTAG_Set(SWD_ENABLE); 
					   }
				  }

			  if(zigbee_net_sta)   //  1 进入配网状态
			  {
				 PWM_RGB_INT(0,8);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风			  
			  }
			  else
			  if(df_stats_f==0)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
			  {
				 PWM_RGB_INT(0,3);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
			  }
			  else
			  if(df_stats_f==1)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
			  {
				 PWM_RGB_INT(0,1);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
			  }
			  else
			  if(df_stats_f==2)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
			  {
				 PWM_RGB_INT(0,2);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
			  }
		  }
		  else
		  {
		  	OLED_Display_On();LCD_PWR=0;	//打开lcd背光
			display(3);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
				  if(key_time==5)	//长按3S
				  {
				  	   key_time++;
					   if(t==2)		//长按档位键5秒进入温度偏差计算
					   {
						 display_wd();
					   }
					   if(t==4)		//长按档位键5秒进入背光亮度设置
					   {
						 oled_open_display_time();
					   }
					   display(1);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
				       display(3);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
				  }

		  }
		  if(key_time==5)	//长按3S
		  {
		  	   key_time++;
			   if(t==9)		//长按模式+开机键5秒进入地址设置
			   {
		          adrr_set=1;   //  1 进入地址设置模式
				  adrr_set_data=adrr;   //  1 进入地址设置模式
				  OLED_Clear(); 
				  display( 4 );  //  0 显示地址 1 显示温度和档位 2设定温度
			      OLED_Display_On(); LCD_PWR=0;	//打开lcd背光
				  open_on=0;     //0 开机 1 关机
				  cmd_crc=0xff;
			   }
		  }

//		  k_on=1;

//		  if(zigbee_net==0x01)	all_data_update();	   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
		}
		else
		{
		  if(t1)  k_on=1;
		  t1=t;
		  key_time=0;
		}


	   if((xt_time%20)==0)
	   get_wd(0);  //  得到ntc温度


	   if(xt_time>99)
	   {
		  xt_time=0;
		  key_time++;
		  sw_init();

		  if(sys_zigbee_use)	    //  1 使用ZigBee
		  {
			  if(zigbee_net_sta)  //  1 进入配网状态
			  {
			  	  zigbee_net_time++;	   //  进入配网状态 计时
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
						zigbee_rf_test_cmd_handle(zigbee_rssi_xd);//ZigBee模块RF功能测试   cmd 信道 11-26 						  
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
	//				display( 4 );  //  0 显示地址 1 显示温度和档位 2设定温度
					zigbee_net_sta=0;	       //  1 进入配网状态
					zigbee_net_time=0;	   //  进入配网状态 计时
				  	all_data_update();	   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
				  }
			  }
			  else
			  {
				  if(open_on==0)   //已经开机
				  {
					  if(zigbee_net==0x01)
					  {
						#if USE_OLED_TFT	// 1 使用tft
				            lcd_show_mode(0,72, 4);	//室内 num= 出风 1 制冷 2 制热	 3抽湿 4 WiFi 5室内	 6 °C	7档位0   8档位1  9档位2 10档位3	 11自动
						#else//0使用OLED
							OLED_ShowChar1_24T(40+1,16,10,16,1); //	WiFi图标
						#endif

					  }
					  else
					  {
					     zigbee_net_time++;	   //  进入配网状态 计时
					  	 if(zigbee_net_time>1) zigbee_net_time=0;	   //  进入配网状态 计时

						 #if USE_OLED_TFT	// 1 使用tft
							 if(zigbee_net_time)
							 {
				                   lcd_show_mode(0,72, 4);	//室内 num= 出风 1 制冷 2 制热	 3抽湿 4 WiFi 5室内	 6 °C	7档位0   8档位1  9档位2 10档位3	 11自动
							 }
							 else  lcd_show_mode(0,72, 12);	//室内 num= 出风 1 制冷 2 制热	 3抽湿 4 WiFi 5室内	 6 °C	7档位0   8档位1  9档位2 10档位3	 11自动
						
						 #else//0使用OLED
							 if(zigbee_net_time)
							 {
							  OLED_ShowChar1_24T(40+1,16,10,16,1); //	WiFi图标
							 }
							 else  OLED_ShowChar1_24T(40+1,16,11,16,1); //	空格图标
						 #endif
					  }
				  }
			  }
	
			  if(net!=zigbee_net)
			  {
				  if(zigbee_net==0x01)
				  {
				  	all_data_update();	   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
				  }
				  net=zigbee_net;
			  }
			 if(zigbee_net==0x01)
			 {
			    set_up_net_time++;
				if(set_up_net_time>5) set_up_net_time=10;
			 }
			 zigbee_beattim++;      //  zigbee心跳时间，上传所有的状态
			 if((zigbee_beattim>600)||(set_up_net_time<5))
			 {
			 	 zigbee_beattim=0;      //  zigbee心跳时间，上传所有的状态
				 if(zigbee_net==0x01)
				 all_data_update();	   //  zigbee设备入网状态  0x00：设备为未入网状态  0x01：设备为已入网状态；	 0x02：设备网络状态异常；
			 }
		 }

		  if(open_on==0) //已经开机
		  {
			  if(set_wd_sta) //进入设置温度显示
			  {
				set_wd_display_time++;     //  
				if(set_wd_display_time>4)  //5秒
				set_wd_sta=0;              //   
			  }
			  else
			  display(1);  //  0 显示地址 1 显示温度和档位 2设定温度
		  }		 
		  else 	PWM_RGB_INT(0,0);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风

		  if(key_led_on_time_int>key_led_on_time_lan)
		  {
			adrr_set=0;   //  1 进入地址设置模式
		  	LED0=0;					 //  按键LED灯
			if(key_led_on_time_int==key_led_on_time_lan+1)
			{
//			  printf("OLED_Display_Off\r\n");
			  if(open_on==1) 
			  display(3);  //  0 显示地址 1 显示温度和档位 2设定温度
			  else
			  display(4);  //  0 显示地址 1 显示温度和档位 2设定温度

			  OLED_Display_Off();LCD_PWR=1;				 //关闭lcd背光
			}
			key_led_on_time_int=key_led_on_time_lan+2;	 //	 按键LED灯亮的时间计数
		  	k_on=0;
//			LED_B=0;
//			LED_Y=0;
			PWM_RGB_INT(0,0);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
		  }
		  if(zigbee_net_sta==0)   //  1 进入配网状态
		  key_led_on_time_int++;	 //	 按键LED灯亮的时间计数

	   }

       #if USE_OLED_TFT==0	// 0 使用oled
			if(TOUCH_IN==0)	  //接近感应有效
			{
				if(TOUCH_IN_CRC!=TOUCH_IN)
				{
					  k_on=1;
			 		  LED0=1;					 //  按键LED灯
					  if(open_on==0)   //已经开机
					  {
						  OLED_Display_On(); LCD_PWR=0;	//打开lcd背光
						  if(zigbee_net_sta)   //  1 进入配网状态
						  {
							 PWM_RGB_INT(0,8);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风			  
						  }
						  else
						  if(df_stats_f==0)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
						  {
							 PWM_RGB_INT(0,3);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
						  }
						  else
						  if(df_stats_f==1)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
						  {
							 PWM_RGB_INT(0,1);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
						  }
						  else
						  if(df_stats_f==2)  //阀门  阀门状态0x00通风0x01冷阀0x02热阀)
						  {
							 PWM_RGB_INT(0,2);  // mode 0 温控模式，u8 cmd 0 全部关  1制冷  2制热 3 送风
						  }
					  }
					  else
					  {
					  	OLED_Display_On(); LCD_PWR=0;	//打开lcd背光
						display(3);  //  0 显示地址 1 显示温度和档位 2设定温度  3 OFF
					  }
					key_led_on_time_int=5;	 //	 按键LED灯亮的时间计数
					TOUCH_IN_CRC=TOUCH_IN;
				}
			}
			else
			TOUCH_IN_CRC=TOUCH_IN;

		 #endif

	}  
} 


