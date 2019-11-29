/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
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
** ��  ��: v2.5.2
** �ա���: 2019��7��5��
** �衡��: 1:����WiFi�����Բ��ԣ�����ָ��·�ɣ�
           2:����������̣��û�ֻ��Ҫ������������ӿڼ���ʹ��������
           3:����ͬ���ϱ�ָ��
           4:���ӻ�ȡ��ǰwifi����״ָ̬��
           5:���ӻ�ȡ����ʱ��ָ��
           6:�Ż�ota�������̣�����ota����ʱ�û�����ѡ�������С
           7:���ӻ�ȡģ��mac��ַָ��
           
** ��  ��: v2.5.1
** �ա���: 2018��10��27��
** �衡��: 1:Ĭ�Ϲر���������
           2:����03Э��wifi״̬�궨��
		   3:�������޸Ĳ��ֺ���ע��
		   
** ��  ��: v2.5.0
** �ա���: 2018��4��18��
** �衡��: 1:Э��汾��Ϊ0x03
           2:����WIFIģ�������رչ���
           3:������������

** ��  ��: v2.3.8
** �ա���: 2018��1��17��
** �衡��: 1:�������volatile��ֹ�������Ż�
           2:���#error��ʾ

** ��  ��: v2.3.7
** �ա���: 2017��4��18��
** �衡��: 1:�Ż����ڶ��н��մ���
		   
** ��  ��: v2.3.6
** �ա���: 2016��7��21��
** �衡��: 1:�޸���ȡ����ʱ�����
           2:���hex_to_bcdת������
		   
** ��  ��: v2.3.5
** �ա���: 2016��6��3��
** �衡��: 1:�޸ķ���Э��汾Ϊ0x01
           2:�̼���������ƫ�����޸�Ϊ4�ֽ�

** ��  ��: v2.3.4
** �ա���: 2016��5��26��
** �衡��: 1:�Ż����ڽ�������
           2:�Ż�������������,ȡ��enum���Ͷ���

** ��  ��: v2.3.3
** �ա���: 2016��5��24��
** �衡��: 1:�޸�mcu��ȡ����ʱ�亯��
           2:���wifi���ܲ���

** ��  ��: v2.3.2
** �ա���: 2016��4��23��
** �衡��: 1:�Ż��������ݽ���
           2:�Ż�MCU�̼���������
           3:�Ż��ϱ�����

** ��  ��: v2.3.1
** �ա���: 2016��4��15��
** �衡��: 1:�Ż��������ݽ���

** ��  ��: v2.3
** �ա���: 2016��4��14��
** �衡��: 1:֧��MCU�̼���������

** ��  ��: v2.2
** �ա���: 2016��4��11��
** �衡��: 1:�޸Ĵ������ݽ��շ�ʽ

** ��  ��: v2.1
** �ա���: 2016��4��8��
** �衡��: 1:����ĳЩ��������֧�ֺ���ָ�����ѡ��

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
           2:��ʡRAM�ռ�
**
**-----------------------------------------------------------------------------
******************************************************************************/

#include "wifi.h"
#include "..\buzzer.h"
#include "..\main.h"
#include "..\pm25.h"

#include "Test.h"
#include "key.h"
#include "mywifi.h"//#include "wifiMcu.h"
#include "filter.h"
#include "..\McuDrv.h"
#include "..\DCMotorDrv.h"
#include "Mode.h"

#ifdef WEATHER_ENABLE
/******************************************************************************
                        �������ݲ���ѡ������
          **�û������Զ�����Ҫ�Ĳ�����ע�ͻ���ȡ��ע�ͼ��ɣ�ע�����**         
******************************************************************************/
const char weather_choose[WEATHER_CHOOSE_CNT][10] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",*/
 };
#endif

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_PM25, DP_TYPE_VALUE},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_FAN_SPEED_ENUM, DP_TYPE_ENUM},
  {DPID_FILTER, DP_TYPE_VALUE},
  {DPID_ANION, DP_TYPE_BOOL},
  {DPID_CHILD_LOCK, DP_TYPE_BOOL},
  {DPID_AIR_QUALITY, DP_TYPE_ENUM},
  {DPID_FAULT, DP_TYPE_BITMAP},
};

