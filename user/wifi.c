#include "wifi.h"

int wifi_set_flag=0;
char wifi_value[8]={0x05,0x05,0x80,0,0,0,'\r','\n'};

unsigned char wifi_cmd1[13]={'A','T','+','C','W','M','O','D','E','=','1','\r','\n'};
unsigned char wifi_cmd2[8]={'A','T','+','R','S','T','\r','\n'};
unsigned char wifi_cmd3_1[9]={'A','T','+','C','W','J','A','P','='};
unsigned char wifi_cmd3_2[11]={'"','G','X','D','S','2','0','1','9','"',','};
unsigned char wifi_cmd3_3[12]={'"','x','n','s','q','2','0','1','9','"','\r','\n'};
unsigned char wifi_cmd4[48]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P','"',',','"','1','9','2','.','1','6','8','.','4','3','.','2','3','1','"',',','8','0','8','0',',','9','0','5','0',',','2','\r','\n'};
unsigned char wifi_cmd5[10]={'A','T','+','C','I','F','S','R','\r','\n'};
unsigned char wifi_sendcmd[11]={'A','T','+','C','I','P','S','E','N','D','='};

void wifi_init()
{
	unsigned char i=0;
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	for(i=0;i<13;i++)
		SendBuffer3[i]=wifi_cmd1[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,13);
	while (wifi_set_flag==0);
	wifi_set_flag=0;
	delay_ms(100);
	
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	wifi_set_flag=0;
	for(i=0;i<8;i++)
		SendBuffer3[i]=wifi_cmd2[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,8);
	delay_ms(1000);
	
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	wifi_set_flag=0;
	for(i=0;i<9;i++)
		SendBuffer3[i]=wifi_cmd3_1[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,9);
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	for(i=0;i<11;i++)
		SendBuffer3[i]=wifi_cmd3_2[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,11);
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	for(i=0;i<12;i++)
		SendBuffer3[i]=wifi_cmd3_3[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,12);
	while (wifi_set_flag==0);
	wifi_set_flag=0;
	
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	wifi_set_flag=0;
	for(i=0;i<48;i++)
		SendBuffer3[i]=wifi_cmd4[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,48);
	while (wifi_set_flag==0);
	wifi_set_flag=0;
	delay_ms(100);
	
/*	for(i=0;i<10;i++)
		SendBuffer3[i]=wifi_cmd5[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,10);
	delay_ms(500);*/
}

void wifi_send(unsigned char*send_buf,int n)
{
	unsigned char i=0;
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	for(i=0;i<11;i++)
		SendBuffer3[i]=wifi_sendcmd[i];
	if(0<n&&n<10)
	{
		SendBuffer3[11]=0x30+n;
		SendBuffer3[12]='\r';
		SendBuffer3[13]='\n';
		Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,14);
	}
	else if(n>=10&&n<30)
	{
		SendBuffer3[11]=0x30+n/10;
		SendBuffer3[12]=0x30+n%10;
		SendBuffer3[13]='\r';
		SendBuffer3[14]='\n';
		Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,15);
	}
	while (DMA_GetCurrDataCounter(DMA1_Channel2));
	for(i=0;i<n;i++)
		SendBuffer3[i]=send_buf[i];
	Dma_Send_Enable(DMA1_Channel2,(u32)SendBuffer3,n);
}

