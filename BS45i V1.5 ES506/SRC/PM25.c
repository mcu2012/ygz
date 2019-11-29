#define PM25_H
#include "includes.h"
//*************** ����PM2.5������ PM7003�������� V1.0 2019-11-21 *************

#define PM25_HEAD1          0x42  //ͷ��1
#define PM25_HEAD2          0x4D  //ͷ��2

#define PM25_FRAME_TIME_OUT 20  //֡��ʱ20ms
#define PM25_ERROR_TIME_OUT 8   //PM2.5�����������ж�ʱ��.

static unsigned char PM25PowerOnCnt=0;//power on count

static unsigned char PM25_RxTemp[PM25RxBufferSize];
static unsigned char PM25_RxData[PM25RxBufferSize];
static volatile unsigned char f_PM25RevFrameCheck;

static unsigned char  u8_PM25RxStage;
static unsigned int u16_PM25RxCounter;
static unsigned int U16_PM25_RevLen;
static volatile unsigned char f_PM25_REV_OK;

static unsigned int u16_PM25RxFrameTimeOut;
static unsigned int u16_PM25RxTimeOutErr;

/*******************************************************************************
** ����������.  PM2.5���պ�����.
*******************************************************************************/
void PM25Deal(void)
{
  unsigned char  i;
  unsigned int u16_PM25Sum;
  unsigned int u16_pm25Tmp;
  unsigned int u16_pm1_0Tmp;
  unsigned int u16_pm10Tmp;
  unsigned int u16_PM25RevSum;

  if(fg_ONOFF)
  {
    SENSOR_POWER_ON;
  }
  else
  {
    SENSOR_POWER_OFF;
  }
  
  if(1 != f_PM25_REV_OK)
  {
    return;
  }

  for(i=0; i<arrLen(PM25_RxTemp); i++)
  {
    PM25_RxData[i]=PM25_RxTemp[i];
  }
  f_PM25_REV_OK=0;
  UART1_RxEnable();
  u16_PM25Sum = 0;
  for(i=0; i<30; i++)
  {
    u16_PM25Sum += PM25_RxData[i];
  }

  u16_PM25RevSum=PM25_RxData[30];
  u16_PM25RevSum=(u16_PM25RevSum<<8) | PM25_RxData[31];
  if(u16_PM25RevSum==u16_PM25Sum)
  {
    u16_pm1_0Tmp=PM25_RxData[4];
    u16_pm1_0Tmp=(u16_pm1_0Tmp<<8) | PM25_RxData[5];

    u16_pm25Tmp=PM25_RxData[6];
    u16_pm25Tmp=(u16_pm25Tmp << 8) | PM25_RxData[7];

    u16_pm10Tmp=PM25_RxData[8];
    u16_pm10Tmp=(u16_pm10Tmp<<8) | PM25_RxData[9];
    
    if(u16_pm25Tmp>999)
    {
      u16_pm25Tmp=999;
    }

    if(PM25PowerOnCnt<10) //ǰ10������ݲ�Ҫ,��ʱ���ݲ�׼,��ֹ������ɫ�仯����.
    {
      u16_Pm25Val=0;
    }
    else
    {
      u16_Pm25Val=u16_pm25Tmp;
    }
    u16_PM25RxTimeOutErr=PM25_ERROR_TIME_OUT;
  }
}

/*******************************************************************************
** ��1ms�ж������. ���ڽ���֡��ʱ�ж�.
*******************************************************************************/
void PM25Ms(void)
{
  if(f_PM25RevFrameCheck)
  {
    if(u16_PM25RxFrameTimeOut != 0)
    {
      u16_PM25RxFrameTimeOut--;
    }
    else
    {
      f_PM25RevFrameCheck=0;
      u8_PM25RxStage=0;
      u16_PM25RxCounter=0;
    }
  }
  else
  {
    u16_PM25RxFrameTimeOut=PM25_FRAME_TIME_OUT;
  }
}

