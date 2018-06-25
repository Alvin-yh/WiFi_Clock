#include "sys.h"
#include <dht11.h>
#include <usart.h>

/*PB9 for DHT11<-Cancel   PA11 for DHT11*/

DHT DHT11;

#define Data_0 GPIOA->BRR  = 0x800
#define Data_1 GPIOA->BSRR = 0x800
#define Data_Out() {GPIOA->CRH &= 0xFFFF0FFF; \
                    GPIOA->CRH |= 0x00003000;}
#define Data_In()  {GPIOA->CRH &= 0xFFFF0FFF; \
                    GPIOA->CRH |= 0x00008000;}

                    
void DHT11_Port_Init(void)
{
  RCC->APB2ENR |= 1<<2;
  Data_Out();
}          


u8 dat[5];
void DHT11_Read(void)
{
  u16 t=1000;
  u16 timeOut=0xFFFF;
  Data_Out();
  Data_0;
  ysm(18);    // output data=0 at least 18ms
  Data_1;
  ysu(50);    // output data=1 at least 20-40us
  Data_In(); 
 
  if(!(GPIOA->IDR&0x800)) //if PB9==0
  {
    while(!(GPIOA->IDR&0x800)&&(t--)); //(read)wait data==0 80us
    if(t==0) return;
    while((GPIOA->IDR&0x800)&&(timeOut--));
    if(timeOut == 0) return;
     // then wait data=1 80us 
  //  U2_Printf("%d",DHT11_Read_Byte());
     
//    dat[0]=DHT11_Read_Byte();
//    dat[1]=DHT11_Read_Byte();
//    dat[2]=DHT11_Read_Byte();
//    dat[3]=DHT11_Read_Byte();
//    dat[4]=DHT11_Read_Byte(); 
    DHT11.humidity_integer = DHT11_Read_Byte();
    DHT11.humidity_point = DHT11_Read_Byte();
    DHT11.temperature_integer = DHT11_Read_Byte();
    DHT11.temperature_point = DHT11_Read_Byte();
    DHT11.checksum = DHT11_Read_Byte();


  }

  DHT11_Port_Init();
//  TemperatueAndHumidityTrans();
  
  

}

u8 DHT11_Read_Byte(void)
{
  u8 data,k;
  u16 timeOut = 0xffff;
  for(k=0;k<8;k++)
  {
    data<<=1;
    while((!(GPIOA->IDR&0x800))&&(timeOut--)); // wait DHT11->put high
    ysu(30);
    if(GPIOA->IDR&0x800) 
    {
      data=data|0x01;
      while(GPIOA->IDR&0x800);
    }
    else
    {
      data=data|0x00;
    } 
    
    
    
  }
  return data;
}




                    
                    
                    
                    
                    
                    
                    
                    
