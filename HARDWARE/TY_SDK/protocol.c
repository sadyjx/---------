/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2018-0704, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.c
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v1.0.0
** �ա���: 2018��7��4��
** �衡��: 1:Э�����
**
**-----------------------------------------------------------------------------
******************************************************************************/

#include "zigbee.h"
//#include "main.h" 
extern void get_c_f(u8 cmd);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
extern u16 get_c_f_data1(u16 cmd_data,u8 cmd);  // cmd_data �¶���ֵ 0���϶�ת���϶� 1 ���϶�ת���϶�

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�zigbee_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�zigbee����ļ����ļ���include "zigbee.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�zigbee_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�zigbee_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_WORK_STATE, DP_TYPE_ENUM},
  {DPID_TEMP_SET, DP_TYPE_VALUE},
  {DPID_TEMP_SET_F, DP_TYPE_VALUE},
  {DPID_UPPER_TEMP_F, DP_TYPE_VALUE},
  {DPID_UPPER_TEMP, DP_TYPE_VALUE},
  {DPID_LOWER_TEMP_F, DP_TYPE_VALUE},
  {DPID_TEMP_UNIT_CONVERT, DP_TYPE_ENUM},
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},
  {DPID_LOWER_TEMP, DP_TYPE_VALUE},
  {DPID_LEVEL, DP_TYPE_ENUM},
  {DPID_TEMP_CURRENT_F, DP_TYPE_VALUE},
  {DPID_VALVE_STATE, DP_TYPE_ENUM},
};


/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
extern unsigned char emberSerialWriteByte(unsigned char port, unsigned char dataByte);
extern void UART2_SEND(u8 ch);
void uart_transmit_output(unsigned char value)
{

  UART2_SEND(value);
//#error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"
/*
  //ʾ��:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //���ڷ��ͺ���
*/
}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/
u16 get_c_f_data(u16 cmd_data)  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯
{
//   	u16 adcx;
	float temp;  
	if(set_c_f)	// 0 ���϶�  1 ���϶�
	{
		temp=(float)cmd_data/10;
		temp=(float)((temp-32)*5/9);
	 	cmd_data=temp*10;
	}

	return cmd_data;
}




//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��mcu����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{

//extern u8 df_stats;  //���״̬
//extern u8 df_stats_s;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
//extern u8 df_stats_f;  
//extern u16 set_t_num;
//extern u8 open_on;     
//extern u8 set_c_f;     // 0 ���϶�  1 ���϶�
//extern u16 wd_display;
//extern u8 set_wd_upper_limit; // �����¶�����  
//extern u8 set_wd_lower_limit; // �����¶�����  
//
//    mcu_dp_bool_update(DPID_SWITCH,��ǰ����); //BOOL�������ϱ�;
//    mcu_dp_enum_update(DPID_MODE,��ǰ����ģʽ); //ö���������ϱ�;
//    mcu_dp_enum_update(DPID_WORK_STATE,��ǰ����״̬); //ö���������ϱ�;
//    mcu_dp_value_update(DPID_TEMP_SET,��ǰĿ���¶�); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_UPPER_TEMP,��ǰ�����¶�����); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_LOWER_TEMP,��ǰ�����¶�����); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_TEMP_CURRENT,��ǰ��ǰ�¶�); //VALUE�������ϱ�;

//    mcu_dp_value_update(DPID_TEMP_SET_F,��ǰĿ���¶�_F); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_UPPER_TEMP_F,��ǰ�����¶�����-�H); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_LOWER_TEMP_F,��ǰ�����¶�����-�H); //VALUE�������ϱ�;
//    mcu_dp_value_update(DPID_TEMP_CURRENT_F,��ǰ��ǰ�¶�_F); //VALUE�������ϱ�;

