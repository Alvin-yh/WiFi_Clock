#ifndef __USART_H
#define __USART_H

extern u8 USART2_RX_BUF[128];
extern u8 USART2_RX_STA;
extern u8 USART2_Flag;

void uart_init(u32 pclk2,u32 bound);
void Usart2Init(u32 pck,u32 bound);
void U2_Printf(const char *restric, ...);

#endif	   
















