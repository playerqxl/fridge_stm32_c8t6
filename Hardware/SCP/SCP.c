#include "global.h"

unsigned short scp_time_start;//����������ʱ
unsigned char scp_dhtj;
//scpi�뵼������Ƭ�̵��������߿�����־λ,scp_time_stopΪ�뵼������Ƭ��ʱ�乤�������־λ
unsigned char scpi,scp_time_stop;
char scp_set_temper[tempers],scp_temper[tempers];

void SCP_Configuration(void)
{
	/*����A10Ϊ�뵼������Ƭ�̵���������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOAʱ��
	//���ýṹ��
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	scpi=scp_time_start=scp_time_stop=0;
}

/*���ٳɹ�����һ���¶�����������δ��ʱ;�趨�¶�С��ʵ���¶������뵼������Ƭ�̵�����
�¶ȴӸߵ���ʱ���趨�¶ȴ���ʵ���¶�-5�رհ뵼������Ƭ�̵�����
�¶ȴӵ͵���ʱ���趨�¶ȴ���ʵ���¶ȹرհ뵼������Ƭ�̵�����
��©������̬Ϊset����ͨ�̵������ӵ�Ϊreset���Ͽ��̵�����*/
void SCP_run(void)
{
	char scpt;//1Ϊ�¶ȴӸߵ��ͣ�0Ϊ�¶ȴӵ͵���
	scp_dhtj=dhtj;
	if(scp_dhtj&&!scp_time_stop)
	{
		scp_set_temper[0]=set_temper[0];
		scp_temper[0]=temper[0];
		if(scp_set_temper[0]<scp_temper[0])
		{
			scpt=1;
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
//			SCP_on();
			scpi=1;
		}
		else 
		if((scp_set_temper[0]-5)>scp_temper[0])
		{
			scpt=0;
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//			SCP_off();
			scpi=0;
		}
		else
//�¶ȴ����м�λ�ÿ�scpt
		if(scpt){GPIO_SetBits(GPIOA, GPIO_Pin_4);scpi=1;}else{GPIO_ResetBits(GPIOA, GPIO_Pin_4);scpi=0;}	
	}
}

void SCP_on(void)//��������ʱ�ӵأ��̵�����Ȧ�պ�
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���ýṹ��
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SCP_off(void)//����������ж����ܷ����ֹ���̵�����Ȧ�Ͽ�
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//���ýṹ��
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

///*���ùر�������������240���ӵİ뵼������Ƭ*/
//void SCP_time_stop(void)
//{
//	unsigned char scp_hour=0;
//	if(scpi)
//	{
//		scp_time_start++;
//		if(scp_time_start>35999)
//		{
//			scp_time_start=0;
//			scp_hour++;
//			if(scp_hour>3)
//			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//			scpi=0;
//			scp_time_stop=1;
//			scp_hour=0;
//		}
//	}
//	else
//	{
//		scp_time_start=scp_hour=0;
//	}
//}