//    mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,��ǰ�±��л�); //ö���������ϱ�;
//    mcu_dp_enum_update(DPID_LEVEL,��ǰ��λ); //ö���������ϱ�;
//    mcu_dp_enum_update(DPID_VALVE_STATE,��ǰ����״̬); //ö���������ϱ�;
//  //#error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
  /* 
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
 */
  if(sys_zigbee_use)	    //  1 ʹ��ZigBee
  {
    //��ǰ����
    if(open_on) //0 ���� 1 �ػ�
    mcu_dp_bool_update(DPID_SWITCH,0x00); //BOOL�������ϱ�;
    else
    mcu_dp_bool_update(DPID_SWITCH,0x01); //BOOL�������ϱ�;

    //��ǰ�±��л� set_c_f;     
	if(set_c_f==0)	 // 0 ���϶�  1 ���϶�
	{
	    //��ǰ�¶�����
	    mcu_dp_value_update(DPID_TEMP_SET,set_t_num/10); //VALUE�������ϱ�;
		//��ǰ��ǰ�¶�
	    mcu_dp_value_update(DPID_TEMP_CURRENT,wd_display/10); //VALUE�������ϱ�;
		mcu_dp_value_update(DPID_UPPER_TEMP,set_wd_upper_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
		mcu_dp_value_update(DPID_LOWER_TEMP,set_wd_lower_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
	}
	else
	{
	    //��ǰ�¶�����
	    mcu_dp_value_update(DPID_TEMP_SET_F,set_t_num/10); //VALUE�������ϱ�;
		//��ǰ��ǰ�¶�
	    mcu_dp_value_update(DPID_TEMP_CURRENT_F,wd_display/10); //VALUE�������ϱ�;
		mcu_dp_value_update(DPID_UPPER_TEMP_F,set_wd_upper_limit_f); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
		mcu_dp_value_update(DPID_LOWER_TEMP_F,set_wd_lower_limit_f); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
	}

	//��ǰģʽ
	switch (df_stats_f)	//����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	{
		case 0x00:
				 mcu_dp_enum_update(DPID_MODE,0x02); //ö���������ϱ�;
			break;
		case 0x01:
				 mcu_dp_enum_update(DPID_MODE,0x00); //ö���������ϱ�;
			break;
		case 0x02:
				 mcu_dp_enum_update(DPID_MODE,0x01); //ö���������ϱ�;
			break;
		default:
			break;
	}
	//��ǰ��λ df_stats_s;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
	if(df_stats_s&0x04)	  //auto
	{
	   mcu_dp_enum_update(DPID_LEVEL,0x03); //ö���������ϱ�;
	}
	else
	switch (df_stats_s&0x03)
	{
		case 0x01: mcu_dp_enum_update(DPID_LEVEL,0x00); //ö���������ϱ�;
			break;
		case 0x02: mcu_dp_enum_update(DPID_LEVEL,0x01); //ö���������ϱ�;
			break;
		case 0x03: mcu_dp_enum_update(DPID_LEVEL,0x02); //ö���������ϱ�;
			break;

		default:
			break;
	}
	//��ǰ�±��л� set_c_f;     // 0 ���϶�  1 ���϶�
    mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,set_c_f); //ö���������ϱ�;

//  mcu_dp_enum_update(DPID_WORK_STATE,��ǰ����״̬); //ö���������ϱ�;
	//��ǰ����״̬
	switch (df_stats_f)	//����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
	{
		case 0x00:
				 mcu_dp_enum_update(DPID_WORK_STATE,0x02); //ö���������ϱ�;
			break;
		case 0x01:
				 mcu_dp_enum_update(DPID_WORK_STATE,0x00); //ö���������ϱ�;
			break;
		case 0x02:
				 mcu_dp_enum_update(DPID_WORK_STATE,0x01); //ö���������ϱ�;
			break;
		default:
			break;
	}


	zigbee_beattim=0;      //  zigbee����ʱ�䣬�ϴ����е�״̬
//  mcu_dp_value_update(DPID_TEMP_SET_F,��ǰĿ���¶�_F); //VALUE�������ϱ�;
//  mcu_dp_value_update(DPID_TEMP_CURRENT_F,��ǰ��ǰ�¶�_F); //VALUE�������ϱ�;
//  mcu_dp_enum_update(DPID_COUNTDOWN_SET,��ǰ����ʱ); //ö���������ϱ�;
//  mcu_dp_value_update(DPID_COUNTDOWN_LEFT,��ǰ����ʱʣ��ʱ��); //VALUE�������ϱ�;
//  mcu_dp_fault_update(DPID_FAULT,��ǰ���ϸ澯); //�����������ϱ�;
//  mcu_dp_value_update(DPID_UPPER_TEMP_F,��ǰ�¶��������ޣ��H��); //VALUE�������ϱ�;
//  mcu_dp_value_update(DPID_LOWER_TEMP_F,��ǰ�¶��������ã��H��); //VALUE�������ϱ�;
//  mcu_dp_enum_update(DPID_VALVE_STATE,��ǰ����״̬); //ö���������ϱ�;
  }
}
void one_data_update(unsigned char cmd)	   //0	 ����/�ػ�  1 //��ǰ�¶�����   2��ǰ��ǰ�¶�	3��ǰģʽ  4��ǰ��λ	5��ǰ�±��л�
{

//extern u8 df_stats;  //���״̬
//extern u8 df_stats_s;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
//extern u8 df_stats_f;  
//extern u16 set_t_num;
//extern u8 open_on;     
//extern u8 set_c_f;     // 0 ���϶�  1 ���϶�
//extern u16 wd_display;
//extern u8 set_wd_upper_limit; // �����¶�����  
//extern u8 set_wd_lower_limit; // �����¶�����  
//

  //#error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
  /* 
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
 */
  if(sys_zigbee_use)	    //  1 ʹ��ZigBee
  {
	zigbee_beattim=0;      //  zigbee����ʱ�䣬�ϴ����е�״̬
//     if(zigbee_net==0x01)
	 switch (cmd)
	 {
	 	case 0:
		    //��ǰ����
		    if(open_on) //0 ���� 1 �ػ�
		    mcu_dp_bool_update(DPID_SWITCH,0x00); //BOOL�������ϱ�;
		    else
		    mcu_dp_bool_update(DPID_SWITCH,0x01); //BOOL�������ϱ�;

			break;
		case 1:
		    //��ǰ�¶�����
//		    mcu_dp_value_update(DPID_TEMP_SET,get_c_f_data(set_t_num)/10); //VALUE�������ϱ�;
		    //��ǰ�±��л� set_c_f;     
			if(set_c_f==0)	 // 0 ���϶�  1 ���϶�
			{
			    //��ǰ�¶�����
			    mcu_dp_value_update(DPID_TEMP_SET,set_t_num/10); //VALUE�������ϱ�;
			}
			else
			{
			    //��ǰ�¶�����
			    mcu_dp_value_update(DPID_TEMP_SET_F,set_t_num/10); //VALUE�������ϱ�;
			}
			break;
		case 2:
			//��ǰ��ǰ�¶�
//		    mcu_dp_value_update(DPID_TEMP_CURRENT,get_c_f_data(wd_display)/10); //VALUE�������ϱ�;
		    //��ǰ�±��л� set_c_f;     
			if(set_c_f==0)	 // 0 ���϶�  1 ���϶�
			{
				//��ǰ��ǰ�¶�
			    mcu_dp_value_update(DPID_TEMP_CURRENT,wd_display/10); //VALUE�������ϱ�;
			}
			else
			{
				//��ǰ��ǰ�¶�
			    mcu_dp_value_update(DPID_TEMP_CURRENT_F,wd_display/10); //VALUE�������ϱ�;
			}
			break;
		case 3:
			//��ǰģʽ
			switch (df_stats_f)	//����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
			{
				case 0x00:
						 mcu_dp_enum_update(DPID_MODE,0x02); //ö���������ϱ�;
					break;
				case 0x01:
						 mcu_dp_enum_update(DPID_MODE,0x00); //ö���������ϱ�;
					break;
				case 0x02:
						 mcu_dp_enum_update(DPID_MODE,0x01); //ö���������ϱ�;
					break;
				default:
					break;
			}

			break;
		case 4:
			//��ǰ��λ df_stats_s;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
			if(df_stats_s&0x04)	  //auto
			{
			   mcu_dp_enum_update(DPID_LEVEL,0x03); //ö���������ϱ�;
			}
			else
			switch (df_stats_s&0x07)
			{
				case 0x01: mcu_dp_enum_update(DPID_LEVEL,0x00); //ö���������ϱ�;
					break;
				case 0x02: mcu_dp_enum_update(DPID_LEVEL,0x01); //ö���������ϱ�;
					break;
				case 0x03: mcu_dp_enum_update(DPID_LEVEL,0x02); //ö���������ϱ�;
					break;
		
				default:
					break;
			}

			break;
		case 5:
			//��ǰ�±��л� set_c_f;     // 0 ���϶�  1 ���϶�
		    mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,set_c_f); //ö���������ϱ�;

			break;
		case 6:
//			mcu_dp_value_update(DPID_UPPER_TEMP,set_wd_upper_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
//			mcu_dp_value_update(DPID_LOWER_TEMP,set_wd_lower_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
		    //��ǰ�±��л� set_c_f;     
			if(set_c_f==0)	 // 0 ���϶�  1 ���϶�
			{
				mcu_dp_value_update(DPID_UPPER_TEMP,set_wd_upper_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
				mcu_dp_value_update(DPID_LOWER_TEMP,set_wd_lower_limit); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
			}
			else
			{
				mcu_dp_value_update(DPID_UPPER_TEMP_F,set_wd_upper_limit_f); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
				mcu_dp_value_update(DPID_LOWER_TEMP_F,set_wd_lower_limit_f); //��ǰ�¶��������ޣ��棩VALUE�������ϱ�;
			}

			break;
		case 7:
			//��ǰ����״̬
			switch (df_stats_f)	//����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
			{
				case 0x00:
						 mcu_dp_enum_update(DPID_WORK_STATE,0x02); //ö���������ϱ�;
					break;
				case 0x01:
						 mcu_dp_enum_update(DPID_WORK_STATE,0x00); //ö���������ϱ�;
					break;
				case 0x02:
						 mcu_dp_enum_update(DPID_WORK_STATE,0x01); //ö���������ϱ�;
					break;
				default:
					break;
			}

			break;
		default:
			break;
	}
   }
}

