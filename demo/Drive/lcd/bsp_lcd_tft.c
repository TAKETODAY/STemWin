#include "sys.h"

#include "bsp_lcd.h"
#include "font.h"

#include "bsp_lcd_tft.h"

/*******************************************************************************
* ������	: LCD_DrawLine
* ����  	: ����
* ����  	: x1,y1:�������
*             x2,y2:�յ����� 
* ����ֵ	: ��
*******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
    
	delta_x = x2-x1; //������������ 
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1;
    
	if(delta_x > 0) //���õ�������
    {
        incx = 1;  
    }
	else if(delta_x == 0) //��ֱ��
    {
        incx = 0; 
    }
	else 
    {
        incx = -1;
        delta_x = -delta_x;
    } 
    
	if(delta_y > 0)
    {
        incy = 1;
    }
	else if(delta_y==0) //ˮƽ��
    {
        incy = 0;
    }
	else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
	if( delta_x>delta_y) //ѡȡ��������������
    {
        distance = delta_x; 
    }
	else 
    {
        distance = delta_y; 
    }
    
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow, uCol);//����
        
		xerr+=delta_x ; 
		yerr+=delta_y ; 
        
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
        
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

/*******************************************************************************
* ������	: LCD_DrawRectangle
* ����  	: ������
* ����  	: x1,y1:�������
*             x2,y2:�յ����� 
* ����ֵ	: ��
*******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*******************************************************************************
* ������	: Draw_Circle
* ����  	: ��ָ��λ�û�һ��ָ����С��Բ
* ����  	: x1,y1:���ĵ�
*             r:�뾶
* ����ֵ	: ��
*******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
    
	a = 0;
    b = r;	  
	di = 3-(r<<1); //�ж��¸���λ�õı�־
    
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
        
		//ʹ��Bresenham�㷨��Բ
		if(di < 0) 
        {
            di += 4*a+6;
        }            
		else
		{
			di += 10+4*(a-b);   
			b--;
		} 						    
	}
} 									  

/*******************************************************************************
* ������	: LCD_ShowChar
* ����  	: ��ָ��λ����ʾһ���ַ�
* ����  	: x,y:��ʼ����
*             num:Ҫ��ʾ���ַ�
*             size:�����С 12/16
*             mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
* ����ֵ	: ��
*******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp = POINT_COLOR;
    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp=asc2_1206[num][t]; //����1206����
			else temp = asc2_1608[num][t];		 //����1608���� 
            
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80) POINT_COLOR = colortemp;
				else POINT_COLOR = BACK_COLOR;
                
				LCD_DrawPoint(x,y);
                
				temp<<=1;
				y++;
                
				if(x >= lcddev.width) //��������
                {
                    POINT_COLOR = colortemp;
                    return;
                }
                
				if((y-y0) == size)
				{
					y = y0;
					x++;
                    
					if(x >= lcddev.width) //��������
                    {
                        POINT_COLOR=colortemp;
                        return;
                    }
                    
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp = asc2_1206[num][t]; //����1206����
			else temp = asc2_1608[num][t];		   //����1608���� 
            
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80) LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
                
				if(x >= lcddev.height)//��������
                {
                    POINT_COLOR=colortemp;
                    return;
                }
                
				if((y-y0) == size)
				{
					y = y0;
					x++;
					if(x >= lcddev.width) //��������
                    {
                        POINT_COLOR = colortemp;
                        return;
                    }
                    
					break;
				}
			}  	 
	    }     
	}
    
	POINT_COLOR = colortemp;	    	   	 	  
}

/*******************************************************************************
* ������	: LCD_Pow
* ����  	: m^n����
* ����  	: ��
* ����ֵ	: m^n�η�.
*******************************************************************************/
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*******************************************************************************
* ������	: LCD_ShowNum
* ����  	: ��ʾ����,��λΪ0,����ʾ
* ����  	: x,y :�������	
*             num:��ֵ(0~4294967295)
*             len :���ֵ�λ��
*             size:�����С
*             color:��ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
}

