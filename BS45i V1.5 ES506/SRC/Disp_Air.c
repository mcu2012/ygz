#if 0
#define  DISP_GLOBALS_H
#include "main.h"
#include "I2C.h"
#include "McuDrv.h"
#include "Disp_Air.h"
#include "Timer.h"

#define _I2C_ADD_3236   0x78
#define _I2C_CH_3236    0 

#define _REG_SHUTDOWN   0x00
#define _REG_PWM01      0x01
#define _REG_PWM02      0x02
#define _REG_PWM03      0x03
#define _REG_PWM04      0x04
#define _REG_PWM05      0x05
#define _REG_PWM06      0x06
#define _REG_PWM07      0x07
#define _REG_PWM08      0x08
#define _REG_PWM09      0x09
#define _REG_PWM10      0x0A
#define _REG_PWM11      0x0B
#define _REG_PWM12      0x0C
#define _REG_PWM13      0x0D
#define _REG_PWM14      0x0E
#define _REG_PWM15      0x0F
#define _REG_PWM16      0x10
#define _REG_PWM17      0x11
#define _REG_PWM18      0x12
#define _REG_PWM19      0x13
#define _REG_PWM20      0x14
#define _REG_PWM21      0x15
#define _REG_PWM22      0x16
#define _REG_PWM23      0x17
#define _REG_PWM24      0x18
#define _REG_PWM25      0x19
#define _REG_PWM26      0x1A
#define _REG_PWM27      0x1B
#define _REG_PWM28      0x1C
#define _REG_PWM29      0x1D
#define _REG_PWM30      0x1E
#define _REG_PWM31      0x1F
#define _REG_PWM32      0x20
#define _REG_PWM33      0x21
#define _REG_PWM34      0x22
#define _REG_PWM35      0x23
#define _REG_PWM36      0x24
#define _REG_UPDATE     0x25
#define _REG_LED01      0x26
#define _REG_LED02      0x27
#define _REG_LED03      0x28
#define _REG_LED04      0x29
#define _REG_LED05      0x2A
#define _REG_LED06      0x2B
#define _REG_LED07      0x2C
#define _REG_LED08      0x2D
#define _REG_LED09      0x2E
#define _REG_LED10      0x2F
#define _REG_LED11      0x30
#define _REG_LED12      0x31
#define _REG_LED13      0x32
#define _REG_LED14      0x33
#define _REG_LED15      0x34
#define _REG_LED16      0x35
#define _REG_LED17      0x36
#define _REG_LED18      0x37
#define _REG_LED19      0x38
#define _REG_LED20      0x39
#define _REG_LED21      0x3A
#define _REG_LED22      0x3B
#define _REG_LED23      0x3C
#define _REG_LED24      0x3D
#define _REG_LED25      0x3E
#define _REG_LED26      0x3F
#define _REG_LED27      0x40
#define _REG_LED28      0x41
#define _REG_LED29      0x42
#define _REG_LED30      0x43
#define _REG_LED31      0x44
#define _REG_LED32      0x45
#define _REG_LED33      0x46
#define _REG_LED34      0x47
#define _REG_LED35      0x48
#define _REG_LED36      0x49
#define _REG_GCTRL      0x4A
#define _REG_RESET      0x4F

#define _OPEN3236       0x01
#define _OFF3236        0x00 

void sub_SN3236_Soft_SW( unsigned char byMode );
void sub_SN3236_Hard_SW( unsigned char byMode );
void sub_SN3236_DataUpdate( void );
void sub_Write_3236REG( unsigned char byRegAdd, unsigned char byRegData );
void sub_Write_3236REG_Series( unsigned char byRegAdd, unsigned char byRegData, unsigned char byUnit);
void sub_Write_3236REG_Table( unsigned char byRegAdd,unsigned char *Table );
void sub_Write_3236REG_PartialTable( unsigned char byRegAdd, unsigned char *Table, unsigned char byTableAdd, unsigned char byUnit );
void sub_Timer_DispAir( void );

const unsigned char PWM_TABLE[] = 
{
   255,   97,   255,   97,   255,   97,   255,   97,   255,   97,
   255,   97,   255,   97,   255,   97,   255,   97,   255,   97, 
   255,   97,   255,   97,   255,   97,   255,   97,   255,   97,
   255,   97,   255,   97,   255,   97
};

const unsigned char PWM_RED_TABLE[] = 
{
   255,   0,     255,   0,     255,   0,     255,   0,     255,   0,
   255,   0,     255,   0,     255,   0,     255,   0,     255,   0, 
   255,   0,     255,   0,     255,   0,     255,   0,     255,   0,
   255,   0,     255,   0,     255,   0
};

const unsigned char PWM_GREEN_TABLE[] = 
{
   0,     255,   0,     255,   0,     255,   0,     255,   0,     255,
   0,     255,   0,     255,   0,     255,   0,     255,   0,     255, 
   0,     255,   0,     255,   0,     255,   0,     255,   0,     255,
   0,     255,   0,     255,   0,     255
};

const unsigned char PWM_TABLE2[] = 
{
   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,   10,   10,   10,   10, 
   10,   10,   10,   10,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10
};

