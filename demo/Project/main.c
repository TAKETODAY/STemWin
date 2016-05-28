#include "sys.h"

#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "bsp_touch.h"
#include "bsp_init.h"

#include "MainTask.h"

static void vTaskGUI(void *pvParameters);
void vTaskTaskLED0(void *pvParameters);
void vTaskTaskLED1(void *pvParameters);
void vTaskTaskKEY(void *pvParameters);
    
int main(void)
{
    bsp_Init();//Ӳ����ʼ��
    printf("Ӳ����ʼ�����\r\n");
    
    xTaskCreate(    vTaskTaskKEY,   /* ������  */
                    "vTaskTaskKEY",     /* ������    */
                    configMINIMAL_STACK_SIZE,               /* stack��С����λword��Ҳ����4�ֽ� */
                    NULL,              /* �������  */
                    3,                 /* �������ȼ�*/
                    NULL );            /* ������  */    
    
    xTaskCreate(    vTaskTaskLED0,   /* ������  */
                    "vTaskTaskLED0",     /* ������    */
                    configMINIMAL_STACK_SIZE,               /* stack��С����λword��Ҳ����4�ֽ� */
                    NULL,              /* �������  */
                    2,                 /* �������ȼ�*/
                    NULL );            /* ������  */

    xTaskCreate(    vTaskTaskLED1,   /* ������  */
                    "vTaskTaskLED1",     /* ������    */
                    configMINIMAL_STACK_SIZE,               /* stack��С����λword��Ҳ����4�ֽ� */
                    NULL,              /* �������  */
                    2,                 /* �������ȼ�*/
                    NULL );            /* ������  */
    
	xTaskCreate(    vTaskGUI,          /* ������  */
                    "vTaskGUI",        /* ������    */
                    configMINIMAL_STACK_SIZE*30,               /* stack��С����λword��Ҳ����4�ֽ� */
                    NULL,              /* �������  */
                    1,                 /* �������ȼ�*/
                    NULL );            /* ������  */
                    
    vTaskStartScheduler();
    
    while(1);
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskGUI
*	����˵��: emWin����
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 1   (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
static void vTaskGUI(void *pvParameters)
{
	while (1) 
	{
		MainTask();
	}
}

void vTaskTaskLED0(void *pvParameters)
{
    while(1)
    {
        LED0 = !LED0;
        vTaskDelay(500);
    }
}

void vTaskTaskLED1(void *pvParameters)
{
    while(1)
    {
        LED1 = !LED1;
        vTaskDelay(250);
    }
}

GUI_PID_STATE State;
void vTaskTaskKEY(void *pvParameters)
{
    static u32 cont = 0;
    
    while(1)
    {
        tp_dev.scan(0);//ɨ������1ms
        if(cont == 10) bsp_KeyScan();//ɨ������10ms
                
		if(KEY_1_DOWN == bsp_GetKey())	//KEY_1_DOWN����,��ִ��У׼����
		{
			LCD_Clear(WHITE);//����
		    TP_Adjust();  //��ĻУ׼ 
			TP_Save_Adjdata();	 
		}
        
        if(tp_dev.sta&TP_PRES_DOWN) //������������
		{	
		 	if((tp_dev.x < lcddev.width) && (tp_dev.y < lcddev.height)) //���Ʒ�Χ
			{	
                State.x = tp_dev.x;
                State.y = tp_dev.y;
                State.Pressed = 1;
                GUI_PID_StoreState(&State); 
			}
		}
        else //�������ͷ�
        {
            State.x = -1;
            State.y = -1;
            State.Pressed = 0;
            GUI_PID_StoreState(&State);
        }

        cont++;
        if(cont > 10) cont = 0;
        
        vTaskDelay(1);
    }
}

//ջ����ص�����
//void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
//{
//	printf("�̣߳�%s ��ջ���\r\n",pcTaskName);
//}

void HardFault_Handler(void)
{
	printf("HardFault_Handler!!!Ӳ������!!!\r\n");
}
