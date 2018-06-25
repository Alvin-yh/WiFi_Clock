#ifndef __SYS_H
#define __SYS_H	   

#include "stm32f10x.h"
void ysu(unsigned int us);
void ysm(unsigned int ms);
void RCC_DeInit(void);	
void Stm32_Clock_Init(void);
void Systick_init(void);
void SysTick_Handler(void);

void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif
