/*****************************************************************************
** ������ͿѻWiFiģ���WiFi���ģ�����
** V1.0 2019-11-8
** #modify ��Ŀ�е�WiFiָʾ�������־��Ҫ��Ϊfg_LED_WIFI.

*****************************************************************************/
#define WIFI_GLOBALS_H
#include "includes.h"

typedef enum 
{
  WIFI_NOT_CFG_STA=0, //������״̬
  WIFI_SMART_STA,     //smart����״̬
  WIFI_AP_STA         //ap����״̬
} CFG_STA;
CFG_STA local_WifiCfgState;

#define  WIFI_REPORT_PERIOD  5  //�ϱ�����

//������������֮���ӳ��ж�WiFiģ��״̬�ļ�ʱ��,��Ϊ������Ѿ�������״̬��������
//������ȡ״̬���������.
static unsigned char local_Dly2ReadWifiState=0;

static unsigned char f_WifiFlash=0;
static unsigned char local_WifiReportCounter=0xF0;
static unsigned int  local_WiFiConfigTimeCount=0; //����״̬��ʱ

/*****************************************************************************
�������� : ����ģʽ���ϱ�
������� : ��
���ز��� : ��
ʹ��˵�� : #modify ����ʵ������޸�
*****************************************************************************/
static void WiFi_WorkModeReport(void)
{
  if(fg_Smart)
  {
    mcu_dp_enum_update(DPID_MODE,AUTO_MODE);//�Զ�
  }
  else if(sleepStage!=SLEEP_NONE)
  {
    mcu_dp_enum_update(DPID_MODE,SLEEP_MODE);//˯��
  }
  else
  {
    mcu_dp_enum_update(DPID_MODE,MANNUAL_MODE);//�ֶ�
  }
}

/*****************************************************************************
�������� : ���ϵ��ϱ�
������� : ��
���ز��� : ��
ʹ��˵�� : #modify ����ʵ������޸�
*****************************************************************************/
static void WiFi_FaultReport(void)
{
  unsigned char ErrCode=0;

  if(f_Err_SensorPM25)//PM2.5����������
  {
    ErrCode|=0x01;
  }

  if(f_Err_DCMotor)//�������
  {
    ErrCode|=0x02;
  }

  mcu_dp_fault_update(DPID_FAULT,ErrCode);
}

/*****************************************************************************
�������� : WiFiָʾ����smart����ģʽ����˸����
������� : ��
���ز��� : ��
ʹ��˵�� : ÿ500ms����һ��. �����޸�.
*****************************************************************************/
static void WiFi_LedSmartFlash(void)
{
  if(WIFI_SMART_STA==local_WifiCfgState)
  {
    if(0==local_WiFiConfigTimeCount)//smart������ʱ,����AP����
    {
      local_WifiCfgState=WIFI_AP_STA;
      mcu_set_wifi_mode(AP_CONFIG);
      local_WiFiConfigTimeCount=100;//100s
    }
    else
    {
      f_WifiFlash = !f_WifiFlash;
      fg_LED_WIFI=f_WifiFlash;
    }
  }
}

/*****************************************************************************
�������� : WiFiָʾ����AP����ģʽ����˸����
������� : ��
���ز��� : ��
ʹ��˵�� : ÿ500ms����һ��.�����޸�.
*****************************************************************************/
static void WiFi_LedAPFlash(void)
{
  static unsigned char wifiCntAp=0;

  if(WIFI_AP_STA==local_WifiCfgState)
  {
    if(0==local_WiFiConfigTimeCount)
    {
      local_WifiCfgState=WIFI_NOT_CFG_STA;
    }
    else
    {
      wifiCntAp++;
      if(wifiCntAp>=2) //1s
      {
        wifiCntAp=0;
        f_WifiFlash = !f_WifiFlash;
        fg_LED_WIFI=f_WifiFlash;
      }
    }
  }
}

