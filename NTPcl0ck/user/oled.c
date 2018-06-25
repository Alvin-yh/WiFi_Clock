#include <sys.h>
#include <oled.h>
#include <stdio.h>
#include <stdarg.h>
#include "char.h"

/*PB5->CLK  PB6->SDA*/


#define SCL_0 GPIOB->BRR = 0x20
#define SCL_1 GPIOB->BSRR  = 0x20
#define SDA_0 GPIOB->BRR = 0x40
#define SDA_1 GPIOB->BSRR  = 0x40


#define	Brightness	0xCF 
#define X_width 128
#define Y_width 64

u8 X_axes=0; 
u8 Y_axes=0;

void delay(void)
{
	volatile u8 i=5;// 取消编译器优化
	while(i--);
}
void IIC_Port_Init(void)
{
  RCC->APB2ENR |= 1<<3;
  GPIOB->CRL &= 0xF00FFFFF;
  GPIOB->CRL |= 0x03300000;
}

void IIC_Start(void)
{
  SDA_1;
  SCL_1;//  SCL_1; PA7
  delay();
  SCL_1;
  SDA_0;
  delay();
  SCL_0;
  delay();
}





void IIC_Stop(void)
{
  SCL_0;
  SDA_0;
  delay();
  SCL_1;
  delay();
  SDA_1;
  delay();
}

void IIC_WaitACK(void)
{
  SCL_0;
  SDA_0;
  delay();
  SCL_1;
  delay();
  SCL_0;
}

void Write_IIC_Byte(u8 data)
{
	u8 i=8;
  while(i--)
  {
    SCL_0;
    delay();
    if(data&0x80) SDA_1;
    else SDA_0;
    data<<=1;
    delay();
    SCL_1;
    delay();
  }
  SCL_0;
}


void Write_data(u8 data)
{
  IIC_Start();
  Write_IIC_Byte(0x78);IIC_WaitACK();
  Write_IIC_Byte(0x40);IIC_WaitACK();
  Write_IIC_Byte(data);IIC_WaitACK();
  IIC_Stop();
}

void Write_cmd(u8 data)
{
  IIC_Start();
  Write_IIC_Byte(0x78);IIC_WaitACK();
  Write_IIC_Byte(0x00);IIC_WaitACK();
  Write_IIC_Byte(data);IIC_WaitACK();
  IIC_Stop();
}

void OLED_xyset(u8 x,u8 y)
{
  X_axes=x;
  Y_axes=y;
  Write_cmd(0xb0+y);
  Write_cmd(((x&0xf0)>>4)|0x10);
  Write_cmd((x&0x0f)|0x01);
}

void OLED_Fill(u8 bmp_data)
{
  u8 y,x;
  for(y=0;y<8;y++)//<8
  {
    Write_cmd(0xb0+y);
    Write_cmd(0x01);
    Write_cmd(0x10);
    for(x=0;x<X_width;x++)
    {
      Write_data(bmp_data);
    }
  }
}

void OLED_Clear(void)
{
  OLED_Fill(0);
}



void OLED_Init(void)
{

  IIC_Port_Init();
  ysm(500);//初始化之前的延时很重要！
  
	Write_cmd(0xAE); //display off
	Write_cmd(0x20);	//Set Memory Addressing Mode	
	Write_cmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	Write_cmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	Write_cmd(0xc8);	//Set COM Output Scan Direction
	Write_cmd(0x00); //---set low column address
	Write_cmd(0x10); //---set high column address
	Write_cmd(0x40); //--set start line address
	Write_cmd(0x81); //--set contrast control register
	Write_cmd(0xff); //亮度调节 0x00~0xff
	Write_cmd(0xa1); //--set segment re-map 0 to 127
	Write_cmd(0xa6); //--set normal display
	Write_cmd(0xa8); //--set multiplex ratio(1 to 64)
	Write_cmd(0x3F); //
	Write_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	Write_cmd(0xd3); //-set display offset
	Write_cmd(0x00); //-not offset
	Write_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	Write_cmd(0xf0); //--set divide ratio
	Write_cmd(0xd9); //--set pre-charge period
	Write_cmd(0x22); //
	Write_cmd(0xda); //--set com pins hardware configuration
	Write_cmd(0x12);
	Write_cmd(0xdb); //--set vcomh
	Write_cmd(0x20); //0x20,0.77xVcc
	Write_cmd(0x8d); //--set DC-DC enable
	Write_cmd(0x14); //
	Write_cmd(0xaf); //--turn on oled panel
	
  OLED_xyset(0,0);
  OLED_Fill(0x00); //初始清屏
    





}

void OLED_Char(char c)
{
  u8 line=0;
  if(c=='\n')
  {
    Y_axes++;
    if(Y_axes>8) return;
    OLED_xyset(0,Y_axes);
    return;
  }
  c-=32;
  while (line<6)
  Write_data(font6x8[c][line++]);
}

void OLED_Char1(u8 num)
{
  u8 line=0;
  while(line<6)
  Write_data(font6x8[num][line++]);
}

void OLED_Str(char *str)
{
  while(*str) OLED_Char(*str++);
}

void OLED_Printf(const char *restric, ...)
{
  char Dis_Buf[128];
  va_list ap;
  va_start(ap,restric);
  vsprintf((char*)Dis_Buf,restric,ap);
  va_end(ap);
  OLED_Str(Dis_Buf);
}



/**************************8x16font***********************************/
void OLED_Char8x16(char c)
{
  u8 i;
  if(c == '\n')
  {
    Y_axes += 2;
    X_axes = 0;
    OLED_xyset(X_axes,Y_axes);
    if( Y_axes > 8) return;
  }
    c -= 32;

    for(i=0;i<8;i++)
    Write_data(font8x16[c*16+i]);
    OLED_xyset(X_axes,Y_axes+1);
    for(i=0;i<8;i++)
    Write_data(font8x16[c*16+i+8]);
    X_axes += 8;
    OLED_xyset(X_axes,--Y_axes);
}

void OLED_Str8x16(char *str)
{
  while(*str) OLED_Char8x16(*str++);
}

void OLED_8x16Printf(const char *restric, ...)
{
  char Dis_Buf[128];
  va_list ap;
  va_start(ap,restric);
  vsprintf((char*)Dis_Buf,restric,ap);
  va_end(ap);
  OLED_Str8x16(Dis_Buf);
}


/**************************24x48font***********************************/


void OLED_Font24x48str(u8 x,u8 y,u8 *str) //
{
  u8 i,j;
  u8 c=0;
  do
  {  
    if((*str<=9))       // font 0-9 16-29
    c=(*str++); // print number
    else c=(*str++)-48;
    
    if(x>104){x=0;y++;}
    OLED_xyset(x,y);
    for(j=0;j<6;j++)
    {  
      for(i=0;i<24;i++)
      Write_data(font24x48[c*6+j][i]);
      if(j<5)OLED_xyset(x,y+j+1);
    }
    x+=24;
  }while((*str)!='x');
}



int fputc(int ch,FILE *f)
{
  OLED_Char(ch);
  return ch;

}

void OLED_Draw(void)
{
  u8 line=127;
  OLED_xyset(0,0);
  while(line--)
  Write_data(0x12);
}
























