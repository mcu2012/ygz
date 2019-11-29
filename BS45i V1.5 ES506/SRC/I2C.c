#define I2C_GLOBALS_H
/*
#include "main.h"
#include "I2C.h"
#include "McuDrv.h"
*/

/*
#include "includes.h"
void sub_SDA_DirectSet( unsigned char byTmp1, unsigned char byI2CChannel );
void sub_SCL_DirectSet( unsigned char byTmp1, unsigned char byI2CChannel );
void sub_SDA_Set( unsigned char byTmp1, unsigned char byI2CChannel );
void sub_SCL_Set( unsigned char byTmp1, unsigned char byI2CChannel );
unsigned char sub_SDA_Read( unsigned char byI2CChannel );


void I2C_Init( unsigned char byI2CChannel )
{
  sub_SDA_DirectSet( _OUTPUT, byI2CChannel );
  sub_SCL_DirectSet( _OUTPUT, byI2CChannel );
  sub_SCL_Set( _HIGH, byI2CChannel );
  I2C_Flash();
  sub_SDA_Set( _HIGH, byI2CChannel );
  I2C_Flash();
}

void I2C_Delay( unsigned int wTmp )
{
  unsigned int i;
  for( i=0; i<wTmp; i++ );
}

void I2C_Flash( void )
{
  unsigned char i;
  
  for( i=0; i<8; i++ );
}

void I2C_Start( unsigned char byI2CChannel )
{
  sub_SDA_DirectSet( _OUTPUT, byI2CChannel );
  sub_SDA_Set( _HIGH, byI2CChannel );
  I2C_Flash();
  sub_SCL_Set( _HIGH, byI2CChannel );
  I2C_Flash();
  sub_SDA_Set( _LOW, byI2CChannel );
  I2C_Flash();
  sub_SCL_Set( _LOW, byI2CChannel );
  I2C_Flash();
}

void I2C_Stop( unsigned char byI2CChannel )
{
  sub_SDA_DirectSet( _OUTPUT, byI2CChannel );
  sub_SDA_Set( _LOW, byI2CChannel );
  I2C_Flash();
  sub_SCL_Set( _HIGH, byI2CChannel );
  I2C_Flash();
  sub_SDA_Set( _HIGH, byI2CChannel );
  I2C_Flash();
}

void I2C_WriteX( unsigned char byTmp, unsigned char byI2CChannel )
{
  unsigned char i,j;
  
  sub_SDA_DirectSet( _OUTPUT, byI2CChannel );
  for( i=0; i<8; i++ )
  {
    j = (byTmp>>(7-i)) & 0x01;
    sub_SCL_Set( _LOW, byI2CChannel );
    I2C_Flash();
    if( j )
      sub_SDA_Set( _HIGH, byI2CChannel );
    else
      sub_SDA_Set( _LOW, byI2CChannel );
    I2C_Flash();
    sub_SCL_Set( _HIGH, byI2CChannel );
    I2C_Flash();
  }
  sub_SCL_Set( _LOW, byI2CChannel );
  I2C_Flash();
  sub_SDA_Set( _HIGH, byI2CChannel );
  I2C_Flash();
}

unsigned char I2C_ReadX( unsigned char byI2CChannel )
{
  unsigned char i,j,k=0;
  unsigned char byTmp;
  
  sub_SCL_Set( _LOW, byI2CChannel );
  I2C_Flash();
  sub_SDA_Set( _HIGH, byI2CChannel );
  sub_SDA_DirectSet( _INPUT, byI2CChannel );

  for( i=0;i<8;i++ )
  {
    I2C_Flash();
    sub_SCL_Set( _HIGH, byI2CChannel );
    I2C_Flash();
    byTmp = sub_SDA_Read( byI2CChannel );
    if( byTmp )
      j = 1;
    else
      j = 0;
        
    k = (k<<1)|j;
    sub_SCL_Set( _LOW, byI2CChannel );
  }
  I2C_Flash();
  return k ;
}

unsigned char I2C_ACK( unsigned char byI2CChannel )
{
  unsigned char byTmp,i=0;
  
  sub_SDA_DirectSet( _INPUT, byI2CChannel );
  sub_SCL_Set( _HIGH, byI2CChannel );
  I2C_Flash();
  byTmp = sub_SDA_Read( byI2CChannel );
  while( (byTmp)&&(i<11) )
    i++;
  sub_SCL_Set( _LOW, byI2CChannel );
  I2C_Flash();
  
  if( i > 9 )
    return 1;
  else
    return 0;
}

void sub_SDA_DirectSet( unsigned char byTmp1, unsigned char byI2CChannel )
{
  switch( byI2CChannel )
  {
    case 1:
      HW_IODirect_SDA1( byTmp1 );
      break;
    default:
      HW_IODirect_SDA0( byTmp1 );
      break;
  }
}

void sub_SCL_DirectSet( unsigned char byTmp1, unsigned char byI2CChannel )
{
  switch( byI2CChannel )
  {
    case 1:
      HW_IODirect_SCL1( byTmp1 );
      break;
    default:
      HW_IODirect_SCL0( byTmp1 );
      break;
  }
}

void sub_SDA_Set( unsigned char byTmp1, unsigned char byI2CChannel )
{
  switch( byI2CChannel )
  {
    case 1:
      HW_IOSet_SDA1( byTmp1 );
      break;
    default:
      HW_IOSet_SDA0( byTmp1 );
      break;
  }
}

void sub_SCL_Set( unsigned char byTmp1, unsigned char byI2CChannel )
{
  switch( byI2CChannel )
  {
    case 1:
      HW_IOSet_SCL1( byTmp1 );
      break;
    default:
      HW_IOSet_SCL0( byTmp1 );
      break;
  }
}

unsigned char sub_SDA_Read( unsigned char byI2CChannel )
{
  unsigned char byTmp1;
  
  switch( byI2CChannel )
  {
    case 1:
      byTmp1 = HW_IORead_SDA1();
      break;
    default:
      byTmp1 = HW_IORead_SDA0();
      break;
  }
  return byTmp1;
}
*/

