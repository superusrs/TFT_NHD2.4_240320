#include "lcd.h"
#include "stdlib.h"
#include "lcdfont.h"
#include "Config.h"
#include "main.h"
#include "gpio.h"

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

#if 1

//д�Ĵ������ã�Ĭ��8bit
__INLINE void LCD_WR_REG(u16 reg)
{   
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_DC(0);
	LCD_CS(0);
//	DATAOUT(reg<<8);
	DATAOUT(reg<<8);				//PIN0~7 connect to data 8~15
	LCD_WR(0);
	LCD_WR(1);
	LCD_CS(1);
	
#else   //ʹ��16λ������������ģʽ
	LCD_RS(0);
	LCD_CS(0);
	DATAOUT(reg);
	LCD_WR(0);
	LCD_WR(1);
	LCD_CS(1);
				
#endif	 
}

//дLCD ��8bit����
//data:Ҫд���ֵ
__INLINE void LCD_WR_DAT8(unsigned char data)
{
	LCD_CS(0);
	LCD_DC(1);
	LCD_WR(0);
	DATAOUT(data<<8);
	LCD_WR(1);
	LCD_CS(1);
}

//дLCD ��16bit����
//data:Ҫд���ֵ
__INLINE void LCD_WR_DAT16(unsigned int data)
{
	LCD_CS(0);
	LCD_DC(1);
	LCD_WR(0);
	DATAOUT(data&0xff00);
	LCD_WR(1);
	LCD_WR(0);
	DATAOUT(data<<8);
	LCD_WR(1);
	LCD_CS(1);
}
#endif

//��LCD����
//����ֵ:������ֵ
__INLINE u16 LCD_RD_DATA(void)
{
	u16 data=0,data0=0;
	LCD_DATA_IN();
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_DC(1);
	LCD_CS(0);
	LCD_RD(0);
	data=DATAIN;
	data=data&0xff00;
	LCD_RD(1);
	LCD_RD(0);
	data0=DATAIN;
	data0=data0<<8;
	LCD_RD(1);
	LCD_CS(1);
	data&=0xff00;
	data=data|data0>>8;
#else //ʹ��16λ������������ģʽ
	LCD_DC(1);
	LCD_CS(0);
	LCD_RD(0);
	data=DATAIN;
	LCD_RD(1);
	LCD_CS(1);
#endif
	LCD_DATA_OUT();
	return data;
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
__INLINE void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_WR_REG(0x2a);//�е�ַ����
	LCD_WR_DAT8(x1>>8);
	LCD_WR_DAT8(x1&0xff);
	LCD_WR_DAT8(x2>>8);
	LCD_WR_DAT8(x2&0xff);
	LCD_WR_REG(0x2b);//�е�ַ����
	LCD_WR_DAT8(y1>>8);
	LCD_WR_DAT8(y1&0xff);
	LCD_WR_DAT8(y2>>8);
	LCD_WR_DAT8(y2&0xff);
	LCD_WR_REG(0x2c);//������д
}

/******************************************************************************
      ����˵�������ù��λ��
      ������ݣ�x,y ���λ��
      ����ֵ��  ��
******************************************************************************/
__INLINE void LCD_SetCursor(u16 x,u16 y)
{
	LCD_WR_REG(0x2a);//�е�ַ����
	LCD_WR_DAT8(x>>8);
	LCD_WR_DAT8(x&0xff);
	LCD_WR_REG(0x2b);//�е�ַ����
	LCD_WR_DAT8(y>>8);
	LCD_WR_DAT8(y&0xff);
}

//LCD������ʾ
void LCD_DisplayOn(void)
{
	LCD_WR_REG(0X29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{
	LCD_WR_REG(0X28);	//�ر���ʾ
}  


//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	LCD_SetCursor(x,y);
	LCD_WR_REG(0X2E);
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_DATA_IN();
	LCD_DC(1);
	LCD_CS(0);
	LCD_RD(0);
	r=DATAIN;
	r=r<<8;
	LCD_RD(1);
	LCD_RD(0);
#else //ʹ��16λ������������ģʽ
	r=LCD_RD_DATA();//dummy read 	
#endif
	r=LCD_RD_DATA();//ʵ��������ɫ
	b=LCD_RD_DATA(); 
	g=r&0XFF;
	g<<=8;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}


void LCD_Clear(u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(0,0,lcddev.width-1,lcddev.height-1);//������ʾ��Χ
	for(i=0;i<lcddev.width;i++)
	{													   	 	
		for(j=0;j<lcddev.height;j++)
		{
			LCD_WR_DAT16(color);
		}
	} 					  	    
}

/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DAT16(color);
		}
	} 					  	    
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DAT16(color);
} 


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
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


