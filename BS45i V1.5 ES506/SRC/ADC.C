#define ADC_H
#include "includes.h"

//----------变量定义------------
static unsigned char adSampleCountKey = 0;
static unsigned char adSampleCountLight = 0;
static unsigned int adThisResult=0;

#define _AD_BUFF_LEN_KEY 3
static unsigned int ADBUFF_KEY[_AD_BUFF_LEN_KEY];

#define _AD_BUFF_LEN_LIGHT 6
static unsigned int ADBUFF_LIGHT[_AD_BUFF_LEN_LIGHT];

static unsigned char u8_InSleepCount;//进睡眠计时
static unsigned char AdChannel;/*0-Key,1-Light*/
static unsigned int LightADMax;
static unsigned int LightADMin;

//@190105
void ADCInitALL(void)
{
  ADC_InitStruType x1;

  GPIO_InitSettingType y1;
  GPIO_InitSettingType y2;

  y1.Signal = GPIO_Pin_Signal_Analog;
  y1.Dir = GPIO_Direction_Input;
  y1.Func = GPIO_Reuse_Func0;//当选择模拟功能FUN4(A)，FUN5(A)时，这个功能无所谓.
  y1.ODE = GPIO_ODE_Output_Disable;
  y1.DS = GPIO_DS_Output_Normal;
  y1.PUE = GPIO_PUE_Input_Disable;
  y1.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_KEY,&y1);


  y2.Signal = GPIO_Pin_Signal_Analog;
  y2.Dir = GPIO_Direction_Input;
  y2.Func = GPIO_Reuse_Func0;
  y2.ODE = GPIO_ODE_Output_Disable;
  y2.DS = GPIO_DS_Output_Normal;
  y2.PUE = GPIO_PUE_Input_Disable;
  y2.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_LIGHT,&y2);

  x1.CLKS = ADC_CLKS_PCLK;
  x1.CLKDIV = ADC_CLKDIV_1_32;     //ADC转换时钟源一定要符合数据手册中ADC转换时钟源选择表
  x1.VREF_SEL = ADC_VREF_SEL_2_6;//ADC_VREF_SEL_1_8;
  x1.VREFP = ADC_VREFP_VDD;//ADC_VREFP_IO;//选择VDD作为参考电压
  x1.VREFN= ADC_VREFN_VSS;
  x1.CHS = ADC_CHS_AIN7;
  x1.SMPS=ADC_SMPS_HARD;//x1.SMPS = ADC_SMPS_SOFT;
  x1.ST = 8;//x1.ST = 0;
  ADC_Init(&x1);

}

void ADCInitKey(void)
{
  ADC->CHS.CHS = ADC_CHS_AIN15;
}

void ADCInitLight(void)
{
  ADC->CHS.CHS = ADC_CHS_AIN14;
}
/*
static void sub_ADKeySet ( unsigned int byADSour )
{
  static unsigned char byKeybak = 0xFF;
  unsigned char i;

  if ( byADSour > byKeybak )
    i = byADSour - byKeybak;
  else
    i = byKeybak - byADSour;

  if ( i < 10 )
    g_byADKey = byADSour;
  else
    g_byADKey = byKeybak;

  byKeybak = byADSour;
}
*/
void adKey(void)
{
  static unsigned char time_40ms = 0;

  if(0==API_Gets20MsFlag())
  {
    time_40ms++;
    if(time_40ms < 2)
    {
      return;
    }
    time_40ms = 0;
  }

  ADBUFF_KEY[adSampleCountKey++]=adThisResult;
  if(adSampleCountKey<_AD_BUFF_LEN_KEY)
  {
    return;
  }

  adSampleCountKey=0;
  //sub_ADKeySet(ADBUFF_KEY[_ADC_KEY]);//keyProcess(ADBUFF_KEY,_AD_BUFF_LEN_KEY);
  g_byADKey=ADBUFF_KEY[_ADC_KEY];
  AdChannel=1;
  ADCInitLight();
}

