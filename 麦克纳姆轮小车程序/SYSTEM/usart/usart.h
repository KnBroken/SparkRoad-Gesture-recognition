#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	
void usart1_send(u8 data);
void uart_init(u32 bound);

void usart2_send(u8 data);
void uart2_init(u32 bound);
void Openmv_Decode(void);
u8 click_RC (void);
extern u8 ok;
extern char CircleX,CircleY,RectsX,RectsY,YellowX,YellowY,WhiteX,WhiteY,BlueX,BlueY,update,blueupdate,paperupdate;
extern u8 a;
#endif	   
















