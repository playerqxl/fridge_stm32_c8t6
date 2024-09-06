#include "global.h"

/*���Ź�����������ʱ5s����Ҫ�Ƚ�����Ź���IWDG_PR��Ԥ��Ƶ���Ĵ�������IWDG_RLR�����ؼĴ�����д������
Ԥ��Ƶ256����LSI 40KHz��ʱ��Դ����Ƶ��һ������Ϊ40K/256=1/156.25s�����Զ�ʱ5s��Ҫ5*156.25=781.25������ȡ782��
���ڿ��Ź����¼������������ʱ��Ҫ����һ�μ���*/
void WWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//���Ĵ���д����
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(782);
	IWDG_ReloadCounter();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);//�ؼĴ���д����
	IWDG_Enable();
}