/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      ����˵������ʾ����12x12����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DAT16(fc);
						else LCD_WR_DAT16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DAT16(fc);
						else LCD_WR_DAT16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DAT16(fc);
						else LCD_WR_DAT16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DAT16(fc);
						else LCD_WR_DAT16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //����6x12����
		else if(sizey==16)temp=ascii_1608[num][i];		 //����8x16����
		else if(sizey==24)temp=ascii_2412[num][i];		 //����12x24����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t))LCD_WR_DAT16(fc);
				else LCD_WR_DAT16(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ����
                pic[]  ͼƬ����    
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u8 picH,picL;
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			picH=pic[k*2];
			picL=pic[k*2+1];
			LCD_WR_DAT16(picH<<8|picL);
			k++;
		}
	}			
}


void Set_Dir(u8 dir)
{
	if((dir>>4)%4)
	{
		lcddev.width=320;
		lcddev.height=240;
	}else
	{
		lcddev.width=240;
		lcddev.height=320;
	}
}



//��ʼ��lcd
void LCD_Init(void)
{
	LCD_RE(0);
	delay_ms(200); 					// delay 50 ms 
	LCD_RE(1);
	
	delay_ms(50); 					// delay 50 ms 
	Set_Dir(DFT_SCAN_DIR);
	LCD_WR_REG(0XD3);
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
  lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//����0X9341
#else   //ʹ��16λ������������ģʽ
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//����0X00
	lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	
#endif
	    LCD_WR_REG(0x11);
		delay_ms(100); //Delay 120ms
	    LCD_WR_REG(0X36);// Memory Access Control
//		if(USE_HORIZONTAL==0)LCD_WR_DATA(0x00);
//		else if(USE_HORIZONTAL==1)LCD_WR_DATA(0xC0);
//		else if(USE_HORIZONTAL==2)LCD_WR_DATA(0x70);
//		else 
        LCD_WR_DAT8(0xA0);
		LCD_WR_REG(0X3A);
		LCD_WR_DAT8(0X05);
	//--------------------------------ST7789S Frame rate setting-------------------------

		LCD_WR_REG(0xb2);
		LCD_WR_DAT8(0x0c);
		LCD_WR_DAT8(0x0c);
		LCD_WR_DAT8(0x00);
		LCD_WR_DAT8(0x33);
		LCD_WR_DAT8(0x33);
		LCD_WR_REG(0xb7);
		LCD_WR_DAT8(0x35);
		//---------------------------------ST7789S Power setting-----------------------------

		LCD_WR_REG(0xbb);
		LCD_WR_DAT8(0x35);
		LCD_WR_REG(0xc0);
		LCD_WR_DAT8(0x2c);
		LCD_WR_REG(0xc2);
		LCD_WR_DAT8(0x01);
		LCD_WR_REG(0xc3);
		LCD_WR_DAT8(0x13);
		LCD_WR_REG(0xc4);
		LCD_WR_DAT8(0x20);
		LCD_WR_REG(0xc6);
		LCD_WR_DAT8(0x0f);
		LCD_WR_REG(0xca);
		LCD_WR_DAT8(0x0f);
		LCD_WR_REG(0xc8);
		LCD_WR_DAT8(0x08);
		LCD_WR_REG(0x55);
		LCD_WR_DAT8(0x90);
		LCD_WR_REG(0xd0);
		LCD_WR_DAT8(0xa4);
		LCD_WR_DAT8(0xa1);
		//--------------------------------ST7789S gamma setting------------------------------
		LCD_WR_REG(0xe0);
		LCD_WR_DAT8(0xd0);
		LCD_WR_DAT8(0x00);
		LCD_WR_DAT8(0x06);
		LCD_WR_DAT8(0x09);
		LCD_WR_DAT8(0x0b);
		LCD_WR_DAT8(0x2a);
		LCD_WR_DAT8(0x3c);
		LCD_WR_DAT8(0x55);
		LCD_WR_DAT8(0x4b);
		LCD_WR_DAT8(0x08);
		LCD_WR_DAT8(0x16);
		LCD_WR_DAT8(0x14);
		LCD_WR_DAT8(0x19);
		LCD_WR_DAT8(0x20);
		LCD_WR_REG(0xe1);
		LCD_WR_DAT8(0xd0);
		LCD_WR_DAT8(0x00);
		LCD_WR_DAT8(0x06);
		LCD_WR_DAT8(0x09);
		LCD_WR_DAT8(0x0b);
		LCD_WR_DAT8(0x29);
		LCD_WR_DAT8(0x36);
		LCD_WR_DAT8(0x54);
		LCD_WR_DAT8(0x4b);
		LCD_WR_DAT8(0x0d);
		LCD_WR_DAT8(0x16);
		LCD_WR_DAT8(0x14);
		LCD_WR_DAT8(0x21);
		LCD_WR_DAT8(0x20);
		LCD_WR_REG(0x29);	

	    //LCD_LED=1;
}




