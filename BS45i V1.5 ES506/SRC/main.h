#ifdef  MAIN_GLOBALS_H
  #define MAIN_EXT
#else
  #define MAIN_EXT extern
#endif

#include "struct.h"

typedef enum 
{
  SLEEP_NONE=0,
  SLEEP_PRE, 
  SLEEP_ING
} SLEEP_STA;
MAIN_EXT volatile SLEEP_STA sleepStage;
MAIN_EXT unsigned char u8_sleepCnt;

MAIN_EXT unsigned char byTimer1S_RedOFFDelay;
MAIN_EXT unsigned char byTimer1S_YellowOFFDelay;

MAIN_EXT unsigned char g_byDCFanRun;
MAIN_EXT unsigned int  g_byADKey;
MAIN_EXT unsigned char g_byRemoteKey;
MAIN_EXT unsigned int  g_byADLight;
MAIN_EXT unsigned char g_LightCountInStrealth;
MAIN_EXT unsigned char g_LightCountOutStrealth;
#define STEALTH_DELAY_TM  30
#define STEALTH_IN_AD  170
#define STEALTH_OUT_AD 190
MAIN_EXT unsigned char g_byKeyFuncBak;
MAIN_EXT unsigned char g_byTimer1S_PowerON;
MAIN_EXT unsigned char g_byTimer1S_SleepRun;
MAIN_EXT unsigned int  g_wTimer1S_Sleep;
MAIN_EXT unsigned char g_byTimer1S_ON;
MAIN_EXT unsigned int  g_wTimer1S_OFF;
MAIN_EXT unsigned char g_byTimer1S_DisplayAll2S;
MAIN_EXT unsigned char g_Timer1S_PowerOn;//power on count
MAIN_EXT unsigned char g_byAQI;
MAIN_EXT unsigned char g_byAQIFan;
MAIN_EXT unsigned char g_byAQIColour;
MAIN_EXT unsigned char g_byAQIColourDisp;

MAIN_EXT unsigned int  g_FanTargetSpeed;

MAIN_EXT unsigned int  g_wDCFanError10M;	//forever error check interval 10M
MAIN_EXT unsigned char g_wPm25Grade;
MAIN_EXT unsigned char g_keyNetCfgJudgeDly;//delay to judge if new config is succ.
MAIN_EXT unsigned int  g_netConfigTImeout_u16; 
//--------------------------------------------------------------------------------------
MAIN_EXT unsigned char g_EERecordDelay;
#define EE_RECORD_DELAY 2

MAIN_EXT union flagbyte g_ucMainFlag1;
#define fg_ONOFF        g_ucMainFlag1.bt.b0
#define fg_ONOFF_LAST   g_ucMainFlag1.bt.b1
#define fg_Ionizer      g_ucMainFlag1.bt.b2
#define fg_Timer        g_ucMainFlag1.bt.b4
#define fg_ChildLock    g_ucMainFlag1.bt.b5
#define fg_Dark         g_ucMainFlag1.bt.b6
#define fg_AirLampFlash g_ucMainFlag1.bt.b7

MAIN_EXT union flagbyte g_ucMainFlag2;
#define fg_KeyNetConfig g_ucMainFlag2.bt.b0 //press key in smart config.
#define fg_LED_FLash    g_ucMainFlag2.bt.b1
#define fg_ShortTimer   g_ucMainFlag2.bt.b4
#define fg_FilterPress  g_ucMainFlag2.bt.b5
#define fg_DustADC       g_ucMainFlag2.bt.b6
#define fg_FactoryRecover g_ucMainFlag2.bt.b7

MAIN_EXT union flagbyte g_ucMainFilterFlag;
#define fg_Smart        g_ucMainFilterFlag.bt.b0
#define fg_Filter2      g_ucMainFilterFlag.bt.b1
#define fg_LED_WIFI      g_ucMainFilterFlag.bt.b2
#define fg_PowerOffWifiCfgSucc  g_ucMainFilterFlag.bt.b3
#define fg_Filter2Test  g_ucMainFilterFlag.bt.b4

MAIN_EXT union flagbyte g_ucMainFlag3;
#define fg_TimerTEST    g_ucMainFlag3.bt.b2
#define fg_Filter2Lock  g_ucMainFlag3.bt.b3
#define fg_Filter3Lock  g_ucMainFlag3.bt.b4

