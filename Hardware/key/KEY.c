#include "global.h"

volatile unsigned char enter,back,plus,minus,plusi,minusi;//����������־λ��ȷ�������ء��ӡ������Ӻͼ�����λ

void KEY_Configuration(void)
{	
/*���ð�ť*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
	//���ýṹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//�ĸ���ť
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	enter=back=plus=minus=plusi=minusi=0;
}
