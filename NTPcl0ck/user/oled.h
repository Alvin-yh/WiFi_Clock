#ifndef _oled_h_
#define _oled_h_

void IIC_Port_Init(void);
void OLED_Init(void);
void OLED_Char(char c);
void OLED_Str(char *str);
void OLED_Clear(void);
void OLED_Fill(u8 bmp_data);
void OLED_Printf(const char *restric, ...);
void OLED_Char1(u8 num);
void OLED_xyset(u8 x,u8 y);
void font_set(u8 font);
void OLED_Draw(void);

void OLED_Char8x16Str(u8 x,u8 y,char *str);
void OLED_Char8x16(char c);
void OLED_Str8x16(char *str);

void OLED_8x16Printf(const char *restric, ...);


void OLED_Font24x48str(u8 x,u8 y,u8 *str);
void OLED_Char24x48(char c);
#endif
