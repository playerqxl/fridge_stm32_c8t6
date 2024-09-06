#include "global.h"

volatile unsigned char enter,back,plus,minus,plusi,minusi;//六个按键标志位，确定、返回、加、减，加和减计数位

void KEY_Configuration(void)
{	
/*设置按钮*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
	//配置结构体
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//四个按钮
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	enter=back=plus=minus=plusi=minusi=0;
}
