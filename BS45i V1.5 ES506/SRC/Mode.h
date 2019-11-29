#ifdef  MODE_GLOBALS_H
  #define MODE_EXT
#else
  #define MODE_EXT extern
#endif

typedef enum 
{
  blue=0,
  green, 
  orange,
  red,
  purple
} RING_LIGHT;
MODE_EXT RING_LIGHT ringColor;
MODE_EXT unsigned char byTimer1S_DarkIn;


MODE_EXT void Init_Mode( void );
MODE_EXT void modeDeal( void );
MODE_EXT void JudgeRingColour ( void );
MODE_EXT void powerSet(unsigned char onOff);
MODE_EXT void modeSet(unsigned char SetMode);
MODE_EXT void anionSet(unsigned char onOff);
MODE_EXT void lockSet(unsigned char onOff);