const unsigned char PWM_RED_TABLE2[] = 
{
   10,   0,     10,   0,     10,   0,     10,   0,     10,   0,
   10,   0,     10,   0,     10,   0,     10,   0,     10,   0, 
   10,   0,     10,   0,     10,   0,     10,   0,     10,   0,
   10,   0,     10,   0,     10,   0
};

const unsigned char PWM_GREEN_TABLE2[] = 
{
   0,     10,   0,     10,   0,     10,   0,     10,   0,     10,
   0,     10,   0,     10,   0,     10,   0,     10,   0,     10, 
   0,     10,   0,     10,   0,     10,   0,     10,   0,     10,
   0,     10,   0,     10,   0,     10
};

unsigned char YELLOW_TABLE[] =  //const unsigned char YELLOW_TABLE[] = 
{
   1,     7,     1,     7,     1,     7,     1,     7,     1,     7,
   1,     7,     1,     7,     1,     7,     1,     7,     1,     7,
   1,     7,     1,     7,     1,     7,     1,     7,     1,     7,
   1,     7,     1,     7,     1,     7
};

static unsigned char byAQIColourBak;
static unsigned char byAQIColourBak2;
static unsigned char byTimer20Ms_AQIDisp;
static unsigned char byAQIColourSet;
static unsigned int  wTimerTest;
static unsigned char byLampStep;

static union flagbyte ucAirFlag1;
#define f_FlashBak    ucAirFlag1.bt.b0
#define f_3236Status  ucAirFlag1.bt.b1
#define f_LampStep    ucAirFlag1.bt.b2
//#define f_DarkBak     ucAirFlag1.bt.b3

void Init_FL3236( void )
{
  unsigned char i,j;
  
  ucAirFlag1.byte = 0;
  
  sub_SN3236_Hard_SW( _OPEN3236 );
  sub_SN3236_Soft_SW( _OPEN3236 );
  sub_Write_3236REG_Series( _REG_PWM01, 0, 36 );
  sub_Write_3236REG_Table( _REG_LED01, YELLOW_TABLE );
  sub_SN3236_DataUpdate();
  
  byAQIColourBak = g_byAQIColourDisp;
  //f_DarkBak = fg_Dark;
} 

void sub_Write_3236REG( unsigned char byRegAdd, unsigned char byRegData )
{
  unsigned char i,j;
  
  I2C_Start( _I2C_CH_3236 );
  I2C_WriteX( _I2C_ADD_3236, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_WriteX( byRegAdd, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_WriteX( byRegData, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_Stop( _I2C_CH_3236 );
}

void sub_Write_3236REG_Series( unsigned char byRegAdd, unsigned char byRegData, unsigned char byUnit)
{
  unsigned char i,j;
  
  I2C_Start( _I2C_CH_3236 );
  I2C_WriteX( _I2C_ADD_3236, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_WriteX( byRegAdd, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  for( i=0; i<byUnit; i++ )
  {
    I2C_WriteX( byRegData, _I2C_CH_3236 );
    j = I2C_ACK( _I2C_CH_3236 );
  }
  I2C_Stop( _I2C_CH_3236 );
}

void sub_Write_3236REG_Table( unsigned char byRegAdd,unsigned char *Table )
{
  unsigned char i,j;
  
  I2C_Start( _I2C_CH_3236 );
  I2C_WriteX( _I2C_ADD_3236, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_WriteX( byRegAdd, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  for( i=0; i<36; i++ )
  {
    I2C_WriteX( Table[i], _I2C_CH_3236 );
    j = I2C_ACK( _I2C_CH_3236 );
  }
  I2C_Stop( _I2C_CH_3236 );
}

void sub_Write_3236REG_PartialTable( unsigned char byRegAdd, unsigned char *Table, unsigned char byTableAdd, unsigned char byUnit )
{
  unsigned char i,j;
  
  I2C_Start( _I2C_CH_3236 );
  I2C_WriteX( _I2C_ADD_3236, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  I2C_WriteX( byRegAdd, _I2C_CH_3236 );
  j = I2C_ACK( _I2C_CH_3236 );
  for( i=byTableAdd; i<(byTableAdd+byUnit); i++ )
  {
    I2C_WriteX( Table[i], _I2C_CH_3236 );
    j = I2C_ACK( _I2C_CH_3236 );
  }
  I2C_Stop( _I2C_CH_3236 );
}

void sub_SN3236_Soft_SW( unsigned char byMode )
{
  sub_Write_3236REG( _REG_SHUTDOWN, byMode );
}

void sub_SN3236_Hard_SW( unsigned char byMode )
{
  if( 0x00 == byMode )
  {
    //HW_IOSet_SDB( 0 );
  }
  else
  {
    //HW_IOSet_SDB( 1 );
  }
}

void sub_SN3236_DataUpdate( void )
{
  sub_Write_3236REG( _REG_UPDATE, 0xff );
}

void sub_Timer_DispAir( void )
{
  if( API_Gets20MsFlag() )
  {
    if( byTimer20Ms_AQIDisp )
      byTimer20Ms_AQIDisp --;
  }
  
  if( API_Gets1SFlag() )
  {
    if( wTimerTest < 0xFFFF )
      wTimerTest ++;
  }
}
#endif