/*****************************************************************************
�������� : WiFiָʾ�ƴ����߼�
������� : ��
���ز��� : ��
ʹ��˵�� : �����޸�.
*****************************************************************************/
static void WiFi_LedJudge(void)
{
  if((_LIGHT_OFF==LIGHT_LEVEL)||(0==fg_ONOFF)||(SLEEP_NONE!=sleepStage))
  {
    fg_LED_WIFI=0;
    return;
  }
  
  switch(mcu_get_wifi_work_state())
  {
    case SMART_CONFIG_STATE:
    case AP_STATE:
      if(WIFI_NOT_CFG_STA==local_WifiCfgState)//while app remove the unit.
      {
        //��APP�Ƴ��豸��,WiFiģ����ܽ�������״̬,��ʱ�����û��������������,
        //WiFiָʾ��Ҫ�ر�.
        fg_LED_WIFI=0;
      }
      break;
    case WIFI_NOT_CONNECTED:
      if(WIFI_NOT_CFG_STA==local_WifiCfgState)
      {
        fg_LED_WIFI=0;
      }
      break;
    case WIFI_CONNECTED:
    case WIFI_CONN_CLOUD:
      if(0==local_Dly2ReadWifiState)
      {
        fg_LED_WIFI=1;
        local_WifiCfgState=WIFI_NOT_CFG_STA;
      }
      break;
    case WIFI_SATE_UNKNOW:
    case WIFI_LOW_POWER:
      break;
    default:
      break;
  }
}

/*****************************************************************************
�������� : WiFi��صļ�ʱ,ÿ�����һ��
������� : ��
���ز��� : ��
ʹ��˵�� : �����޸�.
*****************************************************************************/
static void WiFi_TimerSec(void)
{
  if(local_WifiCfgState!=WIFI_NOT_CFG_STA)
  {
    if(local_Dly2ReadWifiState>0)
    {
      local_Dly2ReadWifiState--;
    }

    if(local_WiFiConfigTimeCount>0)
    {
      local_WiFiConfigTimeCount--;
    }
  }
}

//=========================== �ⲿ���� =======================================
/*****************************************************************************
�������� : WiFi��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : �ϵ��ʱ�����һ��.
*****************************************************************************/
void WiFi_Init(void)
{
  wifi_protocol_init();
  f_WifiTxOver=1;
}
/*****************************************************************************
�������� : UART0��ѯ��ʽ����һ���ֽڵĺ���.
������� : ��
���ز��� : ��
ʹ��˵�� : ֻ������uart_transmit_output����.
           #modify ����MCU�޸�. 
*****************************************************************************/
void WiFi_UartPutChar(unsigned char sendByte)
{
  if(1==f_WifiTxOver)//�ȴ��ж��﷢��һ���ֽ����.
  {
    f_WifiTxOver=0;
    WiFi_TX_Start();       //��������.#modify ���ݲ�ͬ��Ƭ���޸�.
    UART_SendByte(UART0, sendByte);
    while(UART_GetFlagStatus(UART0, UART_FLAG_TC) == RESET);//�ȴ��ж��﷢��һ���ֽ����.
    UART_ClearITPendingBit(UART0, UART_CLR_TC);
  }
}

/*****************************************************************************
�������� : ��������WiFi����ģʽ�Ĵ���
������� : ��
���ز��� : ��
ʹ��˵�� : #modify ���������ð�ʵ������޸�.
*****************************************************************************/
void WiFi_KeyInWifiCfgMode(void)
{
  if(WIFI_NOT_CFG_STA==local_WifiCfgState)
  {
    local_WifiCfgState=WIFI_SMART_STA;
    mcu_set_wifi_mode(SMART_CONFIG);
    API_Buzzer_Set(BUZ_KEY);//#modify 
    local_Dly2ReadWifiState=8;
    local_WiFiConfigTimeCount=120;
  }
}
/*****************************************************************************
�������� : ģ������Լ�
������� : ��
���ز��� : ��
ʹ��˵�� : �����Լ�ʱ����.�����޸�.
*****************************************************************************/
void WiFi_SetWiFiSelfcheck(void)
{
  mcu_start_wifitest();
}

