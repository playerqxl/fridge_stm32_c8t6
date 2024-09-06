#include "global.h"

//unsigned char tim3;//计时器tim3计时结束标志
float tim3;
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数

void TIMX_Configuration(void)
{
	TIM2_Int_Init();
	TIM3_Int_Init();
	TIM4_Int_Init();
}

/*tim2为按键计时消抖，按下按键等待7ms后判断按键状态*/
void TIM2_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //RCC_TIM2时钟使能
	
		/*定时器TIM2初始化，简单进行定时器初始化,设置 预装载值 和 分频系数,10Khz的计数频率，计数
	到499为50ms，给按钮消抖*/
	TIM_TimeBaseStructure.TIM_Period = 69; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断，这行必须在时钟初始化后
	
		//tim2中断优先级NVIC设置，用于按钮中断延时
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM2, DISABLE);  //TIM2先不计时
}

void TIM2_IRQHandler(void)   //TIM2中断，按钮7ms消抖时钟
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//检查TIM2更新中断发生与否
		{
			if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==Bit_RESET)//确定键按下
					{enter=1;} else{enter=0;}
				EXTI_ClearITPendingBit(EXTI_Line12); //清除 EXTI12 线路挂起位
			}		
			if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==Bit_RESET)//返回键按下
				{back=1;} else{back=0;}
				EXTI_ClearITPendingBit(EXTI_Line13); //清除 EXTI13 线路挂起位
			}		
			if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==Bit_RESET)//加键按下
				{plus=1;} else{plus=0;}
				EXTI_ClearITPendingBit(EXTI_Line14); //清除 EXTI14 线路挂起位
			}
			if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)==Bit_RESET)//减键按下
					{minus=1;} else{minus=0;}
					
				EXTI_ClearITPendingBit(EXTI_Line15); //清除 EXTI15 线路挂起位
			}			
		}		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
	TIM_Cmd(TIM2,DISABLE);//消抖结束，定时器关闭
	TIM_SetCounter(TIM2,0);//重置计数器，由于向上计数，所以为0
	dp_open();
}

	/*定时器TIM3初始化，简单进行定时器初始化,设置 预装载值 和 分频系数,10Khz的计数频率，计数
	到999为100ms，给系统周期为100ms的基准*/
void TIM3_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断，这行必须在时钟初始化后
	
	//tim3中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //响应优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3，开始计时
	
	tim3=0;
}

void TIM3_IRQHandler(void)   //TIM3中断，系统时钟周期100ms
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//检查TIM3更新中断发生与否
		{
//			tim3=~tim3;
			DHT22_time();
//			SCP_time_stop();			
			
			dp_setj_counter();
			if(plus){plusi++;}else{plusi=0;}//加键持续按下计数
			if(minus){minusi++;}else{minusi=0;}//减键持续按下计数
			
			DP_close();
		}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
}

/*tim4为DHT22与主机通信计时器*/
void TIM4_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4时钟使能
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断，这行必须在时钟初始化后
	
	//tim4中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM4,DISABLE);  //不使能TIM4
}

void TIM4_IRQHandler(void)   //TIM4中断，该中断触发表明温度传感器通信超时，传感器通信超时标志位置位
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//检查TIM3更新中断发生与否
	{
		DHT22_tst_stop();
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志

}

void TIM4_TInt_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_Cmd(TIM4, DISABLE);  //停止计时
	TIM_SetCounter(TIM4,0);//重置计数,与TIM_SetCounter(TIM4,0)等价
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断，这行必须在时钟初始化后

	TIM_Cmd(TIM4, ENABLE);  //使能TIM4，开始计时	
}