/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_switch_handle	����
�������� : ���DPID_SWITCH�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char switch1;
  
  switch1 = mcu_get_dp_download_bool(value,length);
  if(switch1 == 0)
  {
    //���ع�
	open_on=1;     //0 ���� 1 �ػ�
  }
  else
  {
	open_on=0;     //0 ���� 1 �ػ�
    //���ؿ�
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_SWITCH,switch1);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_temp_set_handle	Ŀ���¶�
�������� : ���DPID_TEMP_SET�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_temp_set_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long temp_set;
  
  temp_set = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
  if((temp_set)>set_wd_upper_limit)	   temp_set=set_wd_upper_limit;
  if((temp_set)<set_wd_lower_limit)	   temp_set=set_wd_lower_limit;

  set_t_num=temp_set*10;
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_TEMP_SET,temp_set);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_mode_handle	 ģʽ ���� ���� �ͷ�
�������� : ���DPID_MODE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
  switch(mode)
  {
    case 0:
      	   df_stats_f=1;  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
      break;
    case 1:
      	   df_stats_f=2;  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
      break;
    case 2:
      	   df_stats_f=0;  //����  ����״̬0x00ͨ��0x01�䷧0x02�ȷ�
      break;
    default:
      
      break;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_level_handle		��λ 
�������� : ���DPID_LEVEL�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_level_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char level;
  
  level = mcu_get_dp_download_enum(value,length);
  switch(level)
  {
    case 0:
      		df_stats_s=0x01;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
      break;
    case 1:
      		df_stats_s=0x02;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
      break;
    case 2:
      		df_stats_s=0x03;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
      break;
    case 3:
      		df_stats_s=df_stats_s|0x04;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
      break;
    case 4:
      		df_stats_s=0;  //����  ��4λ��3~0��������  0x00ͣ 0x01��0x02��0x03��
      break;
    default:
      
      break;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_LEVEL,level);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_temp_unit_convert_handle	  �±��л�
�������� : ���DPID_TEMP_UNIT_CONVERT�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_temp_unit_convert_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char temp_unit_convert;
  
  temp_unit_convert = mcu_get_dp_download_enum(value,length);
  switch(temp_unit_convert)
  {
    case 0:
      		set_c_f=0;     // 0 ���϶�  1 ���϶�
      break;
      
    case 1:
      		set_c_f=1;     // 0 ���϶�  1 ���϶�
      break;
      
    default:
      
      break;
  }

  get_c_f(1);  //  0 �õ��¶ȱ仯  1 �趨�¶ȱ仯


  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_TEMP_UNIT_CONVERT,temp_unit_convert);
  all_data_update();
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_temp_set_f_handle	Ŀ���¶Ȼ��϶�
�������� : ���DPID_TEMP_SET_F�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_temp_set_f_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long temp_set_f;
  
  temp_set_f = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */


  if((temp_set_f)>set_wd_upper_limit_f)	   temp_set_f=set_wd_upper_limit_f;
  if((temp_set_f)<set_wd_lower_limit_f)	   temp_set_f=set_wd_lower_limit_f;

  set_t_num=temp_set_f*10;
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_TEMP_SET_F,set_t_num/10);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
/*****************************************************************************
�������� : dp_download_upper_temp_handle	�¶��������� ��C
�������� : ���DPID_UPPER_TEMP�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_upper_temp_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long upper_temp;
  
  upper_temp = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */

