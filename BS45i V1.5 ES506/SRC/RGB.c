#define RGB_GLOBALS_H
#include "includes.h"

void di(void)
{
	__ASM("CPSID i");
}
void ei(void)
{
	__ASM("CPSIE i");
}

void delay_RGB ( unsigned int dly )
{
  uint i;

  for ( i = dly; i > 0; i-- )
  {
    NOP_NOP();
  }
}

#define delay20us delay_RGB(4)

/*
主控MCU传输方式：
50us low+8bit data（红光pwm占空比）+ 8bit data（蓝光pwm占空比）+8bit data（绿光pwm占空比）
logic 1是20us high+10us low，
logic 0是10us high+20us low。
data高位先发，依HT66F018写的测试程序如附件所示。

HT45F0060端：接收的判断high pulse有15us高即为高电平，复位时间为48us，RGB电流恒流为15ma，
RGB pwm频率为7.8Khz，duty阶数为256级。

*/
void RGBProcess ( void )
{
  unsigned char i;
  unsigned char j;

  if ( 0 == RGB_FRESH_FLAG ) // no new data
  {
    RGB_ON;
    return;
  }
  RGB_FRESH_FLAG = 0;
  di();
  RGB_OFF;
  delay_RGB(20);// 50us,@16Mhz
  for ( j = 0; j < RGB_DATA_BUFF_LEN; j++ )
  {
    for ( i = 8; i > 0; i-- )
    {
      if ( g_RGB2Send[j] & 0x80 )
      {
        RGB_ON;
        delay20us; //20us
        RGB_OFF;
        //NOP_NOP();//delay10us(); //10us
      }
      else
      {
        RGB_ON;
        //NOP_NOP();//delay10us(); //10us
        RGB_OFF;//IO_RGB_LIGHT = 0;
        delay20us; //20us
      }
      g_RGB2Send[j] = ( g_RGB2Send[j] << 1 );
    }
  }
  RGB_ON;
  ei();
}

void RGBOff ( void )
{
  uchar i;

  g_BrigRed = 0;
  g_BrigGreen = 0;
  g_BrigBlue = 0;
  for ( i = 0; i < RGB_DATA_BUFF_LEN; i++ )
  {
    g_RGB[i] = 0;
    g_RGB2Send[i] = g_RGB[i];
  }
  RGB_FRESH_FLAG = 1;
  RGBProcess();
}

void RGBOn ( void )
{
  uchar i;

  g_RGB[RED] = g_BrigRed;
  g_RGB[BLUE] = g_BrigBlue;
  g_RGB[GREEN] = g_BrigGreen;
  for ( i = 0; i < RGB_DATA_BUFF_LEN; i++ )
  {
    g_RGB2Send[i] = g_RGB[i];
  }
  RGB_FRESH_FLAG = 1;
  RGBProcess();
}

void RGB_Pur ( void )
{
  switch(LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_BrigRed = 90;
      g_BrigGreen = 0;
      g_BrigBlue = 128;
      break;
    case _LIGHT_100:
    case _LIGHT_AUTO:
      g_BrigRed = 180;
      g_BrigGreen = 0;
      g_BrigBlue = 255;
      break;
    case _LIGHT_OFF:
      break;
    default:
      break;
  }
  RGBOn();
  ringColor=purple;
}
void RGB_Red ( void )
{
  switch(LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_BrigRed = 100;
      g_BrigBlue = 0;
      g_BrigGreen = 0;
      break;
    case _LIGHT_100:
    case _LIGHT_AUTO:
      g_BrigRed = 255;
      g_BrigBlue = 0;
      g_BrigGreen = 0;
      break;
    case _LIGHT_OFF:
      break;
    default:
      break;
  }
  RGBOn();
  ringColor=red;
}

void RGB_Blue ( void )
{
  switch(LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_BrigBlue = 100;
      g_BrigGreen = 100;
      g_BrigRed = 0;
      break;
    case _LIGHT_100:
    case _LIGHT_AUTO:
      g_BrigBlue = 255;
      g_BrigGreen = 255;
      g_BrigRed = 0;
      break;
    case _LIGHT_OFF:
      break;
    default:
      break;
  }
  RGBOn();
  ringColor=blue;
}
void RGB_Green ( void )
{
  switch(LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_BrigRed = 0;
      g_BrigBlue = 0;
      g_BrigGreen = 100;
      break;
    case _LIGHT_100:
    case _LIGHT_AUTO:
      g_BrigRed = 0;
      g_BrigBlue = 0;
      g_BrigGreen = 255;
      break;
    case _LIGHT_OFF:
      break;
    default:
      break;
  }
  RGBOn();
  ringColor=green;
}
void RGB_Orange ( void )
{
  //orange
  switch(LIGHT_LEVEL)
  {
    case _LIGHT_50:
      g_BrigRed = 100;
      g_BrigBlue = 0;
      g_BrigGreen = 50;
      break;
    case _LIGHT_100:
    case _LIGHT_AUTO:
      g_BrigRed = 255;
      g_BrigBlue = 0;
      g_BrigGreen = 100;
      break;
    case _LIGHT_OFF:
      break;
    default:
      break;
  }
  RGBOn();
  ringColor=orange;
}

