/******************************************************************************************
* Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
* �ļ���:  system_HR8P506.c
* ��  ��:  ESM Application Team in
* ��  ��:  V1.00
* ��  ��:  2015/10/27
* ��  ��:  HR8P506ϵ��оƬ��ʼ���ļ�
* ��  ע:  ������HR8P506ϵ��оƬ
*          ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************/

#include "system_HR8P506.h"
#include "HR8P506.h"
#include "lib_scu.h"
#include "lib_gpio.h"

//uint32_t SystemCoreClock = 48000000;
uint32_t SystemCoreClock = 16000000;

void SystemInit(void)
{
/*
  DeviceClockAllEnable();
  SCU_SysClkSelect(SCU_CLK_HRC);//ԭʼʱ��ѡ��Ϊ�ڲ�16M

  //��PLL���໷,����Ϊ�ڲ�HRC 16M,���Ϊ48M,ϵͳʱ��ʹ��PLL
  PLLClock_Config(Enable,SCU_PLL_IN16M,SCU_PLL_48M,Enable);
  //ϵͳʱ�Ӻ��ƵΪ1:1
  SCU_RegUnLock();
  SCU_SysClk_Div1();
  SCU_RegLock();
  */
}

