////////////////////////////////
// Interficie per LCD 2x16
// a traves de shiftregister
// Autor: M.A. Borrego
// Data: 5/1/2013
///////////////////////////////

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "lcd_3_wire.h"
#include "shiftregister.h"

//Custom Charset support
#include "custom_char.h"

#define POWER_UP_DELAY 30
#define DELAY			10

#define SET_E() (shift_set_bit(LCD_E_POS))
#define SET_RS() (shift_set_bit(LCD_RS_POS))
#define SET_RW() (shift_set_bit(LCD_RW_POS))

#define CLEAR_E() (shift_clear_bit(LCD_E_POS))
#define CLEAR_RS() (shift_clear_bit(LCD_RS_POS))
#define CLEAR_RW() (shift_clear_bit(LCD_RW_POS))

#define BUSY_DELAY() (_delay_ms(5))

#ifdef LCD_TYPE_162
    #define LCD_TYPE_204
#endif

#ifdef LCD_TYPE_202
    #define LCD_TYPE_204
#endif

// declare static global variable
uint8_t output_data=0;

void LCDByte(uint8_t c,uint8_t isdata)
{
//Sends a byte to the LCD in 4bit mode
//cmd=0 for data
//cmd=1 for command


//NOTE: THIS FUNCTION RETURS ONLY WHEN LCD HAS PROCESSED THE COMMAND

uint8_t hn,ln;          //Nibbles

// data is in high side
hn=(c & 0xF0);
ln=(c & 0x0F)<<4;

if(isdata==0)
    CLEAR_RS();
else
    SET_RS();

//Send high nibble
output_data=((output_data & 0x0F) | hn);
shift_write_word(&output_data, 8);

SET_E();
_delay_us(DELAY);
CLEAR_E();
_delay_us(DELAY);

//Send the lower nibble
output_data=((output_data & 0x0F) | ln);
shift_write_word(&output_data, 8);

SET_E();
_delay_us(DELAY);           //tEH
CLEAR_E();
_delay_us(DELAY);

BUSY_DELAY(); // wait for busy
}

void LCDInit(uint8_t style)
{
    /*****************************************************************
    
    This function Initializes the lcd module
    must be called before calling lcd related functions

    Arguments:
    style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
    LS_BLINK :The cursor is blinking type
    LS_ULINE :Cursor is "underline" type else "block" type

    *****************************************************************/
    //Set IO Ports
    shift_init();
    output_data = 0;

    CLEAR_E();
    CLEAR_RW();
    CLEAR_RS();
	
    //After power on Wait for LCD to Initialize
    _delay_ms(POWER_UP_DELAY);

	//Send high nibble

    output_data |=(0b00100000); //[B] To transfer 0b00100000 i was using LCD_DATA_PORT|=0b00100000

    shift_write_word(&output_data, 8);
    SET_E();
    _delay_us(DELAY);
    CLEAR_E();
    _delay_us(DELAY);
	
	
    //Wait for LCD to execute the Functionset Command
    BUSY_DELAY();
    //LCDBusyLoop();                                    //[B] Forgot this delay

    //Now the LCD is in 4-bit mode

    LCDCmd(0b00001100|style);   //Display On
    LCDCmd(0b00101000);         //function set 4-bit,2 line 5x7 dot format

    /* Custom Char */
    LCDCmd(0b01000000);

    uint8_t __i;
    for(__i=0;__i<sizeof(__cgram);__i++)
        LCDData(__cgram[__i]);
    
    LCDGotoXY(0,0);

}
void LCDWriteString(const char *msg)
{
    /*****************************************************************
    
    This function Writes a given string to lcd at the current cursor
    location.

    Arguments:
    msg: a null terminated string to print

    Their are 8 custom char in the LCD they can be defined using
    "LCD Custom Character Builder" PC Software. 

    You can print custom character using the % symbol. For example
    to print custom char number 0 (which is a degree symbol), you 
    need to write

        LCDWriteString("Temp is 30%0C");
                                  ^^
                                  |----> %0 will be replaced by
                                        custom char 0.

    So it will be printed like.
        
        Temp is 30°C
        
    In the same way you can insert any syblom numbered 0-7  


    *****************************************************************/
 while(*msg!='\0')
 {
    //Custom Char Support
    if(*msg=='%')
    {
        msg++;
        int8_t cc=*msg-'0';

        if(cc>=0 && cc<=7)
        {
            LCDData(cc);
        }
        else
        {
            LCDData('%');
            LCDData(*msg);
        }
    }
    else
    {
        LCDData(*msg);
    }
    msg++;
 }
}

void LCDWriteInt(int val,unsigned int field_length)
{
    /***************************************************************
    This function writes a integer type value to LCD module

    Arguments:
    1)int val   : Value to print

    2)unsigned int field_length :total length of field in which the value is printed
    must be between 1-5 if it is -1 the field length is no of digits in the val

    ****************************************************************/

    char str[5]={0,0,0,0,0};
    int i=4,j=0;
    while(val)
    {
    str[i]=val%10;
    val=val/10;
    i--;
    }
    if(field_length==-1)
        while(str[j]==0) j++;
    else
        j=5-field_length;

    if(val<0) LCDData('-');
    for(i=j;i<5;i++)
    {
    LCDData(48+str[i]);
    }
}
void LCDGotoXY(uint8_t x,uint8_t y)
{
    if(x>=20) return;

    #ifdef LCD_TYPE_204
    
    switch(y)
    {
        case 0:
            break;
        case 1:
            x|=0b01000000;
            break;
        case 2:
            x+=0x14;
            break;
        case 3:
            x+=0x54;
            break;
    }

    #endif

    #ifdef LCD_TYPE_164
    switch(y)
    {
        case 0:
            break;
        case 1:
            x|=0b01000000;
            break;
        case 2:
            x+=0x10;
            break;
        case 3:
            x+=0x50;
            break;
    }

    #endif

    x|=0b10000000;
    LCDCmd(x);
}

void shift_set_bit (uint8_t value)
{
     value &= 0x07; // mask value bits
     output_data |= (1 << value);
     shift_write_word(&output_data, 8);
}

void shift_clear_bit(uint8_t value)
{
     value &= 0x07; // mask value bits
     output_data &= ~(1 << value);
     shift_write_word(&output_data, 8);
}





