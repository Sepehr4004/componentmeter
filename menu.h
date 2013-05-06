#ifndef _MENU_H_
#define _MENU_H_


// enumerem polsador
typedef enum 
{
	P_MES,
	P_OK
} polsador_t;

// funcions
void menu_init(void);

void menu(polsador_t tecla);

#endif
