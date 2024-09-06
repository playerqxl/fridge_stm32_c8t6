#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h" 
#include "iwdg.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "DHT22.h"
#include "KEY.h"
#include "display.h"
#include "exti.h"
#include "tim.h"
#include "SCP.h"

#define tempers 2//温度传感器数量2
#define currtemper0 0//当前温湿度0
#define currtemper1 1//当前温湿度1
#define sysinfo 2//系统信息

extern volatile unsigned char dp0j;//dp0i为一级页面标志位,dp0j为二级页面标志位
extern volatile signed char dp0i,seti;//dp0i为一级页面标志位,进入显示设定温度2级界面后，文字闪烁标志位
extern volatile unsigned char setj;//进入显示设定温度2级界面后，文字熄灭为0<=setj<3，亮起为大于3<=setj<6，一个计数为0.5s
extern volatile unsigned char dpi;//决定是否刷新显示，显示静态页面不需要一直刷新
extern volatile unsigned char dptj;
//extern unsigned char tim3;//计时器tim3计时结束标志
extern float tim3;
extern volatile unsigned char enter,back,plus,minus,plusi,minusi;//六个按键标志位，确定、返回、加、减，加和减计数位
extern volatile float temper[tempers],humid[tempers];//存放温度数据和湿度数据
extern float settemper[tempers];//存放设置温度数据
extern float set_temper[tempers];//应用设置温度数据，用于与实际温度比较
/*dht22_tst为传输信号各个阶段标志位，0为不进行传输信号，1为传输开始前3s延时，2为主机开始发送信号，3为主机拉高并延时等待，
4为DHT22响应拉低输出，5为DHT拉高延时准备输出，6为DHT22传输1bit数据时拉低，
7为DHT22传输1bit数据时拉高,8为信号传输结束传感器释放总线，9为校验阶段。
dht22_tst_err为传输信号各个阶段错误标志位，数字为对应dht22_tst的各个阶段超时错误。*/
volatile extern unsigned char dht22_tst,dht22_tst_err;
extern unsigned char dhtj;//至少更新成功一次温度
extern unsigned char scp_time_stop;//半导体制冷片长时间工作错误标志位
extern volatile unsigned short temph_temper[tempers],templ_temper[tempers],temph_humid[tempers],templ_humid[tempers],checksum[tempers];
extern volatile int dcnt,d0cnt;

