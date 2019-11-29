#include "lib_config.h"
#include "Pin.h"
#include "RGB.h"
#include "main.h"
#include "McuDrv.h"
#include "ADC.h"
#include "Buzzer.h"
#include "Timer.h"
#include "Disp_Air.h"
#include "led.h"
#include "DCMotorDrv.h"
#include "Drv_Disp.h"
#include "fanDrv.h"
#include "filter.h"
#include "Mode.h"
#include "Key.h"
#include "PM25.h"
#include "Test.h"
#include "mcu_sdk\wifi.h"
#include "I2C.h"
//#include "I2cDriver.h"
#include "mywifi.h"//#include "wifiMcu.h"

typedef  unsigned char  uchar;
typedef  unsigned int   uint;
typedef  unsigned int   u32;


#define arrLen(arr)   (sizeof(arr) / sizeof(*(arr)))
