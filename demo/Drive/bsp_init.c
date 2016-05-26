/**
  ******************************************************************************
  * @file    bsp_init.c
  * @author  solo
  * @version V0.0.1
  * @date    2016-02-16
  * @brief   Ӳ������
  * @verbatim 
  *   
  * @note    
  *
  * @endverbatim  
  *                                  
  */

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "sys.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "lcd.h"
#include "touch.h" 
#include "delay.h"
#include "SRAM.h" 

#include "bsp_init.h"

/*******************************************************************************
* ������	: NVIC_Configuration
* ����  	: �ж�����
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void NVIC_Configuration(void)
{
//    SCB->VTOR = FLASH_BASE | 0x8000;
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//ֻ��0-15����ռ���ȼ�
}

/*******************************************************************************
* ������	: HW_Init
* ����  	: Ӳ����ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void bsp_Init(void)
{
	NVIC_Configuration();//�жϷ���
    
//    delay_init();	    	 //��ʱ������ʼ��
    uart_init(9600);
    FSMC_SRAM_Init();	 //��ʼ��SRAM	
    LED_Init();
	LCD_init();			 //LCD��ʼ��
    tp_dev.init();
//	DMA_init();			 //��ʼ��DMA   

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}
