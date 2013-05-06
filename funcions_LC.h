/*************************************
 * Nom del arxiu: funcions_LC.c
 * Autor: Miguel Angel Borrego
 * Data: 07/05/2013
 * Compilador: WinAvr 20100110
 * Descripció: Definicions i prototips 
 *  de funcions
 **************************************/

#ifndef _FUNCIONS_LC_H
#define _FUNCIONS_LC_H

#define SWITCH_PORT		PORTD
#define SWITCH_DDR   	(*(&SWITCH_PORT-1))
#define SWITCH_INPUT 	(*(&SWITCH_PORT-2))
#define SWITCH_PIN		4


#define CREF 0.001 // valor en uF
#define LREF 100.0 // valor en uH


#define L 0
#define C 1

// prototips de les funcions
void freq_counter_init(void);
float freq_counter_read(void);
void LC_init(void);
void calibra_LC(void);
float calcula_C(void);
float calcula_L(void);
uint8_t llegir_switch(void);

#endif