//  set_wd_upper_limit=upper_temp; // �����¶����� 
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_UPPER_TEMP,set_wd_upper_limit);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_upper_temp_f_handle	 �¶��������� ��F
�������� : ���DPID_UPPER_TEMP_F�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_upper_temp_f_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long upper_temp_f;
  
  upper_temp_f = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
//  set_wd_upper_limit=upper_temp_f; // �����¶����� 
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_UPPER_TEMP_F,set_wd_upper_limit);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_lower_temp_handle	�¶��������� ��C
�������� : ���DPID_LOWER_TEMP�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_lower_temp_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long lower_temp;
  
  lower_temp = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
//  set_wd_lower_limit=lower_temp; // �����¶����� 
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_LOWER_TEMP,set_wd_lower_limit);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_lower_temp_f_handle	 �¶��������� ��F
�������� : ���DPID_LOWER_TEMP_F�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_lower_temp_f_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long lower_temp_f;
  
  lower_temp_f = mcu_get_dp_download_value(value,length);
  /*
  //VALUE�������ݴ���
  
  */
//  set_wd_lower_limit=lower_temp_f; // �����¶����� 
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_LOWER_TEMP_F,set_wd_lower_limit);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_valve_state_handle	  ����
�������� : ���DPID_VALVE_STATE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_valve_state_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char valve_state;
  
  valve_state = mcu_get_dp_download_enum(value,length);
  switch(valve_state)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    default:
      
      break;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_VALVE_STATE,valve_state);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/

