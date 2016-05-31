/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		    1                               //1����ռʽ���ȣ�0Эͬ����
#define configUSE_IDLE_HOOK			    1                               //���лص�����ʹ�ܿ��ƣ�vApplicationIdleHook
#define configUSE_TICK_HOOK			    0                               //SYSTEM TICK�Ļص�����������OS������չ����Ҫ���Լ�����Ӧ����
#define configCPU_CLOCK_HZ			    ( ( unsigned long ) 72000000 )	//ϵͳCPUƵ�ʣ���λ��Hz
#define configTICK_RATE_HZ			    ( ( TickType_t ) 1000 )         //ϵͳSYSTEM TICKÿ���ӵķ��������� ��ֵԽ��ϵͳ��ӦԽ�죬����CPU���������л��Ŀ�����Խ��
#define configMAX_PRIORITIES		    ( 5 )                           //ϵͳ�������ȼ���,�����ĿԽ��ķ�RAMԽ��
#define configMINIMAL_STACK_SIZE	    ( ( unsigned short ) 128 )      //ϵͳ��С��ջ�ߴ磬ע��128����128�ֽڣ�����128����ջ������ARM32λ��128����ջ����512�ֽ�
#define configTOTAL_HEAP_SIZE		    ( ( size_t ) ( 30 * 1024 ) )    //ϵͳ�����ڴ�
#define configMAX_TASK_NAME_LEN		    ( 16 )                          //�����PC������󳤶�
#define configUSE_TRACE_FACILITY	    0                               //�Ƿ��趨��׷�٣���PC��TraceCon.exe��¼��Ҳ����ת��ϵͳ��ʾ����
#define configUSE_16_BIT_TICKS		    0                               //SYSTEM TICK�ĳ��ȣ�16��16λ�������16λ����CPU�� һ��ѡ1�������32λϵͳ��һ��ѡ0
#define configIDLE_SHOULD_YIELD		    1                               //���������¾��Ǻ�IDLE TASKͬ�����ȼ�������ִ��������������1����ϵͳӰ�첻��
#define configUSE_COUNTING_SEMAPHORES   0                               //�Ƿ��ü���ʽ��SEMAPHORES��SEMAPHORESҲ�������ͨѶ��һ�ַ�ʽ
#define configUSE_MUTEXES			    1                               //MUTEXES�������ͨѶ��һ�ַ�ʽ���ر���������������Դ��Ӧ�ã������ӡ��������A�õ�ʱ����ų�������Ӧ�ã������˱������ſ���Ӧ��
#define configCHECK_FOR_STACK_OVERFLOW	2                               //��������DEBUG�������Ƿ���ջ�������Ҫ���Լ�����Ӧ��麯��void vApplicationStackOverflowHook()


/* Co-routine definitions. */
//����Э�̡�Э�̹��ö�ջ����ʡRAM������û���������ȼ��ߣ�Ҳ�޷�������ͨѶ
#define configUSE_CO_ROUTINES 		0
//����Э�̵�������ȼ�����Э�����ȼ���Զ�������񡣾���ϵͳ��ִ��������������ִ�����˲�ִ��Э��
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		0   //�趨���Ըı��������ȶ�
#define INCLUDE_uxTaskPriorityGet		0   //�趨���Բ�ѯ�������ȶ�
#define INCLUDE_vTaskDelete				0   //�趨����ɾ������
#define INCLUDE_vTaskCleanUpResources	0   //��˵�ǿ��Ի���ɾ����������Դ��RAM�ȣ�
#define INCLUDE_vTaskSuspend			1   //���ÿ��԰��������
#define INCLUDE_vTaskDelayUntil			1   //���������ӳٵľ���ʱ��
#define INCLUDE_vTaskDelay				1   //����������ʱ�������ӳ�30���ӣ���Ե�ʱ�䣬����ʲôʱ�䣬����Ҫ֪��

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
//ϵͳ�ں˵��ж����ȼ����ж����ȼ�Խ�ͣ�Խ����Ӱ�������жϡ�һ��������
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
//ϵͳSVC�ж����ȼ������������M3��PIC32��Ӧ��
//191����>0XBF����>����4λ����>0X0B��11����0~10���������жϣ�11~15�������ж�
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
/*���Ǳ�����ÿ��ST�⣬������16��ֵ���ȼ�ֵ��Ϊ0-15������Ӧ��configKERNEL_INTERRUPT_PRIORITY
���á�����15��Ӧ�����255 NVIC��ֵ*/
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
