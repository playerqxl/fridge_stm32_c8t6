#include "global.h"
int main (void){//主程序
	WWDG_Configuration();//定时5s看门狗
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //设置中断分组
	KEY_Configuration();//按键初始化
	EXTI_Configuration();//外部中断初始化
	TIMX_Configuration();//定时中断初始化

	
	DelayInit();//初始化延时
	I2C_Configuration();//显示屏I2C初始化
	OLED_Init();
	
	DP_Configuration();//显示内容初始化
	DHT22_Configuration();//温度传感器初始化
	SCP_Configuration();//半导体制冷片相关初始化
	while(1){//无限循环程序
		IWDG_ReloadCounter();//喂狗
		dp0_show();
		DHT22_get();
		SCP_run();
	}
}
