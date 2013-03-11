#include <avr/io.h>
#include <util/delay.h>
#include "shiftregister.h"

#include <strings.h>
 
#include "lcd_3_wire.h"
#include "init.h"
#include "adc.h"
#include "freq_counter.h"
#include "transistor.h"
#include "resistencia.h"


// variables globals
char string[16];
struct bjt trt;
struct res r;

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

// Programa principal
int main(void)
{
   init_devices();
   while(1)
   {
/*   PORTC |= 1 << 5;
   _delay_ms(1000);
   PORTC &= ~(1<<5);
   _delay_ms(1000);*/
    LCDClear();
    //sprintf(string,"%lu",freq_counter_read());
    //LCDWriteString("freq:");
    //LCDWriteStringXY(8,0, string );
    //trt = transistor_check();
    //sprintf(string, "%d %d %d", ReadAdc(1),ReadAdc(2),ReadAdc(3));
    //LCDWriteString(string);*/
	uint8_t res = calcula_r(&r);
	
	sprintf(string,"R:%lu",r.valor);
    LCDClear();
    LCDWriteString(string);
    sprintf(string, "res = %d", res );
    LCDGotoXY(0,1);
    LCDWriteString(string);
    _delay_ms(2000);
    
   }

   return 0;
}

