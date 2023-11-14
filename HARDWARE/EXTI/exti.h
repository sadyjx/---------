#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"

extern u8   coder_sta;	 //高4位 0x80左转触发  0x10 往左触发	低4位 0x08右转触发  0x01 往右触发    

void EXTIX_Init(void);//外部中断初始化		 					    
#endif

























