
#define IO_DC_FAN_15V      GPIO_Pin_A5
#define MOTO_15V_ON        GPIO_SetBit(IO_DC_FAN_15V) 
#define MOTO_15V_OFF       GPIO_ResetBit(IO_DC_FAN_15V) 
#define IO_FAN_FG          GPIO_Pin_A2

#define IO_FAN_DRV          GPIO_Pin_A3
#define IO_FANDRV_ON       GPIO_SetBit(IO_FAN_DRV) 
#define IO_FANDRV_OFF      GPIO_ResetBit(IO_FAN_DRV) 

#define IO_RGB_LIGHT       GPIO_Pin_A10
#define RGB_ON             GPIO_SetBit(IO_RGB_LIGHT) 
#define RGB_OFF            GPIO_ResetBit(IO_RGB_LIGHT) 

#define IO_SENSOR_POWER    GPIO_Pin_A28
#define SENSOR_POWER_ON    GPIO_SetBit(IO_SENSOR_POWER) 
#define SENSOR_POWER_OFF   GPIO_ResetBit(IO_SENSOR_POWER) 
#define IO_DUST_TXD        GPIO_Pin_B1
#define IO_DUST_RXD        GPIO_Pin_B0


#define IO_IONIZER         GPIO_Pin_A4
#define IONIZER_ON         GPIO_SetBit(IO_IONIZER) 
#define IONIZER_OFF        GPIO_ResetBit(IO_IONIZER) 

#define IO_TM1623_STB      GPIO_Pin_A11
#define IO_TM1623_STB_ON   GPIO_SetBit(IO_TM1623_STB) 
#define IO_TM1623_STB_OFF  GPIO_ResetBit(IO_TM1623_STB) 

#define IO_TM1623_CLK      GPIO_Pin_A12
#define IO_TM1623_CLK_ON   GPIO_SetBit(IO_TM1623_CLK) 
#define IO_TM1623_CLK_OFF  GPIO_ResetBit(IO_TM1623_CLK) 

#define IO_TM1623_DATA     GPIO_Pin_A13
#define IO_TM1623_DATA_ON  GPIO_SetBit(IO_TM1623_DATA) 
#define IO_TM1623_DATA_OFF GPIO_ResetBit(IO_TM1623_DATA) 


#define IO_WIFI_TXD        GPIO_Pin_A23
#define IO_WIFI_RXD        GPIO_Pin_A22


#define IO_LIGHT_ADC       GPIO_Pin_A8
//#define IO_KEY             GPIO_Pin_A9
#define IO_KEY_ADC          GPIO_Pin_A9


//#define IO_BUZF            GPIO_Pin_A25
#define IO_BUZV            GPIO_Pin_A25
#define IO_BUZ_ON          GPIO_SetBit(IO_BUZV) 
#define IO_BUZ_OFF         GPIO_ResetBit(IO_BUZV) 
#define IO_BZ_PWM          GPIO_Pin_A24


#define IO_TEST            GPIO_Pin_A7 




