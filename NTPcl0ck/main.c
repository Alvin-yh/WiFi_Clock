#include <stdio.h>
#include <sys.h>
#include <usart.h>
#include "oled.h"
#include <dht11.h>
#include <esp8266.h>
#include <timer.h>


extern NTP NetTime;
extern clock localTime;
extern DHT DHT11;
extern u8 oneHourFlag;
//extern u8 *TnH;
char Date[7][4]={{"Sun\0"},{"Mon\0"},{"Tue\0"},{"Wed\0"},{"Thu\0"},{"Fri\0"},{"Sat\0"}};
char Month[12][4]={{"Jan\0"},{"Feb\0"},{"Mar\0"},{"Apr\0"},{"May\0"},{"Jun\0"},{"Jul\0"},{"Aug\0"},{"Sep\0"},{"Oct\0"},{"Nov\0"},{"Dec\0"}};
u8 time[6];


extern u8 USART2_RX_BUF[128];
extern u8 USART2_RX_STA;
extern u8 USART2_Flag;

int main(void)
{
  u8 hourHigh,hourLow,minHigh,minLow;
	Stm32_Clock_Init();
  SCB->AIRCR=0x05af0300;//700 0抢占；600 1抢占；500 2抢占；400 3抢占；300 4抢占；
  RCC->APB2ENR|=0X1d; //1 1101
  Usart2Init(36,115200);
  Usart1forEsp8266_Init(72,115200);
  OLED_Init();
  DHT11_Port_Init();  
  ESP8266_Init();
  if(getTime())U2_Printf("GetTimeFromNTP...ERROR\r\n");
  else U2_Printf("GetTimeFromNTP...OK\r\n");
  timer2ForClock();
  OLED_xyset(0,0);
  OLED_Fill(0x00);
  
  GPIOB->CRL |= 3;
  GPIOB->ODR |= 0x1;
  
  
	while(1)
	{
    
    DHT11_Read();
    OLED_xyset(0,0);
 
    OLED_8x16Printf("%s %s %d ",Date[localTime.date],Month[localTime.month],localTime.day);
    OLED_xyset(88,0);
    OLED_Printf("T :%d",DHT11.temperature_integer);OLED_Char(124);
    OLED_xyset(88,1);
    OLED_Printf("RH:%d%%",DHT11.humidity_integer);
    
    
    hourHigh = localTime.hour/10;
    hourLow = localTime.hour%10;
    minHigh = localTime.min/10;
    minLow = localTime.min%10;
    time[0] = (u8)hourHigh;
    time[1] = (u8)hourLow;
    time[2] = ':';
    time[3] = (u8)minHigh;
    time[4] = (u8)minLow;
    time[5] = 'x';
    OLED_Font24x48str(8,2,time);

    if(oneHourFlag)
    {
      
      getTime();
      oneHourFlag = 0; 
    }


	}
}














