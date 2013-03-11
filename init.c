#include "lcd.h"
#include "freq_counter.h"

void init_devices(void)
{
   //Initialize LCD module
   LCDInit(LS_BLINK|LS_ULINE);
	
   //Clear the screen
   LCDClear();

   //Write Default text
   LCDWriteString("LOADING...");
   _delay_ms(100);
   
   //Freq counter init
   freq_counter_init();
   
}