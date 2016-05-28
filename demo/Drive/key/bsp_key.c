/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  solo
  * @version V0.0.2
  * @date    2016-03-07
  * @brief   ��������
  *
  * @verbatim ����API
  *           - KEY_Init    //������ʼ��
  *           - KEY_Scan    //��������
  *   
  * @note    
  *         1������˲���������
  * @endverbatim  
  *                                  
  */

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "sys.h"

#include "bsp_key.h"

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
*********************************************************************************************************
*/
static uint8_t IsKeyDown1(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}
static uint8_t IsKeyDown2(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}
static uint8_t IsKeyDown3(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}

static uint8_t IsKeyDown4(void)
{
	if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) != 0 && 
        (GPIO_PORT_K2->IDR & GPIO_PIN_K2) != 0 &&
        (GPIO_PORT_K3->IDR & GPIO_PIN_K3) != 0 &&
        (GPIO_PORT_K4->IDR & GPIO_PIN_K4) != 0 )
    {
		return 1;
    }
	else 
    {
		return 0;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
	bsp_InitKeyHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;
    
    #if FREERTOS_ENABLED 	//��FREERTOS_ENABLED������,˵��ʹ��FreeRTOS��.
    vTaskDelay(5/portTICK_RATE_MS);//GUI������������
    #endif
    
	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime    = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;		/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;					/* ���������� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����������ã�����/���/����/ģ��
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//����������ã�����/���/����/ģ��
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_tKey.Read  = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = 0;			            /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count    = KEY_FILTER_TIME / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State    = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
        s_tBtn[i].KeyCodeDown = 3 * i + 1;			/* �������µļ�ֵ���� */
        s_tBtn[i].KeyCodeUp   = 3 * i + 2;			/* ��������ļ�ֵ���� */
        s_tBtn[i].KeyCodeLong = 3 * i + 3;			/* �������������µļ�ֵ���� */
		s_tBtn[i].RepeatSpeed = 0;					/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;					/* ���������� */
        s_tBtn[i].attribute   = 0;                  //�������ԣ�ͨ�ð���
	}

	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
    //������������
	s_tBtn[KID_K1].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K1].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	s_tBtn[KID_K2].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K2].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	s_tBtn[KID_K3].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K3].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

	s_tBtn[KID_K4].LongTime = KEY_LONG_TIME;
	s_tBtn[KID_K4].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

    //��������
//    s_tBtn[KID_K6].attribute  = 2;//shift��
//    s_tBtn[KID_K7].attribute  = 1;//��ϼ�
//    s_tBtn[KID_K8].attribute  = 1;//��ϼ�
//    s_tBtn[KID_K9].attribute  = 1;//��ϼ�
//    s_tBtn[KID_K10].attribute = 1;//��ϼ�
    
	/* �жϰ������µĺ��� */
	s_tBtn[KID_K1].IsKeyDownFunc  = IsKeyDown1;
	s_tBtn[KID_K2].IsKeyDownFunc  = IsKeyDown2;
	s_tBtn[KID_K3].IsKeyDownFunc  = IsKeyDown3;
	s_tBtn[KID_K4].IsKeyDownFunc  = IsKeyDown4;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
    #if KEY_SHIF //��ϼ�ģʽ
    static u8 flag = 0;//��ϰ���������ʶ
    #endif
    
	KEY_T *pBtn = &s_tBtn[i];

	if (pBtn->IsKeyDownFunc()) //������״̬������
	{
		if (pBtn->Count < KEY_FILTER_TIME) 
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME) //�˲�
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0) //��ֻ֤����һ��
			{
				pBtn->State = 1;
                
                #if KEY_SHIF //��ϼ�ģʽ
                if(pBtn->attribute == 1) //��ϰ�������Ҫ�������������ĸ���
                {
                    flag = 1;
                    bsp_ClearKey();//������ϰ���ǰ��Ҫ��һ�μ�ֵ
                }
                #endif
                
				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey(s_tBtn[i].KeyCodeDown);
			}

			if (pBtn->LongTime > 0) //�����������
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime) 
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey(s_tBtn[i].KeyCodeLong);//��һ������������
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0) //������������
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey(s_tBtn[i].KeyCodeDown);//������ֵ���ڷ���
						}
					}
				}
			}
		}
	}
	else //����
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
                
				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey(s_tBtn[i].KeyCodeUp);
                
                #if KEY_SHIF //��ϼ�ģʽ
                if((flag == 1) && //��������ϼ�
                   ((pBtn->attribute == 2) || ((bsp_GetKeyState(KID_SHIFT) == 0) && (pBtn->attribute == 1)))) //shift���ͷŻ�����ϼ�ͬʱ�ͷ����ֵ
                {
                    flag = 0;
                    bsp_ClearKey();//Ҫ��һ�μ�ֵ
                }
                #endif
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
    #if KEY_CLEAR //��ʱ���
    static uint32_t cont = 0;//����
    #endif
	uint8_t i;
    
    #if KEY_CLEAR //��ʱ���
    if(cont++ >= 50) 
    {
        cont = 0;
        bsp_ClearKey();//Ҫ��һ�μ�ֵ
    }
    #endif
    
	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}
