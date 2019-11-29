#ifdef  DPDRV_GLOBALS_H
  #define DPDRV_EXT
#else
  #define DPDRV_EXT extern
#endif

#define _LIGHT_50   0
#define _LIGHT_100  1
#define _LIGHT_AUTO 2
#define _LIGHT_OFF  3

DPDRV_EXT unsigned char LIGHT_LEVEL;

DPDRV_EXT void Api_LEDScanDrv( void );
