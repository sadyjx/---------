#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY0 PAin(11)   	//CLOSE		  �ر�
#define KEY1 PAin(8)	    //CONNECT 	  ����
#define KEY2 PCin(15)	//PE2
#define KEY3 PBin(8)	//PA0  WK_UP
#define TOUCH_IN PBin(12)	//PA0  WK_UP

#define K_A PCin(13)	    //CONNECT 	  ����
#define K_B PBin(9)	//PE2
#define K_C PAin(12)	//PA0  WK_UP

#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
