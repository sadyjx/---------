#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//LED端口定义
#define LED0 PBout(3)// close	   系统运行灯
#define LED1 PAout(12)// connect   连接状态指示灯
#define LED2 PAout(15)// connect   连接状态指示灯

#define LED_B PBout(4)// connect   连接状态指示灯
#define LED_Y PBout(5)// connect   连接状态指示灯
#define LED_R PBout(6)// connect   连接状态指示灯
	
//

// //LED端口定义
//#define LED0 PAout(12)// close	   系统运行灯
//#define LED1 PAout(11)// connect   连接状态指示灯	

void LED_Init(void);//初始化		 				    
#endif

















