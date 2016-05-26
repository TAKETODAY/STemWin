#include <string.h>

#include "stm32f10x_fsmc.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "croutine.h"
//#include "semphr.h"
//#include "portmacro.h"

//#include "g_value.h"
#include "lcd.h"

#define Bank1_LCD_D    ((u32)0x6C000800)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x6C000000)	//Disp Reg ADDR

static void delay_100ms(void);
static void FSMC_init(void);

static vu8 need_Refurbish_LCD = 0;

void LCD_WR_REG(u16 regval)
{ 
	*(__IO u16 *) (Bank1_LCD_C) = regval; 
}

void LCD_WR_DATA(u16 data)
{										    	   
	*(__IO u16 *) (Bank1_LCD_D) = data;		 
}

//��LCD����
//����ֵ:������ֵ
static u16 LCD_RD_DATA(void)
{										    	   
	return *(__IO u16 *) (Bank1_LCD_D);		 
}

/*******************************************************************************
* ������	: WriteComm
* ����  	: д����
* ����  	: CMD:����
* ����ֵ	: ��
*******************************************************************************/
static void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}

/*******************************************************************************
* ������	: WriteData
* ����  	: д����
* ����  	: tem_data:����
* ����ֵ	: ��
*******************************************************************************/
static void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

static void FSMC_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.0 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.0	

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);//ʹ��FSMCʱ��(AHB)

	RCC_APB2PeriphClockCmd//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��
	(
		RCC_APB2Periph_GPIOB|
		RCC_APB2Periph_GPIOD|
		RCC_APB2Periph_GPIOE|
		RCC_APB2Periph_GPIOG|
		RCC_APB2Periph_AFIO
		,
		ENABLE
	);

	//PB0������������⡿
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|
								  GPIO_Pin_1|
								  GPIO_Pin_4|
								  GPIO_Pin_5|
								  GPIO_Pin_8|
								  GPIO_Pin_9|
								  GPIO_Pin_10|
								  GPIO_Pin_14|
								  GPIO_Pin_15;		

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|
								  GPIO_Pin_8|
								  GPIO_Pin_9|
								  GPIO_Pin_10|
								  GPIO_Pin_11|
								  GPIO_Pin_12|
								  GPIO_Pin_13|
								  GPIO_Pin_14|
								  GPIO_Pin_15; 

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//PORTG12��A0��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
	readWriteTiming.FSMC_AddressSetupTime = 0x01;//0x02;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 //���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0X02;//0x02;//���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;//ģʽA 

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 		 
}

//5310Һ����ʼ��
void LCD_init(void)
{
	FSMC_init();

	delay_100ms();

	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0xFE);
	
	LCD_WR_REG(0xEE);
	LCD_WR_DATA(0xDE);
	LCD_WR_DATA(0x21);
	
	LCD_WR_REG(0xF1);
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xDF);
	LCD_WR_DATA(0x10);
	
	//VCOMvoltage//
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x8F);	  //5f
	
	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE2);
	LCD_WR_DATA(0xE2);
	LCD_WR_DATA(0xE2);
	LCD_WR_REG(0xBF);
	LCD_WR_DATA(0xAA);
	
	LCD_WR_REG(0xB0);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x80);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x96);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x96);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA1);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB5);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5E);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x70);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x90);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xEB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xB8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xBA);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC2);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC3);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x26);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x62);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x95);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE6);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x65);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x16);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x46);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x52);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x16);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x22);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x52);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x8B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE2);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x61);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x79);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x97);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE3);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x62);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x78);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x97);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC7);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE4);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x74);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x93);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x5D);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x74);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x84);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x93);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xB3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC4);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xCD);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD3);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xDC);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xF3);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE6);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE7);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x76);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x67);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x23); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x87);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x77);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x88);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xBB);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x99);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x66);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x55);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0xAA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0x00);
	LCD_WR_DATA(0xAA);
	
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xF3);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xF9);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);	
	
	LCD_WR_REG(0x11);
	delay_100ms(); 
	LCD_WR_REG(0x29);
	LCD_WR_REG(0x35);
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0x51);
	LCD_WR_DATA(0xFF);
	LCD_WR_REG(0x53);
	LCD_WR_DATA(0x2C);
	LCD_WR_REG(0x55);
	LCD_WR_DATA(0x82);
	LCD_WR_REG(0x2c);

	{
		u16 regval;
	
		regval|=(1<<7)|(0<<6)|(1<<5);

		LCD_WR_REG(0X36);
		LCD_WR_DATA(regval);

		//����X����
		LCD_WR_REG(0X2A); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);

		LCD_WR_DATA((480-1)>>8);LCD_WR_DATA((480-1)&0XFF);

		LCD_WR_REG(0X2B); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((320-1)>>8);LCD_WR_DATA((320-1)&0XFF);  
	}

