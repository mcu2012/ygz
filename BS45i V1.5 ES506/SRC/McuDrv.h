#ifdef  MCUDRV_GLOBALS_H
  #define MCUDRV_EXT
#else
  #define MCUDRV_EXT extern
#endif



MCUDRV_EXT void Handle_McuStart( void );
MCUDRV_EXT void WDT_Clr( void );
MCUDRV_EXT void HW_IOSet_BuzV( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_BuzF( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_DCDrv( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_TM1623CLK( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_TM1623STB( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_TM1623DATA( unsigned char byTmp );
//MCUDRV_EXT void HW_Buzzer_Start(void);
//MCUDRV_EXT void HW_Buzzer_Stop(void);
//MCUDRV_EXT void HW_Buzzer_SetFreq( unsigned int wFreq );
MCUDRV_EXT void HW_ADC_Start(void);
MCUDRV_EXT void HW_ADC_Stop(void);
MCUDRV_EXT unsigned char HW_ADC_GetsValue( void );
MCUDRV_EXT void HW_INTC0_Start( void );
MCUDRV_EXT void HW_INTC0_Stop( void );
MCUDRV_EXT void HW_INTC1_Start( void );
MCUDRV_EXT void HW_INTC1_Stop( void );
MCUDRV_EXT void HW_INTC2_Start( void );
MCUDRV_EXT void HW_INTC2_Stop( void );
MCUDRV_EXT void HW_TAU0_Channel1Start( void );
MCUDRV_EXT void HW_TAU0_Channel4Start( void );
MCUDRV_EXT void HW_TAU0_Channel4Stop( void );
MCUDRV_EXT void HW_TAU0_Channel5Start( void );
MCUDRV_EXT void HW_TAU0_Channel5Stop( void );
MCUDRV_EXT void HW_DutySet( unsigned int wDuty );
MCUDRV_EXT void HW_UART1_Start( void );
//MCUDRV_EXT void HW_UART1_TX_ENABLE( void );
MCUDRV_EXT void HW_UART1_TX_Buffer( unsigned char byTXD );
MCUDRV_EXT void HW_UART1_TX_DISENABLE( void );
MCUDRV_EXT void HW_UART1_RX_ENABLE( void );
MCUDRV_EXT void HW_UART1_RX_DISENABLE( void );
MCUDRV_EXT void HW_UART1_Stop( void );
//MCUDRV_EXT unsigned char HW_IORead_BuzV( void );
MCUDRV_EXT void HW_IODirect_SDA0( unsigned char byTmp );
MCUDRV_EXT void HW_IODirect_SDA1( unsigned char byTmp );
MCUDRV_EXT void HW_IODirect_SDA2( unsigned char byTmp );
MCUDRV_EXT void HW_IODirect_SCL0( unsigned char byTmp );
MCUDRV_EXT void HW_IODirect_SCL1( unsigned char byTmp );
MCUDRV_EXT void HW_IODirect_SCL2( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SDA0( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SDA1( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SDA2( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SCL0( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SCL1( unsigned char byTmp );
MCUDRV_EXT void HW_IOSet_SCL2( unsigned char byTmp );
MCUDRV_EXT unsigned char HW_IORead_SDA0( void );
MCUDRV_EXT void NOP_NOP( void );
//MCUDRV_EXT unsigned char HW_IORead_Key( void );
MCUDRV_EXT unsigned char HW_IORead_Test( void );
MCUDRV_EXT void HW_IOSet_Ionizer( unsigned char byTmp );
MCUDRV_EXT void HW_TAU0_Channel6Start( void );
MCUDRV_EXT void HW_TAU0_Channel6Stop( void );
MCUDRV_EXT void HW_TAU0_Channel7Start( void );
MCUDRV_EXT void HW_TAU0_Channel7Stop( void );
MCUDRV_EXT void HW_IOSet_RGB( unsigned char byTmp );
//MCUDRV_EXT void HW_UART0_TX_Buffer( unsigned char byTXD );
//MCUDRV_EXT void HW_IOSet_DCFanPowerDrv(unsigned char onoff);
MCUDRV_EXT void HW_IOSet_EEWP(unsigned char byTmp);
MCUDRV_EXT void WiFi_TX_Start(void);

