#ifdef PM25_H
#define PM25_EXT
#else
#define PM25_EXT extern 
#endif

#define PM25RxBufferSize   40

PM25_EXT unsigned int u16_Pm25Val;
PM25_EXT unsigned char g_TestPm25ErrCount;
PM25_EXT unsigned char f_Err_SensorPM25;
PM25_EXT unsigned char g_byAQIPM25;


PM25_EXT void PM25Deal(void);
PM25_EXT void PM25Ms(void);
PM25_EXT void PM25Sec(void);

