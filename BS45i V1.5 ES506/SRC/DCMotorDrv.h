#ifdef  DC_GLOBALS_H
#define DC_EXT
#else
#define DC_EXT extern
#endif

#define FAN_SPEED1 1
#define FAN_SPEED2 2
#define FAN_SPEED3 3
#define FAN_SPEED4 4

/*---------------------电机反馈方式选择,用哪个开哪个--------------------------*/
/*使用的是外部中断加100us定时器方式*/
#define FEEDBACK_INT           0

/*使用的是定时器捕捉输入,可能是指定脉冲个数的时间,也可能是指定脉冲个数的时间,
随单片机而变*/
//#define FEEDBACK_CAP           1 

//#define MOTO_FEEDBACK_METHOD  FEEDBACK_CAP
//-------------------------------------------------------------------------------------
//@MODIFY_ACCORD_PROJECT 根据实际产品调整 
/*120000000 */
#define DC2RO      1200000 
#define Percent10  150 //399  
//#define Percent15 599  /*本机型空载电机最低启动值*/
//#define Percent20 799  
//#define Percent30 1199 
//#define Percent40 1599 
//#define Percent90 3599 
//#define IO_MOTO_15V    IO_MOTO_15V
#define _FAN_PERIOD_ 3999

struct sDCMOTOR
{
  unsigned int  wTimer100UsRun;
  //unsigned int  wTimer100UsSet;
  unsigned int  wDuty;
  unsigned char byPulseCount;
  unsigned char byErrorCount;
  unsigned char f_DCTimerEnd: 1;
  unsigned char f_DCError   : 1;
};

union flagbyteDC
{
  unsigned char byte;
  struct
  {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
  }bt;
};
DC_EXT union flagbyteDC ucDCFlag;
#define f_Err_DCMotor    ucDCFlag.bt.b0
#define f_DCFanStart      ucDCFlag.bt.b1

DC_EXT struct sDCMOTOR sDCMotorVal;

DC_EXT unsigned int g_wDCFanSpeedSetLast;
DC_EXT unsigned int g_wDCFanSpeedSet;//设定转速
DC_EXT volatile unsigned char g_byDcFanErrorHoldS; //风机无反馈持续时间,用于故障判断
#define DC_FAN_POWEROFF_DELAY_10S	30

DC_EXT unsigned char  g_DcFanPower15vOffDelay;		//dc fan power off delay 10s
#define DC_FAN_POWEROFF_DELAY_TIME	30

DC_EXT unsigned int  g_wDCFanSpeedCalc;
DC_EXT unsigned int u16_wind1FbNum;
DC_EXT volatile unsigned char u8_pulseCnt;
DC_EXT volatile unsigned int  u16_fanOnDlyConfirmErr; //@181108 延迟再判为没有反馈
DC_EXT unsigned int  TestFanSpeed;

DC_EXT unsigned char AutoSetting;// 0-5 : setting1-setting6
DC_EXT unsigned char g_byFanSpSet;
#define RING_COLOUR_LAST_TIME 300
DC_EXT unsigned int g_RingLastTime;
DC_EXT unsigned char FanGearSeqInSpeedArray;//record sequence in the speed array.

DC_EXT void Api_Timer100Us_DCMotorDrv( void );
DC_EXT void MotoPwmDutySet ( unsigned int pwm_ratio );
DC_EXT void ISR_MOTO_FEEDFACK ( void );
DC_EXT void T32NxInit(void);
DC_EXT void testFanIni(void);
DC_EXT void fanDeal( void );
DC_EXT void sub_AutoFanSet( void );

