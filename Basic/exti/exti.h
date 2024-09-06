#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f10x.h" 

void EXTI_Configuration(void);
void EXTI15_10_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTIX_ENABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX);
void EXTIX_DISABLE(uint8_t GPIO_PortSourceGPIOX,uint8_t GPIO_PinSourceX,uint32_t EXTI_LineX);
#endif
