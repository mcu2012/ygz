#define BUZ_GLOBALS_H
#include "includes.h"
//const unsigned int BUZ_SOUND_ERROR[]={TN_1,TN_1,0};
//const unsigned int BUZ_TM_ERROR[]={100,300,0};

const unsigned int BUZ_SOUND_ON[]= {TN_3,TN_1,TN_5,0};
const unsigned int BUZ_TM_ON[]= {100,100,500,0};

const unsigned int BUZ_SOUND_OFF[]= {TN_5,TN_3,TN_1,0};
const unsigned int BUZ_TM_OFF[]= {100,100,500,0};

const unsigned int BUZ_SOUND_KEY[]= {TN_3,TN_1,0};
const unsigned int BUZ_TM_KEY[]= {100,300,0};

const unsigned int BUZ_SOUND_ERROR[6][7]=
{
  {TN_5,0},
  {TN_5,TN_5,0},
  {TN_5,TN_5,TN_5,0},
  {TN_5,TN_5,TN_5,TN_5,0},
  {TN_5,TN_5,TN_5,TN_5,TN_5,0},
  {TN_5,TN_5,TN_5,TN_5,TN_5,TN_5,0},
};
//AutoSetting
#define _ERROR_BUZ_TIME  400 //100
const unsigned int BUZ_TM_ERROR[6][7]=
{
  {_ERROR_BUZ_TIME,0},
  {_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,0},
  {_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,0},
  {_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,0},
  {_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,0},
  {_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,_ERROR_BUZ_TIME,0},
};

const unsigned int BUZ_SOUND_FAN_SLOWDOWN[4]= {TN_5,TN_5,TN_5,0};
const unsigned int BUZ_TM_FAN_SLOWDOWN[4]= {100,100,100,0};


void sub_GetsTune( unsigned int *pTune,unsigned int *pTimer );

static struct sBUZZER sBuzzerVal;

static unsigned int byTimer1Ms_Volt;

static union flagbyte ucBuzzer;
#define f_BuzzerStart ucBuzzer.bt.b0

void HW_Buzzer_Start(void)
{
  HW_IOSet_BuzF(1);
  //T16Nx_Enable(T16N1);
}

void HW_Buzzer_Stop(void)
{
  T16Nx_Disable(T16N1);
  HW_IOSet_BuzF(0);
}

void Init_Buzzer(void)
{
  T16Nx_BaseInitStruType x;
  GPIO_InitSettingType y;
  T16Nx_PWMInitStruType z;

  /* 初始化蜂鸣器开关引脚 */
  y.Signal = GPIO_Pin_Signal_Digital;
  y.Dir = GPIO_Direction_Output;
  y.Func = GPIO_Reuse_Func0;
  y.ODE = GPIO_ODE_Output_Enable;//GPIO_ODE_Output_Disable;
  y.DS = GPIO_DS_Output_Normal;
  y.PUE = GPIO_PUE_Input_Disable;
  y.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_BUZV, &y);

  /* 初始化蜂鸣器发音引脚 */
  y.Signal = GPIO_Pin_Signal_Digital;
  y.Dir = GPIO_Direction_Output;
  y.Func = GPIO_Reuse_Func3;//GPIO_Reuse_Func2;
  y.ODE = GPIO_ODE_Output_Disable;
  y.DS = GPIO_DS_Output_Normal;
  y.PUE = GPIO_PUE_Input_Disable;
  y.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_BZ_PWM, &y);

  /* 初始化T16Nx定时器*/
  x.T16Nx_ClkS = T16Nx_ClkS_PCLK;			  //外部时钟PCLK
  x.T16Nx_SYNC = Disable;
  x.T16Nx_EDGE = T16Nx_EDGE_Rise;
  x.T16Nx_Mode = T16Nx_Mode_PWM;			  //PWM调制
  x.T16Nx_PREMAT = 63;						   //预分频比为1:  64
  T16Nx_BaseInit(T16N1,&x); 				  //初始化定时器T16N1

  /*配置T16N0通道01输出*/
  z.T16Nx_MOE0 = Enable;					 //通道0输出关闭
  z.T16Nx_MOE1 = Disable;					   //通道1输出开启
  z.T16Nx_POL0 = POSITIVE;					  //输出正极性
  z.T16Nx_POL1 = POSITIVE;					  //输出正极性
  z.T16Nx_PWMMODE = T16Nx_PWMMode_INDEP;	  //独立模式输出
  z.PWMDZE = Disable;						  //关闭互补模式死区功能
  T16Nx_PMWOutInit(T16N1,&z);

  T16Nx_MAT0ITConfig(T16N1,T16Nx_Go_No);	 //匹配寄存器值匹配后的工作模式:继续计数，不产生中断
  T16Nx_MAT0Out0Config(T16N1,T16Nx_Out_Switch); //匹配寄存器值匹配后输出端口的工作模式：取反
  T16Nx_MAT1ITConfig(T16N1,T16Nx_Clr_Int);	   //匹配寄存器值匹配后的工作模式:清零并重新计数，产生中断
  T16Nx_MAT1Out0Config(T16N1,T16Nx_Out_Switch); //CNT0匹配到MAT0端口0取反
  T16Nx_SetCNT0(T16N1,0);					  //设置T16N1初始值为0
  T16Nx_Enable(T16N1);
}