extern void wifiSendByte(unsigned char val);
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
void uart_transmit_output(unsigned char value)
{
  wifiSendByte(value);
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

//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{  
 /* 
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
  mcu_dp_bool_update(DPID_SWITCH,��ǰ����); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_PM25,��ǰPM2.5); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_MODE,��ǰģʽ); //ö���������ϱ�;
  mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,��ǰ����); //ö���������ϱ�;
  mcu_dp_value_update(DPID_FILTER,��ǰ��оʹ����); //VALUE�������ϱ�;
  mcu_dp_bool_update(DPID_ANION,��ǰ������); //BOOL�������ϱ�;
  mcu_dp_bool_update(DPID_CHILD_LOCK,��ǰͯ������); //BOOL�������ϱ�;
  mcu_dp_enum_update(DPID_AIR_QUALITY,��ǰ��������); //ö���������ϱ�;
  mcu_dp_fault_update(DPID_FAULT,��ǰ���ϸ澯); //�����������ϱ�;
 */
  WiFi_AllDataUpdate();
}
/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_switch_handle
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
  unsigned char sw;

  sw = mcu_get_dp_download_bool(value,length);
  if(sw == 0)
  {
    powerSet(0);//sub_Key_ONOFF();//power off
  }
  else
  {
    powerSet(1);//sub_Key_ONOFF();//power on
  }


  ret = mcu_dp_bool_update(DPID_SWITCH,sw);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_mode_handle
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
    case MANNUAL_MODE:
      //mannual
      modeSet(mode);
      break;
    case AUTO_MODE:
      //auto
      modeSet(mode);
      break;
    case SLEEP_MODE:
      //sleep
      modeSet(mode);
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
�������� : dp_download_fan_speed_enum_handle
�������� : ���DPID_FAN_SPEED_ENUM�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_fan_speed_enum_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char fan_speed_enum;
  
  fg_Smart=0;
  fan_speed_enum = mcu_get_dp_download_enum(value,length);
  switch(fan_speed_enum)
    {
      case 0:
        API_Buzzer_Set ( BUZ_KEY );
        g_byFanSpSet = FAN_SPEED1;
        FanGearSeqInSpeedArray=g_byFanSpSet-1;
        break;
      case 1:
        API_Buzzer_Set ( BUZ_KEY );
        g_byFanSpSet = FAN_SPEED2;
        FanGearSeqInSpeedArray=g_byFanSpSet-1;
        break;
      case 2:
        API_Buzzer_Set ( BUZ_KEY );
        g_byFanSpSet = FAN_SPEED3;
        FanGearSeqInSpeedArray=g_byFanSpSet-1;
        break;
      case 3:
        API_Buzzer_Set ( BUZ_KEY );
        g_byFanSpSet = FAN_SPEED4;
        FanGearSeqInSpeedArray=g_byFanSpSet-1;
        break;
      default:
        break;
    }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,fan_speed_enum);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_anion_handle
�������� : ���DPID_ANION�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_anion_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char anion;
  
  anion = mcu_get_dp_download_bool(value,length);
  if(anion == 0)
  {
    anionSet(0);//anion off
    API_Buzzer_Set ( BUZ_KEY );
  }
  else
  {
    anionSet(1);//anion on,while fan is running
    API_Buzzer_Set ( BUZ_KEY );
  }

  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_ANION,anion);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}
