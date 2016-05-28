#ifndef __BSP_KEY_H
#define __BSP_KEY_H 

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP)

/* �����ڶ�Ӧ��RCCʱ�� */
#define RCC_ALL_KEY 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE)

#define GPIO_PORT_K1    GPIOE
#define GPIO_PIN_K1	    GPIO_Pin_4

#define GPIO_PORT_K2    GPIOE
#define GPIO_PIN_K2	    GPIO_Pin_3

#define GPIO_PORT_K3    GPIOE
#define GPIO_PIN_K3	    GPIO_Pin_2

#define GPIO_PORT_K4    GPIOA
#define GPIO_PIN_K4	    GPIO_Pin_0

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_SHIF            0  //0��ʹ����ϼ���shift������1ʹ����ϼ���shift��
#define KEY_COUNT           4  /* ��������, 6�������� + 4����ϼ� */
#define KEY_FIFO_SIZE	    10  /* ����FIFO�õ����� */
#define KEY_FILTER_TIME     2   /* �˲�ʱ�� */
#define KEY_LONG_TIME       100 /* ��λ10ms�� ����1�룬��Ϊ�����¼� */
#define KEY_CLEAR           50  /* ��ʱ�����������10msΪ��λ����������ʹ�ã�
                                   ����fifo����һ������:�������м��书�ܣ����Ӧ�ó���Ͱ��������ȼ���һЩ���������첽�������������Ӧ*/

/* ����Ӧ�ó���Ĺ��������������� */
#define KEY_LEFT_PRESS	KEY_1_DOWN //������
//#define KEY_LEFT_UNDO	KEY_1_UP
//#define KEY_LEFT_LONG	KEY_1_LONG

#define KEY_RIGHT_PRESS	KEY_2_DOWN //������
//#define KEY_RIGHT_UNDO	KEY_2_UP
//#define KEY_RIGHT_LONG	KEY_2_LONG

#define KEY_UP_PRESS	KEY_3_DOWN //������
//#define KEY_UP_UNDO	    KEY_3_UP
//#define KEY_UP_LONG	    KEY_3_LONG

#define KEY_DOWN_PRESS	KEY_4_DOWN //������
//#define KEY_DOWN_UNDO	KEY_4_UP
//#define KEY_DOWN_LONG	KEY_4_LONG

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_K4,
}KEY_ID_E;

#if KEY_SHIF //��ϼ�ģʽ
    #define KID_SHIFT KID_K6 //ָ��shift��
#endif

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
    uint8_t  KeyCodeDown;	/* �������µļ�ֵ���� */
    uint8_t  KeyCodeUp;	    /* ��������ļ�ֵ���� */
    uint8_t  KeyCodeLong;	/* �������������µļ�ֵ���� */    
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
    uint8_t  attribute;     //���ԣ�0��ͨ�ð�����������������1����ϰ�����2��shift��
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				/* 4������ */
	KEY_4_LONG,				/* 4������ */
}KEY_ENUM;

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t Key_GetState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif //__BSP_KEY_H
