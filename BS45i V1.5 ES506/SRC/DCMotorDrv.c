#define DC_GLOBALS_H
#include "includes.h"


void sub_MotorDrvStart( void );
void sub_MotorDrvStop( void );
void sub_DCMotorSpeedCalc ( struct sDCMOTOR *pDCMotor );
void sub_DCMotorErr ( struct sDCMOTOR *pDCMotor );
void sub_DcMotorDrv_Timer ( void );
void DCMotoPower( unsigned char byTmp );

//@MODIFY_ACCORD_PROJECT
void DCMotoPower( unsigned char byTmp )
{
  //IO_MOTO_15V = byTmp;
  if(0==byTmp)
  {
    MOTO_15V_OFF;
  }
  else
  {
    MOTO_15V_ON;
  }
}

void sub_MotorDrvStart ( void )
{
  MotoPwmDutySet(DCSPINIT);
  T32NxInit();
}

void Fan_Pwm_Off(void)
{
  GPIO_InitSettingType x;

  x.Signal = GPIO_Pin_Signal_Digital;
  x.Dir = GPIO_Direction_Output;
  x.Func = GPIO_Reuse_Func0;
  x.ODE = GPIO_ODE_Output_Disable ;
  x.DS = GPIO_DS_Output_Normal;
  x.PUE = GPIO_PUE_Input_Disable;
  x.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_FAN_DRV,&x);

  GPIO_WriteBit(IO_FAN_DRV,0);
}

//@MODIFY_ACCORD_PROJECT
void sub_MotorDrvStop ( void )
{
  T32Nx_Disable(T32N0);
  Fan_Pwm_Off();
}

void sub_DCMotorSpeedCalc(struct sDCMOTOR *pDCMotor)
{
  if(pDCMotor->f_DCTimerEnd)
  {
    g_wDCFanSpeedCalc=(unsigned int)(DC2RO/(unsigned long)pDCMotor->wTimer100UsRun);
    pDCMotor->wTimer100UsRun = 0;
    pDCMotor->byPulseCount   = 0;
    pDCMotor->f_DCTimerEnd   = 0;
    u16_fanOnDlyConfirmErr=0;
    if(0==fanAdjustInterval)
    {
      //Slow down the adjustment speed before reaching the target speed.
      if((g_wDCFanSpeedCalc>g_wDCFanSpeedSet+200)||(g_wDCFanSpeedSet>g_wDCFanSpeedCalc+200))
      {
        fanAdjustInterval=_FAN_ADJUST_TIME;
      }
      
      pDCMotor->wDuty=TjFanSpeed(g_wDCFanSpeedSet, g_wDCFanSpeedCalc);
      if((pDCMotor->wDuty)>=DCSPINITMAX)
      {
        if(g_wDCFanSpeedCalc<100)// <100r/min
        {
          pDCMotor->f_DCError=1;
          if(pDCMotor->byErrorCount<3)
          {
            pDCMotor->byErrorCount++;
            pDCMotor->wTimer100UsRun=0;
            pDCMotor->wDuty=DCSPINIT;
          }
        }
      }
    }
  }
  else
  {
    sub_DCMotorErr(pDCMotor);//没有反馈
  }

  if(pDCMotor->byErrorCount>=3)
  {
    f_Err_DCMotor=1;
  }
}

//No feedback.
void sub_DCMotorErr(struct sDCMOTOR *pDCMotor)
{
  if(u16_fanOnDlyConfirmErr<1500)//防止风机刚启动时转速低导致的进入
  {
    return;
  }

  if(pDCMotor->wTimer100UsRun>5000)//500ms
  {
    pDCMotor->wTimer100UsRun=0;
    if(pDCMotor->wDuty<DCSPINITMAX)
    {
      pDCMotor->wDuty=(pDCMotor->wDuty)+300;//根据不同负载调整数值，这个值的修改会影响故障判定时间
    }

    if(pDCMotor->wDuty>=DCSPINITMAX)//else
    {
      pDCMotor->wDuty=DCSPINITMAX;
      pDCMotor->f_DCError=1;
      if(pDCMotor->byErrorCount<3)
      {
        pDCMotor->byErrorCount++;
        pDCMotor->wDuty=DCSPINIT;
      }
    }
  }
}

void sub_DcMotorDrv_Timer ( void )
{
  if(API_Gets1SFlag())
  {
    if(1==sDCMotorVal.f_DCError) //dc fan error
    {
      g_byDcFanErrorHoldS++;
      if(g_byDcFanErrorHoldS>10)
      {
        sDCMotorVal.f_DCError=0;
        g_byDcFanErrorHoldS=0;
      }
    }

    if(g_DcFanPower15vOffDelay>0)
    {
      g_DcFanPower15vOffDelay--;
    }
  }
}

