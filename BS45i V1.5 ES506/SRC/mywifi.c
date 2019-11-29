/*****************************************************************************
** 适用于涂鸦WiFi模块的WiFi相关模块程序
** V1.0 2019-11-8
** #modify 项目中的WiFi指示灯亮灭标志需要改为fg_LED_WIFI.

*****************************************************************************/
#define WIFI_GLOBALS_H
#include "includes.h"

typedef enum 
{
  WIFI_NOT_CFG_STA=0, //非配网状态
  WIFI_SMART_STA,     //smart配网状态
  WIFI_AP_STA         //ap配网状态
} CFG_STA;
CFG_STA local_WifiCfgState;

#define  WIFI_REPORT_PERIOD  5  //上报周期

//按键进入配网之后延迟判断WiFi模块状态的计时器,因为如果是已经在联网状态进入配网
//立即读取状态会造成误判.
static unsigned char local_Dly2ReadWifiState=0;

static unsigned char f_WifiFlash=0;
static unsigned char local_WifiReportCounter=0xF0;
static unsigned int  local_WiFiConfigTimeCount=0; //配网状态计时

/*****************************************************************************
功能描述 : 工作模式的上报
输入参数 : 无
返回参数 : 无
使用说明 : #modify 根据实际情况修改
*****************************************************************************/
static void WiFi_WorkModeReport(void)
{
  if(fg_Smart)
  {
    mcu_dp_enum_update(DPID_MODE,AUTO_MODE);//自动
  }
  else if(sleepStage!=SLEEP_NONE)
  {
    mcu_dp_enum_update(DPID_MODE,SLEEP_MODE);//睡眠
  }
  else
  {
    mcu_dp_enum_update(DPID_MODE,MANNUAL_MODE);//手动
  }
}

/*****************************************************************************
功能描述 : 故障的上报
输入参数 : 无
返回参数 : 无
使用说明 : #modify 根据实际情况修改
*****************************************************************************/
static void WiFi_FaultReport(void)
{
  unsigned char ErrCode=0;

  if(f_Err_SensorPM25)//PM2.5传感器故障
  {
    ErrCode|=0x01;
  }

  if(f_Err_DCMotor)//电机故障
  {
    ErrCode|=0x02;
  }

  mcu_dp_fault_update(DPID_FAULT,ErrCode);
}