/*****************************************************************************
�������� : dp_download_child_lock_handle
�������� : ���DPID_CHILD_LOCK�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_child_lock_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char child_lock;
  
  child_lock = mcu_get_dp_download_bool(value,length);
  if(child_lock == 0)
  {
    lockSet(0);//lock off
  }
  else
  {
    lockSet(1);//lock on
  }

  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_CHILD_LOCK,child_lock);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}


/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_GREEN_TIME
/*****************************************************************************
�������� : mcu_get_greentime
�������� : ��ȡ���ĸ���ʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_get_greentime(unsigned char time[])
{
#error "�����������ش���,��ɾ������"
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
*/
  if(time[0] == 1)
  {
    //��ȷ���յ�wifiģ�鷵�صĸ�������
  }
  else
  {
  	//��ȡ����ʱ�����,�п����ǵ�ǰwifiģ��δ����
  }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  /*
  time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
  time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
  time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
  time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
  time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
  time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
  time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
 */
  if(time[0] == 1)
  {
    //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
  if(result == 0)//fail
  {
    //����ʧ��
    if(rssi == 0x00)
    {
      f_TestErr_Wifi=1;//δɨ�赽����Ϊtuya_mdev_test·����,����
    }
    else if(rssi == 0x01)
    {
      f_TestErr_Wifi=1;//ģ��δ��Ȩ
    }
  }
  else //succ
  {
    if(rssi>=60)//���Գɹ�
    {
      f_TestErr_Wifi=0;
    }
  }
}
#endif
#ifdef WIFI_CONNECT_TEST_ENABLE
/*****************************************************************************
�������� : wifi_connect_test_result
�������� : ·����Ϣ���ս��֪ͨ
������� : result:ģ���Ƿ�ɹ����յ���ȷ��·����Ϣ;0:ʧ��/1:�ɹ�
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_connect_test_result(unsigned char result)
{
  #error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
  if(result == 0)
  {
    //·����Ϣ����ʧ�ܣ����鷢����·����Ϣ���Ƿ���������JSON���ݰ�
  }
  else
  {
    //·����Ϣ���ճɹ�����������ע��WIFI_STATE_CMDָ���wifi����״̬
  }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : mac��mac[0]Ϊ�Ƿ��ȡmac�ɹ���־��0x00 ��ʾ�ɹ���Ϊ0x01��ʾʧ��
  mac[1]~mac[6]:����ȡ MAC��ַ��־λ���mac[0]Ϊ�ɹ������ʾģ����Ч��MAC��ַ
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_get_mac(unsigned char mac[])
{
  #error "���������mac��ȡ����,��ɾ������"
  /*
  mac[0]Ϊ�Ƿ��ȡmac�ɹ���־��0x00 ��ʾ�ɹ���Ϊ0x01��ʾʧ��
  mac[1]~mac[6]:����ȡ MAC��ַ��־λ���mac[0]Ϊ�ɹ������ʾģ����Ч��MAC��ַ
 */
 
  if(mac[0] == 1)
  {
  	//��ȡmac����
  }
  else
  {
    //��ȷ���յ�wifiģ�鷵�ص�mac��ַ 
  }
}
#endif





#ifdef GET_WIFI_STATUS_ENABLE
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : ��ȡ WIFI ״̬���
������� : result:ָʾ WIFI ����״̬
           0x00:wifi״̬ 1 smartconfig ����״̬
           0x01:wifi״̬ 2 AP ����״̬
           0x02:wifi״̬ 3 WIFI �����õ�δ����·����
           0x03:wifi״̬ 4 WIFI ������������·����
           0x04:wifi״̬ 5 ������·���������ӵ��ƶ�
           0x05:wifi״̬ 6 WIFI �豸���ڵ͹���ģʽ
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void get_wifi_status(unsigned char result)
{
  #error "���������mac��ȡ����,��ɾ������"
 
  switch(result) {
    case 0:
      //wifi����״̬1
      break;
  
    case 1:
      //wifi����״̬2
      break;
      
    case 2:
      //wifi����״̬3
      break;
      
    case 3:
      //wifi����״̬4
      break;
      
    case 4:
      //wifi����״̬5
      break;
      
    case 5:
      //wifi����״̬6
      break;
      
    default:
    break;
  }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/*****************************************************************************
�������� : get_upload_syn_result
�������� : ״̬ͬ���ϱ����
������� : result:0x00ʧ��  0x01�ɹ�
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void get_upload_syn_result(unsigned char result)
{
  #error "���������mac��ȡ����,��ɾ������"
  /*
  mac[0]Ϊ�Ƿ��ȡmac�ɹ���־��0x00 ��ʾ�ɹ���Ϊ0x01��ʾʧ��
  mac[1]~mac[6]:����ȡ MAC��ַ��־λ���mac[0]Ϊ�ɹ������ʾģ����Ч��MAC��ַ
 */
 
  if(result == 0)
  {
  	//��ȡmac����
  }
  else
  {
    //��ȷ���յ�wifiģ�鷵�ص�mac��ַ 
  }
}
#endif


#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : upgrade_package_choose
�������� : ��������Сѡ��
������� : package_sz:��������С
           0x00��Ĭ�� 256byte
           0x01��512byte 
           0x02��1024byte

���ز��� : ��
*****************************************************************************/
void upgrade_package_choose(unsigned char package_sz)
{
  unsigned short length = 0;
  length = set_wifi_uart_byte(length,package_sz);
  wifi_uart_write_frame(UPDATE_START_CMD,length);
}



