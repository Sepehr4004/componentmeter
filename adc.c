// microcontroller adc library by M.A. Borrego
#include <avr/io.h>

uint16_t ReadAdc(uint8_t channel)
{
   uint8_t result1, result2;
   
   channel = channel & 7; // get only 3 bits ADC0:7
   
   
   // (00 AREF), (01 AVcc), (11 Internal 1.1V)
   //ADMUX = (1<<REFS0) | (1<<REFS1) | (channel); // 1.1V
   ADMUX = (1<<REFS0) | (channel); // 1.1V
   
   // clock preescaler al màxim
   ADCSRA = (1<<ADPS0) | (1<<ADPS2); 
   
   // habilita i comença conversió A/D
   ADCSRA |= (1<<ADEN);
   ADCSRA |= (1<<ADSC);
   
   // esperem a que hagi finalitzat
   while ( !(ADCSRA & (1<<ADIF)));
   ADCSRA |= (1<<ADIF);
   
   // retornem el valor llegit
   result1 = ADCL;
   result2 = ADCH;
   
   return (result2<<8)+result1; // return ADCH:L
}