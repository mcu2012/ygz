#ifdef  WIFI_GLOBALS_H
#define WIFI_EXT
#else
#define WIFI_EXT extern
#endif

//#modify �������ֵҪ��Ϳѻ��վ�ϸ���ģʽ����ֵƥ��
#define  MANNUAL_MODE        0
#define  AUTO_MODE           1
#define  SLEEP_MODE          2

//���ڷ������־.�ⲿֻҪ����һ��:���ⲿ�ķ��ͽ����жϺ�������Ϊ1.
WIFI_EXT volatile unsigned char f_WifiTxOver;

WIFI_EXT void WiFi_Init(void);
WIFI_EXT void WiFi_UartPutChar(unsigned char senddata);
WIFI_EXT void WiFi_Deal(void);
WIFI_EXT void WiFi_KeyInWifiCfgMode(void);
WIFI_EXT void WiFi_SetWiFiSelfcheck(void);
WIFI_EXT void WiFi_AllDataUpdate(void);

