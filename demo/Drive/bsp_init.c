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

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "bsp_24cxx.h" 
#include "bsp_SRAM.h"
#include "bsp_lcd.h"
#include "bsp_touch.h"

#include "bsp_init.h"

/*******************************************************************************
* ������	: NVIC_Configuration
* ����  	: �ж�����
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //ֻ��0-15����ռ���ȼ�
}

/*******************************************************************************
* ������	: HW_Init
* ����  	: Ӳ����ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void bsp_Init(void)
{
	NVIC_Configuration(); //�жϷ���
    
    bsp_InitUsart1(9600); //��ʼ������
    bsp_InitSRAM();	      //��ʼ��SRAM	
    bsp_InitLED();        //��ʼ��LED
    bsp_InitKey();	      //��ʼ������
	bsp_InitLCD();	      //��ʼ��LCD
    TP_Init();            //��ʼ������
//	DMA_init();		      //��ʼ��DMA   

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//ʹ��STemWin������ʹ��CRC
}
