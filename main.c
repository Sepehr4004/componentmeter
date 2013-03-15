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
#include "funcions_LC.h"

#include "menu.h"


// variables globals
char string[16];
struct bjt trt;
struct res r;
uint8_t j;

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))



// Programa principal
int main(void)
{
   init_devices();
   cal_LC();
   while(1)
   {
/*   PORTC |= 1 << 5;
   _delay_ms(1000);
   PORTC &= ~(1<<5);
   _delay_ms(1000);*/
    LCDClear();
    //
    //float cref = , lref
    float calcul;

    calcul = calcula_L();
	/*float fr = freq_counter_read();
	calcul = (1.0/(2.0*3.141592654*fr));
	calcul = calcul * calcul;
	calcul = calcul * (1000.0/0.0845097);
	calcul = calcul - 0.001; // uF
       */
    sprintf(string,"freq: %f",calcul);
    LCDWriteStringXY(0,0, string );
  	_delay_ms(500);
    //trt = transistor_check();
    //sprintf(string, "%d %d %d", ReadAdc(1),ReadAdc(2),ReadAdc(3));
    //LCDWriteString(string);*/
/*	uint8_t res = calcula_r(&r);
	
	sprintf(string,"R:%lu",r.valor);
    LCDClear();
    LCDWriteString(string);
    sprintf(string, "res = %d", res );
    LCDGotoXY(0,1);
    LCDWriteString(string);*/
 /*   
    menu(P_MES);
    _delay_ms(500);
    menu(P_MES);
    _delay_ms(500);
    menu(P_MES);
    _delay_ms(500);
    menu(P_OK);
    _delay_ms(500);
    menu(P_OK);
    _delay_ms(500);
    menu(P_MES);
    _delay_ms(500);
    menu(P_MES);
    _delay_ms(500);
    menu(P_OK);
    _delay_ms(500);
    */
   }

   return 0;
}

