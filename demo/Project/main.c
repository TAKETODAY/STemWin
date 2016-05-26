#include "sys.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_init.h"
#include "MainTask.h"
#include "touch.h"

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

void vTaskTaskKEY(void *pvParameters)
{
    while(1)
    {
        GUI_TOUCH_Exec();
//        tp_dev.scan(0);
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
