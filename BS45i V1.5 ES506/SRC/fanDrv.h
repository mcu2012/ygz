#ifndef _FANDRV_H
#define _FANDRV_H

#ifdef	_FAN_DEFINE
#define	FAN_DRV_EXT 
#else
#define	FAN_DRV_EXT extern
#endif

//could be adjust accord machine.
/* smaller,more sensitive.*/
#define Kp   2 //10  //负载轻时系数小
#define Kd   1 //8 
#define DCSPINIT       750  //根据不同负载调整
#define DCSPINITMAX    3952 
#define _FanDrvMin     750

/*if adjust "KD_VAL_NOW" and "KD_VLA_LAST" is not enough,try adjust this.*/
#define _FAN_ADJUST_TIME  500 
FAN_DRV_EXT unsigned int fanAdjustInterval;
//FAN_DRV_EXT volatile unsigned int fanStartWaitTime;


FAN_DRV_EXT unsigned int TjFanSpeed ( unsigned int obj, unsigned int now );
FAN_DRV_EXT void Fan_OFFctrl ( void );
FAN_DRV_EXT void fanMsDly(void);

#endif

