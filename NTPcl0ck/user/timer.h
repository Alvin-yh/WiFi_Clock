#ifndef _timer_h_
#define _timer_h_

void timer2ForClock(void);

typedef struct
{
  unsigned char sec;
  unsigned char min;
  unsigned char hour;
  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned char year;
  unsigned char dateTemp;
}clock;

#endif
