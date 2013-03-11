
#ifndef _TRANSISTOR_H
#define _TRANSISTOR_H

// definicions de pins i ports

#define TP1_PORT    PORTC
#define TP1_DDR     (*(&TP1_PORT-1))
#define TP1         1

#define TP2_PORT    PORTC
#define TP2_DDR     (*(&TP2_PORT-1))
#define TP2         2

#define TP3_PORT    PORTC
#define TP3_DDR     (*(&TP3_PORT-1))
#define TP3         3

// 
#define RTL1_PORT   PORTC
#define RTL1_DDR    (*(&RTL1_PORT-1))
#define RTL1        0

#define RTL2_PORT   PORTB
#define RTL2_DDR    (*(&RTL2_PORT-1))
#define RTL2        4

#define RTL3_PORT   PORTB
#define RTL3_DDR    (*(&RTL3_PORT-1))
#define RTL3        2

//
#define RTH1_PORT   PORTB
#define RTH1_DDR    (*(&RTH1_PORT-1))
#define RTH1        5

#define RTH2_PORT   PORTB
#define RTH2_DDR    (*(&RTH2_PORT-1))
#define RTH2        3

#define RTH3_PORT   PORTB
#define RTH3_DDR    (*(&RTH3_PORT-1))
#define RTH3        1

//
#define PNP     1
#define NPN     0


// definicio d'estructures de dades
struct bjt
{
  uint8_t base;
  uint8_t colector;
  uint8_t emisor;
  uint16_t beta;

  uint8_t tipus; // PNP o NPN
};

// funcions
struct bjt transistor_check(void);
uint16_t check (uint8_t high, uint8_t low);
uint8_t calculate_beta (struct bjt* tr);
void tr_init(void);

#endif