/*****************************************************************************
功能描述 : WiFi指示灯在smart配网模式的闪烁控制
输入参数 : 无
返回参数 : 无
使用说明 : 每500ms调用一次. 不用修改.
*****************************************************************************/
static void WiFi_LedSmartFlash(void)
{
  if(WIFI_SMART_STA==local_WifiCfgState)
  {
    if(0==local_WiFiConfigTimeCount)//smart配网超时,进入AP配网
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
功能描述 : WiFi指示灯在AP配网模式的闪烁控制
输入参数 : 无
返回参数 : 无
使用说明 : 每500ms调用一次.不用修改.
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
功能描述 : WiFi指示灯处理逻辑
输入参数 : 无
返回参数 : 无
使用说明 : 不用修改.
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
        //当APP移除设备后,WiFi模块可能进入配网状态,这时候不是用户主动进入的配网,
        //WiFi指示灯要关闭.
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
功能描述 : WiFi相关的计时,每秒调用一次
输入参数 : 无
返回参数 : 无
使用说明 : 不用修改.
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

//=========================== 外部调用 =======================================
/*****************************************************************************
功能描述 : WiFi初始化
输入参数 : 无
返回参数 : 无
使用说明 : 上电的时候调用一次.
*****************************************************************************/
void WiFi_Init(void)
{
  wifi_protocol_init();
  f_WifiTxOver=1;
}
/*****************************************************************************
功能描述 : UART0查询方式发送一个字节的函数.
输入参数 : 无
返回参数 : 无
使用说明 : 只被函数uart_transmit_output调用.
           #modify 根据MCU修改. 
*****************************************************************************/
void WiFi_UartPutChar(unsigned char sendByte)
{
  if(1==f_WifiTxOver)//等待中断里发送一个字节完毕.
  {
    f_WifiTxOver=0;
    WiFi_TX_Start();       //开启发送.#modify 根据不同单片机修改.
    UART_SendByte(UART0, sendByte);
    while(UART_GetFlagStatus(UART0, UART_FLAG_TC) == RESET);//等待中断里发送一个字节完毕.
    UART_ClearITPendingBit(UART0, UART_CLR_TC);
  }
}

/*****************************************************************************
功能描述 : 按键进入WiFi配网模式的处理
输入参数 : 无
返回参数 : 无
使用说明 : #modify 蜂鸣器调用按实际情况修改.
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
功能描述 : 模块进入自检
输入参数 : 无
返回参数 : 无
使用说明 : 整机自检时调用.不用修改.
*****************************************************************************/
void WiFi_SetWiFiSelfcheck(void)
{
  mcu_start_wifitest();
}

/*****************************************************************************
功能描述 : 所有数据的上报
输入参数 : 无
返回参数 : 无
使用说明 : 周期调用,周期为APP数据需要更新的周期. 被函数all_data_update调用.
           #modify 根据需要屏蔽或者新增上报的内容.
*****************************************************************************/
void WiFi_AllDataUpdate(void)
{
  static unsigned char WiFiReportStage=0;

  switch(WiFiReportStage)
  {
    case 0:
      mcu_dp_bool_update(DPID_SWITCH,fg_ONOFF);//开关机
      WiFiReportStage++;
      break;
    case 1:
      mcu_dp_value_update(DPID_PM25,u16_Pm25Val);//PM2.5
      WiFiReportStage++;
      break;
    case 2:
      WiFi_WorkModeReport();//工作模式
      WiFiReportStage++;
      break;
    case 3:
      mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,g_wDCFanSpeedSet-1);//风速
      WiFiReportStage++;
      break;
    case 4:
      mcu_dp_value_update(DPID_FILTER,FilterLeftPercent);//滤网剩余寿命
      WiFiReportStage++;
      break;
    case 5:
      mcu_dp_bool_update(DPID_ANION,fg_Ionizer);//负离子
      WiFiReportStage++;
      break;
    case 6:
      mcu_dp_bool_update(DPID_CHILD_LOCK,fg_ChildLock);//童锁
      WiFiReportStage++;
      break;
    case 7:
      WiFi_FaultReport();//故障上报
      WiFiReportStage++;
      break;
    case 8:
      mcu_dp_value_update(DPID_AIR_QUALITY,g_byAQIPM25);//空气质量上报
      WiFiReportStage++;
      break;
    case 9:
      //mcu_dp_bool_update(DPID_SMARTSLEEPSWITCH,SleepModeSwitch);//智能睡眠开关
      WiFiReportStage++;
      break;
    default:
      WiFiReportStage=0;
      local_WifiReportCounter=0;
      break;
  }
}

/*****************************************************************************
功能描述 : 所有的WiFi处理工作.
输入参数 : 无
返回参数 : 无
使用说明 : 主函数循环调用.
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

  if(API_Gets1SFlag())//每1s,#modify 修改为项目中1s的时间标志.
  {
    local_WifiReportCounter++;
    WiFi_TimerSec();
  }

  if(local_WifiReportCounter>=WIFI_REPORT_PERIOD)//周期上报
  {
    local_WifiReportCounter=0;
    WiFi_AllDataUpdate();
  }
  else //变化立即上报.#modify 变量名根据项目修改.
  {
    //模式上报. #modify 模式判断根据项目修改.
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

    //风速等级上报.
    if(fanSpeedSetLast!=g_byFanSpSet)
    {
      fanSpeedSetLast=g_byFanSpSet;
      //注意:只有风速等级从1开始才能这样用.
      mcu_dp_enum_update(DPID_FAN_SPEED_ENUM,g_byFanSpSet-1);
    }

    //开关机状态上报
    if(powerState!=fg_ONOFF)
    {
      powerState=fg_ONOFF;
      mcu_dp_bool_update(DPID_SWITCH,powerState);
    }

    //负离子
    if(ionizerLast!=fg_Ionizer)
    {
      ionizerLast=fg_Ionizer;
      mcu_dp_bool_update(DPID_ANION,ionizerLast);
    }

    //童锁
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
      mcu_dp_value_update(DPID_FILTER,FilterLeftPercent);//滤网剩余寿命
    }
  }
}

