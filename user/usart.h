#ifndef _USART_H
#define _USART_H
#include "head_file.h"
void Init_Usart_Config(USART_TypeDef *Usartx, u32 Bound);
void Init_Usart_Nvic_Config(u32 Usartx_Irqn);
void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Channel_TypeDef* DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Channelx_Irqn);
void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Channel_TypeDef* DMA_Chx,u32 Rx_Buf, u16 Rx_Size);
void Dma_Send_Enable(DMA_Channel_TypeDef* DMA_Chx,u32 Tx_Buf,u16 ndtr);
void DMA1_Channel4_IRQHandler(void);
void USART1_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void USART2_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void USART3_IRQHandler(void);

/**********1***********/
extern int readbuf1[30];
extern u16 rx_len1;
extern u8 SendBuffer1[30];
extern u8 ReceiveBuffer1[30];
extern u16 BufferLen1;
extern u8 rx_flag1;

/**********2***********/
extern int readbuf2[30];
extern u16 rx_len2;
extern u8 SendBuffer2[30];
extern u8 ReceiveBuffer2[30];
extern u16 BufferLen2;
extern u8 rx_flag2;

/**********3***********/
extern char readbuf3[256];
extern u16 rx_len3;
extern u8 SendBuffer3[30];
extern u8 ReceiveBuffer3[256];
extern u16 BufferLen3;
extern u8 rx_flag3;


#endif
