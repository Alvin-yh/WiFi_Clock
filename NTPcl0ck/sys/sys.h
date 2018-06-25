#ifndef __SYS_H
#define __SYS_H	   

#include "stm32f10x.h"
void ysu(unsigned int us);
void ysm(unsigned int ms);
void RCC_DeInit(void);	
void Stm32_Clock_Init(void);
void Systick_init(void);
void SysTick_Handler(void);

void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif
