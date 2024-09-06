#ifndef __DISPLAY_H
#define __DISPLAY_H

void curr_temper(void);//显示当前温湿度
void set_temper0(void);//显示设定温度1级界面
void set_temper1(void);//显示设定温度2级界面
void err_info(void);//显示错误信息
void DP_Configuration(void);//初始化显示界面序列
void dp0_show(void);
void settemper0(void);//显示设定温度0
void settemper1(void);//显示设定温度1
void dp_key(void);//按键按下显示对应
void dp_setj_counter(void);
void sys_info(void);
void dp_open(void);
void DP_close(void);
#endif
