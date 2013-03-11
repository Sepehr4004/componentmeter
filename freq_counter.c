// microcontroller frequency counter by M.A. Borrego
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Global variables
volatile unsigned char overflow;

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


uint32_t freq_counter_read(void)
{
   // init values
   uint32_t freq = 0;
   overflow = 0;
   
   // CLEAR TIMER 1 VALUE
   TCNT1 = 0; 
   
   // TIMER1 INTERRUPT FLAG REGISTER
   // Clear all flags by writing a logic one
   TIFR1 = 0xff;
   
   // TIMER1 INTERRUPT MASK REGISTER
   // enable interrupt on overflow
   TIMSK1 |= (1 << TOIE1);
   
   // ENSURE GLOBAL INTERRUPTS ARE ENABLED
   sei();
   
   _delay_ms(100);
   
   // disable interrupt on overflow
   TIMSK1 &= ~(1 << TOIE1);
   
   freq = ((unsigned long)(overflow*65536)+TCNT1)*10;//(uint32_t)(overflow * 65536)+TCNT1;
   
   return freq;
   
}
