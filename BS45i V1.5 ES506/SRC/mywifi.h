#ifdef  WIFI_GLOBALS_H
#define WIFI_EXT
#else
#define WIFI_EXT extern
#endif

//#modify 这里的数值要和涂鸦网站上各个模式的数值匹配
#define  MANNUAL_MODE        0
#define  AUTO_MODE           1
#define  SLEEP_MODE          2

//串口发送完标志.外部只要处理一次:在外部的发送结束中断函数里置为1.
WIFI_EXT volatile unsigned char f_WifiTxOver;

WIFI_EXT void WiFi_Init(void);
WIFI_EXT void WiFi_UartPutChar(unsigned char senddata);
WIFI_EXT void WiFi_Deal(void);
WIFI_EXT void WiFi_KeyInWifiCfgMode(void);
WIFI_EXT void WiFi_SetWiFiSelfcheck(void);
WIFI_EXT void WiFi_AllDataUpdate(void);