/*******************************************************************************
** ÿ1�����һ��. ��ʱ����.
*******************************************************************************/
void PM25Sec(void)
{
  if(fg_ONOFF)                                                            //����
  {
    if(u16_PM25RxTimeOutErr != 0)
    {
      u16_PM25RxTimeOutErr--;
    }
    else
    {
      u16_PM25RxTimeOutErr=PM25_ERROR_TIME_OUT;
    }

    if(PM25PowerOnCnt<0xFF)
    {
      PM25PowerOnCnt++;
    }
  }
  else
  {
    u16_PM25RxTimeOutErr=PM25_ERROR_TIME_OUT;
    PM25PowerOnCnt=0;
  }
}

/*******************************************************************************
** ���ڽ����ж������. PM2.5���մ���.
*******************************************************************************/
static void ISR_UART_RX(void)
{
  if(0==f_PM25_REV_OK)
  {
    u16_PM25RxFrameTimeOut=PM25_FRAME_TIME_OUT;
    f_PM25RevFrameCheck=1;
    if(0==u8_PM25RxStage) //ͷ���жϽ׶�
    {
      PM25_RxTemp[u16_PM25RxCounter]=UART_RecByte(UART1);//����һ���ֽ�,����MCU�޸�.
      if(0==u16_PM25RxCounter)
      {
        if(PM25_HEAD1==PM25_RxTemp[u16_PM25RxCounter])
        {
          u16_PM25RxCounter++;
        }
      }
      else if(1==u16_PM25RxCounter)
      {
        if(PM25_HEAD2==PM25_RxTemp[u16_PM25RxCounter])
        {
          u16_PM25RxCounter++;
          u8_PM25RxStage=1;
        }
        else
        {
          u16_PM25RxCounter=0;
        }
      }
    }
    else if(1==u8_PM25RxStage)//���ݽ��ս׶�
    {
      PM25_RxTemp[u16_PM25RxCounter++]=UART_RecByte(UART1);//����һ���ֽ�,����MCU�޸�.

      if(4==u16_PM25RxCounter) //����Э��,֡���Ƚ������
      {
        U16_PM25_RevLen= PM25_RxTemp[2];
        U16_PM25_RevLen=( U16_PM25_RevLen << 8 ) | PM25_RxTemp[3];
        U16_PM25_RevLen=U16_PM25_RevLen + 4;
        if(U16_PM25_RevLen > PM25RxBufferSize)
        {
          U16_PM25_RevLen=PM25RxBufferSize;
        }
      }

      if(u16_PM25RxCounter>=4)
      {
        if(u16_PM25RxCounter >= U16_PM25_RevLen)
        {
          f_PM25_REV_OK=1; //���ݳ����жϽ������
          U16_PM25_RevLen=u16_PM25RxCounter;
          u8_PM25RxStage=0;
          f_PM25RevFrameCheck=0;
        }
      }
    }

    if(u16_PM25RxCounter==U16_PM25_RevLen)
    {
      u16_PM25RxCounter=0;
      u8_PM25RxStage=0;
      u16_PM25RxCounter=0;
      f_PM25RevFrameCheck=0;
      if(1==f_PM25_REV_OK)
      {
        UART1_RxDisable();//�رս����ж�
      }
    }
  }
}

/*******************************************************************************
** PM2.5�����������жϺ���.
*******************************************************************************/
void UART1_IRQHandler(void)
{
  if((UART_GetITStatus(UART1,UART_IT_TB)!=RESET)
      &&(UART_GetFlagStatus(UART1,UART_FLAG_TB) != RESET))
  {
    //ISR_UART_TX();
  }

  if((UART_GetITStatus(UART1,UART_IT_RB) != RESET)
      && (UART_GetFlagStatus(UART1, UART_FLAG_RB) != RESET))
  {
    ISR_UART_RX();
  }
}


