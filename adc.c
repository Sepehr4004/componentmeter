// microcontroller adc library by M.A. Borrego
#include <avr/io.h>

uint16_t ReadAdc(uint8_t channel)
{

   uint8_t result1, result2;
   
   channel = channel & 7; // get only 3 bits ADC0:7
   
   
   // (00 AREF), (01 AVcc), (11 Internal 1.1V)
   //ADMUX = (1<<REFS0) | (1<<REFS1) | (channel); // 1.1V
   ADMUX = (1<<REFS0) | (channel); // 1.1V
   
   ADCSRA = (1<<ADPS0) | (1<<ADPS2); // clock-preescaler
   
   ADCSRA |= (1<<ADEN); // Enable adc
   ADCSRA |= (1<<ADSC); // Start conversion
   
   while ( !(ADCSRA & (1<<ADIF)));
   
   ADCSRA |= (1<<ADIF);

   result1 = ADCL; // reading ADCL blocks ADC
   result2 = ADCH; // is necessary to read ADCH after
   
   return (result2<<8)+result1; // return ADCH:L

}