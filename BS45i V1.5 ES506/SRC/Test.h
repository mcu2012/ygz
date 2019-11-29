#ifdef  TEST_GLOBALS_H
  #define TEST_EXT
#else
  #define TEST_EXT extern
#endif

TEST_EXT unsigned char f_Test;
TEST_EXT unsigned char f_selfCheck;  
TEST_EXT unsigned char f_TestErr_Wifi;

TEST_EXT unsigned int g_TestDisAllCnt;
TEST_EXT unsigned char g_TestState;
TEST_EXT unsigned char g_TestDlyJudge;
TEST_EXT unsigned int  g_TestFanSpeedSet;
TEST_EXT unsigned char g_TestRingColourTYpe;
TEST_EXT unsigned char g_TestGotWifi;

TEST_EXT void testIni( void );
TEST_EXT void Init_Test( void );
TEST_EXT void TestDeal( void );
TEST_EXT void ledTestMode ( void );
TEST_EXT void SetWiFiConfig ( void );
TEST_EXT void SetWiFiSelfcheck ( void );
TEST_EXT void testChangeRingColour(void);


