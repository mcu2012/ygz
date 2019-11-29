#define TEST_GLOBALS_H
#include "includes.h"

unsigned char byTimer1S_TestMax;

union flagbyte ucTestFlag;
#define f_TestBak         ucTestFlag.bt.b0

void testTimer( void );

void testIni ( void )
{
  fg_ONOFF = 1;
  if(0==f_selfCheck)
  {
    SetWiFiConfig();
    g_TestDlyJudge = 10;
  }
  else
  {
    //SetWiFiSelfcheck();
    g_TestDlyJudge = 15;
    //g_TestDlyJudge = 10;
    API_Buzzer_Set(BUZ_KEY);
  }
  f_TestErr_Wifi = 1;
  g_TestFanSpeedSet = ( unsigned int ) e2_DCFanSp1 * 10;
  testFanIni();//sDCMotorVal.wDuty = DCSPINITTEST;
  g_TestState = 0;
  byTimer1S_TestMax = 0;
  //u8_THReadIntervalTimerS=0;
  g_TestPm25ErrCount=0;
  //g_TestTempHumErrCount=0;
  g_TestDisAllCnt=0;
}

void Init_Test( void )
{
  unsigned char i, j;

  j = 0;
  for ( i = 0; i < 250; i++ )
  {
    if ( 0 == HW_IORead_Test() )
    {
      j++;
    }
  }

  if ( j > 200 )
  {
    f_Test = 1;
  }

  if ( f_Test )
  {
    testIni();
  }
}
void testChangeRingColour(void)
{
  if(0==f_Test)
  {
    return;
  }

  if(API_Gets1SFlag())
  {
    if(g_TestRingColourTYpe<2)
    {
      RGB_Blue();
    }
    else if(g_TestRingColourTYpe<4)
    {
      RGB_Green();
    }
    else if(g_TestRingColourTYpe<6)
    {
      RGB_Orange();
    }
    else if(g_TestRingColourTYpe<8)
    {
      RGB_Red();
    }
    else if(g_TestRingColourTYpe<10)
    {
      RGB_Pur();
    }
    g_TestRingColourTYpe++;
    if(g_TestRingColourTYpe>=10)
    {
      g_TestRingColourTYpe=0;
    }
  }
}
void TestDeal ( void )
{
  testTimer();
  if(f_Test)
  {
    testChangeRingColour();
    if(f_selfCheck)
    {
      if(API_Gets1SFlag())
      {
        if(1==f_TestErr_Wifi)
        {
          if(WIFI_SATE_UNKNOW!=mcu_get_wifi_work_state())//got state.
          {
            if((g_TestDlyJudge>0)&&(g_TestDlyJudge<=10))
            {
              //if(API_Gets1SFlag())
              //{
              if(0==(g_TestDlyJudge%2))
              {
                SetWiFiSelfcheck();
              }
              //}
            }
          }
        }
      }
    }
  }
  else// ( 0 == f_Test )
  {
    if( API_Gets20MsFlag() )
    {
      if ( 0 == HW_IORead_Test() )
      {
        if( f_TestBak == HW_IORead_Test() )
        {
          if(0==fg_ShortTimer)
          {
            fg_ShortTimer = 1;
            API_Buzzer_Set(BUZ_KEY);
          }
        }
      }
      else
      {
        if( f_TestBak == HW_IORead_Test() )
        {
          if(1==fg_ShortTimer)
          {
            fg_ShortTimer = 0;
            API_Buzzer_Set(BUZ_KEY);
          }
        }
      }
      f_TestBak = HW_IORead_Test();
    }
  }
}

void testTimer ( void )
{
  if ( API_Gets1SFlag() )
  {
    if ( byTimer1S_TestMax < 0xFF )
    {
      byTimer1S_TestMax ++;
    }

    if ( g_TestDlyJudge > 0 )
    {
      g_TestDlyJudge--;
    }
  }
}

