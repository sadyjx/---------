
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
u8 OLED_GRAM[128][12];	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<12;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
#if USE_OLED_TFT==0	// 1 ʹ��tft
	u8 i;
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  

#endif
				  
} 
#endif
	
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
//	for(i=0;i<12;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //������ʾ
	for(i=0;i<12;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ

}


void OLED_ShowCHinese_T1(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
			OLED_WR_Byte(Hzk_T[2*no][t],OLED_DATA);
			adder+=1;
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
				OLED_WR_Byte(Hzk_T[2*no+1][t],OLED_DATA);
				adder+=1;
    }
	
						
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar1(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==16)temp=F8X16[chr][t];	//����1608����
        if(size==24)temp=shuj_12T[chr][t];	//����1608����
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          	         
}
void OLED_ShowChar1_32(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=16;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		temp=F16X32[chr*4][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	 
    for(t=0;t<csize;t++)
    {   
		temp=F16X32[chr*4][t+csize];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
	x=x0;
	y=y0-8;
	y0=y;
    for(t=0;t<csize;t++)
    {   
		temp=F16X32[chr*4][t+csize*2];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
    for(t=0;t<csize;t++)
    {   
		temp=F16X32[chr*4][t+csize*3];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
//    for(t=0;t<csize;t++)
//    {   
//		temp=F16X32[chr][t+csize*3];	//����1608����
//        for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//			else         OLED_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			if((y-y0-8)==size/2)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}  	 
//    }  
		      	         
}
void OLED_ShowChar1_T(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=16;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_T[chr*2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_T[chr*2+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
		      	         
}

void OLED_ShowSHUJ_24T1(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=16;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���

	size=8;
    for(t=0;t<csize;t++)
    {   
		temp=shuj_12T[chr*2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 

	x=x0;
	y0=y0-4; 
	y=y0;
//	size=8;
    for(t=0;t<csize;t++)
    {   
		temp=shuj_12T[chr*2+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
}

void OLED_ShowSHUJ_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=24;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���

	size=16;
    for(t=0;t<csize;t++)
    {   
		temp=shuj_24T[chr*2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 

	x=x0;
	y0=y0-8; 
	y=y0;
//	size=8;
    for(t=0;t<24;t++)
    {   
		temp=shuj_24T[chr*2+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
}


void OLED_ShowChar1_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=24;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���

    for(t=0;t<csize;t++)
    {   
		temp=Hzk_24T[chr*3][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_24T[chr*3+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_24T[chr*3+2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  			      	         
}

void OLED_ShowFGJ_24T(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=24;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���

    for(t=0;t<csize;t++)
    {   
		temp=FGJ_24T[chr*3][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=FGJ_24T[chr*3+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=FGJ_24T[chr*3+2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  			      	         
}

void OLED_ShowChar1_48T(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=24;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���

    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+0][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0+24;
	y0=y0; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 


	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0+24;
	y0=y0; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+3][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
	
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+4][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0+24;
	y0=y0; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_48T[chr*6+5][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
	
	 	  
//	x=x0;
//	y0=y0-8; 
//	y=y0;
//    for(t=0;t<csize;t++)
//    {   
//		temp=Hzk_48T[chr*6+2][t];	//����1608����
//        for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//			else         OLED_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			if((y-y0)==size/2)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}  	 
//    } 
	
	
 			      	         
}


void OLED_ShowChar1_F(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=16;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_Fen[chr*2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk_Fen[chr*2+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
		      	         
}
void OLED_ShowChar1_Cool(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 x0=x;
	u8 csize=16;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
//	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		temp=Hzk[chr*2][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    } 
	x=x0;
	y0=y0-8; 
	y=y0;
    for(t=0;t<csize;t++)
    {   
		temp=Hzk[chr*2+1][t];	//����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else         OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  	  
		      	         
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	u8 temp,t,t1;
	u8 y0=y;
	u8 size=16;
	u8 mode=1;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==16)temp=oled_asc2_1608[chr][t];	//����1608����
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          	         
}

void OLED_ShowChar6(u8 x,u8 y,u8 chr)
{      	
	unsigned char i=0;	
//		if(x>Max_Column-1){x=0;y=y+2;}
		OLED_Set_Pos(x,y);	
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6X12[chr][i],OLED_DATA);
		OLED_Set_Pos(x+6,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6X12[chr][i+6],OLED_DATA);
}
//��ʾ����
void OLED_ShowChar32(u8 x,u8 y,u8 no)
{      			    
	u8 t;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(F16X32[4*no][t],OLED_DATA);
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(F16X32[4*no+2][t],OLED_DATA);
    }	


	OLED_Set_Pos(x+16,y);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(F16X32[4*no+1][t],OLED_DATA);
    }

	OLED_Set_Pos(x+16,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(F16X32[4*no+3][t],OLED_DATA);
    }

						
}

void OLED_ShowChar48(u8 x,u8 y,u8 no)
{      			    
	u8 t;
	OLED_Set_Pos(x,y);	
    for(t=0;t<24;t++)
	{
		OLED_WR_Byte(Hzk_48T[6*no][t],OLED_DATA);
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<24;t++)
	{
		OLED_WR_Byte(Hzk_48T[6*no+2][t],OLED_DATA);
    }	


//	OLED_Set_Pos(x+16,y);	
//    for(t=0;t<16;t++)
//	{	
//		OLED_WR_Byte(Hzk_48T[6*no+1][t],OLED_DATA);
//    }
//
//	OLED_Set_Pos(x+16,y+1);	
//    for(t=0;t<16;t++)
//	{	
//		OLED_WR_Byte(Hzk_48T[6*no+3][t],OLED_DATA);
//    }

						
}



//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			y+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

void OLED_ShowCHinese_FEN(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk_Fen[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk_Fen[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
void OLED_ShowCHinese_T(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk_T[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk_T[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>128||y>96)return;//������Χ��.
	pos=11-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
//#define OLED_CS PAout(1)
//#define OLED_RST  PAout(7)//��MINISTM32��ֱ�ӽӵ���STM32�ĸ�λ�ţ�	
//#define OLED_DC PBout(0)
//#define OLED_SCLK PBout(1)
//#define OLED_SDIN PBout(2)
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
 	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
	
   	 
	GPIOA->CRL&=0X0F00FFFF; 
	GPIOA->CRL|=0X30330000;	//PB9 ���� 	    
	GPIOA->ODR|=3<<4;     	//PB9 ����	�����
	GPIOA->ODR|=1<<7;     	//PB9 ����	�����

	
	GPIOB->CRL&=0XFFFFFF00; 
	GPIOB->CRL|=0X00000033;	//PA12 ���� 	    
	GPIOB->ODR|=3<<0;     	//PA12 ����	�����

 	 
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4);	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��A�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8);	



 
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
					  
//	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
//	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
//	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
//	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
//	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
//	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
//	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0
////	WriteComm(0x5f);//��95�н���   ��96�� 
//	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
//
//
//
//													    
//	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
//	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
//	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
//	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
//	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
//	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
//	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
//	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
//		 
//	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
//	OLED_WR_Byte(0xFF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
//	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
//	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
//	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
//	OLED_WR_Byte(0x00,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
//
//
//
//
//	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
//	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
//	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
//
//
//    OLED_WR_Byte(0xA4,OLED_CMD);//SET DISPLAY MODE 
//    OLED_WR_Byte(0xA8,OLED_CMD);//SET MUX RADIO 
//    OLED_WR_Byte(0x5f,OLED_CMD); 
//

	OLED_WR_Byte(0xfd, OLED_CMD);//Set Command Lock
	OLED_WR_Byte(0x12, OLED_CMD);//Unlock
					  
	OLED_WR_Byte(0xae, OLED_CMD);   //turn off display
	
	OLED_WR_Byte(0x20, OLED_CMD);
	OLED_WR_Byte(0x02, OLED_CMD);    // page addressing mode
	
	OLED_WR_Byte(0x81, OLED_CMD);    //contrast
	OLED_WR_Byte(0x6e, OLED_CMD);
	
	OLED_WR_Byte(0xa1, OLED_CMD);	   //Set Segment Re-map
	OLED_WR_Byte(0xc0, OLED_CMD);		//Set Common Output Scan Direction
	
	OLED_WR_Byte(0xd5, OLED_CMD);    // display divide ratio/osc. freq. mode//����
	OLED_WR_Byte(0x80, OLED_CMD);    // Osc. Freq:320kHz,DivideRation:1
	
	OLED_WR_Byte(0xd9, OLED_CMD);    // set pre-charge period
	OLED_WR_Byte(0x22, OLED_CMD);    // set period 1:1;period 2:15
	
	OLED_WR_Byte(0xda, OLED_CMD);    // //Set COM Pins Hardware Configuration
	OLED_WR_Byte(0x12, OLED_CMD);    //
	
	OLED_WR_Byte(0xdb, OLED_CMD);    // VCOM deselect level mode
	OLED_WR_Byte(0x40, OLED_CMD);    // 
	
	
	OLED_WR_Byte(0xa8, OLED_CMD);    // multiplex ration mode:63//duty����
	OLED_WR_Byte(0x5f, OLED_CMD);    // duty=1/96
		
	OLED_WR_Byte(0xd3, OLED_CMD);    // offset
	OLED_WR_Byte(0x00, OLED_CMD);    // 

	OLED_WR_Byte(0xa4, OLED_CMD);  //    out follows RAM content
	OLED_WR_Byte(0xa6, OLED_CMD);  //    set normal display


	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 


	OLED_Clear();


}  





























