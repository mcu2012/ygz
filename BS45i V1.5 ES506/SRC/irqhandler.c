#include "includes.h"

volatile unsigned char WIFI_TX_OK;


void NMI_IRQHandler(void)
{

}

void HardFault_IRQHandler(void)
{
  while(1);
}

void SVC_IRQHandler(void)
{

}

void PendSV_IRQHandler(void)
{

}

void SysTick_IRQHandler(void)
{

}
//100us
void T16N0_IRQHandler(void)
{
  if(T16Nx_GetFlagStatus(T16N0,T16Nx_IT_MAT0) != RESET)
  {
    T16Nx_ClearITPendingBit(T16N0,T16Nx_IT_MAT0);
    Api_Timer100Us_DCMotorDrv();
  }
}
/*********************************************************
1ms
**********************************************************/
void T16N2_IRQHandler(void)
{
  if(T16Nx_GetFlagStatus(T16N2,T16Nx_IT_MAT0) != RESET)
  {
    T16Nx_ClearITPendingBit(T16N2,T16Nx_IT_MAT0);
    ISR_Timer1MsProcess();
  }
}

/*********************************************************
函数名: void Interrupt_KINT_Handler(void)
描  述: 按键中断处理函数
输入值: 无
输出值: 无
返回值: 无
**********************************************************/
void KINT_IRQHandler(void)
{
  if(KINT_GetITFlag(IO_FAN_FG) != RESET)
  {
    KINT_ClearITFlag(IO_FAN_FG);      // 清除外部中断标志
    ISR_MOTO_FEEDFACK();
  }
}

//------------中断中调用---------------------------
//wifi
void wifiSendByte(unsigned char val)
{
  //UART_SendByte(UART0,val);
  /*EUART_SendByte(val);
  while(EUART_GetFlagStatus(EUART_FLAG_TC) == RESET);
  EUART_ClearITPendingBit(EUART_CLR_TC);
*/
  UART_SendByte(UART0, val);
  while(UART_GetFlagStatus(UART0, UART_FLAG_TC) == RESET);
  UART_ClearITPendingBit(UART0, UART_CLR_TC);
}

void UART0_IRQHandler(void)
{
  unsigned char rxd=0;
/*
  if((EUART_GetITStatus(EUART_IT_TB)!=RESET)&&(EUART_GetFlagStatus(EUART_FLAG_TB) != RESET))
  {
    WIFI_TX_OK=1;
    EUART_ITConfig(EUART_IT_TB, Disable);
  }
*/
/*
  if((EUART_GetITStatus(EUART_IT_RB)!=RESET)&&(EUART_GetFlagStatus(EUART_FLAG_RB)!=RESET))//receive interrupt
  {
    rxd=EUART_RecByte();
    uart_receive_input(rxd);
  }
*/
  if((UART_GetITStatus(UART0,UART_IT_RB)!=RESET)&&(UART_GetFlagStatus(UART0,UART_FLAG_RB)!=RESET))
  {
    rxd = UART_RecByte(UART0);
    uart_receive_input(rxd);
  }
  
}