//@MODIFY_ACCORD_PROJECT
//自动风逻辑
/*
void JudgeAutoFanSpeed ( void )
{
  u8 air;

  if(f_selfCheck||f_Test)
  {
    return;
  }

  if(fg_ONOFF)
  {
    if(_AutoMod==workMod)
    {
      air=getAirAqi();
      switch(air)
      {
        case AQI_EXCELLENT:
          g_FanSpeedGear=FAN_SPEED1;
          break;
        case AQI_FINE:
          g_FanSpeedGear=FAN_SPEED2;
          break;
        case AQI_POOR:
          g_FanSpeedGear=FAN_SPEED3;
          break;
        case AQI_AWFUL:
          g_FanSpeedGear=FAN_SPEED4;
          break;
        default:
          break;
      }
    }
  }
}
*/
//=============== could be call by other functions or modify ===============
#ifdef FEEDBACK_INT
//moto feedback interrupt.
//电机反馈对应的外部中断,上升沿触发
void ISR_MOTO_FEEDFACK ( void )
{
  if(sDCMotorVal.f_DCTimerEnd)//When had got pulse and in calculation.
  {
    return;
  }

  sDCMotorVal.byPulseCount++;
  if(24==sDCMotorVal.byPulseCount)
  {
    sDCMotorVal.f_DCTimerEnd = 1;
    sDCMotorVal.byPulseCount = 0;
  }
  else if(sDCMotorVal.byPulseCount>24)
  {
    sDCMotorVal.byPulseCount=0;
  }
}
#endif

void Api_Timer100Us_DCMotorDrv ( void )
{
  if((sDCMotorVal.wTimer100UsRun<0xffff) && (!sDCMotorVal.f_DCTimerEnd))
  {
    sDCMotorVal.wTimer100UsRun++;
  }
}

//PWM占空比设置,和单片机有关
//@MODIFY_ACCORD_PROJECT
void MotoPwmDutySet ( unsigned int pwm_ratio )
{
  T32Nx_SetMAT3(T32N0,pwm_ratio);//设置占空比
}

//---------------以下针对506芯片-----------------------------------------------
unsigned int Period=_FAN_PERIOD_;

void T32NxInit(void)
{
  T32Nx_BaseInitStruType x;
  GPIO_InitSettingType y;
  T32Nx_PWMInitStruType z;

  //初始化PWM输出口
  y.Signal = GPIO_Pin_Signal_Digital;
  y.Dir = GPIO_Direction_Output;
  y.Func = GPIO_Reuse_Func2;
  y.ODE = GPIO_ODE_Output_Disable;
  y.DS = GPIO_DS_Output_Normal;
  y.PUE = GPIO_PUE_Input_Disable;
  y.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_FAN_DRV, &y);                     //配置PA3为T32N0输出通道

  //初始化T32Nx定时器
  x.T32Nx_ClkS = T32Nx_ClkS_PCLK;                 //内部PCLK
  x.T32Nx_SYNC = Disable;
  x.T32Nx_EDGE = T32Nx_EDGE_Rise;
  x.T32Nx_Mode = T32Nx_Mode_PWM;
  x.T32Nx_PREMAT = 16;//20;                     //预分频比为1:20
  T32Nx_BaseInit(T32N0,&x);

  //初始化T32N0 PWM输出
  z.T32Nx_MOE0 = Disable;                         //禁止0通道
  z.T32Nx_MOE1 = Enable;                          //使能通道1输出
  T32Nx_PMWOutInit(T32N0,&z);                     //初始化T32N0为PWM模式

  T32Nx_MAT2ITConfig(T32N0,T32Nx_Clr_Int);
  T32Nx_MAT3ITConfig(T32N0,T32Nx_Go_Int);
  T32Nx_MAT2Out1Config(T32N0,T32Nx_Out_High);//T32Nx_MAT2Out1Config(T32N0,T32Nx_Out_Low);     //CNT0匹配到MAT2端口输出
  T32Nx_MAT3Out1Config(T32N0,T32Nx_Out_Low); //T32Nx_MAT3Out1Config(T32N0,T32Nx_Out_High);
  T32Nx_SetCNT(T32N0,0);                          //初始值0
  T32Nx_SetMAT2(T32N0,Period);                    //设置MAT2值为周期
  T32Nx_SetMAT3(T32N0,DCSPINIT);                //设置MAT3值为占空比

  T32Nx_ITConfig(T32N0,T32Nx_IT_MAT2,Enable);
  NVIC_Init(NVIC_T32N0_IRQn,NVIC_Priority_1,Enable);
  T32Nx_Enable(T32N0);

  //return;
}
//T32N0中断处理函数
void T32N0_IRQHandler(void)
{
  if(T32Nx_GetFlagStatus(T32N0,T32Nx_IT_MAT2) != RESET)
  {
    T32Nx_ClearITPendingBit(T32N0,T32Nx_IT_MAT2);
  }
}

//电路板自检和整机自检的电机初始化
void testFanIni(void)
{
  TestFanSpeed=500;
  sDCMotorVal.wDuty=DCSPINIT;
}

