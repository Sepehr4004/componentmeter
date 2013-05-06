/*************************************
 * Nom del arxiu: transistor.c
 * Autor: Miguel Angel Borrego
 * Data: 31/03/2013
 * Compilador: WinAvr 20100110
 * Descripció: Funcions necessàries per 
 * comprovar un transistor.
 **************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "transistor.h"

// macros per direccionar bits
#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

uint8_t i;

// posa la configuració del transistor al punter
// que es passa com a paràmetre d'entrada
uint8_t transistor_check(struct bjt* transistor)
{
    uint16_t value[6] = {0,0,0,0,0,0};

    // inicialitzem els valors a 0
    memset(transistor, 0, sizeof(transistor)); 
	
    // comprovem caiguda de tensio entre pins
    value[0] = check(TP1, TP2);
    value[1] = check(TP1, TP3);
    value[2] = check(TP2, TP1);
    value[3] = check(TP2, TP3);
    value[4] = check(TP3, TP1);
    value[5] = check(TP3, TP2);
    
    // mirem si es NPN amb base al pin 1
    if (value[0] < 1000)
    {
        if (value[1] < 1000)
        {
            transistor->base = TP1;
            transistor->tipus = NPN;
            
            if (value[0] < value[1])
            {
                transistor->colector = TP2;
                transistor->emisor = TP3;
            }
            else
            {
                transistor->colector = TP3;
                transistor->emisor = TP2;
            }
        }

        // Mirem si es PNP amb base al pin 2
        else if (value[5] < 1000)
        {
            transistor->base = TP2;
            transistor->tipus = PNP;
            
            if (value[0] < value [5])
            {
                transistor->colector = TP1;
                transistor->emisor = TP3;
            }
            else
            {
                transistor->colector = TP3;
                transistor->emisor = TP1;
            }
       	}
    }

	// mirem si es NPN amb base al pin 2
    else if (value[2] < 1000)
    {
        if (value[3] < 1000)
        {
            transistor->base = TP2;
            transistor->tipus = NPN;
            
            if (value[2] < value[3])
            {
                transistor->colector = TP1;
                transistor->emisor = TP3;
            }
            else
            {
                transistor->colector = TP3;
                transistor->emisor = TP1;
            }
        }
        
        // Mirem si es PNP amb base al pin 1
        else if (value[4] < 1000)
        {
            transistor->base = TP1;
            transistor->tipus = PNP;
            
            if (value[2] < value [4])
            {
                transistor->colector = TP2;
                transistor->emisor = TP3;
            }
            else
            {
                transistor->colector = TP3;
                transistor->emisor = TP2;
            }
      	}
    }

    // Mirem si es NPN amb base al pin 3
    else if ( value[4] < 1000)
    {
        if (value[5] < 1000)
        {
            transistor->base = TP3;
            transistor->tipus = NPN;
            
            if (value[4] < value[5])
            {
                transistor->colector = TP1;
                transistor->emisor = TP2;
            }
            else
            {
                transistor->colector = TP2;
                transistor->emisor = TP1;
            }
        }
    }

	// Mirem si es PNP amb base al pin 3
	if (value[1] < 1000)
	{
        if (value[3] < 1000)
        {
            transistor->base = TP3;
            transistor->tipus = PNP;
            
            if (value[1] < value[3])
            {
                transistor->colector = TP1;
                transistor->emisor = TP2;
            }
            else 
            {
                transistor->colector = TP2;
                transistor->emisor = TP1;
            }
        }
	}

	// retorna 0 (error) si no s'ha trobat una configuració bona
	if (transistor->colector == transistor->emisor) return 0;
    else return 1;
}

// retorna el valor resultant de polaritzar dos pins del transistor
uint16_t check ( uint8_t high, uint8_t low )
{
    uint16_t adc_value = 0;

	tr_init();
	
    // configura el pins amb la resistencia petita
	if (high == TP1)
	{
		BIT_SET(RTL1_DDR, RTL1);
		BIT_SET(RTL1_PORT, RTL1);
	}
	else if (high == TP2)
	{
		BIT_SET(RTL2_DDR, RTL2);
		BIT_SET(RTL2_PORT, RTL2);
	}
	else
	{
		BIT_SET(RTL3_DDR, RTL3);
		BIT_SET(RTL3_PORT, RTL3);
	}

	// configura pins de test
	if (low == TP1)
	{
		BIT_SET(TP1_DDR, TP1);
		BIT_CLEAR(TP1_PORT, TP1);
	}
	else if (low == TP2)
	{
		BIT_SET(TP2_DDR, TP2);
		BIT_CLEAR(TP2_PORT, TP2);
	}
	else if (low == TP3)
	{
		BIT_SET(TP3_DDR, TP3);
		BIT_CLEAR(TP3_PORT, TP3);
	}

    // esperem a que el senyal sigui estable
    _delay_ms(100);

    // mesurem la caiguda de tensio
    adc_value = ReadAdc(high);

	// configura pins a alta impedancia
	tr_init();

    return adc_value;
}



// calcula la beta, es passa el transistor com a paràmetre
uint8_t calculate_beta (struct bjt* tr)
{

	// configura pins a alta impedancia
	tr_init();

    if (tr->tipus == NPN)
    {
   		 // resistencia base a vcc amb rth
    	if (tr->base == TP1)
		{
			BIT_SET(RTH1_DDR, RTH1);
			BIT_SET(RTH1_PORT, RTH1);
		}
		else if (tr->base == TP2)
		{
			BIT_SET(RTH2_DDR, RTH2);
			BIT_SET(RTH2_PORT, RTH2);
		}
		else if (tr->base == TP3)
		{
			BIT_SET(RTH3_DDR, RTH3);
			BIT_SET(RTH3_PORT, RTH3);
		}

		// resistencia colector a vcc amb rtl
		if (tr->colector == TP1)
		{
			BIT_SET(RTL1_DDR, RTL1);
			BIT_SET(RTL1_PORT, RTL1);
		}
		else if (tr->colector == TP2)
		{
			BIT_SET(RTL2_DDR, RTL2);
			BIT_SET(RTL2_PORT, RTL2);
		}
		else if (tr->colector == TP3)
		{
			BIT_SET(RTL3_DDR, RTL3);
			BIT_SET(RTL3_PORT, RTL3);
		}

		// resistencia emisor a gnd
		if (tr->emisor == TP1)
		{
			BIT_SET(TP1_DDR, TP1);
			BIT_CLEAR(TP1_PORT, TP1);
		}
		else if (tr->emisor == TP2)
		{
			BIT_SET(TP2_DDR, TP2);
			BIT_CLEAR(TP2_PORT, TP2);
		}
		else if (tr->emisor == TP3)
		{
			BIT_SET(TP3_DDR, TP3);
			BIT_CLEAR(TP3_PORT, TP3);
		}
		// formula per calcular la beta d'un transistor NPN
		tr->beta = (uint16_t)(1000*((5)-(ReadAdc(tr->colector)*5/1024.0))/(5-0.7));
    }
    else if (tr->tipus == PNP)
    {
       	// resistencia base a gnd amb rth
    	if (tr->base == TP1)
		{
			BIT_SET(RTH1_DDR, RTH1);
			BIT_CLEAR(RTH1_PORT, RTH1);
		}
		else if (tr->base == TP2)
		{
			BIT_SET(RTH2_DDR, RTH2);
			BIT_CLEAR(RTH2_PORT, RTH2);
		}
		else if (tr->base == TP3)
		{
			BIT_SET(RTH3_DDR, RTH3);
			BIT_CLEAR(RTH3_PORT, RTH3);
		}

		// resistencia colector a gnd amb rtl
		if (tr->colector == TP1)
		{
			BIT_SET(RTL1_DDR, RTL1);
			BIT_CLEAR(RTL1_PORT, RTL1);
		}
		else if (tr->colector == TP2)
		{
			BIT_SET(RTL2_DDR, RTL2);
			BIT_CLEAR(RTL2_PORT, RTL2);
		}
		else if (tr->colector == TP3)
		{
			BIT_SET(RTL3_DDR, RTL3);
			BIT_CLEAR(RTL3_PORT, RTL3);
		}

		// resistencia emisor a gnd
		if (tr->emisor == TP1)
		{
			BIT_SET(TP1_DDR, TP1);
			BIT_SET(TP1_PORT, TP1);
		}
		else if (tr->emisor == TP2)
		{
			BIT_SET(TP2_DDR, TP2);
			BIT_SET(TP2_PORT, TP2);
		}
		else if (tr->emisor == TP3)
		{
			BIT_SET(TP3_DDR, TP3);
			BIT_SET(TP3_PORT, TP3);
		}
		// formula per calcular la beta d'un transistor PNP
		tr->beta = (uint16_t)(1000*((ReadAdc(tr->colector)*5/1024.0))/(5-0.7));
    }
	tr_init();

    return 1;
}

// inicialitza el valor dels pins
void tr_init(void)
{
	// pull-up desactivades
	BIT_CLEAR(TP1_PORT,TP1);
	BIT_CLEAR(TP2_PORT,TP2);
	BIT_CLEAR(TP3_PORT,TP3);

	BIT_CLEAR(RTL1_PORT,RTL1);
	BIT_CLEAR(RTL2_PORT,RTL2);
	BIT_CLEAR(RTL3_PORT,RTL3);

	BIT_CLEAR(RTH1_PORT,RTH1);
	BIT_CLEAR(RTH2_PORT,RTH2);
	BIT_CLEAR(RTH3_PORT,RTH3);


	// pins com a entrada
	BIT_CLEAR(TP1_DDR,TP1);
	BIT_CLEAR(TP2_DDR,TP2);
	BIT_CLEAR(TP3_DDR,TP3);

	BIT_CLEAR(RTL1_DDR,RTL1);
	BIT_CLEAR(RTL2_DDR,RTL2);
	BIT_CLEAR(RTL3_DDR,RTL3);

	BIT_CLEAR(RTH1_DDR,RTH1);
	BIT_CLEAR(RTH2_DDR,RTH2);
	BIT_CLEAR(RTH3_DDR,RTH3);
}
