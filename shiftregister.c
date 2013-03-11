///////////////////////////////
// Driver per Controlar
// un shift register
// Autor: M.A. Borrego
// Data: 5/1/2013
///////////////////////////////

#include <avr/io.h>
#include <util/delay.h>
#include "shiftregister.h"

#define BIT_SET(r,b) ((r)|=(1<<(b)))
#define BIT_CLEAR(r,b) ((r)&=~(1<<(b)))

#define DELAY 20

void shift_init( void )
{
    BIT_SET( LATCH_DDR, LATCH_BIT );
    BIT_SET( CLOCK_DDR, CLOCK_BIT );
    BIT_SET( DATA_DDR, DATA_BIT );

    BIT_CLEAR( LATCH_PORT, LATCH_BIT );
    BIT_CLEAR( LATCH_PORT, CLOCK_BIT );
    BIT_CLEAR( DATA_PORT, DATA_BIT );
}

void shift_write_word ( uint8_t *output, uint8_t size )
{
    uint8_t i;

    for ( i=1; i<=size; i++)
    {
        shift_write_bit(output[(size-i)/8] & (1<<(size-i)%8));
    }

    BIT_SET( LATCH_PORT, LATCH_BIT );
    _delay_us(DELAY);
    BIT_CLEAR( LATCH_PORT, LATCH_BIT );
    _delay_us(DELAY);
}

void shift_write_bit ( uint8_t value )
{
    if ( value )
       BIT_SET( DATA_PORT, DATA_BIT );
    else
       BIT_CLEAR( DATA_PORT, DATA_BIT );

    BIT_SET( CLOCK_PORT, CLOCK_BIT );
    _delay_us(DELAY);
    BIT_CLEAR ( CLOCK_PORT, CLOCK_BIT );
    _delay_us(DELAY);
}
