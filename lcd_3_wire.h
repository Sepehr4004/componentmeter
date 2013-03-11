#include <avr/io.h>
#include <util/delay.h>
#include "myutils.h"

#ifndef _LCD_H
#define _LCD_H

// CONNEXIONS DEL LCD
#define LCD_DATA_D7  7
#define LCD_DATA_D6  6
#define LCD_DATA_D5  5
#define LCD_DATA_D4  4

#define LCD_E_POS    3
#define LCD_RW_POS   2
#define LCD_RS_POS   1

// LCD de 16x2
#define LCD_TYPE_162

// INSTRUCCIONS DE CONFIGURACIO
#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE  0B00000000


// FUNCIONS
void shift_set_bit (uint8_t value);
void shift_clear_bit(uint8_t value);


void LCDInit(uint8_t style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,unsigned int field_length);
void LCDGotoXY(uint8_t x,uint8_t y);

// FUNCIONS DE BAIX NIVELL
void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

// MACROS
#define LCDClear() LCDCmd(0b00000001)
#define LCDHome() LCDCmd(0b00000010);

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}

#endif




