#include "usart.h"

int readbuf1[30]={0};
u16 rx_len1 = 0;                          
u8 SendBuffer1[30]={0};                
u8 ReceiveBuffer1[30]={0};               
u16 BufferLen1 = 30;                      
u8 rx_flag1 = 0; 

int readbuf2[30]={0};
u16 rx_len2 = 0;                          
u8 SendBuffer2[30]={0};                
u8 ReceiveBuffer2[30]={0};               
u16 BufferLen2 = 30;                      
u8 rx_flag2 = 0;

char readbuf3[256]={0};
u16 rx_len3 = 0;                          
u8 SendBuffer3[30]={0};                
u8 ReceiveBuffer3[256]={0};               
u16 BufferLen3 = 256;                      
u8 rx_flag3 = 0;

void Init_Usart_Config(USART_TypeDef *Usartx, u32 Bound)
{    
	USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = Bound;                                       
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      
  USART_InitStructure.USART_StopBits = USART_StopBits_1;                        
  USART_InitStructure.USART_Parity = USART_Parity_No;                              
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                      
  USART_Init(Usartx,&USART_InitStructure);                                         
	USART_ITConfig(Usartx, USART_IT_IDLE, ENABLE);
  USART_Cmd(Usartx, ENABLE);          
}

void Init_Usart_Nvic_Config(u32 Usartx_Irqn)
{
		NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Usartx_Irqn;           
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;     
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
    NVIC_Init(&NVIC_InitStructure);     
		NVIC_EnableIRQ(Usartx_Irqn);
}

void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Channel_TypeDef* DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Channelx_Irqn)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_DeInit(DMA_Chx);                                                                                       
                             
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                          
    DMA_InitStructure.DMA_MemoryBaseAddr = Tx_Buf;                             
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                   
    DMA_InitStructure.DMA_BufferSize = 0;                                
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                 
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;          
    DMA_Init(DMA_Chx,&DMA_InitStructure);                               
   
    NVIC_InitStructure.NVIC_IRQChannel = DMA_Channelx_Irqn;                     
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                         
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             
    NVIC_Init(&NVIC_InitStructure);    
		
		NVIC_SetPriority(DMA_Channelx_Irqn,0x0C);
		NVIC_EnableIRQ(DMA_Channelx_Irqn);
    DMA_ITConfig(DMA_Chx,DMA_IT_TC,ENABLE);
    USART_DMACmd(Usartx,USART_DMAReq_Tx,ENABLE);  
		       		
		DMA_Cmd(DMA_Chx, DISABLE);
}

void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Channel_TypeDef* DMA_Chx,u32 Rx_Buf, u16 Rx_Size)
{
	DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA_Chx);                                                                
                          
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                          
    DMA_InitStructure.DMA_MemoryBaseAddr = Rx_Buf;                        
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                
    DMA_InitStructure.DMA_BufferSize = Rx_Size;                              
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;      
    DMA_Init(DMA_Chx, &DMA_InitStructure);  
	
    USART_DMACmd(Usartx,USART_DMAReq_Rx,ENABLE);  		
    DMA_Cmd(DMA_Chx, ENABLE);                                            
}

void Dma_Send_Enable(DMA_Channel_TypeDef* DMA_Chx,u32 Tx_Buf,u16 ndtr)
{
		while (DMA_GetCurrDataCounter(DMA_Chx));
		DMA_Cmd(DMA_Chx, DISABLE);
    DMA_SetCurrDataCounter(DMA_Chx,ndtr);              
    DMA_Cmd(DMA_Chx, ENABLE);                         
}

void DMA1_Channel4_IRQHandler(void)  
{
    if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET)      
    {            
        DMA_Cmd(DMA1_Channel4,DISABLE);                                
        DMA_ClearITPendingBit(DMA1_IT_TC4);  
				DMA_Cmd(DMA1_Channel4,ENABLE);
    }  
} 

