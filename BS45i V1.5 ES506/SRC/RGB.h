#ifdef  RGB_GLOBALS_H
#define RGB_EXT
#else
#define RGB_EXT extern
#endif

#define  RGB_LEN         3
#define  RGB_POINT_NUM  1 // 2
#define  RGB_DATA_BUFF_LEN (RGB_LEN*RGB_POINT_NUM)
#define  RED   0
#define  BLUE  1
#define  GREEN 2

RGB_EXT volatile unsigned char RGB_FRESH_FLAG;
RGB_EXT unsigned char g_RGBByteCount;
RGB_EXT unsigned char g_RGBBitCount;
RGB_EXT unsigned char g_RGBBitValue;
RGB_EXT unsigned char g_RGBBitPulseCount;
RGB_EXT volatile unsigned char g_RGB[RGB_DATA_BUFF_LEN];    
RGB_EXT volatile unsigned char g_RGB2Send[RGB_DATA_BUFF_LEN];
RGB_EXT unsigned char g_BrigRed;
RGB_EXT unsigned char g_BrigGreen;
RGB_EXT unsigned char g_BrigBlue;

RGB_EXT void RGBProcess ( void );
RGB_EXT void RGBOff ( void );
RGB_EXT void RGBOn ( void );
RGB_EXT void RGB_Red ( void );
RGB_EXT void RGB_Blue ( void );
RGB_EXT void RGB_Orange ( void );
//RGB_EXT void RGB_White ( void );
RGB_EXT void RGB_Green ( void );
RGB_EXT void RGB_Pur(void);
//unsigned char b1, unsigned char g2, unsigned char r2, unsigned char b2 );

