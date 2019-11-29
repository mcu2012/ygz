#define TIMER_GLOBALS_H
#include "includes.h"

void sub_Timer20Ms ( void );
void sub_Timer1S ( void );
void sub_Timer1Min ( void );


unsigned int  wTimer1Ms_Counter1S;
unsigned char byTimer1Ms_Counter20Ms;
unsigned char byTimer1S_Counter;
unsigned int  wTimer1S_Counter;

union flagbyte ucTimer_Flag;
#define f_Timer20Ms           ucTimer_Flag.bt.b0
#define f_Timer20MsBak        ucTimer_Flag.bt.b1
#define f_Timer1S             ucTimer_Flag.bt.b2
#define f_Timer1SBak          ucTimer_Flag.bt.b3
#define f_Timer1Min           ucTimer_Flag.bt.b4

void Init_Timer ( void )
{
  wTimer1Ms_Counter1S = 0;
  byTimer1Ms_Counter20Ms = 0;
  byTimer1S_Counter = 0;
  ucTimer_Flag.byte = 0;
  u16_1sCounterRunTime=0;
  u16_1sCounterFilter=0;
}

void ISR_Timer1MsProcess ( void )
{
  wTimer1Ms_Counter1S ++;
  byTimer1Ms_Counter20Ms ++;

  if ( byTimer1Ms_Counter20Ms >= 20 )
  {
    f_Timer20MsBak = 1;
    byTimer1Ms_Counter20Ms = 0;
  }

  if ( wTimer1Ms_Counter1S >= 1000 )
  {
    f_Timer1SBak = 1;
    wTimer1Ms_Counter1S = 0;
  }
  Api_Timer1Ms_Buzzer();
  PM25Ms();
  if(u16_fanOnDlyConfirmErr<0xFFFF)
  {
    u16_fanOnDlyConfirmErr++;
  }
  fanMsDly();
}
/*
void ISR_Timer100UsProcess ( void )
{
  Api_Timer100Us_DCMotorDrv();
}
*/
void timerDeal ( void )
{
  sub_Timer20Ms();
  sub_Timer1S();
  sub_Timer1Min();
/*
  if(fg_ONOFF)
  {
    if(API_Gets1SFlag())
    {
      if(fg_ShortTimer)
      {
        if(u32_totalRunTime < MAX_TOTAL_RUN_TIME-100UL)
        {
          u32_totalRunTime=u32_totalRunTime+100UL;
        }
        else
        {
          u32_totalRunTime=MAX_TOTAL_RUN_TIME;
        }
        //mcu_dp_value_update(DPID_TOTALTIME,u32_totalRunTime*60UL);
      }
      else
      {
        u16_1sCounterRunTime++;
        if(u16_1sCounterRunTime>=3600)
        {
          u16_1sCounterRunTime=0;
          if(u32_totalRunTime < MAX_TOTAL_RUN_TIME)
          {
            u32_totalRunTime++;
            //mcu_dp_value_update(DPID_TOTALTIME,u32_totalRunTime*60UL);
          }
        }
      }
    }
  }
*/
  if(1==g_byTimer1S_DisplayAll2S)
  {
    if(0==f_Test)
    {
      if(0==fg_ONOFF)
      {
        if(1==e2_fg_ONOFF)
        {
          //if(EE_SELFCHECK_SUCC_FLAG==e2_selfCheckSucc)//production test ok.
          //{
            fg_ONOFF = 1;
            g_byTimer1S_ON = 0;
            if(1==e2_sleep_OnOff)
            {
              sleepStage=SLEEP_ING; 
            }
            else
            {
              RGB_Blue();
            }
          //}
        }
        else
        {
          sleepStage=SLEEP_NONE;
        }
      }
    }
  }

  if(API_Gets1SFlag())
  {
    PM25Sec();
    if ( fg_ONOFF )
    {
      if(SLEEP_PRE==sleepStage)
      {
        if(u8_sleepCnt>0)
        {
          u8_sleepCnt--;
        }

        if(0==u8_sleepCnt)
        {
          sleepStage=SLEEP_ING;
        }
      }
    }
  }
}

void sub_Timer20Ms ( void )
{
  f_Timer20Ms = 0;
  if ( f_Timer20MsBak )
  {
    f_Timer20Ms = 1;
    f_Timer20MsBak = 0;
  }
}

void sub_Timer1S ( void )
{
  f_Timer1S = 0;
  if ( f_Timer1SBak )
  {
    f_Timer1S = 1;
    f_Timer1SBak = 0;
    
  }
}

void sub_Timer1Min ( void )
{
  f_Timer1Min = 0;
  if ( f_Timer1S )
  {
    byTimer1S_Counter ++;
    if ( byTimer1S_Counter >= 60 )
    {
      f_Timer1Min = 1;
      byTimer1S_Counter = 0;
    }
  }
}

unsigned char API_Gets20MsFlag ( void )
{
  if ( f_Timer20Ms )
    return 1;
  else
    return 0;
}

unsigned char API_Gets1SFlag ( void )
{
  if ( f_Timer1S )
    return 1;
  else
    return 0;
}

unsigned char API_Gets1MinFlag ( void )
{
  if ( f_Timer1Min )
    return 1;
  else
    return 0;
}

