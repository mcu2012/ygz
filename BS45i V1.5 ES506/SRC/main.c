#define MAIN_GLOBALS_H
#include "includes.h"

extern void delay_RGB ( unsigned int dly );

//�õ�������ʹ��.
void DeviceClockEnable(void)
{
  uint32_t prot_tmp;

  prot_tmp = SCU->PROT.PROT;
  if(prot_tmp != 0)   	//д������
    SCU_RegUnLock();  	//����

  SCU->PCLKEN.Word = 0xFFFFFFFF;//����������ʱ��

  if(prot_tmp != 0)   	//д������
    SCU_RegLock();   	//��д����

  return;
}

void McuIoInit(void)
{
  GPIO_InitSettingType GpioSetOut;
  GPIO_InitSettingType GpioSetIn;
  KINT_InitSettingType InterruptSet;

  //���
  GpioSetOut.Signal = GPIO_Pin_Signal_Digital;   //��������
  GpioSetOut.Dir = GPIO_Direction_Output;        //����Ϊ���
  GpioSetOut.Func = GPIO_Reuse_Func0;            //���ù���0��GPIO����
  GpioSetOut.ODE = GPIO_ODE_Output_Disable ;     //��©��� Disable
  GpioSetOut.DS = GPIO_DS_Output_Normal;         //��ͨ�������
  GpioSetOut.PUE = GPIO_PUE_Input_Disable;       //������
  GpioSetOut.PDE = GPIO_PDE_Input_Disable;       //������

  GPIO_Init(IO_FAN_DRV,&GpioSetOut);
  GPIO_Init(IO_IONIZER,&GpioSetOut);
  GPIO_Init(IO_DC_FAN_15V,&GpioSetOut);
  GPIO_Init(IO_RGB_LIGHT,&GpioSetOut);

  GPIO_Init(IO_DUST_TXD,&GpioSetOut);//dust txd
  GPIO_Init(IO_SENSOR_POWER,&GpioSetOut);

  GPIO_Init(IO_WIFI_TXD,&GpioSetOut);
  GPIO_Init(GPIO_Pin_A14,&GpioSetOut);//ICP SCL
  GPIO_Init(GPIO_Pin_A15,&GpioSetOut);//ICP SDA
  //#ifdef debug
  //GPIO_Init(IO_TEST,&GpioSetOut);
  //#endif
  // 1623
  GPIO_Init(IO_TM1623_STB,&GpioSetOut);
  GPIO_Init(IO_TM1623_CLK,&GpioSetOut);
  GPIO_Init(IO_TM1623_DATA,&GpioSetOut);

  //δʹ�õ�
  GPIO_Init(GPIO_Pin_A6,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_A6);//�����ֲᣬ����̶���ƽ
  GPIO_Init(GPIO_Pin_A16,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_A16);
  GPIO_Init(GPIO_Pin_A27,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_A27);
  GPIO_Init(GPIO_Pin_B8,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_B8);
  GPIO_Init(GPIO_Pin_B9,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_B9);
  GPIO_Init(GPIO_Pin_B10,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_B10);
  GPIO_Init(GPIO_Pin_B11,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_B11);
  GPIO_Init(GPIO_Pin_B13,&GpioSetOut);
  GPIO_SetBit(GPIO_Pin_B13);

  //--------------- ���� --------------------------
  GpioSetIn.Signal = GPIO_Pin_Signal_Digital;
  GpioSetIn.Dir = GPIO_Direction_Input;
  GpioSetIn.Func = GPIO_Reuse_Func0;
  GpioSetIn.ODE = GPIO_ODE_Output_Disable;
  GpioSetIn.DS = GPIO_DS_Output_Normal;
  GpioSetIn.PUE = GPIO_PUE_Input_Disable;
  GpioSetIn.PDE = GPIO_PDE_Input_Disable;

  GPIO_Init(IO_FAN_FG, &GpioSetIn);
  GPIO_Init(IO_TEST,&GpioSetIn);
  GPIO_Init(IO_LIGHT_ADC,&GpioSetIn);
  GPIO_Init(IO_KEY_ADC,&GpioSetIn);
  GPIO_Init(IO_DUST_RXD,&GpioSetIn);
  GPIO_Init(IO_WIFI_RXD,&GpioSetIn);

  /* 
  datasheet:
  PA0/AVREFN�ܽ�δ��������ڷ�װ�ڲ����߽ӵ�VSS��������ADCģ�鸺�ο���ѹ AVREFN��
  ��Ӧ�ó���  �У��轫PA0/AVREFN����Ϊ����ܽţ���GPIO_PADIR<0>=1����ֹ���� PA0 
  �ܽ�����ߵ�ƽ
  */
  GPIO_Init(GPIO_Pin_A0,&GpioSetIn);
  //------------�ⲿ�ж�--------------------------
  //�������
  InterruptSet.IE_Set = KINT_IE_Set_Enable;
  InterruptSet.Trigger_Style = KINT_Trigger_Rising_Edge;
  KINT_Init(IO_FAN_FG, &InterruptSet);
}
//���Ź���ʼ��
void WdtInit(void)
{
  WDT_InitStruType x;

  WDT_RegUnLock();
  x.WDT_Tms = 2000;           //ι������2s.
  x.WDT_IE = Enable;
  x.WDT_Rst = Enable;
  x.WDT_Clock = WDT_CLOCK_PCLK;
  WDT_Init(&x);
  WDT_Enable();               //WDT���ڽ�������Enable.
  WDT_RegLock();

  NVIC_Init(NVIC_WDT_IRQn, NVIC_Priority_0, Enable);
}