/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : local_or_utc: true��local time�� false��UTCtime
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(bool local_or_utc, time_t* time)
{
  //#error "���������RTCʱ��д�����,��ɾ������"
  if(1 == local_or_utc)
  {
    //��ȷ���յ�zigbeeģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȷ���յ�zigbeeģ�鷵�ص�UTCʱ������ 
  }
}

#ifdef ZIGBEE_TEST_ENABLE
/*****************************************************************************
�������� : zigbee_test_result
�������� : zigbee���ܲ��Է���
������� : result:zigbee���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾzigbee�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void zigbee_test_result(bool result,unsigned char rssi)
{
    //#error "������ʵ��zigbee���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
//    if(result == 0)
//    {
//        //����ʧ��
//    }
//
//    else if(rssi == 0x00)
//    {
//        //��ָ���ŵ���δ��⵽��zigbee dongle,����dongle�ŵ�
//    }
//    else
//    {
//        //���Գɹ�
//        //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
//    }
  	  zigbee_rssi=rssi;	    //  �豸�����ź�ǿ��
	  if(zigbee_rssi==0) zigbee_net_time=18;
	  else zigbee_net_time=15;
}
#endif

unsigned char download_handle_sta=0;   //�в�����Ϣ
/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret = 0;
  download_handle_sta=1;
  zigbee_net=0x01;
  switch(dpid)
  {
    case DPID_SWITCH:
      //���ش�����
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_TEMP_SET:
      //�¶����ô�����
      ret = dp_download_temp_set_handle(value,length);
      break;
    case DPID_MODE:
      //ģʽ������
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_LEVEL:
      //��λ������
      ret = dp_download_level_handle(value,length);
      break;
    case DPID_TEMP_UNIT_CONVERT:
      //�±��л�������
      ret = dp_download_temp_unit_convert_handle(value,length);
      break;
    case DPID_TEMP_SET_F:
      //Ŀ���¶�_F������
      ret = dp_download_temp_set_f_handle(value,length);
      break;
    case DPID_UPPER_TEMP:
      //�¶��������ޣ��棩������
      ret = dp_download_upper_temp_handle(value,length);
      break;
    case DPID_UPPER_TEMP_F:
      //�¶��������ޣ��H��������
      ret = dp_download_upper_temp_f_handle(value,length);
      break;
    case DPID_LOWER_TEMP:
      //�¶��������ޣ��棩������
      ret = dp_download_lower_temp_handle(value,length);
      break;
    case DPID_LOWER_TEMP_F:
      //�¶��������ã��H��������
      ret = dp_download_lower_temp_f_handle(value,length);
      break;
    case DPID_VALVE_STATE:
      //����״̬������
      ret = dp_download_valve_state_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
