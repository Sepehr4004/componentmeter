/*************************************
 * Nom del arxiu: funcions_LC.c
 * Autor: Miguel Angel Borrego
 * Data: 07/05/2013
 * Compilador: WinAvr 20100110
 * Descripció: Funcions necessàries per 
 * calcular el valor de capacitat i inductància
 **************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h> 

#include "funcions_LC.h"

// aquesta instruccio genera una variable dins la memoria eeprom
// el compilador crea un fitxer .eep
float EEMEM freq0_cal = 0;

// ho definim static per nomes utilitzar-ho a dins d'aquest fitxer
static float freq1;
static float freq0;
static float calcul;

// ho definim volatile per dir-li al compilador que no faci optimitzacions
// amb aquesta variable ja que està a dins d'una interrupció
volatile unsigned char overflow;

// vector d'interrupció del comptador
ISR(TIMER1_OVF_vect)
{
   overflow++;
}

void freq_counter_init(void)
{
   // configura el pin com a entrada
   DDRD &= ~(1 << DDD5);

   // TIMER1 CONTROL REGISTER A
   TCCR1A = 0;
   
   // TIMER1 CONTROL REGISTER B
   // clock extern i activat per flanc de baixada
   TCCR1B = (1 << CS12) | (1 << CS11);
   
   // TIMER1 CONTROL REGISTER C
   TCCR1C = 0;
 
}


float freq_counter_read(void)
{
   // inicialitza valors
   float freq = 0;
   overflow = 0;
   
   // neteja valor timer1
   TCNT1 = 0; 

   // neteja tots els flags escrivint un 1
   TIFR1 = 0xff;
   
   // habilita la interrupcio d'overflow
   TIMSK1 |= (1 << TOIE1);
   
   // activem interrupcions globals
   sei();

   // 100ms
   _delay_ms(100);
   
   // deshabilitem la interrupcio 
   TIMSK1 &= ~(1 << TOIE1);

   // calcula la freqüencia en khz
   freq = ((float)(overflow*65536)+TCNT1)/100.0;
   
   return freq;
   
}


void LC_init(void)
{
	// configurem els registres per llegir l'interruptor
    SWITCH_DDR &= ~(1<<SWITCH_PIN);
    SWITCH_PORT |= (1<<SWITCH_PIN);

	freq0 = eeprom_read_float( &freq0_cal );
	if (freq0 == 0 )
	{
		calibra_LC();
	}
}

void calibra_LC (void)
{

	freq0 = freq_counter_read();

	eeprom_update_float(&freq0_cal, freq0);
}

float calcula_C (void)
{

	freq0 = eeprom_read_float( &freq0_cal );
	freq1 = freq_counter_read();
	

    calcul = freq0/freq1;
    calcul = calcul * calcul;
    calcul = calcul - 1.0;
    calcul = CREF*calcul;

    return calcul;

}

float calcula_L (void)
{
	freq0 = eeprom_read_float( &freq0_cal );
	freq1 = freq_counter_read();

    calcul = freq0/freq1;
    calcul = calcul * calcul;
    calcul = calcul - 1.0;
    calcul = LREF*calcul;

    return calcul;
}

uint8_t llegir_switch(void)
{
	return (SWITCH_INPUT & (1<<SWITCH_PIN)) >> SWITCH_PIN;
}
