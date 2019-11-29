#define KEY_GLOBALS_H
#include "includes.h"

#define   _KEY_NONE     0
#define   _KEY_ONOFF    1
#define   _KEY_FAN      2
#define   _KEY_LIGHT    3
#define   _KEY_LOCK     4
#define   _KEY_FILTER   5
#define   _KEY_IONIZER_FILTER  6
#define   _KEY_IONIZER  7
#define   _KEY_AUTO     8

#define _AD_ERR     20
#define _AD_IONIZER 2499
#define _AD_FILTER  2849
#define _AD_TIMER   2089
#define _AD_POWER   871
#define _AD_SPEED   3295//3306
#define _AD_AUTO    459
#define _AD_LOCK    1284
#define _AD_ION_FIL 1976

static union flagbyte ucKeyFlag;
#define f_Key_Done            ucKeyFlag.bt.b0

static unsigned int  wTimer20Ms_KeyPress;
static unsigned char  FanSpeedVal[6]= {1,2,3,4,3,2};//由于客户要求按键按此顺序循环.

/*******************************************************************************
** 根据AD数值判断是哪个按键.
*******************************************************************************/
static unsigned char readKey(void)
{
  unsigned char byKeyFunction;

  if((g_byADKey<=_AD_AUTO+_AD_ERR) && (g_byADKey>=_AD_AUTO-_AD_ERR))
  {
    byKeyFunction=_KEY_AUTO;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_POWER+_AD_ERR) && (g_byADKey>=_AD_POWER-_AD_ERR))
  {
    byKeyFunction = _KEY_ONOFF;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_LOCK+_AD_ERR) && (g_byADKey>=_AD_LOCK-_AD_ERR))
  {
    byKeyFunction = _KEY_LOCK;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_ION_FIL+_AD_ERR) && (g_byADKey>=_AD_ION_FIL-_AD_ERR))
  {
    byKeyFunction = _KEY_IONIZER_FILTER;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_TIMER+_AD_ERR) && (g_byADKey>=_AD_TIMER-_AD_ERR))
  {
    byKeyFunction = _KEY_LIGHT;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_IONIZER+_AD_ERR) && (g_byADKey>=_AD_IONIZER-_AD_ERR))
  {
    byKeyFunction = _KEY_IONIZER;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_FILTER+_AD_ERR) && (g_byADKey>=_AD_FILTER-_AD_ERR))
  {
    byKeyFunction = _KEY_FILTER;
    byTimer1S_DarkIn=0;
  }
  else if((g_byADKey<=_AD_SPEED+_AD_ERR) && (g_byADKey>=_AD_SPEED-_AD_ERR))
  {
    byKeyFunction = _KEY_FAN;
    byTimer1S_DarkIn=0;
  }
  else
  {
    byKeyFunction = _KEY_NONE;
  }
  return byKeyFunction;
}
/*******************************************************************************
** 恢复出厂设置
*******************************************************************************/
static void KeyFactoryDefault(void)
{
  fg_FactoryRecover=1;
  g_factoryCnt=0;
  fg_Smart=0;
  g_byFanSpSet=FAN_SPEED1;
  FanGearSeqInSpeedArray=0;
  fg_Ionizer=0;
  u32_totalRunTime=0;
  g_FilerUseTimeCnt_fl=0;
  u16_Pm25Val=0;
  g_RingLastTime=0;
  AutoSetting=2;
  CalFilterRecValEE();
  filterPercentCal();
  mcu_set_wifi_mode(SMART_CONFIG);
}

/*******************************************************************************
** 睡眠唤醒逻辑
*******************************************************************************/
static unsigned char APISleepAwaken(void)
{
  if(sleepStage!=SLEEP_NONE)
  {
    sleepStage=SLEEP_NONE;
    API_Buzzer_Set ( BUZ_KEY );
    JudgeRingColour();

    if(_LIGHT_OFF==LIGHT_LEVEL)
    {
      LIGHT_LEVEL=_LIGHT_50;
    }
    return 1;
  }
  else if(_LIGHT_OFF==LIGHT_LEVEL)
  {
    LIGHT_LEVEL=_LIGHT_50;
    API_Buzzer_Set(BUZ_KEY);
    JudgeRingColour();
    return 1;
  }
  else if(1==fg_Dark)
  {
    fg_Dark=0;
    API_Buzzer_Set(BUZ_KEY);
    JudgeRingColour();
    return 1;
  }
  else
  {
    return 0;
  }
}

