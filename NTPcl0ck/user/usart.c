#include <sys.h>
#include <stdio.h>
#include <stdarg.h>
#include <usart.h>

/**
  *@brief PA2->TXD,PA3->RXD
  **/

void Usart2Init(u32 pck,u32 bound)
{
  RCC->APB1ENR |= 1<<17;
  GPIOA->CRL   &= 0xFFFF00FF;
  GPIOA->CRL   |= 0x00008B00;
  GPIOA->LCKR  |= 0xc;
  USART2->BRR   = pck*1000000/bound;
  USART2->CR1  |= 0x203c;
  USART2->CR1  |= 1<<5; 
  USART2->CR1  |= 1<<4; //IDE 
  NVIC->ISER[1]|= 1<<6;
  NVIC->IP[38]  = 1<<4;
}


void U2Putchar(char data)
{
  while(0 == (USART2->SR&0x40)){}
  USART2->DR = data;
}

void U2Putstr(char *sp)
{
  while(*sp) U2Putchar(*sp++);
}


void U2_Printf(const char *restric, ...)
{
  char Dis_Buf[128];
  va_list ap;
  va_start(ap,restric);
  vsprintf((char*)Dis_Buf,restric,ap);
  va_end(ap);
  U2Putstr(Dis_Buf);
}


u8 USART2_RX_BUF[128];
u8 USART2_RX_STA;
u8 USART2_Flag;
void USART2_IRQHandler(void)
{ 
	if(USART2->SR&0x20) {USART2_RX_BUF[USART2_RX_STA++] = USART2->DR;}
	if(USART2->SR&0x10)
  {
    USART2->DR;
    USART2_RX_BUF[USART2_RX_STA] = '\0';
    USART2_Flag = 1;
  }
  if(USART2_RX_STA>127) USART2_Flag = 1;
  

} 						 














