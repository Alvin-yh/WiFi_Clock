#ifndef esp8266_h_
#define esp8266_h_

void Usart1forEsp8266_Init(unsigned char Fck,unsigned int Bot);
void U1Putchar(char data);
void U1Putstr(char *sp);
void U1_Printf(const char *restric, ...);
void ESP8266_Init(void);
unsigned char getTimeFromNTPServer(void);
unsigned char getTime(void);
typedef struct
{
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
  unsigned int secTemp;
  char li;
  int daysInMonth;
  char date; // 0->Sun 1->Mon 2->Tue 3->Wed 4->Thur 5->Fri. 6->Sat.
}NTP;

#endif
