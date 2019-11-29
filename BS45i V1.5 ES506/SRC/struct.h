#define _ADC_LIGHT            0 // P2.0 <-> P2.1
#define _ADC_KEY              1 // P2.0 <-> P2.1
//#define _ADC_DUST             2

#define _TIMER_POWERON        30
#define _TIMER_OFFREDFLASH    600
#define _TIMER_ONFIXED        30
#define _TIMER_ONG2Y          5
#define _TIMER_SLEEPOFF       30
#define _TIMER_DARKKEYOFF     30
#define _TIMER_DARKIN         3//30
#define _TIMER_DARKEXIT       3//30
#define _TIMER_EEERRDISP      10
#define _TIMER_AIRLAMPHOLD    3
#define _TIMER_AIRLAMPFORCED  60

#define _FLOFF            0
#define _FLWEI            1
#define _FLLOW            2
#define _FLMID            3
#define _FLHIGH           4
#define _FLHIADD          5

#define _OUTPUT           0
#define _INPUT            1
#define _HIGH             1
#define _LOW              0

#define _OFF              0
#define _GREEN            1
#define _YELLOW           2
#define _RED              3
#define _REDFLASH         4
#define _YELFLASH         5
#define _GREEN2           11
#define _YELLOW2          12
#define _RED2             13

union flagbyte
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

union flagword
{
  unsigned int word;
  struct
  {
    unsigned char lo;
    unsigned char hi;
  }byte;
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
    unsigned char b8 : 1;
    unsigned char b9 : 1;
    unsigned char bA : 1;
    unsigned char bB : 1;
    unsigned char bC : 1;
    unsigned char bD : 1;
    unsigned char bE : 1;
    unsigned char bF : 1;
  }bt;
};

struct sBUZZER
{
  unsigned char byType;
  unsigned int  wCycle;
  
  unsigned char byCount;
};

struct sUARTPM
{
  unsigned char byBuff[6];
  unsigned char byTimer1S_Uart2Pm;
  unsigned char byPoint;
  unsigned char f_Head : 1;
  unsigned char f_RxOK : 1;
};
/*
struct sDCMOTOR
{
  unsigned int  wTimer100UsRun;
  unsigned int  wTimer100UsSet;
  unsigned int  wDuty;
  unsigned char byPulseCount;
  unsigned char byErrorCount;
  unsigned char f_DCTimerEnd:1;
  unsigned char f_DCError   :1;
};
*/
union uAlly
{
  struct
  {
    unsigned char MateL;
    unsigned char MateH;
  }by;
  unsigned  int Mate;
};

struct sRMT
{
  union flagbyte ucRMTBuff[21];
  unsigned char byType;
  unsigned char byPulseTime;
  unsigned char byBitpoint;
  unsigned char f_Rece_OK  :1;
  unsigned char f_Head     :1;
  unsigned char f_Remote152:1;
  unsigned char f_Remote96 :1;
};
