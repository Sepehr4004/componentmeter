

#include "freq_counter.h"
#include <avr/eeprom.h> 

#define CREF 0.001 // valor en uF
#define LREF 84.5097 // valor en mH

// aquesta instruccio genera una variable dins la memoria eeprom
// el compilador crea un fitxer .eep
float EEMEM freq0_cal = 0;

void cal_LC (void)
{
	float freq;

	freq = freq_counter_read();

	eeprom_update_float(&freq0_cal, freq);
}

float calcula_C (void)
{
	float freq1;
	float freq0;
	float calcul;

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
	float freq1;
	float freq0;
	float calcul;

	freq0 = eeprom_read_float( &freq0_cal );
	freq1 = freq_counter_read();

    calcul = freq0/freq1;
    calcul = calcul * calcul;
    calcul = calcul - 1.0;
    calcul = LREF*calcul;

    return calcul;
}