void ledTestMode ( void )
{
  unsigned char i;

  switch(g_TestState)
  {
    case 0:   //display all
      for ( i = 0; i < 14; i ++ )
      {
        g_ucDisData[i].byte = 0xff;
      }

      if(API_Gets20MsFlag())
      {
        if(g_TestDisAllCnt<50)
        {
          g_TestDisAllCnt++;
        }
        else
        {
          g_TestState=1;
        }
      }
      break;
    case 1:
      break;
    case 2:  //anion
      fg_LED_ANION_ONOFF = 1;
      fg_LED_ANION_KEY = 1;
      break;
    case 3:  //filter
      fg_LED_GREEN_FILTER = 1;
      fg_LED_FILTER_KEY = 1;
      fg_LED_ORANGE_FILTER = 1;
      fg_LED_RED_FILTER = 1;
      break;
    case 4: //timer
      fg_LED_WHITE_TIMER = 1;
      fg_LED_BLUE_TIMER_2H = 1;
      fg_LED_BLUE_TIMER_4H = 1;
      fg_LED_BLUE_TIMER_12H = 1;
      break;
    case 5: //power
      fg_LED_WHITE_ONOFF=1;
      break;
    case 6: //fan
      fg_LED_BLUE_FAN_L = 1;
      fg_LED_BLUE_FAN_M = 1;
      fg_LED_BLUE_FAN_H = 1;
      fg_LED_BLUE_FAN_T = 1;
      fg_LED_WHITE_FAN=1;
      break;
    case 7: //auto
      fg_LED_AUTO_MODE = 1;
      fg_LED_AUTO = 1;
      break;
    case 8: //lock
    case 9:
      fg_LED_LOCK_ONOFF=1;
      fg_LED_WIFI_ONOFF=1;
      fg_LED_LOCK_KEY=1;
      if(API_Gets1SFlag())
      {
        g_TestState++;
      }
      break;
    case 10:
      if(0==g_TestDlyJudge)
      {
        if(g_TestPm25ErrCount>=5)//if(g_TestPm25ErrCount>=3)
        {
          fg_Err_PM25=1;
        }

        //if(g_TestTempHumErrCount>=3)
        //{
        //TempAndHemiErrorF=1;
        //}

        //if((0==fg_Err_EEprom)&&(0==fg_Err_PM25)
        //&&(0==TempAndHemiErrorF)&&(0==fg_Err_DCMotor)
        //&&(0==f_TestErr_Wifi))//error dis
        if((0==fg_Err_EEprom)&&(0==fg_Err_PM25)
            &&(0==f_Err_DCMotor)
            &&(0==f_TestErr_Wifi))//error dis
        {
          if(1==f_selfCheck)
          {
            //RecordByteData ( e2SelfCheckFlagAdd, EE_SELFCHECK_SUCC_FLAG);
          }
          f_Test = 0;
          f_selfCheck=0;
          fg_ONOFF = 0;
          RGBOff();
        }
        else
        {
          if(fg_LED_FLash)
          {
            if(fg_Err_EEprom)
            {
              fg_LED_ANION_KEY=1;
            }

            if(fg_Err_PM25)
            {
              fg_LED_FILTER_KEY= 1;
            }

            //if(1==TempAndHemiErrorF)
            //{
            //fg_LED_WHITE_TIMER=1;
            //}

            if ( f_Err_DCMotor )
            {
              fg_LED_WHITE_FAN = 1;
            }

            if(f_TestErr_Wifi)
            {
              fg_LED_WIFI_ONOFF=1;
            }
          }
        }
      }
      break;
    default:
      break;
  }
  //#ifdef debug
  //if(1==g_TestGotWifi)
  //{
  //fg_LED_RED_FILTER = 1;
  //}
  //#endif
  Api_LEDScanDrv();     //led display driver
}

void SetWiFiConfig ( void )
{
  mcu_set_wifi_mode(SMART_CONFIG);
}

void SetWiFiSelfcheck ( void )
{
  mcu_start_wifitest();
}