void T16N0Init(void)
{
  T16Nx_BaseInitStruType timer0;

  timer0.T16Nx_ClkS = T16Nx_ClkS_PCLK;   //�ڲ�ʱ��PCLK
  timer0.T16Nx_EDGE = T16Nx_EDGE_Rise;
  timer0.T16Nx_Mode = T16Nx_Mode_TC0;  //��ʱ��������ģʽ
  timer0.T16Nx_PREMAT = 1;
  timer0.T16Nx_SYNC = Disable;
  T16Nx_BaseInit(T16N0,&timer0);

  /* ���ö�ʱ����ÿ��100us����һ���ж�, 1600*(1/16Mhz) */
  T16Nx_MAT0ITConfig(T16N0,T16Nx_Clr_Int); //MAT0ƥ������

  T16Nx_SetCNT0(T16N0,0);//����T16N1��ʼֵΪ0
  T16Nx_SetMAT0(T16N0,1600);//����T16N0MAT0ֵ
  T16Nx_ITConfig(T16N0,T16Nx_IT_MAT0,Enable);//ʹ��T16N0ƥ��0�ж�

  NVIC_Init(NVIC_T16N0_IRQn,NVIC_Priority_1,Enable);//�ж�����
  T16Nx_Enable(T16N0);
}

/*********************************************************
������: void T16N2Init(void)
��  ��: T16N2��ʼ���ӳ���,1ms
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void T16N2Init(void)
{
  T16Nx_BaseInitStruType timer2;

  timer2.T16Nx_ClkS = T16Nx_ClkS_PCLK;   //�ڲ�ʱ��PCLK
  timer2.T16Nx_EDGE = T16Nx_EDGE_Rise;
  timer2.T16Nx_Mode = T16Nx_Mode_TC0;  //��ʱ��������ģʽ
  timer2.T16Nx_PREMAT = 1;
  timer2.T16Nx_SYNC = Disable;
  T16Nx_BaseInit(T16N2,&timer2); //��ʼ����ʱ��T16N2

  /* ���ö�ʱ����ÿ��1ms����һ���ж�, 16000*(1/16Mhz) */
  T16Nx_MAT0ITConfig(T16N2,T16Nx_Clr_Int); //MAT0ƥ������

  T16Nx_SetCNT0(T16N2,0);//����T16N0��ʼֵΪ0
  T16Nx_SetMAT0(T16N2,16000);//����T16N0MAT0ֵ
  T16Nx_ITConfig(T16N2,T16Nx_IT_MAT0,Enable);//ʹ��T16N0ƥ��0�ж�

  NVIC_Init(NVIC_T16N2_IRQn,NVIC_Priority_1,Enable);//�ж�����
  T16Nx_Enable(T16N2);

}


void UART1Init(void)
{
  GPIO_InitSettingType uart1Set;
  UART_InitStruType uart1Type;

  uart1Set.Signal = GPIO_Pin_Signal_Digital;
  uart1Set.Func = GPIO_Reuse_Func2;
  uart1Set.Dir = GPIO_Direction_Output;
  uart1Set.ODE = GPIO_ODE_Output_Disable;
  uart1Set.DS = GPIO_DS_Output_Normal;	//GPIO_DS_Output_Strong;
  GPIO_Init(IO_DUST_TXD, &uart1Set);		//TXD

  uart1Set.Signal = GPIO_Pin_Signal_Digital;
  uart1Set.Func = GPIO_Reuse_Func2;
  uart1Set.Dir = GPIO_Direction_Input;
  uart1Set.PUE = GPIO_PUE_Input_Disable;
  uart1Set.PDE = GPIO_PDE_Input_Disable;
  GPIO_Init(IO_DUST_RXD, &uart1Set);		//RXD

  uart1Type.UART_BaudRate = 9600;			//2����??��
  uart1Type.UART_ClockSet = UART_Clock_3;		//����?��????��o
  uart1Type.UART_RxMode = UART_DataMode_8;	//?����?��y?Y??��?��o8??��y?Y
  uart1Type.UART_RxPolar = UART_Polar_Normal;	//?����????��??D?��o?y3��
  uart1Type.UART_StopBits = UART_StopBits_1;	//����?1??��o1
  uart1Type.UART_TxMode = UART_DataMode_8;	//����?����y?Y??��?��o8??��y?Y
  uart1Type.UART_TxPolar = UART_Polar_Normal;	//����?��???��??D?��o?y3��

  UART_Init(UART1, &uart1Type);

  UART_TBIMConfig(UART1, UART_TBIM_Byte);//����?��?o3??�¡�??��??2������?D??
  UART_RBIMConfig(UART1, UART_TBIM_Byte);//?����??o3??�¡�??��??2������?D??
  UART_ITConfig(UART1, UART_IT_RB, Enable);//Uart1 ?����??D??��1?��
  NVIC_Init(NVIC_UART1_IRQn, NVIC_Priority_1, Enable);//��1?��?D??
  //NVIC_Init(NVIC_UART1_IRQn, NVIC_Priority_3, Enable);//��1?��?D??

  UART1_TxDisable();//???1����?��
  UART1_RxEnable();//��1?��?����?
}

