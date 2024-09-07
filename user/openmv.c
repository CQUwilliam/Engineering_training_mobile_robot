#include "openmv.h"

int openmv_cmd=0;
int QR_value[8]={0x05,0x05,0x25,0,0,0,'\r','\n'},qr_flag=0;
int CMR_value[13]={0x05,0x05,0x3a,0,0,0,0,0,0,0,0,'\r','\n'},cmr_flag=0;
unsigned char QR_read_cmd[2]={'Q','R'};
unsigned char Camera_on_cmd[5]={'C','M','R','O','N'};
unsigned char Camera_off_cmd[6]={'C','M','R','O','F','F'};

void QR_read()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel4));
	for(i=0;i<2;i++)
		SendBuffer1[i]=QR_read_cmd[i];
	Dma_Send_Enable(DMA1_Channel4,(u32)SendBuffer1,2);
}

void QR_send()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel7));
	for(i=0;i<8;i++)
		SendBuffer2[i]=QR_value[i];
	Dma_Send_Enable(DMA1_Channel7,(u32)SendBuffer2,8);
}

void Camera_on()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel4));
	for(i=0;i<5;i++)
		SendBuffer1[i]=Camera_on_cmd[i];
	Dma_Send_Enable(DMA1_Channel4,(u32)SendBuffer1,5);
}

void Camera_off()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel4));
	for(i=0;i<6;i++)
		SendBuffer1[i]=Camera_off_cmd[i];
	Dma_Send_Enable(DMA1_Channel4,(u32)SendBuffer1,6);
}

void CMR_send()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel7));
	for(i=0;i<13;i++)
		SendBuffer2[i]=CMR_value[i];
	Dma_Send_Enable(DMA1_Channel7,(u32)SendBuffer2,13);
}

void wifi_back()
{
	unsigned char i=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel7));
	for(i=0;i<8;i++)
		SendBuffer2[i]=wifi_value[i];
	Dma_Send_Enable(DMA1_Channel7,(u32)SendBuffer2,8);
}
