
#define DPDRV_GLOBALS_H
#include "main.h"
#include "Drv_Disp.h"
#include "McuDrv.h"
/*
#define SEG_A   0x02
#define SEG_B   0x80
#define SEG_C   0x40
#define SEG_D   0x10
#define SEG_E   0x20
#define SEG_F   0x01
#define SEG_G   0x08
#define SEG_H   0x04


#define _____    0
#define ____X    1
#define X____    2
#define X___X    3
#define XXXXX    4
*/
#define TM1623_DIS_MODE 0x03      //7 com,11 seg
#define TM1623_DATA_COMMAND 0x40  //data address auto add mode
#define TM1623_ADDRESS_SET 0xc0   //address start from 00
#define TM1623_DIS_CONTROLL 0x88    //display light control
//bit3--bit0:light level
//

//#define LED_CHR(a,b,c,d,e)        _LED_CHAR(a>>2,b>>1,b&1,c>>2,d>>1,d&1,e>>2)
/*
#define TURNON_LEVEL    1

#if TURNON_LEVEL
#define _LED_CHAR(a,b,c,d,e,f,g)  \
    (((a)*SEG_A)|((b)*SEG_F)|((c)*SEG_B)|((d)*SEG_G)|((e)*SEG_E)|((f)*SEG_C)|((g)*SEG_D))
#else
#define _LED_CHAR(a,b,c,d,e,f,g)  \
    (((a)*SEG_A)|((b)*SEG_F)|((c)*SEG_B)|((d)*SEG_G)|((e)*SEG_E)|((f)*SEG_C)|((g)*SEG_D))
#endif
*/
void SubTm1623Drv ( void );
void sub_Delay_TM1623 ( void );

//static union uAlly uc595Data;
//static unsigned char byTimer1Ms_DLEDScan;
//static unsigned char byComSelect;

void Api_LEDScanDrv ( void )      //led display driver
{
  switch (LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_ucDisLightLevel = 0x02;//0x03; //50%
      break;
    case _LIGHT_100:
      g_ucDisLightLevel = 0x07;//100%
      break;
    case _LIGHT_AUTO:
      g_ucDisLightLevel = 0x07;//Auto
      break;
    case _LIGHT_OFF: //off
      break;
    default:
      break;
  }
  SubTm1623Drv();   //tm1623 driver
}

//#define TM1623_DIS_MODE 0x03      //7 com,11 seg
//#define TM1623_DATA_COMMAND 0x40  //data address auto add mode
//#define TM1623_ADDRESS_SET 0xc0   //address start from 00
//#define TM1623_DIS_CONTROLL 0x88    //display light control
void SubTm1623Drv ( void )  //tm1623 driver
{
  unsigned char i, j;

  HW_IOSet_TM1623STB ( 0 );
  sub_Delay_TM1623();
  //output tm1623 display mode
  j = TM1623_DIS_MODE;
  for ( i = 0; i < 8; i++ )
  {
    sub_Delay_TM1623();
    HW_IOSet_TM1623CLK ( 0 );

    if ( j & ( 1 << i ) )
    {
      HW_IOSet_TM1623DATA ( 1 );
    }
    else
    {
      HW_IOSet_TM1623DATA ( 0 );
    }

    sub_Delay_TM1623();
    HW_IOSet_TM1623CLK ( 1 );
  }

  //output STB pulse
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 1 );
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 0 );

  //output tm1623 data command
  j = TM1623_DATA_COMMAND;
  for ( i = 0; i < 8; i++ )
  {
    sub_Delay_TM1623();

    HW_IOSet_TM1623CLK ( 0 );
    if ( j & ( 1 << i ) )
    {
      HW_IOSet_TM1623DATA ( 1 );
    }
    else
    {
      HW_IOSet_TM1623DATA ( 0 );
    }
    sub_Delay_TM1623();

    HW_IOSet_TM1623CLK ( 1 );
  }

  //output STB pulse
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 1 );
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 0 );

  //output tm1623 data address
  j = TM1623_ADDRESS_SET;
  for ( i = 0; i < 8; i++ )
  {
    sub_Delay_TM1623();

    HW_IOSet_TM1623CLK ( 0 );
    if ( j & ( 1 << i ) )
    {
      HW_IOSet_TM1623DATA ( 1 );
    }
    else
    {
      HW_IOSet_TM1623DATA ( 0 );
    }
    sub_Delay_TM1623();

    HW_IOSet_TM1623CLK ( 1 );
  }
  //output data
  for ( j = 0; j < 14; j++ )
  {
    for ( i = 0; i < 8; i++ )
    {
      sub_Delay_TM1623();

      HW_IOSet_TM1623CLK ( 0 );
      if ( g_ucDisData[j].byte & ( 1 << i ) )
      {
        HW_IOSet_TM1623DATA ( 1 );
      }
      else
      {
        HW_IOSet_TM1623DATA ( 0 );
      }
      sub_Delay_TM1623();

      HW_IOSet_TM1623CLK ( 1 );
    }
  }
  //output STB pulse
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 1 );
  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 0 );
  sub_Delay_TM1623();

  //output tm1623 display controll
  j = TM1623_DIS_CONTROLL | ( g_ucDisLightLevel & 0x07 );
  for ( i = 0; i < 8; i++ )
  {
    sub_Delay_TM1623();
    HW_IOSet_TM1623CLK ( 0 );
    if ( j & ( 1 << i ) )
    {
      HW_IOSet_TM1623DATA ( 1 );
    }
    else
    {
      HW_IOSet_TM1623DATA ( 0 );
    }

    sub_Delay_TM1623();
    HW_IOSet_TM1623CLK ( 1 );
  }

  sub_Delay_TM1623();
  HW_IOSet_TM1623STB ( 1 );
}


void sub_Delay_TM1623 ( void )
{
  //unsigned char i;
  //{
    NOP_NOP();
    NOP_NOP();
    NOP_NOP();
    NOP_NOP();
    
    // 20190808 add
    NOP_NOP();
    NOP_NOP();
    NOP_NOP();
    NOP_NOP();
  //};
}

