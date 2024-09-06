#include "global.h"

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//�ⲿ�жϣ���Ҫʹ�� AFIO ʱ��
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12|GPIO_PinSource13|GPIO_PinSource14|GPIO_PinSource15);//�ĸ���ť���ڹܽ�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //�����ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
//���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���

 //GPIOA �ж����Լ��жϳ�ʼ�����ã�PA0ΪDHT22_0�ź������ߣ�PA1ΪDHT22_1�ź�������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//��ʹ���ⲿ�ж���
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
//���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���

}

/*Ϊ�˷�ֹ�������𣬽�����������ŵ���ʱ����*/
void EXTI15_10_IRQHandler(void)
{		
	TIM_Cmd(TIM2, ENABLE);  //����������ʹ��TIM2����ʼ��ʱ	
}

void EXTI0_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
{
	DHT22_ts();
}
EXTI_ClearITPendingBit(EXTI_Line0); //��� EXTI0 ��·����λ
}

void EXTIX_ENABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 //GPIOX �ж����Լ��жϳ�ʼ�����ã�PA0ΪDHT22_0�ź������ߣ�PA1ΪDHT22_1�ź�������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOX,GPIO_PinSourceX);
	EXTI_InitStructure.EXTI_Line=EXTI_LineX;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ⲿ�ж���
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
}

void EXTIX_DISABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 //GPIOX �ж����Լ��жϳ�ʼ�����ã�PA0ΪDHT22_0�ź������ߣ�PA1ΪDHT22_1�ź�������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOX,GPIO_PinSourceX);
	EXTI_InitStructure.EXTI_Line=EXTI_LineX;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���½��ؾ�����
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;//��ʹ���ⲿ�ж���
	EXTI_Init(&EXTI_InitStructure);
//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
}
