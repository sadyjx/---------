#ifndef MYAPP_H
#define MYAPP_H

#include "stdio.h"
#include "stm32f10x.h"
//#include "uipopt.h"

//#define UIP_APPCALL			MY_MAIN_APP
#define UIP_UDP_APPCALL			MY_UDP_APP

void MY_MAIN_APP(void);
void MY_UDP_APP(void);

#endif


