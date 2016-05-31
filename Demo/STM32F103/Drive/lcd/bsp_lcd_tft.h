#ifndef __BSP_LCD_TFT_H_
#define __BSP_LCD_TFT_H_		 

//��Ҫ����UCGUI�Ľӿں���
void LCD_TFT_PutPixel(u16 _usX , u16 _usY, u16 _usColor);                             //��1������
u16 LCD_TFT_GetPixel(u16 _usX , u16 _usY);                                            //��ȡ1������
void LCD_TFT_DrawHLine(u16 _usX1 , u16 _usY1 , u16 _usX2 , u16 _usColor);             //����һ��ˮƽ��
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor); //����һ����ɫˮƽ��
void LCD_TFT_DrawVLine(u16 _usX1 , u16 _usY1 , u16 _usY2 , u16 _usColor);             //����һ����ֱ��
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor); //����һ����ɫ��ֱ��
void LCD_TFT_FillRect(u16 _usX, u16 _usY, u16 _usHeight, u16 _usWidth, u16 _usColor); //������

void Draw_Circle(u16 x0,u16 y0,u8 r);                                //��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);                   //����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);              //������
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);               //��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);                //��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);       //��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p); //��ʾһ���ַ���,12/16����

#endif //__BSP_LCD_TFT_H_ 
