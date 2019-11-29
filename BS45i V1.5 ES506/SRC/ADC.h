#ifdef  ADC_H
#define ADC_EXT
#else
#define ADC_EXT extern
#endif

#define IO_LIGHT  GPIO_Pin_B9
#define IO_KEY    GPIO_Pin_A1

#define AD_0_L 15   //60  ����˯��
#define AD_1_L 60  //�˳�˯��
#define AD_1_H 100   //200  ����һ����ʾ
#define AD_2   200   //���������ʾ

//ADC_EXT unsigned int LightAD;
//ADC_EXT unsigned char LightClass; 
//ͨ�����������ֻ�ܰ����˳�
//ͨ�����������:1.�����˳�;2.�����൱�ڻ��ѡ�
ADC_EXT unsigned char InSleepByKey;//1=���������˯�ߣ�0=���������˯��.


ADC_EXT void ADCInitALL(void);//@190105
ADC_EXT void ADCInitKey(void);
ADC_EXT void adSample(void);
ADC_EXT uint16_t Get_AD_Value(void);