/*******************************************************************************
** 负离子按键
*******************************************************************************/
static void KeyIonizer(void)
{
  if(f_Test)
  {
    if(1==g_TestState) //now is fan key test mode
    {
      g_TestState++;     //stage 3: pressed key ionizer
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  if(1==APISleepAwaken())
  {
    return;
  }

  fg_Ionizer=!fg_Ionizer;
  g_EERecordDelay=EE_RECORD_DELAY;
  API_Buzzer_Set(BUZ_KEY);
  if(fg_Ionizer)
  {
    mcu_dp_bool_update(DPID_ANION,1);
  }
  else
  {
    mcu_dp_bool_update(DPID_ANION,0);
  }
}
/*******************************************************************************
** 负离子按键长按.
*******************************************************************************/
static void KeyIonizerLongPressed(void)
{
  g_byKeyFuncBak = _KEY_NONE;

  if((0==fg_ONOFF)||(sleepStage!=SLEEP_NONE))
  {
    return;
  }
  API_Buzzer_Set ( BUZ_KEY );

  if(g_FilerUseTimeCnt_fl<0xFFFF-100)
  {
    g_FilerUseTimeCnt_fl=g_FilerUseTimeCnt_fl+100.0;
  }
  else
  {
    g_FilerUseTimeCnt_fl=65535.0;
  }
  CalFilterRecValEE();
  filterPercentCal();
}
/*******************************************************************************
** 负离子按键和滤网按键同时按下
*******************************************************************************/
static void KeyIonizerFilter(void)
{
  if(1==fg_ONOFF)
  {
    return;
  }

  if(1==f_selfCheck)
  {
    return;
  }

  f_selfCheck=1;
  f_Test=1;
  testIni();
}
/*******************************************************************************
** 滤网按键
*******************************************************************************/
static void KeyFilter(void)
{
  if(f_Test)
  {
    if(2==g_TestState)
    {
      g_TestState++;
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  if(1==APISleepAwaken())  //stealth awaken
  {
    return;
  }

  //if(fg_ChildLock)
  //{
    //return;
  //}
}
/*******************************************************************************
** 长按滤网键
*******************************************************************************/
static void KeyFilterReset(void)
{
  g_byKeyFuncBak=_KEY_NONE;

  if(f_Test)
  {
    if(2==g_TestState)
    {
      g_TestState++;
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(sleepStage!=SLEEP_NONE)
  {
    g_byKeyFuncBak=_KEY_FILTER;
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  g_FilerUseTimeCnt_fl=0;
  CalFilterRecValEE();
  filterPercentCal();
  API_Buzzer_Set(BUZ_KEY);
}
/*******************************************************************************
** LIGHT按键
*******************************************************************************/
static void KeyLight(void)
{
  if(f_Test)
  {
    if(3==g_TestState)  
    {
      g_TestState++; //stage 5
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(1==APISleepAwaken())  
  {
    //if(_LIGHT_OFF!=LIGHT_LEVEL)
    //{
      return;
    //}
  }

  if(fg_ChildLock)
  {
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  API_Buzzer_Set(BUZ_KEY);
  LIGHT_LEVEL++;
  if(LIGHT_LEVEL>_LIGHT_OFF)
  {
    LIGHT_LEVEL=_LIGHT_50;
  }

  //if(_LIGHT_OFF==LIGHT_LEVEL)
  //{
    //modeSet(SLEEP_MODE);
  //}
}

/*******************************************************************************
** LIGHT键长按
*******************************************************************************/
static void KeyLightLongPressed(void)
{
  g_byKeyFuncBak=_KEY_LIGHT;
  if(sleepStage!=SLEEP_NONE)
  {
    g_byKeyFuncBak=_KEY_FILTER;
    return;
  }

  if(0==fg_PromptSetting)
  {
    fg_PromptSetting=1;
  }
  else
  {
    AutoSetting++;
    if(AutoSetting>5)
    {
      AutoSetting=0;
    }
    g_EERecordDelay=EE_RECORD_DELAY;
  }
  API_Buzzer_Set(BUZ_Continuous);
}

/*******************************************************************************
** 开机处理.
*******************************************************************************/
void PowerOnDeal(void)
{
  fg_ONOFF = 1;
  sleepStage=SLEEP_NONE;
  g_byTimer1S_ON = 0;
  JudgeRingColour();
  API_Buzzer_Set ( BUZ_ON );
  mcu_dp_bool_update(DPID_SWITCH,1);
  SENSOR_POWER_ON;
}

/*******************************************************************************
** 关机处理.
*******************************************************************************/
void PowerOffDeal(void)
{
  fg_ONOFF=0;
  sleepStage=SLEEP_NONE;
  RGBOff();
  API_Buzzer_Set(BUZ_OFF);
  mcu_dp_bool_update(DPID_SWITCH,0);
  mcu_dp_value_update(DPID_PM25,0);
  g_Timer1S_PowerOn=0;
  fg_ChildLock = 0;
  mcu_dp_bool_update(DPID_CHILD_LOCK,0);
  u16_Pm25Val=0;
  if(1==fg_Smart)
  {
    g_byFanSpSet=FAN_SPEED1;
    FanGearSeqInSpeedArray=g_byFanSpSet-1;
  }
  g_RingLastTime=0;
  ringColor=blue;
  if(_LIGHT_OFF==LIGHT_LEVEL)
  {
    LIGHT_LEVEL=_LIGHT_100;
  }
  SENSOR_POWER_OFF;
}
/*******************************************************************************
** 开关机键
*******************************************************************************/
static void KeyPower(void)
{
  if(f_Test)
  {
    if(4==g_TestState)
    {
      g_TestState++;
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(fg_ChildLock)
  {
    return;
  }

  fg_PowerOffWifiCfgSucc=0;
  if(fg_ONOFF)
  {
    switch(sleepStage)
    {
      case SLEEP_NONE:
        if((fg_Dark)&&(_LIGHT_AUTO==LIGHT_LEVEL))
        {
          fg_Dark=0;
          JudgeRingColour();
        }
        else if(_LIGHT_OFF==LIGHT_LEVEL)
        {
          LIGHT_LEVEL=_LIGHT_50;
          JudgeRingColour();
        }
        else
        {
          sleepStage=SLEEP_PRE;
          u8_sleepCnt=6;
          RGBOff();
          g_EERecordDelay=EE_RECORD_DELAY;
        }
        API_Buzzer_Set(BUZ_KEY);
        break;
      case SLEEP_PRE:
        sleepStage=SLEEP_NONE;
        PowerOffDeal();
        break;
      case SLEEP_ING:
        sleepStage=SLEEP_NONE;
        API_Buzzer_Set(BUZ_KEY);
        break;
      default:
        break;
    }
  }
  else
  {
    PowerOnDeal();
  }
}

/*******************************************************************************
** 风速键
*******************************************************************************/
static void KeyFan(void)
{
  if(f_Test)
  {
    if(5==g_TestState) 
    {
      g_TestState++;     //stage 2
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(1==APISleepAwaken())  
  {
    return;
  }

  if(fg_ChildLock)
  {
    return;
  }

  if(fg_Filter2Test)
  {
    fg_Filter2Test=0;
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  API_Buzzer_Set ( BUZ_KEY );
  FanGearSeqInSpeedArray++;
  if(FanGearSeqInSpeedArray>=6)
  {
    FanGearSeqInSpeedArray=0;
  }
  g_byFanSpSet=FanSpeedVal[FanGearSeqInSpeedArray];

  fg_Smart = 0;
  g_EERecordDelay = EE_RECORD_DELAY;   //eeprom record delay
}

/*******************************************************************************
** 风速键长按
*******************************************************************************/
static void KeyFanLongPressed(void)
{
  g_byKeyFuncBak = _KEY_FAN;

  if(sleepStage!=SLEEP_NONE)
  {
    g_byKeyFuncBak = _KEY_FAN;
    return;
  }

  if(0==AutoSlowDown)
  {
    AutoSlowDown=1;
  }
  else
  {
    AutoSlowDown=0;
  }

  API_Buzzer_Set(BUZ_FAN_SLOWDOWN_ENABLE);
}

/*******************************************************************************
** 自动键
*******************************************************************************/
static void KeyAuto( void )
{
  if(f_Test)
  {
    if(6==g_TestState) //now is fan key test mode
    {
      g_TestState++;     //stage 3: pressed key ionizer
      API_Buzzer_Set ( BUZ_KEY );
    }
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  if(1==APISleepAwaken())
  {
    return;
  }

  if(0==fg_Smart)
  {
    fg_Smart=1;
    sub_AutoFanSet();
    g_EERecordDelay=EE_RECORD_DELAY;   
  }
  else
  {
    fg_Smart=0;
    g_EERecordDelay=EE_RECORD_DELAY;   
  }
  API_Buzzer_Set ( BUZ_KEY );
}

/*******************************************************************************
** 自动键长按
*******************************************************************************/
void KeyAutoLongPressed(void)
{
  g_byKeyFuncBak = _KEY_AUTO;
  if(sleepStage!=SLEEP_NONE)
  {
    g_byKeyFuncBak = _KEY_FILTER;
    return;
  }
  WiFi_KeyInWifiCfgMode();
}

/*******************************************************************************
** 短按童锁键
*******************************************************************************/
void KeyLockShortPress ( void )
{
  if(f_Test)
  {
    if(7==g_TestState) 
    {
      g_TestState++;     //stage 3
      API_Buzzer_Set(BUZ_KEY);
    }
    return;
  }

  if(0==fg_ONOFF)
  {
    return;
  }

  if(1==APISleepAwaken())  
  {
    return;
  }
}

/*******************************************************************************
** 长按童锁键
*******************************************************************************/
void KeyLockLongPress(void)
{
  g_byKeyFuncBak = _KEY_NONE;
  if((0==fg_ONOFF)||(sleepStage!=SLEEP_NONE))
  {
    return;
  }
  fg_ChildLock = !fg_ChildLock;
  API_Buzzer_Set ( BUZ_KEY );
  if(fg_ChildLock)
  {
    mcu_dp_bool_update(DPID_CHILD_LOCK,1);
  }
  else
  {
    mcu_dp_bool_update(DPID_CHILD_LOCK,0);
  }
}

/*******************************************************************************
** 按键按下处理.
*******************************************************************************/
static void KeyPressing(unsigned char byKeyFunction) // Pressing
{
  if(1==f_Key_Done)
  {
    return;
  }

  f_Key_Done=1;
  g_byKeyFuncBak=_KEY_NONE;
  switch(byKeyFunction)
  {
    case _KEY_FAN:
      f_Key_Done=0;
      g_byKeyFuncBak=_KEY_FAN;
      KeyFan();
      break;
    case _KEY_LIGHT:
      KeyLight();
      break;
    case _KEY_LOCK:
      f_Key_Done=0;
      g_byKeyFuncBak=_KEY_LOCK;
      if(f_selfCheck)
      {
        if(7==g_TestState)
        {
          g_TestState=8;
          API_Buzzer_Set(BUZ_KEY);
        }
      }
      else if(f_Test)
      {
        if(g_TestState<8)
        {
          g_TestState=8;
          API_Buzzer_Set(BUZ_KEY);
        }
      }
      break;
    case _KEY_FILTER:
      f_Key_Done = 0;
      g_byKeyFuncBak=_KEY_FILTER;
      if(f_Test)
      {
        if(2==g_TestState)
        {
          g_TestState++;
          API_Buzzer_Set(BUZ_KEY);
        }
      }
      break;
    case _KEY_ONOFF:
      KeyPower();
      break;
    case _KEY_IONIZER:
      KeyIonizer();
      break;
    case _KEY_AUTO:
      KeyAuto();
      break;
    default:
      break;
  }
}

/*******************************************************************************
** 按键长按处理.
*******************************************************************************/
static void keyHoldDeal(unsigned char keyVal)
{
  if(0==fg_ChildLock)
  {
    if(4==wTimer20Ms_KeyPress)//防抖 80ms
    {
      KeyPressing(keyVal);
      wTimer20Ms_KeyPress++;
    }

    if((250==wTimer20Ms_KeyPress)&&(keyVal==_KEY_FILTER))
    {
      KeyFilterReset();
      wTimer20Ms_KeyPress++;
    }

    if((250==wTimer20Ms_KeyPress)&&(keyVal==_KEY_LIGHT)) //after 5 second,every 2 second call this function one times
    {
      KeyLightLongPressed();
      switch(AutoSetting)
      {
        case 0:
          wTimer20Ms_KeyPress=130;//400+130*20=3000
          break;
        case 1:
          wTimer20Ms_KeyPress=110;//800+110*20=3000
          break;
        case 2:
          wTimer20Ms_KeyPress=90;//1200+90*20=3000
          break;
        case 3:
          wTimer20Ms_KeyPress=70;//1600+70*20=3000
          break;
        case 4:
          wTimer20Ms_KeyPress=50;//2000+20*50=3000
          break;
        case 5:
          wTimer20Ms_KeyPress=30;//30*20+2400=3000ms
          break;
        default:
          break;
      }
    }

    if((250==wTimer20Ms_KeyPress)&&(keyVal==_KEY_FAN))
    {
      KeyFanLongPressed();
      wTimer20Ms_KeyPress++;
    }

    if((250==wTimer20Ms_KeyPress)&&(keyVal==_KEY_AUTO)) //after 5 second,every 2 second call this function one times
    {
      KeyAutoLongPressed();
      wTimer20Ms_KeyPress ++;
    }

    /*
    #ifdef debug
          if ( ( 200 == wTimer20Ms_KeyPress ) && ( byKeyFunction == _KEY_ONOFF) )
          {
            KeyFactoryDefault();
            wTimer20Ms_KeyPress ++;
          }
    #else
    */
    if((500 == wTimer20Ms_KeyPress ) && ( keyVal == _KEY_ONOFF))// 10sec
    {
      KeyFactoryDefault();
      wTimer20Ms_KeyPress ++;
    }
    //#endif

    if((150==wTimer20Ms_KeyPress)&&(keyVal ==_KEY_IONIZER_FILTER))// 3S
    {
      KeyIonizerFilter();
      wTimer20Ms_KeyPress ++;
    }

    if((wTimer20Ms_KeyPress>=400) && (keyVal==_KEY_IONIZER))
    {
      if(0==(wTimer20Ms_KeyPress%50))//each second.
      {
        KeyIonizerLongPressed();
        wTimer20Ms_KeyPress++;
      }
    }
  }

  if((150 == wTimer20Ms_KeyPress ) && ( keyVal == _KEY_LOCK ))
  {
    KeyLockLongPress();
    wTimer20Ms_KeyPress++;
  }
}

/*******************************************************************************
** 按键释放处理.
*******************************************************************************/
static void KeyReleased(unsigned char byKeyFunction)
{
  switch(byKeyFunction)
  {
    case _KEY_LOCK:
      KeyLockShortPress();
      break;
    case _KEY_FILTER:
      KeyFilter();
      break;
    case _KEY_IONIZER:
      KeyIonizer();
      break;
    default:
      break;
  }
}
/*******************************************************************************
** 按键时间计数
*******************************************************************************/
static void KeyTimeCount(void)
{
  if(API_Gets20MsFlag())
  {
    if(wTimer20Ms_KeyPress<0xFFF0)
    {
      wTimer20Ms_KeyPress++;
    }
  }

  if(API_Gets1SFlag())
  {
    if(fg_ONOFF)
    {
      g_wTimer1S_OFF=0;
      if(g_byTimer1S_ON<0xFF)
      {
        g_byTimer1S_ON++;
      }
    }
    else
    {
      g_byTimer1S_ON=0;
      if(g_wTimer1S_OFF<0xFFFF)
      {
        g_wTimer1S_OFF++;
      }
    }
  }
}

/*******************************************************************************
** 按键处理函数,主程序调用.
*******************************************************************************/
void keyDeal(void)
{
  unsigned char keySoure;

  keySoure=readKey();
  if(g_byTimer1S_DisplayAll2S>1)//全显之后
  {
    if(_KEY_NONE==keySoure)
    {
      wTimer20Ms_KeyPress=0;
      f_Key_Done=0;
      if(0==fg_ChildLock)
      {
        KeyReleased(g_byKeyFuncBak);
      }
      g_byKeyFuncBak=_KEY_NONE;
    }
    else
    {
      keyHoldDeal(keySoure);
    }
  }
  KeyTimeCount();
  fg_Filter2Test=0;
}


