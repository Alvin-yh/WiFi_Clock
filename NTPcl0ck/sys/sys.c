#include "stm32f10x.h"


/*------------------------------------------------------------
                         us��ʱ���� 
------------------------------------------------------------*/
void ysu(unsigned int us)
{
	SysTick->LOAD=9*us;
	SysTick->VAL=0;
	SysTick->CTRL=1;
	while(!(0x10000&SysTick->CTRL));
	SysTick->CTRL=0;
}

/*------------------------------------------------------------
                         ms��ʱ����
------------------------------------------------------------*/
void ysm(unsigned int ms)
{
	u32 ss ;
	ss=ms*9000;
	while(ss)
	{
		if(ss>16000000)
		{
			SysTick->LOAD=16000000;
			ss-=16000000;
		}
		else
		{
			SysTick->LOAD=ss;
			ss=0;
		}
		SysTick->VAL=0;
		SysTick->CTRL=1;
		while(!(0x10000&SysTick->CTRL));
		SysTick->CTRL=0;
	}
}

/*------------------------------------------------------------
                      ������ʱ�ӼĴ�����λ
------------------------------------------------------------*/
static void RCC_DeInit(void)
{									   
	RCC->APB2RSTR = 0x00000000;
	RCC->APB1RSTR = 0x00000000;   	  
  	RCC->AHBENR = 0x00000014;  //flashʱ��,����ʱ��ʹ��.DMAʱ�ӹر�	  
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�
}


/*------------------------------------------------------------
                  �ⲿ8M,��õ�72M��ϵͳʱ��
------------------------------------------------------------*/	
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
	u8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON

	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//�ȴ��ⲿʱ�Ӿ���	 

	//0-24M �ȴ�0;24-48M �ȴ�1;48-72M�ȴ�2;(�ǳ���Ҫ!)	   
	FLASH->ACR|=0x32;//FLASH 2����ʱ����

	RCC->CFGR|=0X001D8400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE��ΪPLLʱ��Դ  0X001D8400
	RCC->CR|=0x01000000;  //PLLON

	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//�ȴ�PLL����

	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��
	while(temp!=0x02&&timeout<200)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
}

__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�
__asm void INTX_DISABLE(void)
{
	CPSID I;		  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE I;		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

