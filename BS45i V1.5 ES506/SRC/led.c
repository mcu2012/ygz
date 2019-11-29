#define  DISPPAN_GLOBALS_H
#include "includes.h"
//----------------------- local function -------------------------------------
static void sub_Disp_Timer(void)
{
  if(API_Gets1SFlag())
  {
    if(g_byTimer1S_PowerON<0xFF)
    {
      g_byTimer1S_PowerON++;
    }

    if(g_byTimer1S_DisplayAll2S<0xFF)
    {
      g_byTimer1S_DisplayAll2S++;
    }

    if(fg_ONOFF)
    {
      if(g_Timer1S_PowerOn<0xFF)
      {
        g_Timer1S_PowerOn++;
      }
    }
    else
    {
      g_Timer1S_PowerOn=0;
    }

    if((1==g_byTimer1S_DisplayAll2S )&&(0==f_Test))
    {
      RGBOff();
    }
  }
}

static void factoryRecoverDis(void)
{
  unsigned char i;

  if(0==API_Gets1SFlag())
  {
    return;
  }

  if((0==g_factoryCnt)||(2==g_factoryCnt)||(4==g_factoryCnt))
  {
    API_Buzzer_Set(BUZ_KEY);
    for(i=0; i<14; i++)
    {
      g_ucDisData[i].byte = 0xff;
    }
    JudgeRingColour();
  }
  else
  {
    for(i=0; i<14; i++)
    {
      g_ucDisData[i].byte = 0;
    }
    RGBOff();
  }

  Api_LEDScanDrv();

  if(g_factoryCnt<0xFF)
  {
    g_factoryCnt++;
  }

  if(6==g_factoryCnt)
  {
    fg_FactoryRecover=0;
    fg_ONOFF = 0;
    RGBOff();
    sleepStage=SLEEP_NONE;
    g_Timer1S_PowerOn=0;
    u16_Pm25Val=0;
  }
}

static FillLedBuff(void)
{
  if(1==fg_Dark)
  {
    return;
  }

  if((1==fg_ONOFF) && (SLEEP_NONE==sleepStage) && (_LIGHT_OFF!=LIGHT_LEVEL))
  {
    //fan display
    if(f_Err_DCMotor)
    {
      if(fg_LED_FLash)
      {
        fg_LED_WHITE_FAN = 1;
      }
    }
    else
    {
      fg_LED_WHITE_FAN = 1;
    }

    switch(g_byFanSpSet)
    {
      case FAN_SPEED1:
        fg_LED_BLUE_FAN_L = 1;
        break;
      case FAN_SPEED2:
        fg_LED_BLUE_FAN_M = 1;
        break;
      case FAN_SPEED3:
        fg_LED_BLUE_FAN_H = 1;
        break;
      case FAN_SPEED4:
        fg_LED_BLUE_FAN_T = 1;
        break;
      default:
        fg_LED_BLUE_FAN_L = 1;
        break;
    }

    if(fg_Smart)
    {
      fg_LED_AUTO = 1;
    }

    //display timer
    fg_LED_WHITE_TIMER = 1;
    switch(LIGHT_LEVEL)
    {
      case _LIGHT_50:
        fg_LED_BLUE_TIMER_2H = 1;
        break;
      case _LIGHT_100:
        fg_LED_BLUE_TIMER_4H = 1;
        break;
      case _LIGHT_AUTO:
        fg_LED_BLUE_TIMER_12H = 1;
        break;
      case _LIGHT_OFF:
        break;
      default:
        break;
    }

    //display filter
    if(1==fg_Filter2Lock) //filter error
    {
      if(fg_LED_FLash)
      {
        fg_LED_FILTER_KEY = 1;
      }
    }
    else
    {
      fg_LED_FILTER_KEY = 1;
    }

    if(g_FilerUseTimeCnt_fl<U16_FilterGreenLedTime)
    {
      fg_LED_GREEN_FILTER = 1;
    }
    else if(g_FilerUseTimeCnt_fl<=U16_FilterOrangeLedTime)
    {
      fg_LED_ORANGE_FILTER = 1;
    }
    else
    {
      fg_LED_RED_FILTER = 1;
    }

    if(1==fg_Filter2Lock) //filter error
    {
      if ( fg_LED_FLash )
      {
        fg_LED_ANION_ONOFF = 1;
        fg_LED_ANION_KEY = 1;
      }
    }
    else //no filter error
    {
      fg_LED_ANION_ONOFF = 1;
      fg_LED_ANION_KEY = 1;
    }

    //display Lock
    fg_LED_LOCK_KEY = 1;
    fg_LED_AUTO_MODE = 1;
    if(1==fg_ChildLock)
    {
      fg_LED_LOCK_ONOFF = 1;
    }

    if(fg_Ionizer)
    {
      fg_LED_ANION_ONOFF = 1;
    }
    else
    {
      fg_LED_ANION_ONOFF = 0;
    }
  }

  if(fg_LED_WIFI)
  {
    fg_LED_WIFI_ONOFF = 1;
  }
  else
  {
    fg_LED_WIFI_ONOFF = 0;
  }
}

static void normalDis(void)
{
  FillLedBuff();
  Api_LEDScanDrv();
}

static void led(void)
{
  unsigned char i;

  for(i=0; i<14; i++)
  {
    g_ucDisData[i].byte=0;
  }

  if(f_Test)
  {
    ledTestMode();
    return;
  }

  //display all
  if(g_byTimer1S_DisplayAll2S<1)
  {
    for(i=0; i<14; i++)
    {
      g_ucDisData[i].byte = 0xff;
    }
    Api_LEDScanDrv();   //led display driver
    RGB_Blue();
    return;
  }

  if(fg_FactoryRecover)
  {
    factoryRecoverDis();
    return;
  }
  normalDis();
}

static void RGB_Deal(void)
{
  static unsigned char cnt20ms=0;

  if(g_byTimer1S_DisplayAll2S<=1)
  {
    return;
  }

  if(1==fg_FactoryRecover)
  {
    return;
  }

  if(0==API_Gets20MsFlag())
  {
    return;
  }

  cnt20ms++;
  if(cnt20ms>=5)
  {
    cnt20ms=0;
    if((0==fg_ONOFF)||(_LIGHT_OFF==LIGHT_LEVEL)||(1==fg_Dark))
    {
      RGBOff();
    }
    else
    {
      JudgeRingColour();
    }
  }
}
//-----------------------------------------------------------------------------
void Init_Disp(void)
{
  g_byTimer1S_PowerON = 0;
  g_byTimer1S_DisplayAll2S = 0;
  HW_IOSet_RGB(1);
}

void display(void)
{
  sub_Disp_Timer();
  led();
  RGB_Deal();
}


