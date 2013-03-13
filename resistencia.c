#include <avr/io.h>
#include <util/delay.h>
#include "resistencia.h"
#include "adc.h"
#include "lcd_3_wire.h"

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

// macros per fer el codi llegible
#define OUTPUT_H(a) BIT_SET(a ## _DDR, (a)); \
					BIT_SET(a ## _PORT, (a))
#define OUTPUT_Z(a) BIT_CLEAR(a ## _DDR, (a)); \
					BIT_CLEAR(a ## _PORT, (a))


//
#define VAL 700
#define R_TOP   1000
// variables 
uint8_t i;
extern char string[16];

// funcions
uint32_t formula_r ( uint16_t adcval, uint8_t mult )
{
 uint32_t multiplicador = 1;
 uint8_t j;
 for (j=1; j<mult; j++)
 {
	multiplicador *= 10; // multipliquem per 10 el valor
 }
 return (multiplicador)*(((uint32_t)adcval*560)/(1024-adcval));
}


void r_init(void)
{
	// pins a alta impedancia
	OUTPUT_Z(RM1);
	OUTPUT_Z(RM2);
	OUTPUT_Z(RM3);
	OUTPUT_Z(RM4);
	OUTPUT_Z(RVIN);
}

uint8_t calcula_r (struct res *r)
{
	uint16_t adcval=0;
	
	// pins a alta impedancia
	r_init();
	
		// mirem vin amb r1 a vcc
	OUTPUT_H(RM1);
	_delay_ms(100);
	adcval = ReadAdc(RVIN);
	OUTPUT_Z(RM1);

    if (adcval <= VAL)
	{
		r->valor = formula_r(adcval, 1);
		return 1;
	}
	
	// mirem vin amb r2 a vcc
	OUTPUT_H(RM2);
	_delay_ms(100);
	adcval = ReadAdc(RVIN);
	OUTPUT_Z(RM2);
	if (adcval <= VAL) 
	{
		r->valor = formula_r(adcval, 2);
		return 2;
	}
	
	// mirem vin amb r3 a vcc
	OUTPUT_H(RM3);
	_delay_ms(100);
	adcval = ReadAdc(RVIN);
	OUTPUT_Z(RM3);
	if (adcval <= VAL) 
	{
		r->valor = formula_r(adcval, 3);
		return 3;
	}
	
	// mirem vin amb r4 a vcc
	OUTPUT_H(RM4);
	_delay_ms(100);
	adcval = ReadAdc(RVIN);
	OUTPUT_Z(RM4);
	if ( adcval < VAL ) 
	{
		r->valor = formula_r(adcval, 4);
		return 4;
	}
	
	return 10;

}
