#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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
volatile polsador_t polsador;
volatile uint8_t flag_polsador;

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

#define PMES_PORT	PORTD
#define PMES_DDR    (*(&PMES_PORT-1))
#define PMES_INPUT  (*(&PMES_PORT-2))
#define PMES_PIN	2

#define POK_PORT	PORTD
#define POK_DDR   	(*(&POK_PORT-1))
#define POK_INPUT 	(*(&POK_PORT-2))
#define POK_PIN		3


ISR(INT0_vect)
{
	flag_polsador = 1;
	polsador = P_MES;
   _delay_ms(100);
}

ISR(INT1_vect)
{
	flag_polsador = 1;
	polsador = P_OK;
   _delay_ms(100);
}



void polsadors_init(void)
{
	// configurem pins PMES i POK com a entrada
    PMES_DDR &= ~(1<<PMES_PIN);
    POK_DDR &= ~(1<<POK_PIN);

    // activem pull-up
    PMES_PORT |= (1<<PMES_PIN);
    POK_PORT |= (1<<POK_PIN);

    // configurem interrupcions INT0 i INT1
    // activacio per nivell baix
    EICRA = 0x00;

    // habilitem les interrupcions
    EIMSK |= (1<<INT1) | (1<<INT0);

	// habilitem les interrupcions generals
    sei();
}


// Programa principal
int main(void)
{
   	//Inicialitza el display LCD
   	LCDInit(LS_NONE);
     
   	//Neteja la pantalla
   	LCDClear();
    
   	//Inicialitza el comptador
   	freq_counter_init();

   	// inicialitza el mesurador de inductancia i capacitat
   	LC_init();

   	//inicialitza el mesurador de transistors
   	tr_init();

   	//inicialitza el mesurador de resistencia
   	r_init();

   	//inicialitza els polsadors i les interrupcions d'aquets
   	polsadors_init();

	
   	// inicialitza el menu del lcd
	menu_init();
   	while(1)
   	{
		if (flag_polsador == 1)
		{
			flag_polsador = 0;
    		menu(polsador);
		}
	}

   return 0;
}

