#include <sys.h>
#include "timer.h"
#include "esp8266.h"

extern NTP NetTime;

clock localTime;
/**
  *@brief timer2 used for local time counting
  *      TIM2 is 72MHz
  */
void timer2ForClock(void)
{
  RCC->APB1ENR |= 0x00000001;
  TIM2->PSC = 7199;
  TIM2->ARR = 9999;
  TIM2->EGR  |= 0x0001;
  TIM2->DIER |= 0x0001;
  TIM2->CR1  |= 0x0001;
  NVIC->ISER[0] |= 1<<28;
  NVIC->IP[28] = 2<<4;
}


int localmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 oneHourFlag=0;
void TIM2_IRQHandler(void)
{
  if(TIM2->SR&1)
  {
    localTime.sec += 1;
    if(60 == localTime.sec)
    {
      localTime.sec = 0;
      localTime.min += 1;
      if(60 == localTime.min)
      {
        localTime.min = 0;
        localTime.hour += 1;
        if(24 == localTime.hour)
        {
          oneHourFlag = 1;
          localTime.hour = 0;
          localTime.day += 1;
          localTime.dateTemp +=1;
          localTime.date = localTime.dateTemp%7;
          if(localTime.day > NetTime.daysInMonth )
          {
            localTime.day = 1;
            localTime.month += 1;
            NetTime.daysInMonth = localmonth[localTime.month-1];
            if(localTime.month > 11)
            {
              localTime.month = 0;
              localTime.year += 1;
            }
          }
        }
      }
    }
    
  }
  
  TIM2->SR = 0;
}