//转速获取
unsigned char sub_DCFanSet(unsigned char byFanSpSet)
{
  unsigned char byDCFansp;

  //目标转速
  switch ( byFanSpSet )
  {
    case FAN_SPEED1:
      byDCFansp = e2_DCFanSp1;
      break;
    case FAN_SPEED2:
      byDCFansp = e2_DCFanSp2;
      break;
    case FAN_SPEED3:
      byDCFansp = e2_DCFanSp3;
      break;
    case FAN_SPEED4:
      byDCFansp = e2_DCFanSp4;
      break;
    default:
      byDCFansp = 0;
      break;
  }

  if(0==fg_ONOFF)
  {
    byDCFansp = 0;
  }

  return byDCFansp;
}

void Handle_DCMotorDrv(unsigned char byDCSp)
{
  //unsigned char test_time_left;

  g_wDCFanSpeedSet = ( unsigned int ) byDCSp * 10UL;
  //因为环灯RGB需要关闭中断，影响风机调速，所以做一下补偿
  switch(g_byFanSpSet)
  {
    case FAN_SPEED1:
      g_wDCFanSpeedSet=g_wDCFanSpeedSet+4;//15
      break;
    case FAN_SPEED2:
      g_wDCFanSpeedSet=g_wDCFanSpeedSet+12;//2
      break;
    case FAN_SPEED3:
      g_wDCFanSpeedSet=g_wDCFanSpeedSet+20;//5
      break;
    case FAN_SPEED4:
      g_wDCFanSpeedSet=g_wDCFanSpeedSet+23;//20
      break;
    default:
      break;
  }

  if(g_wDCFanSpeedSetLast!=g_wDCFanSpeedSet)
  {
    g_wDCFanSpeedSetLast=g_wDCFanSpeedSet;
    fanAdjustInterval=_FAN_ADJUST_TIME;
  }

  if((0==fg_ONOFF)||(f_Err_DCMotor))
  {
    g_wDCFanSpeedSet=0;
  }

  if((f_Test)||(f_selfCheck))
  {
    if(0==g_TestDlyJudge)
    {
      g_wDCFanSpeedSet=0;
    }
    else
    {
      g_wDCFanSpeedSet=TestFanSpeed;
    }
  }

  //dc fan power control
  if(0!=g_wDCFanSpeedSet) //dc fan on
  {
    g_DcFanPower15vOffDelay=DC_FAN_POWEROFF_DELAY_TIME;
    DCMotoPower(1);   //fan power on
  }
  else    //dc fan want to off
  {
    if(0==g_DcFanPower15vOffDelay)
    {
      DCMotoPower(0); //fan power off
    }
  }

  //dc fan control
  if((0!=g_wDCFanSpeedSet) && (0==sDCMotorVal.f_DCError))
  {
    if(0==f_DCFanStart)
    {
      f_DCFanStart=1;
      sub_MotorDrvStart();
      sDCMotorVal.wTimer100UsRun = 0;
      u16_fanOnDlyConfirmErr=0;
    }
    sub_DCMotorSpeedCalc(&sDCMotorVal);
    MotoPwmDutySet(sDCMotorVal.wDuty);
  }
  else
  {
    f_DCFanStart = 0;
    sub_MotorDrvStop();
    sDCMotorVal.wDuty = DCSPINIT;
    Fan_OFFctrl();
  }
  sub_DcMotorDrv_Timer();

  if((f_Test)||(f_selfCheck))
  {
    //test_time_left=getTestTimeLeft();
    if((g_TestDlyJudge>0)&&(g_TestDlyJudge<2))//判断时间之内
    {
      if((g_wDCFanSpeedCalc<(TestFanSpeed-150))
          ||(g_wDCFanSpeedCalc>(TestFanSpeed+150)))
      {
        f_Err_DCMotor = 1;
      }
      else
      {
        f_Err_DCMotor = 0;
      }
    }
  }
}

void fanDeal( void )
{
  unsigned char DCFanRun;

  sub_AutoFanSet();
  DCFanRun=sub_DCFanSet(g_byFanSpSet);
  Handle_DCMotorDrv(DCFanRun);
}

void fanAuto(void)
{
  switch(ringColor)
  {
    case blue:
      g_byFanSpSet=FAN_SPEED1;
      FanGearSeqInSpeedArray=g_byFanSpSet-1;
      break;
    case green:
      g_byFanSpSet=FAN_SPEED2;
      FanGearSeqInSpeedArray=g_byFanSpSet-1;
      break;
    case orange:
      g_byFanSpSet=FAN_SPEED3;
      FanGearSeqInSpeedArray=g_byFanSpSet-1;
      break;
    case red:
    case purple:
      g_byFanSpSet=FAN_SPEED4;
      FanGearSeqInSpeedArray=g_byFanSpSet-1;
      break;
    default:
      break;
  }
}
void sub_AutoFanSet( void )
{
  if((0==fg_Smart)||(0==fg_ONOFF))
  {
    return;
  }

  fanAuto();
}

