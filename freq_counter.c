// microcontroller frequency counter by M.A. Borrego
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Global variables
volatile unsigned char overflow;
uint32_t calfreq = 500000;

ISR(TIMER1_OVF_vect)
{
   overflow++;
}

void freq_counter_init(void)
{
   // Configure pin as input
   DDRD &= ~(1 << DDD5);

   // TIMER1 CONTROL REGISTER A
   TCCR1A = 0;
   
   // TIMER1 CONTROL REGISTER B
   //  External clock source T1 pin, falling edge
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

   // calcula la freqüencia
   freq = ((float)(overflow*65536)+TCNT1)*10;
   
   return freq;
   
}
