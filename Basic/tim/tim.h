#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

void TIMX_Configuration(void);
void TIM2_Int_Init(void);
void TIM3_Int_Init(void);
void TIM4_Int_Init(void);
void TIM4_TInt_Init(uint16_t arr,uint16_t psc);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
#endif
