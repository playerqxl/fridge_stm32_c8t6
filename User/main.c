#include "global.h"
int main (void){//������
	WWDG_Configuration();//��ʱ5s���Ź�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //�����жϷ���
	KEY_Configuration();//������ʼ��
	EXTI_Configuration();//�ⲿ�жϳ�ʼ��
	TIMX_Configuration();//��ʱ�жϳ�ʼ��

	
	DelayInit();//��ʼ����ʱ
	I2C_Configuration();//��ʾ��I2C��ʼ��
	OLED_Init();
	
	DP_Configuration();//��ʾ���ݳ�ʼ��
	DHT22_Configuration();//�¶ȴ�������ʼ��
	SCP_Configuration();//�뵼������Ƭ��س�ʼ��
	while(1){//����ѭ������
		IWDG_ReloadCounter();//ι��
		dp0_show();
		DHT22_get();
		SCP_run();
	}
}
