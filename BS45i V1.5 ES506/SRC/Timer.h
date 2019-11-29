#ifdef  TIMER_GLOBALS_H
  #define TIMER_EXT
#else
  #define TIMER_EXT extern
#endif

TIMER_EXT void Init_Timer( void );
TIMER_EXT void ISR_Timer1MsProcess( void );
//TIMER_EXT void ISR_Timer100UsProcess( void );
//TIMER_EXT void ISR_Timer320Us_Process( void );
//TIMER_EXT void ISR_Timer240Us_Process( void );
TIMER_EXT void timerDeal( void );
TIMER_EXT unsigned char API_Gets20MsFlag( void );
TIMER_EXT unsigned char API_Gets1SFlag( void );
TIMER_EXT unsigned char API_Gets1MinFlag( void );
//TIMER_EXT unsigned char API_Gets1HFlag( void );
//TIMER_EXT unsigned char API_Gets30MinFlag( void );