//	LCD_Fill(0,0,479,31,WHITE);	 //��Ϣ����ɫΪ��ɫ
//   	LCD_Fill(0,31,479,319,BLACK);//�����������ɫ��ɫ
	
	//��������
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	
//	//���ù��λ��
//	LCD_WR_REG(0x2A); 
//	LCD_WR_DATA(0); 
//	LCD_WR_DATA(0);	 
//	LCD_WR_REG(0x2B); 
//	LCD_WR_DATA(0); 
//	LCD_WR_DATA(0);
//	 
//	//��ʼд��GRAM	 
//	LCD_WR_REG(0X2C); 
}

u16 *GDRAM = (u16*)(0x68000000);

/*
	W = 480
	H = 320
	
	RGB565ģʽ
	
	һ����30720�ֽ� 300K
	
	һ��960�ֽ�
*/

void LCD_Clear(u16 color)
{
	u32 i;
	
	for(i=0;i<153600;i++)
	{
		GDRAM[i] = color;
	}
}

void LCD_DrawPoint(int x,int y,u16 color)
{
//	if(x >= 480)return(0);
//	if(y >= 320)return(0);
    
    //���ù��λ��
    WriteComm(0x2A); 
    WriteData(x>>8); 
    WriteData(x&0XFF);
    
    WriteComm(0x2B); 
    WriteData(y>>8); 
    WriteData(y&0XFF);
    
    LCD_WR_REG(0X2C);//��ʼд��GRAM
    
	LCD_WR_DATA(color); 
}

u16 LCD_ReadPoint(int x, int y)
{
 	u16 r=0,g=0,b=0;
    
	if(x >= 480)return(0);
	if(y >= 320)return(0);
    
    //���ù��λ��
    WriteComm(0x2A); 
    WriteData(x>>8); 
    WriteData(x&0XFF);
    
    WriteComm(0x2B); 
    WriteData(y>>8); 
    WriteData(y&0XFF);
    
	LCD_WR_REG(0X2E);//9341/6804/3510 ���Ͷ�GRAMָ��
   
	if(LCD_RD_DATA()) r=0;							//dummy Read	  
 	r = LCD_RD_DATA();  		  						//ʵ��������ɫ
    b = LCD_RD_DATA();
    g = r&0XFF;//����9341/5310,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
    g<<=8;
        
    return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310��Ҫ��ʽת��һ��
}

/************************************************************************************
					   			  ��ɫ��亯��
��ָ����������䵥����ɫ			 
(x1,y1),(x2,y2):�����ζԽ�����,�����СΪ:(x1-x2+1)*(y2-y2+1)   
color:Ҫ������ɫ
*************************************************************************************/
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u32 i,j,h,l;
	u32 rel;
	
	if(sy>ey)return;
	if(sx>ex)return;
	
	h = ey-sy+1;//��
	l = ex-sx+1;//��
	h += sy;
	
	for(i=sy;i<h;i++)
	{
		rel = (i*480)+sx;
		for(j=0;j<l;j++)
		{
			GDRAM[rel+j] = color;	
		}
	}
	
	need_Refurbish_LCD = 1;
}	

static void delay_100ms(void)
{
	u32 i=800000;
	
	while(i--);	
}
