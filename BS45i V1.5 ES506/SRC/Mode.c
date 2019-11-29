#define MODE_GLOBALS_H
#include "includes.h"

#define _LIGHT_ERR         0 //10
#define _LIGHT_DARK        2160//静音室全黑2148,灯光2210

void sub_Mode_Timer ( void );
void sub_Mode_Light ( void );
void sub_Ionizer ( void );

static unsigned char byTimer20Ms_Flash;
static unsigned int  wTimer1S_DarkExit;
unsigned char PM25ValSection[4];


void Init_Mode ( void )
{
  byTimer1S_RedOFFDelay = 0;
  byTimer1S_YellowOFFDelay = 0;
}

void modeDeal ( void )
{
  sub_Mode_Timer();
  filterLife();
  sub_Mode_Light();
  sub_Ionizer();
}

void sub_Ionizer ( void )
{
  if ( f_Test )
  {
    if(2==g_TestState)
    {
      HW_IOSet_Ionizer(fg_Ionizer);
    }
    return;
  }

  if ( fg_ONOFF )
  {
    HW_IOSet_Ionizer ( fg_Ionizer );
  }
  else
  {
    HW_IOSet_Ionizer ( 0 );
  }
}

void sub_Mode_Light ( void )
{
  if(0==fg_ONOFF)
  {
    fg_Dark=0;
    byTimer1S_DarkIn=0;
    wTimer1S_DarkExit=0;
    return;
  }

  if(_LIGHT_AUTO!=LIGHT_LEVEL)
  {
    fg_Dark=0;
    byTimer1S_DarkIn=0;
    wTimer1S_DarkExit=0;
    return;
  }

  if(SLEEP_NONE!=sleepStage)
  {
    fg_Dark = 0;
    byTimer1S_DarkIn=0;
    return;
  }

  if(API_Gets1SFlag())
  {
    if(fg_Dark)
    {
      byTimer1S_DarkIn=0;
      if(g_byADLight>=(_LIGHT_DARK+_LIGHT_ERR))//越亮,AD值越大
      {
        if(wTimer1S_DarkExit<0xFFFF)
        {
          wTimer1S_DarkExit++;
        }
      }
    }
    else
    {
      wTimer1S_DarkExit = 0;
      if(g_byADLight<(_LIGHT_DARK+_LIGHT_ERR))
      {
        if(byTimer1S_DarkIn<0xFF)
        {
          byTimer1S_DarkIn++;
        }
      }
    }
  }

  if(byTimer1S_DarkIn>=_TIMER_DARKIN)
  {
    fg_Dark = 1;
  }

  if(wTimer1S_DarkExit>=_TIMER_DARKEXIT)
  {
    fg_Dark = 0;
  }
}

void sub_Mode_Timer ( void )
{
  if(API_Gets20MsFlag())
  {
    byTimer20Ms_Flash++;
    if(byTimer20Ms_Flash>=25)
    {
      fg_LED_FLash = !fg_LED_FLash;
      byTimer20Ms_Flash = 0;
    }
  }

  if(API_Gets1SFlag())
  {
    if(1==fg_ParticleDown5MinStart)
    {
      if(g_RingLastTime>0)
      {
        g_RingLastTime--;
      }
    }

    /*
    if((0==fg_ONOFF) || (_LIGHT_AUTO!=LIGHT_LEVEL))
    {
      byTimer1S_DarkIn=0;
      wTimer1S_DarkExit = 0;
    }
    else
    {
      if(fg_Dark)
      {
        byTimer1S_DarkIn=0;
        if(wTimer1S_DarkExit<0xFFFF)
        {
          wTimer1S_DarkExit++;
        }
      }
      else
      {
        wTimer1S_DarkExit = 0;
        if(byTimer1S_DarkIn<0xFF)
        {
          byTimer1S_DarkIn++;
        }
      }
    }
    */
  }
}

void anionSet(unsigned char onOff)
{
  if ( 1 == onOff )
  {
    fg_Ionizer = 1;
  }
  else
  {
    fg_Ionizer = 0;
  }
}

void powerSet ( unsigned char onOff )
{
  fg_PowerOffWifiCfgSucc=0;
  if ( 1 == onOff )
  {
    fg_ONOFF = 1;
    PowerOnDeal();
  }
  else
  {
    fg_ONOFF = 0;
    PowerOffDeal();
  }
}

