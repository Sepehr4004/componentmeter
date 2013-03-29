#include <avr/io.h>
#include <util/delay.h>

#ifndef _SHIFT_REG_H_
#define _SHIFT_REG_H_

#define DATA_PORT   PORTD
#define DATA_DDR    (*(&DATA_PORT-1))
#define DATA_BIT    7

#define CLOCK_PORT  PORTB
#define CLOCK_DDR   (*(&CLOCK_PORT-1))
#define CLOCK_BIT   0

#define LATCH_PORT  PORTD
#define LATCH_DDR   (*(&LATCH_PORT-1))
#define LATCH_BIT   6


// FUNCIONS
void shift_init( void );

void shift_write_word ( uint8_t *output, uint8_t size );

void shift_write_bit ( uint8_t value );
#endif


