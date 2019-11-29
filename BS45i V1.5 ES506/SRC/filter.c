#define FILTER_GLOBALS_H
#include "includes.h"

unsigned int u16_filterLifeInt;

void CalFilterRecValEE(void)
{
  float filterCal;
  unsigned long u32_dec;

  u16_filterLifeInt = (unsigned int)g_FilerUseTimeCnt_fl;
  //g_FilerUseTimeHigh=(unsigned char)(u16_filterLifeInt>>8);
  //g_FilerUseTimeLow=(unsigned char)(u16_filterLifeInt&0x00FF);

  filterCal=(g_FilerUseTimeCnt_fl*10);
  u32_dec=(unsigned long)filterCal;
  g_FilerUseTimeDecimal=(unsigned char)(u32_dec%10);
}

void filterLife(void)
{
  float filterLifeCal=0.0;

  if (0==fg_ONOFF)
  {
    return;
  }

  if(1==f_Err_DCMotor)  //fan error, filter time not calculate
  {
    return;
  }

  if(0==g_byDCFanRun)
  {
    return;
  }

  if(fg_ShortTimer)
  {
    return;
  }

  if(0==API_Gets1SFlag())
  {
    return;
  }

  u16_1sCounterFilter++;
  if(u16_1sCounterFilter<3600)
  {
    return;
  }

  u16_1sCounterFilter=0;

  //If not calculate each hour, here should be multiplied the time.
  switch(g_byFanSpSet)
  {
    case FAN_SPEED1://SFLR=0.7;
      switch(ringColor)
      {
        case blue:
          filterLifeCal=1.0;
          break;
        case green:
          filterLifeCal=1.1;
          break;
        case orange:
          filterLifeCal=1.15;
          break;
        case red:
        case purple:
          filterLifeCal=1.2;
          break;
        default:
          break;
      }
      break;
    case FAN_SPEED2://SFLR=0.8;
      switch(ringColor)
      {
        case blue:
          filterLifeCal=1.05;
          break;
        case green:
          filterLifeCal=1.15;
          break;
        case orange:
          filterLifeCal=1.2;
          break;
        case red:
        case purple:
          filterLifeCal=1.25;
          break;
        default:
          break;
      }
      break;
    case FAN_SPEED3: //SFLR=0.9;
      switch(ringColor)
      {
        case blue:
          filterLifeCal=1.1;
          break;
        case green:
          filterLifeCal=1.2;
          break;
        case orange:
          filterLifeCal=1.25;
          break;
        case red:
        case purple:
          filterLifeCal=1.3;
          break;
        default:
          break;
      }
      break;
    case FAN_SPEED4://SFLR=1.0;
      switch(ringColor)
      {
        case blue:
          filterLifeCal=1.15;
          break;
        case green:
          filterLifeCal=1.25;
          break;
        case orange:
          filterLifeCal=1.3;
          break;
        case red:
        case purple:
          filterLifeCal=1.35;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  g_FilerUseTimeCnt_fl=g_FilerUseTimeCnt_fl+filterLifeCal;
  if(g_FilerUseTimeCnt_fl>65535.0)
  {
    g_FilerUseTimeCnt_fl=65535.0;//Because just 2 bytes in the EE.
  }
  CalFilterRecValEE();
  filterPercentCal();
}

void filterPercentCal(void)
{
  float fl_fliterTemp1,fl_fliterTemp2;

  fl_fliterTemp1=g_FilerUseTimeCnt_fl;
  fl_fliterTemp1=fl_fliterTemp1*100.0;

  fl_fliterTemp2=U16_FilterOrangeLedTime;

  fl_fliterTemp1=fl_fliterTemp1/fl_fliterTemp2;
  if(fl_fliterTemp1>100.0)
  {
    fl_fliterTemp1=100.0;
  }
  FilterLeftPercent=100-(unsigned char)fl_fliterTemp1;
}

#define START_ADDR  0x00008C00//第36页,用最后1K flash存储数据用 

#define CHECK_LEN 9
extern ErrorStatus IAPRead(uint32_t *Ram_Addr, uint32_t Flash_Addr, uint8_t Len);
extern ErrorStatus IAP_PageProgram(uint32_t address, uint32_t data[], uint32_t length, uint32_t erase);

void iap_init(void)
{
  unsigned char i,j,res=0;
  u32 sum_check=0;

  for(i=0; i<5; i++)
  {
    IAPRead(u32_flashData,START_ADDR, flashlen);//unit is word.

    sum_check=0;
    for(j=0; j<CHECK_LEN; j++)
    {
      sum_check+=u32_flashData[j+1];
    }

    if((e2_Head==0xAAAAAAAA)&&(e2_Tail==0x55555555)&&(sum_check==e2_Check))
    {
      res=1;
      break;
    }
  }

  if(0==res) //fail
  {
    e2_Head=0xAAAAAAAA;
    e2_FilterDec=0x00000000;
    e2_FilterInt=0x00000000;
    e2_Mode=0x00000000;
    e2_Speed=0x00000001;
    e2_Ionizer=0x00000000;
    e2_fg_ONOFF=0x00000000;
    e2_sleep_OnOff=0x00000000;
    e2_AutoSetting=0x00000000;
    e2_Runtime=0x00000000;
    e2_Check=0x00000001;
    e2_Tail=0x55555555;
    IAP_PageProgram(START_ADDR, u32_flashData, flashlen*4, 1);//单位字节，1=先擦除
  }
  //-------------------------------------------------------------
  U16_FilterGreenLedTime=7344;
  U16_FilterOrangeLedTime=8640;
  g_FilerUseTimeCnt_fl=e2_FilterDec;//小数点后1位
  g_FilerUseTimeCnt_fl=g_FilerUseTimeCnt_fl/10.0;
  g_FilerUseTimeCnt_fl=g_FilerUseTimeCnt_fl+e2_FilterInt;
  CalFilterRecValEE();
  filterPercentCal();

  if(1==e2_Mode)
  {
    fg_Smart=1;
  }
  else
  {
    fg_Smart=0;
  }

  switch(e2_Speed)
  {
    case 1:
    case 2:
    case 3:
    case 4:
      g_byFanSpSet=(unsigned char)e2_Speed;
      break;
    default:
      g_byFanSpSet=1;
      break;
  }
  FanGearSeqInSpeedArray=g_byFanSpSet-1;

  if(1==e2_Ionizer)
  {
    fg_Ionizer=1;
  }
  else
  {
    fg_Ionizer=0;
  }

  u32_totalRunTime=e2_Runtime;
  if(u32_totalRunTime>MAX_TOTAL_RUN_TIME)
  {
    u32_totalRunTime=MAX_TOTAL_RUN_TIME;
  }

  if(e2_AutoSetting<=5)
  {
    AutoSetting=e2_AutoSetting;
  }
  else
  {
    AutoSetting=2; //default use setting 3.
  }

}

void delay_ms(int nms)
{
  unsigned int i=0, j=0;

  for(i=0; i<nms; i++)
  {
    for(j=0; j<840; j++)
    {
      __NOP();
    }
  }
}

u32 u32_flashRead[flashlen];
u32 sumIap=0;

static unsigned char JudgeWhetherNeedWriteFlash(void)
{
  unsigned char f_needWr=0;
  unsigned char i=0;
  
  if(g_byTimer1S_DisplayAll2S<2)
  {
    return 0;
  }

  if(API_Gets1SFlag())
  {
    if(g_EERecordDelay) //eeprom record delay
    {
      g_EERecordDelay--;
    }
  }

  if(g_EERecordDelay>0)
  {
    return 0;
  }

  f_needWr=0;
  if(g_FilerUseTimeDecimal!=e2_FilterDec)
  {
    e2_FilterDec=(u32)g_FilerUseTimeDecimal;
    f_needWr=1;
  }

  if(u16_filterLifeInt!=e2_FilterInt)//滤网寿命小时整数部分
  {
    e2_FilterInt=(u32)u16_filterLifeInt;
    f_needWr=1;
  }

  if(fg_Smart!=e2_Mode)
  {
    e2_Mode=(u32)fg_Smart;
    f_needWr=2;
  }

  if(g_byFanSpSet!=e2_Speed)
  {
    e2_Speed=(u32)g_byFanSpSet;
    f_needWr=3;
  }

  if(fg_Ionizer!=e2_Ionizer)
  {
    e2_Ionizer=(u32)fg_Ionizer;
    f_needWr=4;
  }

  if(fg_ONOFF!=e2_fg_ONOFF)
  {
    e2_fg_ONOFF=(u32)fg_ONOFF;
    f_needWr=5;
  }

  if((SLEEP_NONE==sleepStage)&&(1==e2_sleep_OnOff))
  {
    e2_sleep_OnOff=0;
    f_needWr=6;
  }

  if((SLEEP_NONE!=sleepStage)&&(0==e2_sleep_OnOff))
  {
    e2_sleep_OnOff=1;
    f_needWr=7;
  }


  if(AutoSetting!=e2_AutoSetting)
  {
    e2_AutoSetting=(u32)AutoSetting;
    f_needWr=8;
  }
  /*
    if(u32_totalRunTime!=e2_Runtime)
    {
  	e2_Runtime=(u32)u32_totalRunTime;
  	f_needWrFlash=9;
    }
  */
  sumIap=0;
  for(i=0; i<CHECK_LEN; i++)
  {
    sumIap=sumIap+u32_flashData[i+1];
  }

  if(sumIap!=e2_Check)
  {
    e2_Check=sumIap;
    f_needWr=10;
  }

  return (f_needWr);
}

void FlashWrite(void)
{
  unsigned char j,k,resIap;
  unsigned char f_needWrFlash=0;//need write

  f_needWrFlash=JudgeWhetherNeedWriteFlash();
  if(0!=f_needWrFlash)
  {
    e2_Head=0xAAAAAAAA;
    e2_Tail=0x55555555;
    for(j=0; j<5; j++)
    {
      IAP_PageProgram(START_ADDR,u32_flashData,flashlen*4,1);
      delay_ms(5);
      IAPRead(u32_flashRead,START_ADDR,flashlen);//单位为字
      WDT_Clr();
      resIap=0;
      for(k=0; k<flashlen; k++)
      {
        if(u32_flashRead[k]!=u32_flashData[k])
        {
          resIap=1;//fail
          break;
        }
      }

      if(0==resIap) //succ
      {
        break;
      }
    }
  }
}

