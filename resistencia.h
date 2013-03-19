
#ifndef _RESISTENCIA_H
#define _RESISTENCIA_H

// definicions de pins i ports

#define RM1_PORT 	PORTC
#define RM1_DDR 	(*(&RM1_PORT-1))
#define RM1			5

#define RM2_PORT	PORTC
#define RM2_DDR     (*(&RM2_PORT-1))
#define RM2			6

#define RM3_PORT	PORTD
#define RM3_DDR		(*(&RM3_PORT-1))
#define RM3			0

#define RM4_PORT	PORTD
#define RM4_DDR		(*(&RM4_PORT-1))
#define RM4			1

#define RVIN_PORT	PORTC
#define RVIN_DDR    (*(&RVIN_PORT-1))
#define RVIN		4


// definicio d'estructures de dades
struct res
{
  uint32_t valor;
};

// funcions
uint8_t calcula_r (struct res* r);
void r_init(void);

#endif
