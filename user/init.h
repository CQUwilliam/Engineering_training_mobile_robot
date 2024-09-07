#ifndef _INIT_H
#define _INIT_H
#include "head_file.h"

void uart_init(void);
void tim_init(int arr,int psc);
void TIM2_IRQHandler(void);
void init_all(void);

#endif