static void HW_Buzzer_SetFreq(unsigned int wFreq)
{
  //计算频率的公式简化为f=250000/周期,则周期=250000/f.
  switch(wFreq)
  {
    case TN_3://1.6K
      T16Nx_SetMAT0(T16N1,125000/1625); //占空比
      T16Nx_SetMAT1(T16N1,250000/1625);//周期
      break;
    case TN_5:
      T16Nx_SetMAT0(T16N1,125000/1950); //占空比
      T16Nx_SetMAT1(T16N1,250000/1950);//周期
      break;
    case TN_1:
      T16Nx_SetMAT0(T16N1,125000/1300); //占空比
      T16Nx_SetMAT1(T16N1,250000/1300);//周期
      break;
    default:
      break;
  }
  Init_Buzzer();
}

void buzzerDeal(void)
{
  unsigned int wTemp1;
  unsigned int wTemp2;

  if(f_BuzzerStart)
  {
    //if((0==sBuzzerVal.wCycle)&&(0==fg_BuzV))
    if(0==sBuzzerVal.wCycle)
    {
      sBuzzerVal.byCount++;
      sub_GetsTune( &wTemp2,&wTemp1 );

      if((0==wTemp1)||(0==wTemp2))
      {
        f_BuzzerStart = 0;
        HW_Buzzer_Stop();
      }
      else
      {
        HW_Buzzer_SetFreq(wTemp2);
        sBuzzerVal.wCycle = wTemp1;
        HW_Buzzer_Start();
      }
    }
  }
}

void sub_GetsTune( unsigned int *pTune,unsigned int *pTimer )
{
  if( BUZ_ON == sBuzzerVal.byType )
  {
    *pTune  = BUZ_SOUND_ON[sBuzzerVal.byCount];
    *pTimer = BUZ_TM_ON[sBuzzerVal.byCount];
  }
  else if( BUZ_OFF == sBuzzerVal.byType )
  {
    *pTune  = BUZ_SOUND_OFF[sBuzzerVal.byCount];
    *pTimer = BUZ_TM_OFF[sBuzzerVal.byCount];
  }
  else if( BUZ_FAN_SLOWDOWN_ENABLE == sBuzzerVal.byType )
  {
    *pTune  = BUZ_SOUND_FAN_SLOWDOWN[sBuzzerVal.byCount];
    *pTimer = BUZ_TM_FAN_SLOWDOWN[sBuzzerVal.byCount];
  }
  else if( BUZ_Continuous == sBuzzerVal.byType )
  {
    *pTune  = BUZ_SOUND_ERROR[AutoSetting][sBuzzerVal.byCount];
    *pTimer = BUZ_TM_ERROR[AutoSetting][sBuzzerVal.byCount];
  }
  else
  {
    *pTune  = BUZ_SOUND_KEY[sBuzzerVal.byCount];
    *pTimer = BUZ_TM_KEY[sBuzzerVal.byCount];
  }
}

void API_Buzzer_Set( unsigned char byType )
{
  unsigned int wM;
  unsigned int wN;

  wM = 0;
  wN = 0;
  byTimer1Ms_Volt = 0;

  f_BuzzerStart = 1;
  sBuzzerVal.byType = byType;
  sBuzzerVal.byCount = 0;

  sub_GetsTune( &wN, &wM );

  HW_Buzzer_SetFreq( wN );
  sBuzzerVal.wCycle = wM;
  HW_Buzzer_Start();
}

void Api_Timer1Ms_Buzzer( void )
{
  if(sBuzzerVal.wCycle)
  {
    sBuzzerVal.wCycle--;
  }

  if(sBuzzerVal.wCycle)
  {
    byTimer1Ms_Volt++;
    if(byTimer1Ms_Volt<40)
    {
      HW_IOSet_BuzV(1);
    }
    else
    {
      HW_IOSet_BuzV(0);
    }
  }
  else
  {
    byTimer1Ms_Volt = 0;
    if(f_BuzzerStart)
    {
      HW_Buzzer_Stop();
      HW_IOSet_BuzV(0);
    }
  }
}