/*********************************************************
������: void UART0Init(void)
��  ��: UART0��ʼ���ӳ���
����ֵ: ��
���ֵ: ��
����ֵ: ��
**********************************************************/
void UART0Init(void)
{
    GPIO_InitSettingType x;
    UART_InitStruType y;

    x.Signal = GPIO_Pin_Signal_Digital;
    x.Dir = GPIO_Direction_Output;
    x.Func = GPIO_Reuse_Func1;
    x.ODE = GPIO_ODE_Output_Disable;
    x.DS = GPIO_DS_Output_Strong;
    x.PUE = GPIO_PUE_Input_Disable;
    x.PDE = GPIO_PDE_Input_Disable;
    GPIO_Init(GPIO_Pin_A23, &x);        //TXD - PA23

    x.Signal = GPIO_Pin_Signal_Digital;
    x.Func = GPIO_Reuse_Func1;
    x.Dir = GPIO_Direction_Input;
    x.ODE = GPIO_ODE_Output_Disable;
    x.DS = GPIO_DS_Output_Strong;
    x.PUE = GPIO_PUE_Input_Disable;
    x.PDE = GPIO_PDE_Input_Disable;
    GPIO_Init(GPIO_Pin_A22, &x);        //RXD - PA22

    y.UART_StopBits = UART_StopBits_1;  //����?1??��o1
    y.UART_TxMode = UART_DataMode_8;    //����?����y?Y??��?��o8??��y?Y
    y.UART_TxPolar = UART_Polar_Normal; //����?��???��??D?��o?y3��
    y.UART_RxMode = UART_DataMode_8;    //?����?��y?Y??��?��o8??��y?Y
    y.UART_RxPolar = UART_Polar_Normal; //?����????��??D?��o?y3��
    y.UART_BaudRate = 9600;             //2����??��
    y.UART_ClockSet = UART_Clock_1;     //����?��????��oPclk
    UART_Init(UART0, &y);

    UART_TBIMConfig(UART0, UART_TBIM_Byte);
    UART_RBIMConfig(UART0, UART_TBIM_Byte);
    UART_ITConfig(UART0, UART_IT_RB, Enable);
    NVIC_Init(NVIC_UART0_IRQn, NVIC_Priority_1, Enable);

    UART0_TxEnable();
    UART0_RxEnable();
}

void SysInit(void)
{
  iap_init();  //���ڲ�flash
  T16N0Init();//100us,fan drive.
  T16N2Init();// 1ms��ʱ��
  UART0Init(); //wifi
  UART1Init(); //UART1��ʼ��,PM2.5.
#ifdef _DOG_OFF

#else
  WdtInit();
#endif
  ADCInitALL();
  Init_Timer();
  Init_Disp();
  Init_Mode();
  WiFi_Init();
  WDT_Clr();
  Init_Test();
  fg_ONOFF_LAST=1;
  LIGHT_LEVEL=_LIGHT_100;
  e2_DCFanSp1=42;//���ñ����ӦΪ100Hz
  e2_DCFanSp2=62;
  e2_DCFanSp3=84;
  e2_DCFanSp4=100;
  API_Buzzer_Set(BUZ_KEY);
}

// 16Mhzʵ��1.032ms
/*
void delay_ms(int nms)
{
  unsigned int i=0, j=0;

  //testOut(1);
  for(i=0; i<nms; i++)
  {
    for(j=0; j<840; j++)
    {
      __NOP();
    }
  }
  //testOut(0);
}
*/
int main ( void )
{
  //delay_RGB(0xFFFF);
  DeviceClockEnable();//����������ʱ��
  McuIoInit();
  SysInit();
  while(1)
  {
    WDT_Clr();
    timerDeal();
    display();
    adSample();
    buzzerDeal();
    fanDeal();
    keyDeal();
    modeDeal();
    PM25Deal();
    TestDeal();
    WiFi_Deal();
    FlashWrite();
  }
}
