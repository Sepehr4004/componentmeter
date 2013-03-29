
#include "funcions_LC.h"
#include "freq_counter.h"
#include <avr/eeprom.h> 

#define CREF 0.001 // valor en uF
#define LREF 100.0 // valor en uH

// aquesta instruccio genera una variable dins la memoria eeprom
// el compilador crea un fitxer .eep
float EEMEM freq0_cal = 0;

// ho definim static per nomes utilitzar-ho a dins d'aquest fitxer
static float freq1;
static float freq0;
static float calcul;

void init_LC(void)
{
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
