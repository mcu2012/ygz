/******************************************************************************************
* Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd
* 文件名:  system_HR8P506.c
* 作  者:  ESM Application Team in
* 版  本:  V1.00
* 日  期:  2015/10/27
* 描  述:  HR8P506系列芯片初始化文件
* 备  注:  适用于HR8P506系列芯片
*          本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
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
  SCU_SysClkSelect(SCU_CLK_HRC);//原始时钟选择为内部16M

  //打开PLL锁相环,输入为内部HRC 16M,输出为48M,系统时钟使用PLL
  PLLClock_Config(Enable,SCU_PLL_IN16M,SCU_PLL_48M,Enable);
  //系统时钟后分频为1:1
  SCU_RegUnLock();
  SCU_SysClk_Div1();
  SCU_RegLock();
  */
}

