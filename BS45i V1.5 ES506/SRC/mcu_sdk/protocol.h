/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: protocol.h
**描        述: 下发/上报数据处理函数
**使 用 说 明 :

                  *******非常重要，一定要看哦！！！********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
** 3、当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
**
**--------------当前版本修订---------------------------------------------------
** 版  本: v2.5.2
** 日　期: 2019年7月5日
** 描　述: 1:增加WiFi功能性测试（连接指定路由）
           2:简化流服务过程，用户只需要调用流服务传输接口即可使用流服务
           3:增加同步上报指令
           4:增加获取当前wifi联网状态指令
           5:增加获取格林时间指令
           6:优化ota升级流程，启动ota升级时用户可以选择传输包大小
           7:增加获取模块mac地址指令

** 版  本: v2.5.1
** 日　期: 2018年10月27日
** 描　述: 1:默认关闭流服务功能
           2:增加03协议wifi状态宏定义
		   3:更新与修改部分函数注释
		   
** 版  本: v2.5.0
** 日　期: 2018年4月18日
** 描　述: 1:协议版本改为0x03
           2:增加WIFI模组心跳关闭功能
           3:增加天气功能

** 版  本: v2.3.8
** 日　期: 2018年1月17日
** 描　述: 1:变量添加volatile防止编译器优化
           2:添加#error提示

** 版  本: v2.3.7
** 日　期: 2017年4月18日
** 描　述: 1:优化串口队列接收处理
		   
** 版  本: v2.3.6
** 日　期: 2016年7月21日
** 描　述: 1:修复获取本地时间错误
           2:添加hex_to_bcd转换函数
		   
** 版  本: v2.3.5
** 日　期: 2016年6月3日
** 描　述: 1:修改返回协议版本为0x01
           2:固件升级数据偏移量修改为4字节

** 版  本: v2.3.4
** 日　期: 2016年5月26日
** 描　述: 1:优化串口解析函数
           2:优化编译器兼容性,取消enum类型定义

** 版  本: v2.3.3
** 日　期: 2016年5月24日
** 描　述: 1:修改mcu获取本地时间函数
           2:添加wifi功能测试

** 版  本: v2.3.2
** 日　期: 2016年4月23日
** 描　述: 1:优化串口数据解析
           2:优化MCU固件升级流程
           3:优化上报流程

** 版  本: v2.3.1
** 日　期: 2016年4月15日
** 描　述: 1:优化串口数据解析

** 版  本: v2.3
** 日　期: 2016年4月14日
** 描　述: 1:支持MCU固件在线升级

** 版  本: v2.2
** 日　期: 2016年4月11日
** 描　述: 1:修改串口数据接收方式

** 版  本: v2.1
** 日　期: 2016年4月8日
** 描　述: 1:加入某些编译器不支持函数指针兼容选项

** 版  本: v2.0
** 日　期: 2016年3月29日
** 描　述: 1:优化代码结构
           2:节省RAM空间
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_



/******************************************************************************
                            用户相关信息配置
******************************************************************************/
/******************************************************************************
                            1:修改产品信息                
******************************************************************************/
#define PRODUCT_KEY "hyyjuehxra2ahlxg"    //开发平台创建产品后生成的16位字符产品唯一标识

#define MCU_VER "1.0.0"                                 //用户的软件版本,用于MCU固件升级,MCU升级版本需修改

//配网方式选择,默认为CONFIG_MODE_DEFAULT,只能三选一
//#define CONFIG_MODE     CONFIG_MODE_DEFAULT             //默认配网方式
#define CONFIG_MODE     CONFIG_MODE_LOWPOWER            //低功耗配网方式
//#define CONFIG_MODE     CONFIG_MODE_SPECIAL             //特殊配网方式