/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "���������MCU�̼���������,��ɺ���ɾ������"
  if(length == 0)
  {
    //�̼����ݷ������
    
  }
  else
  {
    //�̼����ݴ���
  }
  
  return SUCCESS;
}
#endif
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
  unsigned char ret;
  switch(dpid)
  {
    case DPID_SWITCH:
      //���ش�����
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_MODE:
      //ģʽ������
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_FAN_SPEED_ENUM:
      //���ٴ�����
      ret = dp_download_fan_speed_enum_handle(value,length);
      break;
    case DPID_ANION:
      //�����Ӵ�����
      ret = dp_download_anion_handle(value,length);
      break;
    case DPID_CHILD_LOCK:
      //ͯ�����ش�����
      ret = dp_download_child_lock_handle(value,length);
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

#ifdef WEATHER_ENABLE

/*****************************************************************************
�������� : weather_open_return
�������� : ���������ܷ����û��Դ�����
������� : 1:res:���������ܷ��ؽ����1:�ɹ���0��ʧ��

���ز��� : ��
*****************************************************************************/
void weather_open_return_handle(unsigned char res, unsigned char err)
{
  #error "���������M���������ܷ������ݴ������,��ɺ���ɾ������"
  unsigned char err_num = 0;
  
  if(res == 1)
  {
    //���������سɹ�
  }
  else if(res == 0)
  {
    //����������ʧ��
    err_num = err;//��ȡ������
  }
}

/*****************************************************************************
�������� : weather_data_user_handle
�������� : ���������û��Դ�����
������� : name:������
           type:�������ͣ�0��int�ͣ�1��string��
           data:����ֵ�ĵ�ַ
���ز��� : ��
*****************************************************************************/
void weather_data_user_handle(char *name, unsigned char type, char *data)
{
  #error "���������ʾ���������������������ݴ������,��ɺ���ɾ������"
  int value_int;
  char value_string[50];//�����еĲ������ݽ϶࣬����Ĭ��Ϊ50�������Ը��ݶ���Ĳ����������ʵ����ٸ�ֵ
  
  my_memset(value_string, '/0', 50);
  
  //���Ȼ�ȡ��������
  if(type == 0)//������INT��
  {
    value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
  }
  else if(type == 1)
  {
    my_strcpy(value_string, data);
  }
  
  //ע��Ҫ������ѡ������������ò���ֵ������
  if(my_strcmp(name, "temp") == 0)
  {
    //printf("temp value is:%d", value_int);            //int��
  }
  else if(my_strcmp(name, "humidity") == 0)
  {
    //printf("humidity value is:%d", value_int);        //int��
  }
  else if(my_strcmp(name, "pm25") == 0)
  {
    //printf("pm25 value is:%d", value_int);            //int��
  }
  else if(my_strcmp(name, "condition") == 0)
  {
    //printf("condition value is:%s", value_string);    //string��
  }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/*****************************************************************************
�������� : stream_file_trans
�������� : �������ļ�����
������� : id:ID��
          buffer:���Ͱ��ĵ�ַ
          buf_len:���Ͱ�����
���ز��� : ��
*****************************************************************************/
unsigned char stream_file_trans(unsigned int id, unsigned char *buffer, unsigned long buf_len)
{
  #error "���������ʾ�������������������������,��ɺ���ɾ������"
  unsigned short length = 0;
  unsigned long map_offset = 0;
  unsigned int pack_num = 0;
  unsigned int rest_length = 0;

  if(stop_update_flag == ENABLE)
    return SUCCESS;

  pack_num = buf_len / STREM_PACK_LEN;
  rest_length = buf_len - pack_num * STREM_PACK_LEN;
  if (rest_length > 0)
  {
    pack_num++;
  }

  int this_len = STREM_PACK_LEN;
  for (int cnt = 0; cnt < pack_num; cnt++)
  {
    if (cnt == pack_num - 1 && rest_length > 0)
    {
      this_len = rest_length;
    }
    else
    {
      this_len = STREM_PACK_LEN;
    }

    if(SUCCESS == stream_trans(id, map_offset, buffer + map_offset, this_len))
    {
      //mcu���������У������Խ�����������
      //printf("is upgrade\n");
      return SUCCESS;
    }

    //while(stream_status == 0xff);//�յ�����
    
    if(stream_status != 0)
    {
      return ERROR;
    }
  }
  
  return SUCCESS;
}

#endif