/*******************************************************************************
* ������	: LCD_ShowxNum
* ����  	: ��ʾ����,��λΪ0,������ʾ
* ����  	: x,y :�������	
*             num:��ֵ(0~999999999)
*             len :���ֵ�λ��
*             size:�����С
*             mode: [7]:0,�����;1,���0.
*                   [6:1]:����
*                   [0]:0,�ǵ�����ʾ;1,������ʾ
* ����ֵ	: ��
*******************************************************************************/
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow = 0;
    
	for(t=0;t<len;t++)
	{
		temp = (num/LCD_Pow(10,len-t-1))%10;
        
		if((enshow == 0) && (t < (len-1)))
		{
			if(temp == 0)
			{
				if(mode&0X80) LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}
            else 
            {  
                enshow = 1;
            }
		}
        
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

/*******************************************************************************
* ������	: LCD_ShowString
* ����  	: ��ʾ�ַ���
* ����  	: x,y :�������	
*             width,height:�����С
*             size:�����С
*             *p:�ַ�����ʼ��ַ
* ����ֵ	: ��
*******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0 = x;
    
	width+=x;
	height+=y;
    
    while((*p<='~') && (*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x >= width) 
        {
            x = x0;
            y+=size;
        }
        
        if(y >= height) break;//�˳�
        
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/******************************************************************************
                          ���º��������Ż�GUI
******************************************************************************/
/*******************************************************************************
* ������	: LCD_TFT_PutPixel
* ����  	: ��1������ ����Ҫ����UCGUI�Ľӿں�����
* ����  	: _usX,_usY : ��������
*			  _usColor  ��������ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_TFT_PutPixel(u16 _usX , u16 _usY, u16 _usColor)
{
    //���ù��λ��
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = _usX>>8;
    LCD->LCD_RAM = _usX&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = _usY>>8;
    LCD->LCD_RAM = _usY&0XFF;

	LCD->LCD_REG = 0X2C;//��ʼд��GRAM

    LCD->LCD_RAM = _usColor;
}

/*******************************************************************************
* ������	: LCD_TFT_GetPixel
* ����  	: ��ȡ1������ ����Ҫ����UCGUI�Ľӿں�����
* ����  	: _usX,_usY : ��������
*			  _usColor  ��������ɫ
* ����ֵ	: ��
*******************************************************************************/
u16 LCD_TFT_GetPixel(u16 _usX , u16 _usY)
{
    u16 r = 0,g = 0,b = 0;
    
    //���ù��λ��
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = _usX>>8;
    LCD->LCD_RAM = _usX&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = _usY>>8;
    LCD->LCD_RAM = _usY&0XFF;

    LCD->LCD_REG = 0x2E;//���Ͷ�GRAMָ��
    
    if(LCD->LCD_RAM) r = 0; //dummy Read
 	r = LCD->LCD_RAM; //ʵ��������ɫ
    
    b = LCD->LCD_RAM; 
    g = r&0XFF;//��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
    g <<= 8;

	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));//��Ҫ��ʽת��һ��
}

/*******************************************************************************
* ������	: LCD_TFT_DrawHLine
* ����  	: ����һ��ˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
* ����  	: _usX1    :��ʼ��X����
*			  _usY1    :ˮƽ�ߵ�Y����
*			  _usX2    :������X����
*			  _usColor :��ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_TFT_DrawHLine(u16 _usX1 , u16 _usY1 , u16 _usX2 , u16 _usColor)
{
	u16 i;

    LCD_Set_Window(_usX1, _usY1, _usX2 - _usX1 + 1, 1);//������ʾ����

	LCD->LCD_REG = 0X2C;//��ʼд��GRAM

	/* д�Դ� */
	for (i = 0; i <_usX2-_usX1 + 1; i++)
	{
		LCD->LCD_RAM = _usColor;
	}
}

/*******************************************************************************
* ������	: LCD_TFT_DrawHColorLine
* ����  	: ����һ����ɫˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
* ����  	: _usX1    :��ʼ��X����
*			  _usY1    :ˮƽ�ߵ�Y����
*			  _usWidth ��ֱ�ߵĿ��
*			  _usColor :��ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor)
{
	u16 i;
	
    LCD_Set_Window(_usX1, _usY1, _usWidth, 1);//������ʾ����
    
	LCD->LCD_REG = 0X2C;//��ʼд��GRAM

	/* д�Դ� */
	for (i = 0; i <_usWidth; i++)
	{
		LCD->LCD_RAM = *_pColor++;
	}
}

/*******************************************************************************
* ������	: LCD_TFT_DrawVLine
* ����  	: ����ֱƽ�� ����Ҫ����UCGUI�Ľӿں�����
* ����  	: X,Y���������ɫ
*			  _usY1    :ˮƽ�ߵ�Y����
*			  _usX2    :������X����
*			  _usColor :��ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_TFT_DrawVLine(u16 _usX1 , u16 _usY1 , u16 _usY2 , u16 _usColor)
{
	u16 i;
	
    LCD_Set_Window(_usX1, _usY1, 1, _usY2 - _usY1 + 1);//������ʾ����

	LCD->LCD_REG = 0X2C;//��ʼд��GRAM
    
	for (i = _usY1; i <=_usY2; i++)
	{	
		LCD->LCD_RAM = _usColor;
	}
}

/*******************************************************************************
* ������	: LCD_TFT_FillRect
* ����  	: ������ ����Ҫ����UCGUI�Ľӿں�����
* ����  	: _usX,_usY���������Ͻǵ�����
*			  _usHeight �����εĸ߶�
*			  _usWidth  �����εĿ��
*			  _usColor :��ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_TFT_FillRect(u16 _usX, u16 _usY, u16 _usHeight, u16 _usWidth, u16 _usColor)
{
	u32 i;

	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/
    LCD_Set_Window(_usX, _usY, _usWidth, _usHeight);//������ʾ����

	LCD->LCD_REG = 0X2C;//��ʼд��GRAM
    
	for (i = 0; i < _usHeight * _usWidth; i++)
	{
		LCD->LCD_RAM = _usColor;
	}
}
