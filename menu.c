#include "menu.h"

#include "lcd_3_wire.h"
#include "transistor.h"
#include "resistencia.h"
#include "freq_counter.h"

#include <avr/pgmspace.h> 
#include <string.h>
#include <stdio.h>


#define MENU1 1
#define MENU2 2

// Strings emmagatzemats a la ROM
char menu1_C[] PROGMEM = "1.Condensador   ";
char menu1_L[] PROGMEM = "2.Inductancia   ";
char menu1_R[] PROGMEM = "3.Resistencia   ";
char menu1_T[] PROGMEM = "4.Transistor    ";

char menu2_Llegir[] PROGMEM = "LLEGIR          ";
char menu2_Cal[] PROGMEM = 	  "CAL             ";
char menu2_Enrere[] PROGMEM = "ENRERE          ";

PGM_P menu1_strings[] PROGMEM = 
{
	menu1_C,
	menu1_L,
	menu1_R,
	menu1_T
};

PGM_P menu2_strings[] PROGMEM = 
{
	menu2_Llegir,
	menu2_Cal,
	menu2_Enrere
};
enum menu2_estats
{
	LLEGIR,
	CAL,
	ENRERE
};



static uint8_t menu1 = 0; // menu1 actiu opcio1
static uint8_t menu2 = 0;
static uint8_t menu_flag = MENU1; // inicialitzem el punter amb el menu1 

char string[17];

// funcions de lectura
void llegir_transistor(void)
{
	struct bjt transistor;
	char pins[3];
	
	transistor = transistor_check();
	calculate_beta(&transistor);
	pins[transistor.base-1] = 'B';
	pins[transistor.colector-1] = 'C';
	pins[transistor.emisor-1] = 'E';
	
	memset(string, ' ', sizeof(string));
	sprintf(string, "1:%c 2:%c 3:%c %s", pins[0], pins[1], pins[2], transistor.tipus == NPN ? "NPN" : "PNP" );
	LCDWriteStringXY(0, 0, string);
	sprintf(string, "Beta:%4d       ", transistor.beta);
	LCDWriteStringXY(0, 1, string);
	
}

void llegir_condensador(void)
{

}

void llegir_resistencia(void)
{
	struct res r;
	uint8_t i;
	
    calcula_r(&r);
        
    sprintf(string,"R:%lu",r.valor);
    LCDWriteStringXY(0, 0, string);
    for(i=strlen(string); i < 16; i++) LCDData(' ');
}

void llegir_inductancia(void)
{

}

// funcions de calibració
void cal_condensador(void)
{

}

void cal_inductancia(void)
{

}

void cal_resistencia(void)
{

}

// punters de funcions per fer el codi del menu més llegible
void (*llegir_f_ptr[4])(void) = 
{
	&llegir_condensador,
	&llegir_inductancia,
	&llegir_resistencia,
	&llegir_transistor
};

void (*cal_f_ptr[4])(void) = 
{
	&cal_condensador,
	&cal_inductancia,
	&llegir_resistencia,
	NULL,
};


void menu1_print(uint8_t i)
{
    memset(string, ' ', sizeof(string));
	strcpy_P(string, (PGM_P)pgm_read_word(&(menu1_strings[i]))); // copiem el valor al string
	LCDWriteStringXY(0, 0, string);
}

void menu2_print(uint8_t i)
{
    memset(string, ' ', sizeof(string));
	strcpy_P(string, (PGM_P)pgm_read_word(&(menu2_strings[i]))); // copiem el valor al string
	LCDWriteStringXY(0, 1, string);
}

void menu(polsador_t tecla)
{

 if (tecla == P_MES)													
 {
	if ( menu_flag == MENU1 )
	{
		menu1++;
        menu1 = menu1 % 4; // tenim 4 opcions al menu1
		menu1_print(menu1);
	}
	else 
    {   
    	menu2++;
        menu2 = menu2 % 3;// tenim 3 opcions al menu2
    	menu2_print(menu2);
	}
 }

 else if (tecla == P_OK)
 {
 	 // si estem dins del menu1 anem al menu2
     if ( menu_flag == MENU1 )
     {
		menu_flag = MENU2;
		menu2_print(menu2);
     }

     // si estem dins del menu2 executem l'accio
     else 
     {   
         switch (menu2)
         {
         
         	// s'ha seleccionat llegir
			case LLEGIR:
				(*llegir_f_ptr[menu1])();
				break;
				
			// s'ha seleccionat calibrar
		    case CAL:
				(*cal_f_ptr[menu1])();
				break;

			case ENRERE:
				menu_flag = MENU1;
				menu2 = 0; // reiniciem el contador del menu2
				menu1_print(menu1);
				break;
				
         }
     }


 }

}