/******************************************************************************
                          2:MCU是否需要支固件升级                  
如需要支持MCU固件升级,请开启该宏
MCU可调用mcu_api.c文件内的mcu_firm_update_query()函数获取当前MCU固件更新情况
                        ********WARNING!!!**********
当前接收缓冲区为关闭固件更新功能的大小,固件升级包可选择，默认256字节大小
如需要开启该功能,串口接收缓冲区会变大

******************************************************************************/
//#define         SUPPORT_MCU_FIRM_UPDATE                 //开启MCU固件升级功能(默认关闭)
//固件包大小选择
#ifdef SUPPORT_MCU_FIRM_UPDATE
#define PACKAGE_SIZE                   0        //包大小为256字节
//#define PACKAGE_SIZE                 1        //包大小为512字节
//#define PACKAGE_SIZE                 2        //包大小为1024字节
#endif
/******************************************************************************
                         3:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_QUEUE_LMT             16              //数据接收队列大小,如MCU的RAM不够,可缩小
#define WIFI_UART_RECV_BUF_LMT          24              //根据用户DP数据大小量定,必须大于24
#else
#define WIFI_UART_QUEUE_LMT             128             //数据接收队列大小,如MCU的RAM不够,可缩小

//请在此处选择合适的mcu升级缓存大小（根据上面固件包选择大小来选择开启多大的mcu升级缓存）
#define WIFI_UART_RECV_BUF_LMT          300             //固件升级缓冲区,需大缓存,如单包大小选择256，则缓存必须大于260
//#define WIFI_UART_RECV_BUF_LMT          600             //固件升级缓冲区,需大缓存,如单包大小选择512，则缓存必须大于520
//#define WIFI_UART_RECV_BUF_LMT          1200            //固件升级缓冲区,需大缓存,如单包大小选择1024，则缓存必须大于1030


#endif

#define WIFIR_UART_SEND_BUF_LMT         48              //根据用户DP数据大小量定,必须大于48
/******************************************************************************
                        4:定义模块工作方式
模块自处理:
          wifi指示灯和wifi复位按钮接在wifi模块上(开启WIFI_CONTROL_SELF_MODE宏)
          并正确定义WF_STATE_KEY和WF_RESET_KEY
MCU自处理:
          wifi指示灯和wifi复位按钮接在MCU上(关闭WIFI_CONTROL_SELF_MODE宏)
          MCU在需要处理复位wifi的地方调用mcu_api.c文件内的mcu_reset_wifi()函数,并可调用mcu_get_reset_wifi_flag()函数返回复位wifi结果
          或调用设置wifi模式mcu_api.c文件内的mcu_set_wifi_mode(WIFI_CONFIG_E mode)函数,并可调用mcu_get_wifi_work_state()函数返回设置wifi结果
******************************************************************************/
//#define         WIFI_CONTROL_SELF_MODE                       //wifi自处理按键及LED指示灯;如为MCU外界按键/LED指示灯请关闭该宏
#ifdef          WIFI_CONTROL_SELF_MODE                      //模块自处理
  #define     WF_STATE_KEY            14                    //wifi模块状态指示按键，请根据实际GPIO管脚设置
  #define     WF_RESERT_KEY           0                     //wifi模块重置按键，请根据实际GPIO管脚设置
#endif

/******************************************************************************
                      5:MCU是否需要支持校时功能                     
如需要请开启该宏,并在Protocol.c文件内mcu_write_rtctime实现代码
mcu_write_rtctime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_system_time()函数发起校时功能
******************************************************************************/
//#define         SUPPORT_MCU_RTC_CHECK                //开启校时功能

/******************************************************************************
                      6:MCU是否需要支持wifi功能测试                     
如需要请开启该宏,并且mcu在需要wifi功能测试处调用mcu_api.c文件内mcu_start_wifitest
并在protocol.c文件wifi_test_result函数内查看测试结果,
wifi_test_result内部有#err提示,完成函数后请删除该#err
******************************************************************************/
#define         WIFI_TEST_ENABLE                //开启WIFI产测功能（扫描指定路由）

/******************************************************************************
                      7:是否开启天气功能                   
如需要请开启该宏,并在protocol.c文件内weather_open_return_handle与weather_data_u\
ser_handle两个用户处理函数中实现显示等代码
此两函数内#err提示,完成函数后请删除该#err
******************************************************************************/
//#define         WEATHER_ENABLE                 //打开天气功能
#ifdef          WEATHER_ENABLE
#define         WEATHER_CHOOSE_CNT             4
#endif

/******************************************************************************
                      8:是否开启WIFI模组心跳关闭功能                   
如需要请开启该宏,调用mcu_api.c文件内wifi_heart_stop函数即可停止心跳
******************************************************************************/
//#define         WIFI_HEARTSTOP_ENABLE           //开启心跳停止功能

/******************************************************************************
                      9:是否支持流服务功能                 
STREM_PACK_LEN为流服务传输一包的大小，目前模块串口最大可以缓存的数据部分可以达到 1024 字节，一包地图 
数据包数据部分不能超过 1024 字节，每包地图数据内容建议 512 字节每包。
******************************************************************************/
//#define         WIFI_STREAM_ENABLE              //支持流服务相关功能
#ifdef WIFI_STREAM_ENABLE
#define         STREM_PACK_LEN                 256
#endif

/******************************************************************************
                      10:MCU是否需要支持wifi功能测试(连接指定路由)                     
如需要请开启该宏,并且mcu在需要wifi功能测试处调用mcu_api.c文件内mcu_start_connect_wifitest
并在protocol.c文件wifi_connect_test_result函数内查看测试结果,
wifi_connect_test_result内部有#err提示,完成函数后请删除该#err
******************************************************************************/
//#define         WIFI_CONNECT_TEST_ENABLE                //开启WIFI产测功能（连接指定路由）