void USART1_IRQHandler(void)  
{  
    u16 i; 
    if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)           //读取中断位标志位
    {
				USART_ReceiveData(USART1);//接收1串口数据
				USART_ClearITPendingBit(USART1,USART_IT_IDLE);  //清除中断标志位
        DMA_Cmd(DMA1_Channel5, DISABLE);                 //禁用DMA                                                        

        rx_len1 = BufferLen1 - DMA_GetCurrDataCounter(DMA1_Channel5); //计算数据长度
				
				i = USART1->SR;        //读取缓存区起始位                                     
        i = USART1->DR;
			
        if(rx_len1 !=0)                                  //如果数据长度不为0，则读取标志位为1，开始读取此数据           
        { 
            rx_flag1 = 1;                                       
            for(i=0;i<rx_len1;i++)
            {
                readbuf1[i]= ReceiveBuffer1[i]; //将缓存区数据读取到自己定义的数组
            }
            for(i=0;i<rx_len1;i++)                                   
            {
                ReceiveBuffer1[i]=0;//清空缓存区
            }
        }  
				for(i=0;i<rx_len1;i++)
				{
					if(readbuf1[i]==0x05&&readbuf1[i+1]==0x05)//判断数据，当数据为0x05时，则识别帧头
					{
						if(readbuf1[i+2]==0x25)
						{
							QR_value[3]=readbuf1[i+3];
							QR_value[4]=readbuf1[i+4];
							QR_value[5]=readbuf1[i+5];
							qr_flag=1;
						}
						else if(readbuf1[i+2]==0x5a)
						{
							if(readbuf1[i+4]=='\r'&&readbuf1[i+5]=='\n')
							{
								CMR_value[3]=0;
								CMR_value[4]=readbuf1[i+3]-0x30;
							}
							else if(readbuf1[i+5]=='\r'&&readbuf1[i+6]=='\n')
							{
								CMR_value[3]=0;
								CMR_value[4]=(readbuf1[i+3]-0x30)*10+readbuf1[i+4]-0x30;
							}
							else if(readbuf1[i+6]=='\r'&&readbuf1[i+7]=='\n')
							{
								CMR_value[3]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))/256;
								CMR_value[4]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))%256;
							}
						}
						else if(readbuf1[i+2]==0x5b)
						{
							if(readbuf1[i+4]=='\r'&&readbuf1[i+5]=='\n')
							{
								CMR_value[5]=0;
								CMR_value[6]=readbuf1[i+3]-0x30;
							}
							else if(readbuf1[i+5]=='\r'&&readbuf1[i+6]=='\n')
							{
								CMR_value[5]=0;
								CMR_value[6]=(readbuf1[i+3]-0x30)*10+readbuf1[i+4]-0x30;
							}
							else if(readbuf1[i+6]=='\r'&&readbuf1[i+7]=='\n')
							{
								CMR_value[5]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))/256;
								CMR_value[6]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))%256;
							}
						}
						else if(readbuf1[i+2]==0x5c)
						{
							if(readbuf1[i+4]=='\r'&&readbuf1[i+5]=='\n')
							{
								CMR_value[7]=0;
								CMR_value[8]=readbuf1[i+3]-0x30;
							}
							else if(readbuf1[i+5]=='\r'&&readbuf1[i+6]=='\n')
							{
								CMR_value[7]=0;
								CMR_value[8]=(readbuf1[i+3]-0x30)*10+readbuf1[i+4]-0x30;
							}
							else if(readbuf1[i+6]=='\r'&&readbuf1[i+7]=='\n')
							{
								CMR_value[7]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))/256;
								CMR_value[8]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))%256;
							}
						}
						else if(readbuf1[i+2]==0x5d)
						{
							if(readbuf1[i+6]=='\r'&&readbuf1[i+7]=='\n')
							{
								CMR_value[9]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))/256;
								CMR_value[10]=((readbuf1[i+3]-0x30)*100+(readbuf1[i+4]-0x30)*10+(readbuf1[i+5]-0x30))%256;
								cmr_flag=1;
							}
						}
					}
				}
				
        DMA_ClearFlag(DMA1_FLAG_TC5 | DMA1_FLAG_GL5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5); 
        DMA_SetCurrDataCounter(DMA1_Channel5, BufferLen1);            
        DMA_Cmd(DMA1_Channel5, ENABLE);                               

    }  
}

