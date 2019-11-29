#define MCUDRV_GLOBALS_H
#include "includes.h"

void WDT_Clr( void )
{
  WDT_Clear();
}

void HW_IOSet_BuzV(unsigned char byTmp)
{
  if(1==byTmp)
  {
    IO_BUZ_ON;
  }
  else
  {
    IO_BUZ_OFF;
  }
}

void HW_IOSet_BuzF( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IO_BUZ_ON;
  }
  else
  {
    IO_BUZ_OFF;
  }
}

void HW_IOSet_Ionizer( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IONIZER_ON;
  }
  else
  {
    IONIZER_OFF;
  }
}

void HW_IOSet_DCDrv( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IO_FANDRV_ON;
  }
  else
  {
    IO_FANDRV_OFF;
  }
}

void HW_IOSet_TM1623CLK( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IO_TM1623_CLK_ON;
  }
  else
  {
    IO_TM1623_CLK_OFF;
  }
}

void HW_IOSet_TM1623STB( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IO_TM1623_STB_ON;
  }
  else
  {
    IO_TM1623_STB_OFF;
  }
}

void HW_IOSet_TM1623DATA( unsigned char byTmp )
{
  if(1==byTmp)
  {
    IO_TM1623_DATA_ON;
  }
  else
  {
    IO_TM1623_DATA_OFF;
  }
}

unsigned char HW_ADC_GetsValue( void )
{
  unsigned char byTmp;
  
  //byTmp = ADCRH;
  
  return byTmp;
}

unsigned char HW_IORead_Test( void )
{
  unsigned char byTmp;
  
  //byTmp = IO_TEST;
  if(0==GPIO_ReadBit(IO_TEST))
  {
    byTmp=0;
  }
  else
  {
    byTmp=1;
  }
  
  return byTmp;
}


void NOP_NOP( void )
{
  __NOP();
}

void HW_IOSet_RGB( unsigned char byTmp )
{
  if(1==byTmp)
  {
    RGB_ON;
  }
  else
  {
    RGB_OFF;
  }
}

void WiFi_TX_Start(void)
{
  EUART_ITConfig(EUART_IT_TB, Enable);	//使能发送中断
  EUART_TxEnable();
}