/******************************************************************************
                      11:MCU是否需要开启获取当前WIFI联网状态功能                 
******************************************************************************/
//#define         GET_WIFI_STATUS_ENABLE                  //开启获取当前WIFI联网状态功能

/******************************************************************************
                      12:MCU是否需要开启获取模块mac地址功能                 
******************************************************************************/
//#define         GET_MODULE_MAC_ENABLE                   //开启获取模块mac地址功能

/******************************************************************************
                      13:MCU是否需要支持获取格林时间功能                 
如需要请开启该宏,并在Protocol.c文件内mcu_get_greentime实现代码
mcu_get_greentime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_green_time()函数发起校时功能
******************************************************************************/
//#define         SUPPORT_GREEN_TIME                //开启格林时间功能

/******************************************************************************
                      14:MCU是否需要开启同步状态上报功能
1) 此命令为同步指令，MCU 数据状态上报后，需要等待模块返回结果；
2) 每次发送模组都会有响应，WIFI 模组未响应前不可多次上报；
3) 网络不好，数据难以及时上报时，模块会在 5 后返回失败，MCU 需要等待大于 5 秒。
******************************************************************************/
//#define         MCU_DP_UPLOAD_SYN                   //开启同步状态上报功能






/******************************************************************************
                        1:dp数据点序列号重新定义
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
//开关(可下发可上报)
//备注:
#define DPID_SWITCH 1
//PM2.5(只上报)
//备注:
#define DPID_PM25 2
//模式(可下发可上报)
//备注:
#define DPID_MODE 3
//风速(可下发可上报)
//备注:
#define DPID_FAN_SPEED_ENUM 4
//滤芯使用率(只上报)
//备注:
#define DPID_FILTER 5
//负离子(可下发可上报)
//备注:
#define DPID_ANION 6
//童锁开关(可下发可上报)
//备注:
#define DPID_CHILD_LOCK 7
//空气质量(只上报)
//备注:
#define DPID_AIR_QUALITY 21
//故障告警(只上报)
//备注:
#define DPID_FAULT 22


/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统所有dp点信息上传
输入参数 : 无
返回参数 : 无
使用说明 : MCU必须实现该函数内数据上报功能
*****************************************************************************/
void all_data_update(void);

#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[]);
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_test_result
功能描述 : wifi功能测试反馈
输入参数 : result:wifi功能测试结果;0:失败/1:成功
           rssi:测试成功表示wifi信号强度/测试失败表示错误类型
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi);
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : 获取 WIFI 状态结果
输入参数 : result:指示 WIFI 工作状态
           0x00:wifi状态 1 smartconfig 配置状态
           0x01:wifi状态 2 AP 配置状态
           0x02:wifi状态 3 WIFI 已配置但未连上路由器
           0x03:wifi状态 4 WIFI 已配置且连上路由器
           0x04:wifi状态 5 已连上路由器且连接到云端
           0x05:wifi状态 6 WIFI 设备处于低功耗模式
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void get_wifi_status(unsigned char result);
#endif

#ifdef MCU_DP_UPLOAD_SYN
/*****************************************************************************
函数名称 : get_upload_syn_result
功能描述 : 状态同步上报结果
输入参数 : result:0x00失败  0x01成功
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void get_upload_syn_result(unsigned char result);
#endif

#ifdef GET_MODULE_MAC_ENABLE
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : mac：mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
  mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_get_mac(unsigned char mac[]);
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_connect_test_result
功能描述 : 路由信息接收结果通知
输入参数 : result:模块是否成功接收到正确的路由信息;0:失败/1:成功
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void wifi_connect_test_result(unsigned char result);
#endif

#ifdef SUPPORT_GREEN_TIME
/*****************************************************************************
函数名称 : mcu_get_greentime
功能描述 : 获取到的格林时间
输入参数 : 无
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void mcu_get_greentime(unsigned char time[]);
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : upgrade_package_choose
功能描述 : 升级包大小选择
输入参数 : package_sz:升级包大小
           0x00：默认 256byte
           0x01：512byte 
           0x02：1024byte

返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
void upgrade_package_choose(unsigned char package_sz);

/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲区
           position:当前数据包在于固件位置
           length:当前固件包长度(固件包长度为0时,表示固件包发送完成)
返回参数 : 无
使用说明 : MCU需要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length);
#endif

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
value:dp数据缓冲区地址
length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取所有dp命令总和
输入参数 : 无
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修改
*****************************************************************************/
unsigned char get_download_cmd_total(void);

#ifdef WEATHER_ENABLE
extern const char weather_choose[WEATHER_CHOOSE_CNT][10];

void weather_open_return_handle(unsigned char res, unsigned char err);
void weather_data_user_handle(char *name, unsigned char type, char *data);
#endif

#endif