void DMA1_Channel7_IRQHandler(void)  
{
    if(DMA_GetITStatus(DMA1_IT_TC7)!=RESET)      
    {            
        DMA_Cmd(DMA1_Channel7,DISABLE);                                
        DMA_ClearITPendingBit(DMA1_IT_TC7);  
				DMA_Cmd(DMA1_Channel7,ENABLE);
    }  
} 

void USART2_IRQHandler(void)  
{  
    u16 i; 
    if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)            
    {
				USART_ReceiveData(USART2);
				USART_ClearITPendingBit(USART2,USART_IT_IDLE);  
        DMA_Cmd(DMA1_Channel6, DISABLE);                                                                         

        rx_len2 = BufferLen2 - DMA_GetCurrDataCounter(DMA1_Channel6); 
				
				i = USART2->SR;                                             
        i = USART2->DR;
			
				if(rx_len2 !=0)                                              
        { 
            rx_flag2 = 1;                                       
            for(i=0;i<rx_len2;i++)
            {
                readbuf2[i]= ReceiveBuffer2[i]; 
            }
            for(i=0;i<rx_len2;i++)                                   
            {
                ReceiveBuffer2[i]=0;
            }
        } 			
				for(i=0;i<rx_len2;i++)
				{
					if(readbuf2[i]==0x05&&readbuf2[i+1]==0x05)
						openmv_cmd=readbuf2[i+2];
				}
				
        DMA_ClearFlag(DMA1_FLAG_TC6 | DMA1_FLAG_GL6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6); 
        DMA_SetCurrDataCounter(DMA1_Channel6, BufferLen2);            
        DMA_Cmd(DMA1_Channel6, ENABLE);                               

    }  
}

void DMA1_Channel2_IRQHandler(void)  
{
    if(DMA_GetITStatus(DMA1_IT_TC2)!=RESET)      
    {           
        DMA_Cmd(DMA1_Channel2,DISABLE);                                
        DMA_ClearITPendingBit(DMA1_IT_TC2);
				DMA_Cmd(DMA1_Channel2,ENABLE);
    }  
} 

void USART3_IRQHandler(void)  
{  
    u16 i; 
    if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)            
    {
				USART_ReceiveData(USART3);
				USART_ClearITPendingBit(USART3,USART_IT_IDLE);  
        DMA_Cmd(DMA1_Channel3, DISABLE);                                                                         

        rx_len3 = BufferLen3 - DMA_GetCurrDataCounter(DMA1_Channel3); 
				
				i = USART3->SR;                                             
        i = USART3->DR;
			
       if(rx_len3 !=0)                                              
        { 
            rx_flag3 = 1;                                       
            for(i=0;i<rx_len3;i++)
							readbuf3[i]=ReceiveBuffer3[i];
            for(i=0;i<rx_len3;i++)                                   
            {
                ReceiveBuffer3[i]=0;
            }
        }  
				for(i=0;i<rx_len3;i++)
				{
					if(readbuf3[i]=='O'&&readbuf3[i+1]=='K'&&readbuf3[i+2]=='\r'&&readbuf3[i+3]=='\n')
					{wifi_set_flag=1;}
					if(readbuf3[i]=='+'&&readbuf3[i+1]=='I'&&readbuf3[i+2]=='P'&&readbuf3[i+3]=='D'&&readbuf3[i+4]==','&&readbuf3[i+5]=='5'&&readbuf3[i+6]==':')
					{
						wifi_value[3]=readbuf3[i+7];
						wifi_value[4]=readbuf3[i+8];
						wifi_value[5]=readbuf3[i+9];
					}
				}
				
				
        DMA_ClearFlag(DMA1_FLAG_TC3 | DMA1_FLAG_GL3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3); 
        DMA_SetCurrDataCounter(DMA1_Channel3, BufferLen3);            
        DMA_Cmd(DMA1_Channel3, ENABLE);                               

    }  
}