/*****************************************************************************
�������� : �������ݵ��ϱ�
������� : ��
���ز��� : ��
ʹ��˵�� : ���ڵ���,����ΪAPP������Ҫ���µ�����. ������all_data_update����.
           #modify ������Ҫ���λ��������ϱ�������.
*****************************************************************************/
void WiFi_AllDataUpdate(void)
{
  static unsigned char WiFiReportStage=0;

  switch(WiFiReportStage)
  {
    case 0:
      mcu_dp_bool_update(DPID_SWITCH,fg_ONOFF);//���ػ�
      WiFiReportStage++;
      break;
    case 1:
      mcu_dp_value_update(DPID_PM25,u16_Pm25Val);//PM2.5
      WiFiReportStage++;
      break;
    case 2:
      WiFi_WorkModeReport();//����ģʽ
      WiFiReportStage++;
      break;
    case 3:
      mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,g_wDCFanSpeedSet-1);//����
      WiFiReportStage++;
      break;
    case 4:
      mcu_dp_value_update(DPID_FILTER,FilterLeftPercent);//����ʣ������
      WiFiReportStage++;
      break;
    case 5:
      mcu_dp_bool_update(DPID_ANION,fg_Ionizer);//������
      WiFiReportStage++;
      break;
    case 6:
      mcu_dp_bool_update(DPID_CHILD_LOCK,fg_ChildLock);//ͯ��
      WiFiReportStage++;
      break;
    case 7:
      WiFi_FaultReport();//�����ϱ�
      WiFiReportStage++;
      break;
    case 8:
      mcu_dp_value_update(DPID_AIR_QUALITY,g_byAQIPM25);//���������ϱ�
      WiFiReportStage++;
      break;
    case 9:
      //mcu_dp_bool_update(DPID_SMARTSLEEPSWITCH,SleepModeSwitch);//����˯�߿���
      WiFiReportStage++;
      break;
    default:
      WiFiReportStage=0;
      local_WifiReportCounter=0;
      break;
  }
}

/*****************************************************************************
�������� : ���е�WiFi������.
������� : ��
���ز��� : ��
ʹ��˵�� : ������ѭ������.
*****************************************************************************/
void WiFi_Deal(void)
{
  static unsigned char workMode=0;
  static unsigned char fanSpeedSetLast=0;
  static unsigned char workModeLast=0;
  static unsigned char powerState=0;
  static unsigned char ionizerLast=0;
  static unsigned char childlockLast=0;
  static unsigned int  u16_Pm25ValLast=0;
  static unsigned int  u8_500msCnt=0;
  static unsigned char u8_filterLeftPercent=0;

  wifi_uart_service();

  if(API_Gets20MsFlag())
  {
    u8_500msCnt++;
    if(u8_500msCnt>=25)
    {
      u8_500msCnt=0;
      WiFi_LedSmartFlash();
      WiFi_LedAPFlash();
    }
  }

  WiFi_LedJudge();

  if(API_Gets1SFlag())//ÿ1s,#modify �޸�Ϊ��Ŀ��1s��ʱ���־.
  {
    local_WifiReportCounter++;
    WiFi_TimerSec();
  }

  if(local_WifiReportCounter>=WIFI_REPORT_PERIOD)//�����ϱ�
  {
    local_WifiReportCounter=0;
    WiFi_AllDataUpdate();
  }
  else //�仯�����ϱ�.#modify ������������Ŀ�޸�.
  {
    //ģʽ�ϱ�. #modify ģʽ�жϸ�����Ŀ�޸�.
    if(SLEEP_NONE!=sleepStage)
    {
      workMode=SLEEP_MODE;
    }
    else if(fg_Smart)
    {
      workMode=AUTO_MODE;
    }
    else
    {
      workMode=MANNUAL_MODE;
    }

    if(workModeLast!=workMode)
    {
      workModeLast=workMode;
      WiFi_WorkModeReport();
    }

    //���ٵȼ��ϱ�.
    if(fanSpeedSetLast!=g_byFanSpSet)
    {
      fanSpeedSetLast=g_byFanSpSet;
      //ע��:ֻ�з��ٵȼ���1��ʼ����������.
      mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,g_byFanSpSet-1);
    }

    //���ػ�״̬�ϱ�
    if(powerState!=fg_ONOFF)
    {
      powerState=fg_ONOFF;
      mcu_dp_bool_update(DPID_SWITCH,powerState);
    }

    //������
    if(ionizerLast!=fg_Ionizer)
    {
      ionizerLast=fg_Ionizer;
      mcu_dp_bool_update(DPID_ANION,ionizerLast);
    }

    //ͯ��
    if(childlockLast!=fg_ChildLock)
    {
      childlockLast=fg_ChildLock;
      mcu_dp_bool_update(DPID_CHILD_LOCK,childlockLast);
    }

    if(u16_Pm25ValLast != u16_Pm25Val)
    {
      u16_Pm25ValLast=u16_Pm25Val;
      mcu_dp_value_update(DPID_PM25,u16_Pm25Val);
    }

    if(u8_filterLeftPercent!=FilterLeftPercent)
    {
      u8_filterLeftPercent=FilterLeftPercent;
      mcu_dp_value_update(DPID_FILTER,FilterLeftPercent);//����ʣ������
    }
  }
}

