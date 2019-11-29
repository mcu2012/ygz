#ifdef  FILTER_GLOBALS_H
  #define FILTER_EXT
#else
  #define FILTER_EXT extern
#endif

//#define FILTER_GREEN_LED_TIME  4000
//#define FILTER_ORANGE_LED_TIME 6500

FILTER_EXT unsigned int U16_FilterGreenLedTime; 
FILTER_EXT unsigned int U16_FilterOrangeLedTime; 
FILTER_EXT float  g_FilerUseTimeCnt_fl;
//FILTER_EXT unsigned char g_FilerUseTimeHigh;
//FILTER_EXT unsigned char g_FilerUseTimeLow;
FILTER_EXT unsigned char g_FilerUseTimeDecimal;
FILTER_EXT unsigned int  u16_1sCounterFilter;
FILTER_EXT unsigned char FilterLeftPercent;//@180711

FILTER_EXT void CalFilterRecValEE(void);
FILTER_EXT void filterLife(void);
FILTER_EXT void filterPercentCal(void);
FILTER_EXT void iap_init(void);
FILTER_EXT void FlashWrite(void);