MAIN_EXT union flagbyte g_ucMainErrFlag;
#define fg_Err_EEprom   g_ucMainErrFlag.bt.b0
#define fg_Err_PM25     g_ucMainErrFlag.bt.b2
#define fg_UpdateSettinginfInEE    g_ucMainErrFlag.bt.b3 
#define fg_ParticleDown5MinStart   g_ucMainErrFlag.bt.b4 //=1,5mins start to count.
#define fg_PromptSetting           g_ucMainErrFlag.bt.b5 //power on,first time, propmt which setting is being used.

MAIN_EXT union flagbyte g_ucDisData[14];		

#define fg_LED_AUTO_MODE  		g_ucDisData[0].bt.b0 //auto mode 
#define fg_LED_AUTO   		    g_ucDisData[0].bt.b1 //auto led
#define fg_LED_WHITE_FAN		g_ucDisData[0].bt.b2//speed mode

#define fg_LED_BLUE_FAN_L       g_ucDisData[2].bt.b0 //fan speed 1-3
#define fg_LED_BLUE_FAN_M   	 g_ucDisData[2].bt.b1
#define fg_LED_BLUE_FAN_H		 g_ucDisData[2].bt.b2

#define fg_LED_BLUE_FAN_T       g_ucDisData[4].bt.b0
#define fg_LED_BLUE_TIMER_2H 	g_ucDisData[4].bt.b1
#define fg_LED_BLUE_TIMER_4H	g_ucDisData[4].bt.b2

#define fg_LED_BLUE_TIMER_12H   g_ucDisData[6].bt.b0
#define fg_LED_ANION_ONOFF		 g_ucDisData[6].bt.b1 //anion led
#define fg_LED_WHITE_ONOFF		 g_ucDisData[6].bt.b2

#define fg_LED_WHITE_TIMER		g_ucDisData[8].bt.b0
#define fg_LED_GREEN_FILTER	 	g_ucDisData[8].bt.b1
#define fg_LED_FILTER_KEY		g_ucDisData[8].bt.b2

#define fg_LED_ORANGE_FILTER	g_ucDisData[0x0a].bt.b0
#define fg_LED_ANION_KEY		 	g_ucDisData[0x0a].bt.b1//anion mode
#define fg_LED_LOCK_ONOFF		 	g_ucDisData[0x0a].bt.b2//lock led

#define fg_LED_RED_FILTER			g_ucDisData[12].bt.b0
#define fg_LED_LOCK_KEY			    g_ucDisData[12].bt.b1 // white lock key
#define fg_LED_WIFI_ONOFF			g_ucDisData[12].bt.b2 //wifi led

MAIN_EXT unsigned char g_ucDisLightLevel;
#define MAX_TOTAL_RUN_TIME 87600UL //(24*365*10)
MAIN_EXT unsigned long  u32_totalRunTime;
MAIN_EXT unsigned int  u16_1sCounterRunTime;
MAIN_EXT unsigned char WifiCfgState;
MAIN_EXT unsigned char g_factoryCnt;
MAIN_EXT unsigned char AutoSlowDown;// 0:enable;1:disable.

#define EE_DATA_LEN  0x40 //96 
#define EE_CHK_SUM   0x2E
#define EE_HEAD      0xAA
#define EE_SELFCHECK_SUCC_FLAG  0x5A

MAIN_EXT unsigned char e2_DCFanSp1;
MAIN_EXT unsigned char e2_DCFanSp2;
MAIN_EXT unsigned char e2_DCFanSp3;
MAIN_EXT unsigned char e2_DCFanSp4;

#define flashlen 12
MAIN_EXT unsigned int u32_flashData[flashlen]; //u32=unsigned int

#define e2_Head         u32_flashData[0]
#define e2_FilterDec    u32_flashData[1] //滤网寿命小数点部分
#define e2_FilterInt    u32_flashData[2] //滤网寿命整数部分
#define e2_Mode         u32_flashData[3] 
#define e2_Speed        u32_flashData[4] 
#define e2_Ionizer      u32_flashData[5] 
#define e2_fg_ONOFF     u32_flashData[6]
#define e2_sleep_OnOff  u32_flashData[7] //睡眠模式是否有效
#define e2_AutoSetting  u32_flashData[8] //自动模式选用的PM2.5数值区间编号（0-5）
#define e2_Runtime      u32_flashData[9] //reserved
#define e2_Check        u32_flashData[10]
#define e2_Tail         u32_flashData[11]

//#define CYC32Mhz
//#define   debug
#define   _DOG_OFF

