#include <stdio.h>

#include "stm32f10x_dma.h"

static DMA_InitTypeDef DMA_InitStructure;

extern u16 *GDRAM;
	  
void DMA_init(void)
{												    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//ʹ��DMA����
	
    DMA_DeInit(DMA2_Channel5);//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&GDRAM[0];//(u32)0X68000000;//SRAM
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0X6C000800;//LCD
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//SRAM->LCD
	DMA_InitStructure.DMA_BufferSize = 38400;//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	  	
}

//����һ��LCD DMA����
//x:��ʵ�����ַ���(0~240)
void LCD_DMA_Enable(u16 x)
{	
	u16 len;
	len = 38400;
	
 	DMA_Cmd(DMA2_Channel5,DISABLE);				   //�ر�DMA����
	DMA_ClearFlag(DMA2_FLAG_TC5);				   //����ϴεĴ�����ɱ�� 
	
	DMA2_Channel5->CNDTR=len;					   //��������ݳ���
	DMA2_Channel5->CPAR=(u32)&GDRAM[0];			   //DMA2,�ı�洢����ַ	
	
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //����DMA����
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//����	
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //����ϴεĴ�����ɱ�� 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[38400];		   //DMA2,�ı�洢����ַ
	DMA_Cmd(DMA2_Channel5,DISABLE);				   //�ر�DMA����
	DMA2_Channel5->CNDTR=len;					   //��������ݳ���
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //����DMA����
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//����
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //����ϴεĴ�����ɱ�� 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[76800];		   //DMA2,�ı�洢����ַ
	DMA_Cmd(DMA2_Channel5,DISABLE);			       //�ر�DMA����
	DMA2_Channel5->CNDTR=len;					   //��������ݳ���
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //����DMA����
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//����
	
	///////////////////////////////////////////////////////////////////////////
 	DMA_ClearFlag(DMA2_FLAG_TC5);				   //����ϴεĴ�����ɱ�� 	
	DMA2_Channel5->CPAR=(u32)&GDRAM[115200];	   //DMA2,�ı�洢����ַ
	DMA_Cmd(DMA2_Channel5,DISABLE);				   //�ر�DMA����
	DMA2_Channel5->CNDTR=len;
	DMA_Cmd(DMA2_Channel5, ENABLE);				   //����DMA����
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==RESET);//����
}
 