//static unsigned char byADC_Lt[10];
//static unsigned char byADC_LtMax;
//static unsigned char byADC_LtMin;
//static unsigned int  wADC_LtSum;
//static unsigned int  wSampleLtA;
//static unsigned int  wSampleLtB;
//光敏采样后的处理，去掉最大值和最小值后取平均
/*void lightProcess(u16 lightData)
{
  unsigned char i;

  //g_byADLight=lightData;

  byADC_Lt[9] = lightData;
  wADC_LtSum = byADC_Lt[9];

  for( i=0; i<9; i++ )
  {
    if( 0 == i )
    {
      byADC_LtMin = 255;
      byADC_LtMax = 0;
    }

    byADC_Lt[i] = byADC_Lt[i+1];
    wADC_LtSum += byADC_Lt[i];

    if( byADC_Lt[i] < byADC_LtMin )
    {
      byADC_LtMin = byADC_Lt[i];
    }
    if( byADC_Lt[i] > byADC_LtMax )
    {
      byADC_LtMax = byADC_Lt[i];
    }
  }

  if(byADC_Lt[9] < byADC_LtMin)
  {
    byADC_LtMin = byADC_Lt[9];
  }

  if(byADC_Lt[9] > byADC_LtMax)
  {
    byADC_LtMax = byADC_Lt[9];
  }

  wSampleLtA = (wADC_LtSum-byADC_LtMin-byADC_LtMax)>>3;
  g_byADLight = (unsigned char)wSampleLtA;
}
*/
//光敏采样
void adLight(void)
{
  unsigned int lightSum=0;
  unsigned char i=0;

  adSampleCountLight++;
  if(adSampleCountLight<2)//Discard first one, cuz it is inaccuracy.
  {
    return;
  }

  ADBUFF_LIGHT[adSampleCountLight-2]=adThisResult;
  if(LightADMax<adThisResult)
  {
    LightADMax=adThisResult;
  }

  if(LightADMin>adThisResult)
  {
    LightADMin=adThisResult;
  }

  if(adSampleCountLight>=_AD_BUFF_LEN_LIGHT+1)
  {
    adSampleCountLight=0;
    lightSum=0;
    for(i=0; i<_AD_BUFF_LEN_LIGHT; i++)
    {
      lightSum+=ADBUFF_LIGHT[i];
    }
    lightSum=lightSum-LightADMax-LightADMin;
    lightSum=(lightSum>>2);
    g_byADLight=lightSum;
    //lightProcess(lightSum);
    LightADMax=0;
    LightADMin=0xFFFF;
    AdChannel=0;
    ADCInitKey();
    return;
  }
}

uint16_t Get_AD_Value(void)
{
  uint8_t i;
  uint32_t max, min, sum, ad_temp;

  static unsigned char ADOnOff=0;
  unsigned int AdSampTimeOut=1000;//AD采样获取完成标志位超时退出计时

  max = 0x00;
  min = 0x0FFF;
  sum = 0x00;

  for (i=0; i<18; i++)
  {
    if(0==ADOnOff)
    {
      ADOnOff=1;
      ADC_Start();
    }

    while( ADC_GetIFStatus(ADC_IF) == RESET)
    {
      if(AdSampTimeOut>0)
      {
        AdSampTimeOut--;
      }
      else
      {
        ADOnOff=0;
        return 0;
      }
    }

    ADOnOff=0;

    ad_temp = ADC_GetConvValue();
    ADC_ClearIFStatus(ADC_IF);
    if (ad_temp > max)
    {
      max = ad_temp;
    }
    if (ad_temp < min)
    {
      min = ad_temp;
    }
    sum += ad_temp;
  }
  sum -= min;
  sum -= max;

  return sum >>4;
}

//按键和光敏采样,被主函数调用
void adSample(void)
{
  if(API_Gets1SFlag())
  {
    if(u8_InSleepCount>0)
    {
      u8_InSleepCount--;
    }
  }

  adThisResult = Get_AD_Value();
  ADC_ClearIFStatus(ADC_IF);

  if(0==AdChannel)
  {
    adKey();
  }
  else
  {
    adLight();
  }
}

