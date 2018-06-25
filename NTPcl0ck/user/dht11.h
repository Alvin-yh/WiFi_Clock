#ifndef _dht11_h_
#define _dht11_h_

void DHT11_Port_Init(void);
void DHT11_Read(void);
u8 DHT11_Read_Byte(void);

typedef struct
{
  u8 humidity_integer;
  u8 humidity_point;
  u8 temperature_integer;
  u8 temperature_point;
  u8 checksum;
}DHT;

void TemperatueAndHumidityTrans(void);

extern u8 dat[5];
void PortC4Test(void);

#endif
