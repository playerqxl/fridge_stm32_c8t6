#include "global.h"

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//外部中断，需要使能 AFIO 时钟
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12|GPIO_PinSource13|GPIO_PinSource14|GPIO_PinSource15);//四个按钮所在管脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //子优先级 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
//根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器

 //GPIOA 中断线以及中断初始化配置，PA0为DHT22_0信号输入线，PA1为DHT22_1信号输入线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//不使能外部中断线
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
//根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器

}

/*为了防止反复挂起，将清除挂起函数放到定时器上*/
void EXTI15_10_IRQHandler(void)
{		
	TIM_Cmd(TIM2, ENABLE);  //按键消抖，使能TIM2，开始计时	
}

void EXTI0_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
{
	DHT22_ts();
}
EXTI_ClearITPendingBit(EXTI_Line0); //清除 EXTI0 线路挂起位
}

void EXTIX_ENABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 //GPIOX 中断线以及中断初始化配置，PA0为DHT22_0信号输入线，PA1为DHT22_1信号输入线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOX,GPIO_PinSourceX);
	EXTI_InitStructure.EXTI_Line=EXTI_LineX;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能外部中断线
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
}

void EXTIX_DISABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 //GPIOX 中断线以及中断初始化配置，PA0为DHT22_0信号输入线，PA1为DHT22_1信号输入线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOX,GPIO_PinSourceX);
	EXTI_InitStructure.EXTI_Line=EXTI_LineX;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上下降沿均触发
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//不使能外部中断线
	EXTI_Init(&EXTI_InitStructure);
//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
}
