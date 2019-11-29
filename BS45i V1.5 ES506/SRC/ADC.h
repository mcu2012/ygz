#ifdef  ADC_H
#define ADC_EXT
#else
#define ADC_EXT extern
#endif

#define IO_LIGHT  GPIO_Pin_B9
#define IO_KEY    GPIO_Pin_A1

#define AD_0_L 15   //60  进入睡眠
#define AD_1_L 60  //退出睡眠
#define AD_1_H 100   //200  进入一档显示
#define AD_2   200   //进入二挡显示

//ADC_EXT unsigned int LightAD;
//ADC_EXT unsigned char LightClass; 
//通过按键进入的只能按键退出
//通过光敏进入的:1.光敏退出;2.按键相当于唤醒。
ADC_EXT unsigned char InSleepByKey;//1=按键进入的睡眠；0=光敏进入的睡眠.


ADC_EXT void ADCInitALL(void);//@190105
ADC_EXT void ADCInitKey(void);
ADC_EXT void adSample(void);
ADC_EXT uint16_t Get_AD_Value(void);

