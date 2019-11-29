#define _FAN_DEFINE
//#include "fanDrv.h"
#include "includes.h"
union S_FLAG
{
  unsigned char  Byte;
  struct
  {
    unsigned char b0	: 1;
    unsigned char b1	: 1;
    unsigned char b2	: 1;
    unsigned char b3	: 1;
    unsigned char b4	: 1;
    unsigned char b5	: 1;
    unsigned char b6	: 1;
    unsigned char b7	: 1;
  } Bit;
};

unsigned int   OBJSPEED;               //mu biao feng su/10
unsigned int   NOWSPEED;              //dang qian feng su/10
unsigned int   D_VALUE_NOW;                  //[(KP+KD)*EN-KD*EN_1]*KT
unsigned int   D_VALUE_LAST;                //last en
unsigned int   KD_LAST;              //KD*EN_1
unsigned int   SN_LAST;                //last SN_1
unsigned int   SN;

#define _NEED_FAST  0
#define _NEED_SLOW  1
#define _KD_MUCH 0
#define _KD_LESS 1
unsigned char  SpeedNeedFlagtNow; //0:speed up;1:speed down.
unsigned char  SpeedNeedFlagLast;


unsigned int TjFanSpeed(unsigned int obj, unsigned int now)
{
  unsigned int l_uiTemp;
  unsigned char  KdState;
  unsigned int   Dvalue;

  OBJSPEED = obj / 10;
  NOWSPEED = now / 10;
  l_uiTemp = OBJSPEED;
  if(OBJSPEED>NOWSPEED)
  {
    SpeedNeedFlagtNow = _NEED_FAST;//speed up
    D_VALUE_NOW = l_uiTemp - NOWSPEED;
  }
  else
  {
    SpeedNeedFlagtNow = _NEED_SLOW;//speed down
    D_VALUE_NOW = NOWSPEED - l_uiTemp;
  }

  Dvalue=D_VALUE_NOW*Kp;//TMPSN = EN * 10;//TMPSN = EN * 11;
  KD_LAST=D_VALUE_LAST*Kd;//KDEN_1 = EN_1 * 8;//KDEN_1 = EN_1 * 10;
  if(((_NEED_FAST==SpeedNeedFlagtNow)&&(_NEED_FAST==SpeedNeedFlagLast))
      ||((_NEED_SLOW==SpeedNeedFlagtNow)&&(_NEED_SLOW==SpeedNeedFlagLast)))//same direction
  {
    if(Dvalue>=KD_LAST)//Integral is more than last.
    {
      KD_LAST = Dvalue - KD_LAST;
      KdState=_KD_MUCH;
    }
    else
    {
      KD_LAST = KD_LAST - Dvalue;
      KdState=_KD_LESS;
    }

    if(_NEED_FAST==SpeedNeedFlagtNow)
    {
      //do nothing
    }
    else if(_NEED_SLOW==SpeedNeedFlagtNow)
    {
      if(_KD_LESS==KdState)
      {
        KdState=_KD_MUCH;
      }
      else
      {
        KdState=_KD_LESS;
      }
    }
  }
  else
  {
    KdState = SpeedNeedFlagtNow;
    KD_LAST = Dvalue + KD_LAST;
  }

  //KD_LAST=(KD_LAST<<1);//KDEN_1 = (KDEN_1 << 3);// [(KP+KD)*EN-KD*EN_1]*KT ,KT = 2;
  if(_KD_LESS==KdState)
  {
    //adjust slow
    if(KD_LAST>18)
    {
      KD_LAST=18;
    }

    if(SN_LAST>KD_LAST)
    {
      SN = SN_LAST - KD_LAST;
    }
    else
    {
      SN = KD_LAST - SN_LAST;
    }
  }
  else
  {
    //adjust fast
    /*if(KD_LAST>10)
    {
      KD_LAST=10;
    }
    */
    if(KD_LAST>100)//加快启动速度
    {
      KD_LAST=100;
    }
    SN=SN_LAST+KD_LAST;
  }

  if(SN>DCSPINITMAX)
  {
    SN=DCSPINITMAX;
  }
  else if(SN<_FanDrvMin)
  {
    SN=_FanDrvMin;
  }

  SN_LAST = SN;
  D_VALUE_LAST = D_VALUE_NOW;
  SpeedNeedFlagLast=SpeedNeedFlagtNow;
  return SN;
}
/*******************************************************************************
*******************************************************************************/
void Fan_OFFctrl ( void )
{
  //IO_INDOORFAN = 0;
  //StopPgFan;
  SpeedNeedFlagtNow=_NEED_FAST;
  SpeedNeedFlagLast=_NEED_FAST;
  //D_VALUE_NOW = 0;
  D_VALUE_LAST = 0;
  //KD_LAST = 0;
  SN_LAST = DCSPINIT;
}

//should be call every millisecond.
void fanMsDly(void)
{
  if(fanAdjustInterval>0)
  {
    fanAdjustInterval--;
  }
  else
  {
    //fanAdjustInterval=_FAN_ADJUST_TIME;
  }
}