void lockSet(unsigned char onOff)
{
  API_Buzzer_Set ( BUZ_KEY );
  if ( 1 == onOff )
  {
    fg_ChildLock = 1;
  }
  else
  {
    fg_ChildLock = 0;
  }
}

void modeSet(unsigned char SetMode)
{
  API_Buzzer_Set ( BUZ_KEY );
  switch(SetMode)
  {
    case MANNUAL_MODE:
      fg_Smart = 0;
      sleepStage=SLEEP_NONE;
      break;
    case AUTO_MODE:
      sleepStage=SLEEP_NONE;
      if(0==fg_Smart)
      {
        fg_Smart=1;
        sub_AutoFanSet();
        g_EERecordDelay = EE_RECORD_DELAY;   //eeprom record delay
      }
      break;
    case SLEEP_MODE:
      sleepStage=SLEEP_ING;
      RGBOff();
      break;
    default:
      break;
  }
}


static void judgeUseWhichPM25Zone(void)
{
  switch(AutoSetting)
  {
    case 0:
      PM25ValSection[0]=2;
      PM25ValSection[1]=3;
      PM25ValSection[2]=5;
      PM25ValSection[3]=10;
      break;
    case 1:
      PM25ValSection[0]=2;
      PM25ValSection[1]=3;
      PM25ValSection[2]=10;
      PM25ValSection[3]=30;
      break;
    case 2:
      PM25ValSection[0]=2;
      PM25ValSection[1]=4;
      PM25ValSection[2]=15;
      PM25ValSection[3]=35;
      break;
    case 3:
      PM25ValSection[0]=3;
      PM25ValSection[1]=8;
      PM25ValSection[2]=20;
      PM25ValSection[3]=35;
      break;
    case 4:
      PM25ValSection[0]=4;
      PM25ValSection[1]=8;
      PM25ValSection[2]=25;
      PM25ValSection[3]=40;
      break;
    case 5:
      PM25ValSection[0]=6;
      PM25ValSection[1]=12;
      PM25ValSection[2]=35;
      PM25ValSection[3]=50;
      break;
    default:
      break;
  }
}

void JudgeRingColour ( void )
{
  RING_LIGHT ringShouldBe;

  ringShouldBe=blue;

  if((f_Test)||(f_selfCheck))
  {
    return;
  }

  judgeUseWhichPM25Zone();
  if(u16_Pm25Val<PM25ValSection[0])//2
  {
    ringShouldBe=blue;
  }
  else if(u16_Pm25Val<PM25ValSection[1])//3
  {
    ringShouldBe=green;
  }
  else if(u16_Pm25Val<PM25ValSection[2])//5
  {
    ringShouldBe=orange;
  }
  else if(u16_Pm25Val<PM25ValSection[3])
  {
    ringShouldBe=red;
  }
  else
  {
    ringShouldBe=purple;
  }

  if(1==AutoSlowDown)//disable
  {
    g_RingLastTime=0;
  }

  if(ringShouldBe<ringColor)//become better
  {
    if(1==fg_ParticleDown5MinStart)
    {
      if(0==g_RingLastTime)
      {
        switch(ringColor)
        {
          case blue:
            fg_ParticleDown5MinStart=0;
            break;
          case green:
            ringColor=blue;
            g_RingLastTime=RING_COLOUR_LAST_TIME;
            break;
          case orange:
            ringColor=green;
            g_RingLastTime=RING_COLOUR_LAST_TIME;
            break;
          case red:
            ringColor=orange;
            g_RingLastTime=RING_COLOUR_LAST_TIME;
            break;
          case purple:
            ringColor=red;
            g_RingLastTime=RING_COLOUR_LAST_TIME;
            break;
          default:
            break;
        }
      }
    }
    else
    {
      fg_ParticleDown5MinStart=1;
    }
  }
  else if(ringShouldBe>=ringColor)//become worse or no change.
  {
    fg_ParticleDown5MinStart=0;
    g_RingLastTime=RING_COLOUR_LAST_TIME;
    ringColor=ringShouldBe;
  }

  if(sleepStage!=SLEEP_NONE)//in sleep mode.
  {
    RGBOff();
  }
  else
  {
    switch(ringColor)
    {
      case blue:
        RGB_Blue();
        break;
      case green:
        RGB_Green();
        break;
      case orange:
        RGB_Orange();
        break;
      case red:
        RGB_Red();
        break;
      case purple:
        RGB_Pur();
        break;
      default:
        break;
    }
  }
}
