#include "init.h"
//初始化、定时器中断函数
void tim_init(int arr,int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  TIM_TimeBaseStructure.TIM_Period = arr;
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void uart_init()
{
	GPIO_InitTypeDef     GPIO_InitStruct;	
	/**************USART1**********************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	Init_Usart_Nvic_Config(USART1_IRQn);
	Init_Usart_Config(USART1, 115200);
	
	Init_Dma_Tx_Config(USART1, (u32)(&USART1->DR), DMA1_Channel4, (u32)SendBuffer1, BufferLen1, DMA1_Channel4_IRQn);
	Init_Dma_Rx_Config(USART1, (u32)(&USART1->DR), DMA1_Channel5, (u32)ReceiveBuffer1, BufferLen1);
	
/**************USART2**********************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	Init_Usart_Nvic_Config(USART2_IRQn);
	Init_Usart_Config(USART2, 115200);
	
	Init_Dma_Tx_Config(USART2, (u32)(&USART2->DR), DMA1_Channel7, (u32)SendBuffer2, BufferLen2, DMA1_Channel7_IRQn);
	Init_Dma_Rx_Config(USART2, (u32)(&USART2->DR), DMA1_Channel6, (u32)ReceiveBuffer2, BufferLen2);
/***************USART3*********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	Init_Usart_Nvic_Config(USART3_IRQn);
	Init_Usart_Config(USART3, 115200);
	
	Init_Dma_Tx_Config(USART3, (u32)(&USART3->DR), DMA1_Channel2, (u32)SendBuffer3, BufferLen3, DMA1_Channel2_IRQn);
	Init_Dma_Rx_Config(USART3, (u32)(&USART3->DR), DMA1_Channel3, (u32)ReceiveBuffer3, BufferLen3);

	USART_Cmd(USART1, ENABLE);
  USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);

}

void init_all()
{
	delay_init();
	uart_init();
	tim_init(2000,720);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
			
			if(openmv_cmd==0x25)
				QR_read();
			else if(openmv_cmd==0x3a)
			{
				Camera_on();
				cmr_flag=1;
			}
			else if(openmv_cmd==0x4a)
			{
				Camera_off();
				cmr_flag=0;
			}
			else if(openmv_cmd==0x80)
				wifi_init();
			openmv_cmd=0;
			
			if(qr_flag==1)
			{
				QR_send();
				qr_flag=0;
			}
			if(cmr_flag==1)
			{
				CMR_send();
			}
			if(wifi_value[3]!=0&&wifi_value[4]!=0&&wifi_value[5]!=0)
			{
				wifi_back();
				wifi_value[3]=0;
				wifi_value[4]=0;
				wifi_value[5]=0;
			}
    }
}

