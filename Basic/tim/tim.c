#include "global.h"

//unsigned char tim3;//��ʱ��tim3��ʱ������־
float tim3;
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��

void TIMX_Configuration(void)
{
	TIM2_Int_Init();
	TIM3_Int_Init();
	TIM4_Int_Init();
}

/*tim2Ϊ������ʱ���������°����ȴ�7ms���жϰ���״̬*/
void TIM2_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //RCC_TIM2ʱ��ʹ��
	
		/*��ʱ��TIM2��ʼ�����򵥽��ж�ʱ����ʼ��,���� Ԥװ��ֵ �� ��Ƶϵ��,10Khz�ļ���Ƶ�ʣ�����
	��499Ϊ50ms������ť����*/
	TIM_TimeBaseStructure.TIM_Period = 69; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������жϣ����б�����ʱ�ӳ�ʼ����
	
		//tim2�ж����ȼ�NVIC���ã����ڰ�ť�ж���ʱ
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��Ӧ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM2, DISABLE);  //TIM2�Ȳ���ʱ
}

void TIM2_IRQHandler(void)   //TIM2�жϣ���ť7ms����ʱ��
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//���TIM2�����жϷ������
		{
			if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==Bit_RESET)//ȷ��������
					{enter=1;} else{enter=0;}
				EXTI_ClearITPendingBit(EXTI_Line12); //��� EXTI12 ��·����λ
			}		
			if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==Bit_RESET)//���ؼ�����
				{back=1;} else{back=0;}
				EXTI_ClearITPendingBit(EXTI_Line13); //��� EXTI13 ��·����λ
			}		
			if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==Bit_RESET)//�Ӽ�����
				{plus=1;} else{plus=0;}
				EXTI_ClearITPendingBit(EXTI_Line14); //��� EXTI14 ��·����λ
			}
			if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
			{
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)==Bit_RESET)//��������
					{minus=1;} else{minus=0;}
					
				EXTI_ClearITPendingBit(EXTI_Line15); //��� EXTI15 ��·����λ
			}			
		}		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
	TIM_Cmd(TIM2,DISABLE);//������������ʱ���ر�
	TIM_SetCounter(TIM2,0);//���ü��������������ϼ���������Ϊ0
	dp_open();
}

	/*��ʱ��TIM3��ʼ�����򵥽��ж�ʱ����ʼ��,���� Ԥװ��ֵ �� ��Ƶϵ��,10Khz�ļ���Ƶ�ʣ�����
	��999Ϊ100ms����ϵͳ����Ϊ100ms�Ļ�׼*/
void TIM3_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = 999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������жϣ����б�����ʱ�ӳ�ʼ����
	
	//tim3�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //��Ӧ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3����ʼ��ʱ
	
	tim3=0;
}

void TIM3_IRQHandler(void)   //TIM3�жϣ�ϵͳʱ������100ms
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���TIM3�����жϷ������
		{
//			tim3=~tim3;
			DHT22_time();
//			SCP_time_stop();			
			
			dp_setj_counter();
			if(plus){plusi++;}else{plusi=0;}//�Ӽ��������¼���
			if(minus){minusi++;}else{minusi=0;}//�����������¼���
			
			DP_close();
		}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
}

/*tim4ΪDHT22������ͨ�ż�ʱ��*/
void TIM4_Int_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4ʱ��ʹ��
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������жϣ����б�����ʱ�ӳ�ʼ����
	
	//tim4�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��Ӧ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM4,DISABLE);  //��ʹ��TIM4
}

void TIM4_IRQHandler(void)   //TIM4�жϣ����жϴ��������¶ȴ�����ͨ�ų�ʱ��������ͨ�ų�ʱ��־λ��λ
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���TIM3�����жϷ������
	{
		DHT22_tst_stop();
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־

}

void TIM4_TInt_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_Cmd(TIM4, DISABLE);  //ֹͣ��ʱ
	TIM_SetCounter(TIM4,0);//���ü���,��TIM_SetCounter(TIM4,0)�ȼ�
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������жϣ����б�����ʱ�ӳ�ʼ����

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4����ʼ��ʱ	
}
