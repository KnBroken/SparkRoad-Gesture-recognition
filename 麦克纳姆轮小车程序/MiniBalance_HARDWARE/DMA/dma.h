#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x_dma.h"
#include "sys.h"


void MYDMA_Init(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);

void MYDMA_Init2(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable2(DMA_Channel_TypeDef*DMA_CHx);

void MYDMA_Init3(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable3(DMA_Channel_TypeDef*DMA_CHx);

void DMA_printf(const char *format,...);
u32 USART_SendBuffer(const char* buffer, u32 length);

#endif



