#include <avr/io.h>
#include <util/delay.h>
#include "transistor.h"
#include "adc.h"
#include "lcd_3_wire.h"

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

uint8_t i;
extern char string[16];


struct bjt transistor_check(void)
{
    uint16_t value[6] = {0,0,0,0,0,0};

	struct bjt transistor = {0,0,0,0,0};
	
    // comprovem caiguda de tensio entre TP1 i TP2
    value[0] = check(TP1, TP2);
    value[1] = check(TP1, TP3);
    value[2] = check(TP2, TP1);
    value[3] = check(TP2, TP3);
    value[4] = check(TP3, TP1);
    value[5] = check(TP3, TP2);
    

    if (value[0] < 1000)
    {
        if (value[1] < 1000)
        {
            transistor.base = TP1;
            transistor.tipus = NPN;
            
            if (value[0] < value[1])
            {
                transistor.colector = TP2;
                transistor.emisor = TP3;
            }
            else
            {
                transistor.colector = TP3;
                transistor.emisor = TP2;
            }
        }
        else if (value[5] < 1000)
        {
            transistor.base = TP2;
            transistor.tipus = PNP;
            
            if (value[0] < value [5])
            {
                transistor.colector = TP1;
                transistor.emisor = TP3;
            }
            else
            {
                transistor.colector = TP3;
                transistor.emisor = TP1;
            }
        }
    }
    else if (value[1] < 1000)
    {
        
        if (value[3] < 1000)
        {
            transistor.base = TP3;
            transistor.tipus = PNP;
            
            if (value[1] < value[3])
            {
                transistor.colector = TP1;
                transistor.emisor = TP2;
            }
            else 
            {
                transistor.colector = TP2;
                transistor.emisor = TP1;
            }
        }
        
    }
    else if (value[2] < 1000)
    {
        if (value[3] < 1000)
        {
            transistor.base = TP2;
            transistor.tipus = NPN;
            
            if (value[2] < value[3])
            {
                transistor.colector = TP1;
                transistor.emisor = TP3;
            }
            else
            {
                transistor.colector = TP3;
                transistor.emisor = TP1;
            }
        }
        else if (value[4] < 1000)
        {
            transistor.base = TP1;
            transistor.tipus = PNP;
            
            if(value[2] < value[4])
            {
                transistor.colector = TP2;
                transistor.emisor = TP3;
            }
            else
            {
                transistor.colector = TP3;
                transistor.emisor = TP2;
            }
        }
    }
    else if( value[4] < 1000)
    {
        if (value[5] < 1000)
        {
            transistor.base = TP3;
            transistor.tipus = NPN;
            
            if (value[4] < value[5])
            {
                transistor.colector = TP1;
                transistor.emisor = TP2;
            }
            else
            {
                transistor.colector = TP2;
                transistor.emisor = TP1;
            }
        }
    }
    
    calculate_beta(&transistor);
    sprintf(string, "B:%d C:%d E:%d %s", transistor.base, transistor.colector, transistor.emisor,
										transistor.tipus == 0 ? "NPN" : "PNP");
    LCDClear();
    LCDWriteString(string);
    LCDGotoXY(0,1);
	i=i%6;
	//tr_init();
    sprintf(string, "Beta: %d %d:%d", transistor.beta,i, value[i] );
	//sprintf(string, "%x %x %x %x", DDRB, DDRC, PORTB, PORTC);
	i++;
    LCDWriteString(string);

	
	return transistor;
}

uint16_t check ( uint8_t high, uint8_t low )
{
    uint16_t adc_value = 0;

	tr_init();
	
 /*   // inicialitza els pins
    ADCDDR &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // ADC com a entrada
    ADCPORT &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // deshabilita resistencies pull-up

    RDDR &= ~((1 << R0) | (1 << R1) | (1 << R2)); // resistencies sense cap voltage
    RPORT &= ~((1 << R0) | (1 << R1) | (1 << R2)); // deshabilita resistencies pull-up
	
    RHDDR &= ~((1 << RH0) | (1 << RH1) | (1 << RH2));
    RHPORT &= ~((1 << RH0) | (1 << RH1) | (1 << RH2)); 
 */
    // configura els pins segons els parametres
 /*   RDDR |= (1 << high);
    RPORT |= (1 << high);*/
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
/*
    ADCDDR |= (1 << low);
    ADCPORT &= ~(1 << low);
 */   
    // esperem a que el senyal sigui estable
    _delay_ms(100);

    // mesurem la caiguda de tensio
    adc_value = ReadAdc(high);

    // inicialitza els pins
    /*ADCDDR &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // ADC com a entrada
    ADCPORT &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // deshabilita resistencies pull-up

    RDDR &= ~((1 << R0) | (1 << R1) | (1 << R2)); // resistencies sense cap voltage
    RPORT &= ~((1 << R0) | (1 << R1) | (1 << R2)); // deshabilita resistencies pull-up*/

	// configura pins a alta impedancia
	tr_init();

    return adc_value;
}



//
uint8_t calculate_beta (struct bjt* tr)
{
    /*ADCDDR &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // ADC com a entrada
    ADCPORT &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // deshabilita resistencies pull-up

    RDDR &= ~((1 << R0) | (1 << R1) | (1 << R2)); // resistencies sense cap voltage
    RPORT &= ~((1 << R0) | (1 << R1) | (1 << R2)); // deshabilita resistencies pull-up

    RHDDR &= ~((1 << RH0) | (1 << RH1) | (1 << RH2));
    RHPORT &= ~((1 << RH0) | (1 << RH1) | (1 << RH2));*/

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

		
/*	   RHDDR |= (1 << (tr->base+RH_SHIFT));
       RHPORT |= (1 << (tr->base+RH_SHIFT));

    // configurem la resistencia de colector vcc
       RDDR |= (1 << tr->colector);
       RPORT |= (1 << tr->colector);

    // posem emissor a massa
       ADCDDR |= (1 << tr->emisor);
       ADCPORT &= ~(1 << tr->emisor);
       */
	// formula per calcular beta amb npn
	//_delay_ms(100);
	//tr->beta = ReadAdc(tr->colector);
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
		
 /*   // configurem la resistencia de base vcc
       RHDDR |= (1 << (tr->base+RH_SHIFT));
       RHPORT &= ~(1 << (tr->base+RH_SHIFT));

    // configurem la resistencia de colector vcc
       RDDR |= (1 << tr->colector);
       RPORT &= ~(1 << tr->colector);

    // posem emissor a massa
       ADCDDR |= (1 << tr->emisor);
       ADCPORT |= (1 << tr->emisor);
  */
  	// formula per calcular beta amb pnp
  	//_delay_ms(100);
  	//tr->beta = ReadAdc(tr->colector);
	tr->beta = (uint16_t)(1000*((ReadAdc(tr->colector)*5/1024.0))/(5-0.7));

    }
	tr_init();
	
//	while(1);
	// configura pins a alta impedancia
	//tr_init();
/*    ADCDDR &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // ADC com a entrada
    ADCPORT &= ~((1 << ADC0) | (1 << ADC1) | (1 << ADC2)); // deshabilita resistencies pull-up

    RDDR &= ~((1 << R0) | (1 << R1) | (1 << R2)); // resistencies sense cap voltage
    RPORT &= ~((1 << R0) | (1 << R1) | (1 << R2)); // deshabilita resistencies pull-up

    RHDDR &= ~((1 << RH0) | (1 << RH1) | (1 << RH2));
    RHPORT &= ~((1 << RH0) | (1 << RH1) | (1 << RH2)); */

    return 1;
}

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
