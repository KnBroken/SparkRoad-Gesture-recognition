#ifndef __USRATX_H
#define __USRATX_H 
#include "sys.h"	  	
void send_data(void);
void usart3_send(u8 data);
void uart3_init(u32 bound);
//int USART3_IRQHandler(void);

void Read_Data(u16 *Data);
//extern u16 USART_RX_STA[3],Num;
extern char receivestate;
#define UART_RX_LEN     128  
extern uint8_t Uart_Rx[UART_RX_LEN]; 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
//extern	u8 Roll_High8,Roll_Low8,Roll_decimal;
//extern	u8 Encoder_A_High8,Encoder_A_Low8,Encoder_B_High8,Encoder_B_Low8;
//extern	u8 Encoder_C_High8,Encoder_C_Low8,Encoder_D_High8,Encoder_D_Low8;
//extern	int Roll_median,Encoder_A_median,Encoder_B_median,Encoder_C_median,Encoder_D_median;
//extern	short Temp_GZ;
//extern	u8 GZ_Low8,GZ_High8,Encoder_Direction,check_bit;
#endif

