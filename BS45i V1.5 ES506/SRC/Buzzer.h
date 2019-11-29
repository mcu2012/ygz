#ifdef  BUZ_GLOBALS_H
  #define BUZ_EXT
#else
  #define BUZ_EXT extern
#endif


/* C Tune  */
//#define TN_1    0x0EEF  /*   1046  */
//#define TN_2    0x0D4F  /*   1175  */
//#define TN_3    0x0BD7  /*   1318  */
//#define TN_4    0x0B2F  /*   1397  */
//#define TN_5    0x09F7  /*   1568  */
//#define TN_6    0x08DF  /*   1760  */
//#define TN_7    0x07EF  /*   1967  */
//#define TN_H1   0x0777  /*   1967  */

// buzzer fixfreq=1.5Khz~~2.5KHz
#define TN_1          0x0C07  //    1300Hz
#define TN_2          0x0AAF  //    1462.5Hz
#define TN_3          0x099F  //    1625Hz
#define TN_4          0x0907  //    1733Hz
#define TN_5          0x07FF  //    1950Hz
#define TN_6          0x0737  //    2166.7Hz
#define TN_7          0x0667  //    2437.5Hz
#define TN_H1         0x05FF  //    2600Hz

#define BUZ_ON        0
#define BUZ_OFF       1
#define BUZ_KEY       2
#define BUZ_Continuous     3
#define BUZ_FAN_SLOWDOWN_ENABLE   4

BUZ_EXT void buzzerDeal( void );
BUZ_EXT void API_Buzzer_Set( unsigned char byType );
BUZ_EXT void Api_Timer1Ms_Buzzer( void );
BUZ_EXT void Init_Buzzer( void );


