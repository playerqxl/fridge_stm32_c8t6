#include "global.h"

/*看门狗防死机，定时5s，需要先解除看门狗的IWDG_PR（预分频器寄存器）和IWDG_RLR（重载寄存器）写保护，
预分频256，分LSI 40KHz的时钟源，分频后一个计数为40K/256=1/156.25s，所以定时5s需要5*156.25=781.25计数，取782。
由于看门狗向下计数，因此配置时还要重载一次计数*/
void WWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//开寄存器写保护
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(782);
	IWDG_ReloadCounter();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);//关寄存器写保护
	IWDG_Enable();
}
